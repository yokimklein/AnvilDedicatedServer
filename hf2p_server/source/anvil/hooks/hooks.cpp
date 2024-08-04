#include "hooks.h"
#include <cseries\cseries.h>
#include <anvil\hooks\hooks_ds.h>
#include <anvil\hooks\hooks_session.h>
#include <anvil\hooks\simulation\hooks_simulation.h>
#include <anvil\hooks\simulation\hooks_statborg.h>
#include <anvil\hooks\simulation\hooks_simulation_globals.h>
#include <anvil\hooks\simulation\hooks_object_creation.h>
#include <anvil\hooks\simulation\hooks_object_deletion.h>
#include <anvil\hooks\simulation\hooks_object_updates.h>
#include <anvil\hooks\simulation\hooks_physics_updates.h>
#include <anvil\hooks\simulation\hooks_damage_updates.h>
#include <anvil\hooks\simulation\hooks_weapon_updates.h>
#include <anvil\hooks\simulation\hooks_player_updates.h>
#include <anvil\hooks\simulation\hooks_damage_events.h>
#include <anvil\hooks\simulation\hooks_miscellaneous.h>
#define NMD_ASSEMBLY_IMPLEMENTATION
#include <nmd_assembly.h>
#include <stdio.h>
#include <Patch.hpp> // TODO: replace ED hook system

// helper function for insert_hook, this updates call & jump offsets for the new code destination & verifies short jumps land within the shellcode buffer
void insert_hook_copy_instructions(void* destination, void* source, size_t length, bool redirect_oob_jumps)
{
    // copy the instructions we're replacing into a new buffer
    byte* code_buffer = new byte[length];
    memcpy(code_buffer, source, length);

    size_t i = 0;
    do
    {
        size_t instruction_length = nmd_x86_ldisasm((void*)((size_t)source + i), length, NMD_X86_MODE_32);
        // ensure nmd_x86_ldisasm ran successfully
        assert(instruction_length != 0);
        // ensure instruction length fits within the length of the buffer provided
        assert(i + instruction_length <= length);
        // ensure operand exists after opcode
        assert(i + 1 <= length);

        // switch opcode
        long offset;
        char short_offset;
        bool near_jump_is_in_bounds;
        switch (code_buffer[i])
        {
            // fix jump & call offsets
            case 0xE8: // call
            case 0xE9: // jump
                // ensure instruction length is valid for a jump/call
                assert(instruction_length == 5);
                offset = *(long*)&code_buffer[i + 1]; // near offset
                offset = (long)(((size_t)source + i) + offset + 5); // destination address
                offset = offset - ((size_t)destination + i) - 5; // new offset
                memcpy(&code_buffer[i + 1], &offset, sizeof(offset));
                break;
            // fix short jump offsets
            case 0x74: // jz
            case 0x75: // jnz
            case 0x76: // jbe
            case 0x7C: // jl
            case 0x7E: // jle
            case 0xEB: // jmp short
                // ensure instruction length is valid for a short jump
                assert(instruction_length == 2);
                // verify offset fits in buffer
                short_offset = code_buffer[i + 1];
                near_jump_is_in_bounds = (short_offset + 2 + i) < length; // if this assert fails you've likely ended your replaced instructions with a short jump
                if (!near_jump_is_in_bounds && redirect_oob_jumps)
                {
                    offset = (length - (i + 2)); // redirect near jump to NOP at the end of the buffer
                    assert(offset <= 255); // Ensure we don't go beyond the short jump offset range
                    short_offset = static_cast<char>(offset);
                    code_buffer[i + 1] = short_offset;
                }
                else
                {
                    // Not in bounds and set to not redirect!!
                    assert(near_jump_is_in_bounds);
                }
                break;

            // TODO: unimplemented/untested short jump instructions
            case 0x70: // jo
            case 0x71: // jno
            case 0x72: // jb
            case 0x73: // jnb
            case 0x77: // ja
            case 0x78: // js
            case 0x79: // jns
            case 0x7A: // jp
            case 0x7B: // jnp
            case 0x7D: // jge
            case 0x7F: // jg
            case 0xE3: // jcxz
                assert(false);
                break;

            // Near jumps
            case 0x0F:
                switch (code_buffer[i + 1])
                {
                    case 0x85: // jnz
                        // ensure instruction length is valid for a near jump
                        assert(instruction_length == 6);
                        offset = *(long*)&code_buffer[i + 2]; // near offset
                        offset = (long)(((size_t)source + i) + offset + 6); // destination address
                        offset = offset - ((size_t)destination + i) - 6; // new offset
                        *(long*)&code_buffer[i + 2] = offset;
                        break;
                    // TODO: unimplemented/untested near jump instructions
                    case 0x80: // jo
                    case 0x81: // jno
                    case 0x88: // js
                    case 0x89: // jns
                    case 0x84: // jz
                    case 0x82: // jb
                    case 0x83: // jnb
                    case 0x86: // jbe
                    case 0x87: // ja
                    case 0x8C: // jl
                    case 0x8D: // jge
                    case 0x8E: // jle
                    case 0x8F: // jg
                    case 0x8A: // jp
                    case 0x8B: // jnp
                        assert(false);
                        break;
                }
                break;
        }
        i += instruction_length;
    }
    while (i < length);

    memcpy(destination, code_buffer, length);
    delete[] code_buffer;
}

// This only works with parameterless functions - start_address is a baseless offset
// Inserted function must have runtime checks, safebuffers & JustMyCode disabled
// Make sure the destination of any short jumps included in your replaced instructions is also included
// NOTE: if you use the disassembly debug view whilst this is writing, the view will NOT update to the new instructions & bytes - this can make it look like there's a bug when there isn't
// NOTE: set redirect_oob_jumps to true to redirect out of bounds near jumps to the end of the replaced instructions
// NOTE: this does not preserve xmm registers - ensure that required xmm registers after your call aren't overwritten
// NOTE: ensure the area you're overwriting is at least 5 bytes in size
// NOTE: if using _hook_stack_frame_increase or _hook_stack_frame_cleanup, inserted_function becomes the number of bytes you wish to (de)allocate on the stack
void insert_hook(size_t start_address, size_t return_address, void* inserted_function, e_hook_type hook_type, bool redirect_oob_jumps)
{
    long code_offset = 0;
    byte preserve_registers[3] = { 0x50, 0x51, 0x52 }; // push eax, push ecx, push edx
    byte restore_registers[3] = { 0x5A, 0x59, 0x58 }; // pop edx, pop ecx, pop eax
    byte call_code[5] = { 0xE8, 0x90, 0x90, 0x90, 0x90 }; // call w/ 4x placeholder bytes where the function address will go
    byte return_code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder bytes
    byte jump_code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder bytes
    byte sub_esp_code[3] = { 0x83, 0xEC, 0x00 }; // sub esp, 0
    byte add_esp_code[3] = { 0x83, 0xC4, 0x00 }; // add esp, 0

    long length = return_address - start_address;
    if (length < sizeof(jump_code))
    {
        printf("The hook requires at least %d bytes available to overwrite!\n", sizeof(jump_code));
        return;
    }

    // initialise our new code block
    long inserted_code_size = sizeof(return_code);
    if (hook_type != _hook_replace && hook_type != _hook_replace_no_nop)
        inserted_code_size += length;
    if (hook_type == _hook_stack_frame_increase || hook_type == _hook_stack_frame_cleanup)
        inserted_code_size += sizeof(sub_esp_code);
    else
        inserted_code_size += sizeof(call_code) + sizeof(preserve_registers) + sizeof(restore_registers);
    byte* inserted_code = (byte*)VirtualAlloc(NULL, inserted_code_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (inserted_code == NULL)
    {
        printf("Failed to allocate memory for hook!\n");
        return;
    }

    if (hook_type == _hook_execute_replaced_first || hook_type == _hook_stack_frame_cleanup)
    {
        // copy & format the instructions we're replacing into our new code block
        insert_hook_copy_instructions(inserted_code + code_offset, (void*)BASE_ADDRESS(start_address), length, redirect_oob_jumps);
        code_offset += length;
    }

    if (hook_type == _hook_stack_frame_increase)
    {
        assert((char)inserted_function < INT8_MAX); // ensure we have the space to increase by the desired amount
        char increase_bytes = (char)inserted_function;
        memcpy(&sub_esp_code[2], &increase_bytes, sizeof(increase_bytes));
        memcpy(inserted_code + code_offset, sub_esp_code, sizeof(sub_esp_code));
        code_offset += sizeof(sub_esp_code);
    }
    else if (hook_type == _hook_stack_frame_cleanup)
    {
        assert((char)inserted_function < INT8_MAX); // ensure we have the space to increase by the desired amount
        char decrease_bytes = (char)inserted_function;
        memcpy(&add_esp_code[2], &decrease_bytes, sizeof(decrease_bytes));
        memcpy(inserted_code + code_offset, add_esp_code, sizeof(add_esp_code));
        code_offset += sizeof(add_esp_code);
    }
    else
    {
        // preserve registers across call
        memcpy(inserted_code + code_offset, preserve_registers, sizeof(preserve_registers));
        code_offset += sizeof(preserve_registers);

        // call inserted function
        size_t call_offset = ((size_t)inserted_function - (size_t)(inserted_code + code_offset) - sizeof(call_code));
        memcpy(&call_code[1], &call_offset, sizeof(call_offset));
        memcpy(inserted_code + code_offset, call_code, sizeof(call_code));
        code_offset += sizeof(call_code);

        // restore registers
        memcpy(inserted_code + code_offset, restore_registers, sizeof(restore_registers));
        code_offset += sizeof(restore_registers);
    }


    if (hook_type == _hook_execute_replaced_last || hook_type == _hook_stack_frame_increase)
    {
        // copy & format the instructions we're replacing into our new code block
        insert_hook_copy_instructions(inserted_code + code_offset, (void*)BASE_ADDRESS(start_address), length, redirect_oob_jumps);
        code_offset += length;
    }

    // return
    size_t return_offset = (BASE_ADDRESS(return_address) - (size_t)(inserted_code + code_offset) - sizeof(return_code));
    memcpy(&return_code[1], &return_offset, sizeof(return_offset));
    memcpy(inserted_code + code_offset, return_code, sizeof(return_code));
    code_offset += sizeof(return_code);

    // write jump to inserted function
    size_t jump_offset = ((size_t)(inserted_code)-BASE_ADDRESS(start_address) - sizeof(jump_code));
    memcpy(&jump_code[1], &jump_offset, sizeof(jump_offset));
    memcpy((void*)BASE_ADDRESS(start_address), jump_code, sizeof(jump_code));

    if (hook_type != _hook_replace_no_nop)
    {
        // nop the bytes leftover between the original overwritten instructions and the return point for sanity
        // makes looking at the modified disassembly less chaotic
        memset((void*)(BASE_ADDRESS(start_address) + sizeof(jump_code)), 0x90, length - sizeof(jump_code));
    }
}

// Now unused - I tried to create new variables by adding to the stack pointer and correcting all the esp offsets, but there were plenty of spaces where I couldn't do this without overflowing the offsets
void increase_esp_offsets(size_t function_start, size_t function_end, size_t offset_increase)
{
    size_t i = 0;
    size_t length = function_end - function_start;
    size_t base_function = BASE_ADDRESS(function_start);
    do
    {
        void* function_address = (void*)(base_function + i);
        size_t instruction_length = nmd_x86_ldisasm(function_address, length, NMD_X86_MODE_32);
        assert(instruction_length != 0); // ensure nmd_x86_ldisasm ran successfully

        nmd_x86_instruction instruction{};
        nmd_x86_decode(function_address, instruction_length, &instruction, NMD_X86_MODE_32, NMD_X86_DECODER_FLAGS_MINIMAL | NMD_X86_DECODER_FLAGS_OPERANDS);
        assert(instruction.valid && instruction.length > 0); // ensure decode ran successfully

        // find instructions with esp offsets
        for (size_t j = 0; j < instruction.num_operands; j++)
        {
            if (!instruction.operands[j].is_implicit && instruction.operands[j].type == NMD_X86_OPERAND_TYPE_MEMORY && instruction.operands[j].fields.mem.base == NMD_X86_REG_ESP)
            {
                // i'm unsure as to what scale does right now, and all instructions I've seen have it as zero
                assert(instruction.operands[j].fields.mem.scale == 0); // leaving this here so it'll alert me in case I need to handle this
                //assert(instruction.operands[j].fields.mem.index == 0); // can safely ignore this?
                //printf("ESP instruction(%p) w/ offset: %d prefixes: %d\n", function_address, instruction.displacement, instruction.num_prefixes);

                // prefixes, opcode, modrm, register, displacement
                // ensure disp_mask is as we expect it and not a combination of values
                assert(instruction.disp_mask == NMD_X86_DISP_NONE || instruction.disp_mask == NMD_X86_DISP8 || instruction.disp_mask == NMD_X86_DISP16 || instruction.disp_mask == NMD_X86_DISP32 || instruction.disp_mask == NMD_X86_DISP64);

                long displacement_offset = instruction.num_prefixes + instruction.opcode_size + 2; // prefixes + opcode size + modrm + register
                long assumed_size = displacement_offset + instruction.disp_mask;
                assert(instruction.length >= assumed_size); // ensure our assumption about the instruction length is correct (GTE because immediate operands might add bytes onto the end?)
                //assert(((byte*)function_address)[assumed_prefix_size] == instruction.opcode); // check opcode is where it should be
                assert(((byte*)function_address)[instruction.num_prefixes + instruction.opcode_size] == instruction.modrm.modrm); // check modrm is where it should be
                // this assert is generally true but not for operands where fields.mem.index is not 0
                //assert(((byte*)function_address)[instruction.num_prefixes + instruction.opcode_size + 1] == instruction.operands[j].fields.mem.base); // check register is where it should be

                ulong64 displacement = 0;
                ulong64 new_displacement = 0;
                void* displacement_address = (void*)(base_function + i + displacement_offset);
                switch (instruction.disp_mask)
                {
                    case NMD_X86_DISP_NONE:
                        displacement = 0;
                        // TODO: handle this
                        break;
                    case NMD_X86_DISP8:
                        displacement = *(char*)displacement_address;
                        assert(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        assert(new_displacement <= INT8_MAX); // ensure we don't go beyond the max byte value
                        *(char*)displacement_address = new_displacement;
                        break;
                    case NMD_X86_DISP16:
                        displacement = *(short*)displacement_address;
                        assert(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        assert(new_displacement <= INT16_MAX); // ensure we don't go beyond the max short value
                        *(short*)displacement_address = new_displacement;
                        break;
                    case NMD_X86_DISP32:
                        displacement = *(long*)displacement_address;
                        assert(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        assert(new_displacement <= INT32_MAX); // ensure we don't go beyond the max long value
                        *(long*)displacement_address = new_displacement;
                        break;
                    case NMD_X86_DISP64:
                        displacement = *(long64*)displacement_address;
                        assert(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        assert(new_displacement <= INT64_MAX); // ensure we don't go beyond the max long64 value
                        *(long64*)displacement_address = new_displacement;
                        break;
                }
            }
        }

        i += instruction_length;
    }
    while (i < length);
}

// correct ebp+ offsets to accomodate for newly pushed variables onto the stack at the start of functions
void increase_positive_ebp_offsets(size_t function_start, size_t function_end, size_t offset_increase)
{
    size_t i = 0;
    size_t length = function_end - function_start;
    size_t base_function = BASE_ADDRESS(function_start);
    do
    {
        void* instruction_address = (void*)(base_function + i);
        size_t instruction_length = nmd_x86_ldisasm(instruction_address, length, NMD_X86_MODE_32);
        assert(instruction_length != 0); // ensure nmd_x86_ldisasm ran successfully

        nmd_x86_instruction instruction{};
        bool result = nmd_x86_decode(instruction_address, instruction_length, &instruction, NMD_X86_MODE_32, NMD_X86_DECODER_FLAGS_MINIMAL | NMD_X86_DECODER_FLAGS_OPERANDS);
        assert(result && instruction.valid && instruction.length > 0); // ensure decode ran successfully

        // find instructions with positive ebp offsets
        for (size_t j = 0; j < instruction.num_operands; j++)
        {
            if (!instruction.operands[j].is_implicit && instruction.operands[j].type == NMD_X86_OPERAND_TYPE_MEMORY && instruction.operands[j].fields.mem.base == NMD_X86_REG_EBP)
            {
                // i'm unsure as to what scale does right now, and all instructions I've seen have it as zero
                //assert(instruction.operands[j].fields.mem.scale == 0); // leaving this here so it'll alert me in case I need to handle this

                // prefixes, opcode, modrm, displacement
                // ensure disp_mask is as we expect it and not a combination of values
                assert(instruction.disp_mask == NMD_X86_DISP_NONE || instruction.disp_mask == NMD_X86_DISP8 || instruction.disp_mask == NMD_X86_DISP16 || instruction.disp_mask == NMD_X86_DISP32 || instruction.disp_mask == NMD_X86_DISP64);

                long displacement_offset = instruction.num_prefixes + instruction.opcode_size + 1; // prefixes + opcode size + modrm
                long assumed_size = displacement_offset + instruction.disp_mask;
                assert(instruction.length >= assumed_size); // ensure our assumption about the instruction length is correct (GTE because immediate operands might add bytes onto the end?)
                //assert(((byte*)function_address)[assumed_prefix_size] == instruction.opcode); // check opcode is where it should be
                assert(((byte*)instruction_address)[instruction.num_prefixes + instruction.opcode_size] == instruction.modrm.modrm); // check modrm is where it should be
                long64 displacement = 0;
                long64 new_displacement = 0;
                void* displacement_address = (void*)(base_function + i + displacement_offset);
                switch (instruction.disp_mask)
                {
                    case NMD_X86_DISP_NONE:
                        displacement = 0;
                        assert(false); // TODO: handle this
                        break;
                    case NMD_X86_DISP8:
                        displacement = *(char*)displacement_address;
                        if (displacement >= 0)
                        {
                            assert(displacement == *(char*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            assert(new_displacement <= INT8_MAX); // ensure we don't go beyond the max byte value
                            *(char*)displacement_address = new_displacement;
                        }
                        break;
                    case NMD_X86_DISP16:
                        displacement = *(short*)displacement_address;
                        if (displacement >= 0)
                        {
                            assert(displacement == *(short*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            assert(new_displacement <= INT16_MAX); // ensure we don't go beyond the max short value
                            *(short*)displacement_address = new_displacement;
                        }
                        break;
                    case NMD_X86_DISP32:
                        displacement = *(long*)displacement_address;
                        if (displacement >= 0)
                        {
                            assert(displacement == *(long*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            assert(new_displacement <= INT32_MAX); // ensure we don't go beyond the max long value
                            *(long*)displacement_address = new_displacement;
                        }
                        break;
                    case NMD_X86_DISP64:
                        displacement = *(long64*)displacement_address;
                        if (displacement >= 0)
                        {
                            assert(displacement == *(long64*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            assert(new_displacement <= INT64_MAX); // ensure we don't go beyond the max long64 value
                            *(long64*)displacement_address = new_displacement;
                        }
                        break;
                }
                //if (new_displacement != 0)
                //{
                //    printf("EBP instruction(%p) w/ disp: %lld, new disp: %lld\n", instruction_address, displacement, new_displacement);
                //}
            }
        }

        i += instruction_length;
    } while (i < length);
}

// ALWAYS compare a function's instructions *before* AND *after* this has been called to ensure it has worked properly.
// NOTE: You have to manually clean up the stack after calling this, it does not handle it for you! There isn't always 5 bytes worth of space at the return to replace. You may have to cleanup after the function call.
void add_variable_space_to_stack_frame(size_t function_start, size_t function_end, size_t space_in_bytes)
{
    size_t bytes_to_overwrite = 0;
    size_t length = function_end - function_start;
    size_t base_function = BASE_ADDRESS(function_start);
    do
    {
        void* instruction_address = (void*)(base_function + bytes_to_overwrite);
        bytes_to_overwrite += nmd_x86_ldisasm(instruction_address, length, NMD_X86_MODE_32);

    }
    while (bytes_to_overwrite < 5);
    assert(bytes_to_overwrite < length); // ensure the space we want to overwrite hasn't exceeded the bounds of the function

    // make new hook flag to jump to raw assembly shellcode?
    insert_hook(function_start, function_start + bytes_to_overwrite, (void*)space_in_bytes, _hook_stack_frame_increase);

    // correct positive ebp offsets (usually function arguments) to account for the extra space we've pushed to the stack
    increase_positive_ebp_offsets(function_start, function_end, space_in_bytes);
}

void nop_region(size_t address, size_t length)
{
    memset((void*)BASE_ADDRESS(address), 0x90, length);
}

void hook_function(size_t function_address, size_t length, void* hook_function)
{
    byte jump_code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder bytes

    // ensure we have the space to write a jump
    assert(length >= 5);

    // cleanup old function code
    nop_region(function_address, length);

    // write jump to hook function
    size_t jump_offset = ((size_t)(hook_function)-BASE_ADDRESS(function_address) - sizeof(jump_code));
    memcpy(&jump_code[1], &jump_offset, sizeof(jump_offset));
    memcpy((void*)BASE_ADDRESS(function_address), jump_code, sizeof(jump_code));
}

void anvil_patches_apply()
{
    // enable tag edits
    Patch(0x082DB4, { 0xEB }).Apply();
    Patch::NopFill(Pointer::Base(0x083120), 2);
    Patch::NopFill(Pointer::Base(0x083AFC), 2);

    // contrail gpu freeze fix - twister
    Patch(0x1D6B70, { 0xC3 }).Apply();

    // enable netdebug
    //g_network_interface_show_latency_and_framerate_metrics_on_chud = true; // set this to true to enable
    //g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;

    // test to replace tutorial map id with diamondback id in damage function
    //Patch(0x40F290, { 0xC1, 0x02 }).Apply(); // replace 0x36D with 0x2C1
    //Patch(0x40F2E3, { 0xC1, 0x02 }).Apply(); // replace 0x36D with 0x2C1
    //Patch(0x411E02, { 0xC1, 0x02 }).Apply(); // replace 0x36D with 0x2C1
}

void anvil_hooks_apply()
{
    anvil_hooks_ds_apply(); // DEDICATED SERVER HOOKS
    anvil_hooks_session_apply(); // SESSION HOOKS
    anvil_hooks_simulation_apply(); // SIMULATION HOOKS
    anvil_hooks_statborg_apply(); // STATBORG UPDATES
    anvil_hooks_simulation_globals_apply(); // GLOBALS UPDATES
    anvil_hooks_object_creation_apply(); // OBJECT CREATION
    anvil_hooks_object_deletion_apply(); // OBJECT DELETION
    anvil_hooks_object_updates_apply(); // OBJECT UPDATES
    anvil_hooks_physics_updates_apply(); // OBJECT PHYSICS UPDATES
    anvil_hooks_damage_updates_apply(); // OBJECT DAMAGE UPDATES
    anvil_hooks_weapon_updates_apply(); // WEAPON UPDATES
    anvil_hooks_player_updates_apply(); // PLAYER UPDATES
    anvil_hooks_damage_events_apply(); // OBJECT DAMAGE EVENTS
    anvil_hooks_miscellaneous_apply(); // MISCELLANEOUS                                                   
}