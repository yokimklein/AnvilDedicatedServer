#include "hooks.h"
#include <anvil\hooks\effects\hooks_effect_system.h>
#include <anvil\hooks\hooks_ds.h>
#include <anvil\hooks\hooks_session.h>
#include <anvil\hooks\hooks_miscellaneous.h>
#include <anvil\hooks\hooks_debug.h>
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
#include <anvil\hooks\simulation\hooks_simulation_events.h>
#define NMD_ASSEMBLY_IMPLEMENTATION
#include <nmd_assembly.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memoryapi.h>
#include <game\game.h>

// helper function for insert, this updates call & jump offsets for the new code destination & verifies short jumps land within the shellcode buffer
void insert_hook_copy_instructions(void* destination, void* source, size_t length, bool redirect_oob_jumps)
{
    // copy the instructions we're replacing into a new buffer
    byte* code_buffer = new byte[length];
    csmemcpy(code_buffer, source, length);

    size_t i = 0;
    do
    {
        size_t instruction_length = nmd_x86_ldisasm((void*)((size_t)source + i), length, NMD_X86_MODE_32);
        // ensure nmd_x86_ldisasm ran successfully
        ASSERT(instruction_length != 0);
        // ensure instruction length fits within the length of the buffer provided
        ASSERT(i + instruction_length <= length);
        // ensure operand exists after opcode
        ASSERT(i + 1 <= length);

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
                ASSERT(instruction_length == 5);
                offset = *(long*)&code_buffer[i + 1]; // near offset
                offset = (long)(((size_t)source + i) + offset + 5); // destination address
                offset = offset - ((size_t)destination + i) - 5; // new offset
                csmemcpy(&code_buffer[i + 1], &offset, sizeof(offset));
                break;
            // fix short jump offsets
            case 0x74: // jz
            case 0x75: // jnz
            case 0x76: // jbe
            case 0x7C: // jl
            case 0x7E: // jle
            case 0xEB: // jmp short
                // ensure instruction length is valid for a short jump
                ASSERT(instruction_length == 2);
                // verify offset fits in buffer
                short_offset = code_buffer[i + 1];
                near_jump_is_in_bounds = (short_offset + 2 + i) < length; // if this assert fails you've likely ended your replaced instructions with a short jump
                if (!near_jump_is_in_bounds && redirect_oob_jumps)
                {
                    offset = (length - (i + 2)); // redirect near jump to NOP at the end of the buffer
                    ASSERT(offset <= 255); // Ensure we don't go beyond the short jump offset range
                    short_offset = static_cast<char>(offset);
                    code_buffer[i + 1] = short_offset;
                }
                else
                {
                    // Not in bounds and set to not redirect!!
                    ASSERT(near_jump_is_in_bounds);
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
                ASSERT(false);
                break;

            // Near jumps
            case 0x0F:
                switch (code_buffer[i + 1])
                {
                    case 0x85: // jnz
                        // ensure instruction length is valid for a near jump
                        ASSERT(instruction_length == 6);
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
                        ASSERT(false);
                        break;
                }
                break;
        }
        i += instruction_length;
    }
    while (i < length);

    csmemcpy(destination, code_buffer, length);
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
void hook::insert(size_t start_address, size_t return_address, void* inserted_function, e_hook_type hook_type, bool redirect_oob_jumps)
{
    // $TODO: push esp, push ebp immediately, call hook cdecl with these
    // not every hook will expect this, 

    long code_offset = 0;

    //push        esp
    //push        ebp
    //push        edi
    //push        esi
    //push        edx
    //push        ecx
    //push        ebx
    //push        eax
    byte preserve_registers[] = { 0x54, 0x55, 0x57, 0x56, 0x52, 0x51, 0x53, 0x50 };

    //pop         eax
    //pop         ebx
    //pop         ecx
    //pop         edx
    //pop         esi
    //pop         edi
    //add         esp, 8
    byte restore_registers[] = { 0x58, 0x5B, 0x59, 0x5A, 0x5E, 0x5F, 0x83, 0xC4, 0x08 };

    //add         esp, 32
    byte nopreserve_registers[] = { 0x83, 0xC4, 0x20 };


    byte call_code[] = { 0xE8, 0x90, 0x90, 0x90, 0x90 }; // call w/ 4x placeholder bytes where the function address will go
    byte return_code[] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder bytes
    byte jump_code[] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder bytes
    byte sub_esp_code[] = { 0x83, 0xEC, 0x00 }; // sub esp, 0
    byte add_esp_code[] = { 0x83, 0xC4, 0x00 }; // add esp, 0

    long length = return_address - start_address;
    if (length < sizeof(jump_code))
    {
        VASSERT(0, c_string_builder("The hook requires at least %d bytes available to overwrite!", sizeof(jump_code)).get_string());
        return;
    }

    // initialise our new code block
    long inserted_code_size = sizeof(return_code);
    if ((hook_type != _hook_replace && hook_type != _hook_replace_no_preserve) && hook_type != _hook_replace_no_nop)
    {
        inserted_code_size += length;
    }
    if (hook_type == _hook_stack_frame_increase || hook_type == _hook_stack_frame_cleanup)
    {
        inserted_code_size += sizeof(sub_esp_code);
    }
    else
    {
        inserted_code_size += sizeof(call_code);
    }
    if (hook_type != _hook_replace_no_preserve)
    {
        inserted_code_size += sizeof(preserve_registers) + sizeof(restore_registers);
    }
    else
    {
        inserted_code_size += sizeof(preserve_registers) + sizeof(nopreserve_registers);
    }

    byte* inserted_code = (byte*)VirtualAlloc(NULL, inserted_code_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (inserted_code == NULL)
    {
        VASSERT(0, "Failed to allocate memory for hook!");
        return;
    }

    if (hook_type == _hook_execute_replaced_first || hook_type == _hook_stack_frame_cleanup)
    {
        // copy & format the instructions we're replacing into our new code block
        insert_hook_copy_instructions(inserted_code + code_offset, base_address<void*>(start_address), length, redirect_oob_jumps);
        code_offset += length;
    }

    if (hook_type == _hook_stack_frame_increase)
    {
        ASSERT((char)inserted_function < INT8_MAX); // ensure we have the space to increase by the desired amount
        char increase_bytes = (char)inserted_function;
        csmemcpy(&sub_esp_code[2], &increase_bytes, sizeof(increase_bytes));
        csmemcpy(inserted_code + code_offset, sub_esp_code, sizeof(sub_esp_code));
        code_offset += sizeof(sub_esp_code);
    }
    else if (hook_type == _hook_stack_frame_cleanup)
    {
        ASSERT((char)inserted_function < INT8_MAX); // ensure we have the space to increase by the desired amount
        char decrease_bytes = (char)inserted_function;
        csmemcpy(&add_esp_code[2], &decrease_bytes, sizeof(decrease_bytes));
        csmemcpy(inserted_code + code_offset, add_esp_code, sizeof(add_esp_code));
        code_offset += sizeof(add_esp_code);
    }
    else
    {
        // preserve registers across call
        csmemcpy(inserted_code + code_offset, preserve_registers, sizeof(preserve_registers));
        code_offset += sizeof(preserve_registers);

        // call inserted function
        size_t call_offset = ((size_t)inserted_function - (size_t)(inserted_code + code_offset) - sizeof(call_code));
        csmemcpy(&call_code[1], &call_offset, sizeof(call_offset));
        csmemcpy(inserted_code + code_offset, call_code, sizeof(call_code));
        code_offset += sizeof(call_code);

        if (hook_type != _hook_replace_no_preserve)
        {
            // restore registers
            csmemcpy(inserted_code + code_offset, restore_registers, sizeof(restore_registers));
            code_offset += sizeof(restore_registers);
        }
        else
        {
            // cleanup call
            csmemcpy(inserted_code + code_offset, nopreserve_registers, sizeof(nopreserve_registers));
            code_offset += sizeof(nopreserve_registers);
        }
    }


    if (hook_type == _hook_execute_replaced_last || hook_type == _hook_stack_frame_increase)
    {
        // copy & format the instructions we're replacing into our new code block
        insert_hook_copy_instructions(inserted_code + code_offset, base_address<void*>(start_address), length, redirect_oob_jumps);
        code_offset += length;
    }

    // return
    size_t return_offset = (base_address(return_address) - (size_t)(inserted_code + code_offset) - sizeof(return_code));
    csmemcpy(&return_code[1], &return_offset, sizeof(return_offset));
    csmemcpy(inserted_code + code_offset, return_code, sizeof(return_code));
    code_offset += sizeof(return_code);

    // enable memory writing for insert
    ulong old_protect = patch::set_memory_protect(start_address, PAGE_READWRITE, sizeof(jump_code));

    // write jump to inserted function
    size_t jump_offset = ((size_t)(inserted_code)-base_address(start_address) - sizeof(jump_code));
    csmemcpy(&jump_code[1], &jump_offset, sizeof(jump_offset));
    csmemcpy(base_address<void*>(start_address), jump_code, sizeof(jump_code));

    // restore memory protection
    patch::set_memory_protect(start_address, old_protect, sizeof(jump_code));
    patch::set_memory_protect((size_t)inserted_code - base_address(), PAGE_EXECUTE_READ, inserted_code_size);

    if (hook_type != _hook_replace_no_nop)
    {
        // nop the bytes leftover between the original overwritten instructions and the return point for sanity
        // makes looking at the modified disassembly less chaotic
        long nop_length = length - sizeof(jump_code);
        if (nop_length > 0)
        {
            patch::nop_region(start_address + sizeof(jump_code), nop_length);
        }
    }
}

/*
// Now unused - I tried to create new variables by adding to the stack pointer and correcting all the esp offsets, but there were plenty of spaces where I couldn't do this without overflowing the offsets
void increase_esp_offsets(size_t function_start, size_t function_end, size_t offset_increase)
{
    size_t i = 0;
    size_t length = function_end - function_start;
    size_t base_function = base_address(function_start);
    do
    {
        void* function_address = (void*)(base_function + i);
        size_t instruction_length = nmd_x86_ldisasm(function_address, length, NMD_X86_MODE_32);
        ASSERT(instruction_length != 0); // ensure nmd_x86_ldisasm ran successfully

        nmd_x86_instruction instruction{};
        nmd_x86_decode(function_address, instruction_length, &instruction, NMD_X86_MODE_32, NMD_X86_DECODER_FLAGS_MINIMAL | NMD_X86_DECODER_FLAGS_OPERANDS);
        ASSERT(instruction.valid && instruction.length > 0); // ensure decode ran successfully

        // find instructions with esp offsets
        for (size_t j = 0; j < instruction.num_operands; j++)
        {
            if (!instruction.operands[j].is_implicit && instruction.operands[j].type == NMD_X86_OPERAND_TYPE_MEMORY && instruction.operands[j].fields.mem.base == NMD_X86_REG_ESP)
            {
                // i'm unsure as to what scale does right now, and all instructions I've seen have it as zero
                ASSERT(instruction.operands[j].fields.mem.scale == 0); // leaving this here so it'll alert me in case I need to handle this
                //ASSERT(instruction.operands[j].fields.mem.index == 0); // can safely ignore this?
                //printf("ESP instruction(%p) w/ offset: %d prefixes: %d\n", function_address, instruction.displacement, instruction.num_prefixes);

                // prefixes, opcode, modrm, register, displacement
                // ensure disp_mask is as we expect it and not a combination of values
                ASSERT(instruction.disp_mask == NMD_X86_DISP_NONE || instruction.disp_mask == NMD_X86_DISP8 || instruction.disp_mask == NMD_X86_DISP16 || instruction.disp_mask == NMD_X86_DISP32 || instruction.disp_mask == NMD_X86_DISP64);

                long displacement_offset = instruction.num_prefixes + instruction.opcode_size + 2; // prefixes + opcode size + modrm + register
                long assumed_size = displacement_offset + instruction.disp_mask;
                ASSERT(instruction.length >= assumed_size); // ensure our assumption about the instruction length is correct (GTE because immediate operands might add bytes onto the end?)
                //ASSERT(((byte*)function_address)[assumed_prefix_size] == instruction.opcode); // check opcode is where it should be
                ASSERT(((byte*)function_address)[instruction.num_prefixes + instruction.opcode_size] == instruction.modrm.modrm); // check modrm is where it should be
                // this assert is generally true but not for operands where fields.mem.index is not 0
                //ASSERT(((byte*)function_address)[instruction.num_prefixes + instruction.opcode_size + 1] == instruction.operands[j].fields.mem.base); // check register is where it should be

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
                        ASSERT(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        ASSERT(new_displacement <= INT8_MAX); // ensure we don't go beyond the max byte value
                        *(char*)displacement_address = new_displacement;
                        break;
                    case NMD_X86_DISP16:
                        displacement = *(short*)displacement_address;
                        ASSERT(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        ASSERT(new_displacement <= INT16_MAX); // ensure we don't go beyond the max short value
                        *(short*)displacement_address = new_displacement;
                        break;
                    case NMD_X86_DISP32:
                        displacement = *(long*)displacement_address;
                        ASSERT(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        ASSERT(new_displacement <= INT32_MAX); // ensure we don't go beyond the max long value
                        *(long*)displacement_address = new_displacement;
                        break;
                    case NMD_X86_DISP64:
                        displacement = *(long64*)displacement_address;
                        ASSERT(displacement == instruction.displacement);
                        new_displacement = displacement + offset_increase;
                        ASSERT(new_displacement <= INT64_MAX); // ensure we don't go beyond the max long64 value
                        *(long64*)displacement_address = new_displacement;
                        break;
                }
            }
        }

        i += instruction_length;
    }
    while (i < length);
}
*/

// correct ebp+ offsets to accomodate for newly pushed variables onto the stack at the start of functions
void increase_positive_ebp_offsets(size_t function_start, size_t function_end, size_t offset_increase)
{
    size_t i = 0;
    size_t length = function_end - function_start;
    size_t base_function = base_address(function_start);

    // enable memory writing for insert
    ulong old_protect = patch::set_memory_protect(function_start, PAGE_READWRITE, length);
    do
    {
        void* instruction_address = (void*)(base_function + i);
        size_t instruction_length = nmd_x86_ldisasm(instruction_address, length, NMD_X86_MODE_32);
        ASSERT(instruction_length != 0); // ensure nmd_x86_ldisasm ran successfully

        nmd_x86_instruction instruction{};
        bool result = nmd_x86_decode(instruction_address, instruction_length, &instruction, NMD_X86_MODE_32, NMD_X86_DECODER_FLAGS_MINIMAL | NMD_X86_DECODER_FLAGS_OPERANDS);
        ASSERT(result && instruction.valid && instruction.length > 0); // ensure decode ran successfully

        // find instructions with positive ebp offsets
        for (size_t j = 0; j < instruction.num_operands; j++)
        {
            if (!instruction.operands[j].is_implicit && instruction.operands[j].type == NMD_X86_OPERAND_TYPE_MEMORY && instruction.operands[j].fields.mem.base == NMD_X86_REG_EBP)
            {
                // i'm unsure as to what scale does right now, and all instructions I've seen have it as zero
                //ASSERT(instruction.operands[j].fields.mem.scale == 0); // leaving this here so it'll alert me in case I need to handle this

                // prefixes, opcode, modrm, displacement
                // ensure disp_mask is as we expect it and not a combination of values
                ASSERT(instruction.disp_mask == NMD_X86_DISP_NONE || instruction.disp_mask == NMD_X86_DISP8 || instruction.disp_mask == NMD_X86_DISP16 || instruction.disp_mask == NMD_X86_DISP32 || instruction.disp_mask == NMD_X86_DISP64);

                long displacement_offset = instruction.num_prefixes + instruction.opcode_size + 1; // prefixes + opcode size + modrm
                long assumed_size = displacement_offset + instruction.disp_mask;
                ASSERT(instruction.length >= assumed_size); // ensure our assumption about the instruction length is correct (GTE because immediate operands might add bytes onto the end?)
                //ASSERT(((byte*)function_address)[assumed_prefix_size] == instruction.opcode); // check opcode is where it should be
                ASSERT(((byte*)instruction_address)[instruction.num_prefixes + instruction.opcode_size] == instruction.modrm.modrm); // check modrm is where it should be
                long64 displacement = 0;
                long64 new_displacement = 0;
                void* displacement_address = (void*)(base_function + i + displacement_offset);
                switch (instruction.disp_mask)
                {
                    case NMD_X86_DISP_NONE:
                        displacement = 0;
                        ASSERT(false); // TODO: handle this
                        break;
                    case NMD_X86_DISP8:
                        displacement = *(char*)displacement_address;
                        if (displacement >= 0)
                        {
                            ASSERT(displacement == *(char*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            ASSERT(new_displacement <= INT8_MAX); // ensure we don't go beyond the max byte value
                            *(char*)displacement_address = new_displacement;
                        }
                        break;
                    case NMD_X86_DISP16:
                        displacement = *(short*)displacement_address;
                        if (displacement >= 0)
                        {
                            ASSERT(displacement == *(short*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            ASSERT(new_displacement <= INT16_MAX); // ensure we don't go beyond the max short value
                            *(short*)displacement_address = new_displacement;
                        }
                        break;
                    case NMD_X86_DISP32:
                        displacement = *(long*)displacement_address;
                        if (displacement >= 0)
                        {
                            ASSERT(displacement == *(long*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            ASSERT(new_displacement <= INT32_MAX); // ensure we don't go beyond the max long value
                            *(long*)displacement_address = new_displacement;
                        }
                        break;
                    case NMD_X86_DISP64:
                        displacement = *(long64*)displacement_address;
                        if (displacement >= 0)
                        {
                            ASSERT(displacement == *(long64*)&instruction.displacement); // ensure displacement value is correct
                            new_displacement = displacement + offset_increase;
                            ASSERT(new_displacement <= INT64_MAX); // ensure we don't go beyond the max long64 value
                            *(long64*)displacement_address = new_displacement;
                        }
                        break;
                }
                //if (new_displacement != 0)
                //{
                //    printf("EBP instruction(%p) w/ disp: %lld, new disp: %lld\n", instruction_address, displacement, new_displacement);
                //}
            }
            // handle fstp using non standard ebp value - TODO: what is going on here?
            else if (instruction.opcode == 0xD9 && instruction.modrm.modrm == 0x5D) // 0xD9 - fstp opcode, 0xD9 - ebp register (normal register values + 0x38)
            {
                char offset = ((char*)instruction_address)[2];
                if (offset > 0)
                {
                    ((char*)instruction_address)[2] = offset + offset_increase;
                }
            }
        }

        i += instruction_length;
    } while (i < length);

    // restore memory protection
    patch::set_memory_protect(function_start, old_protect, length);
}

// ALWAYS compare a function's instructions *before* AND *after* this has been called to ensure it has worked properly.
// NOTE: You have to manually clean up the stack after calling this, it does not handle it for you! There isn't always 5 bytes worth of space at the return to replace. Cleanup must occur at the start of the function epilogue so the pops return the correct values
void hook::add_variable_space_to_stack_frame(size_t function_start, size_t function_end, size_t space_in_bytes)
{
    size_t bytes_to_overwrite = 0;
    size_t length = function_end - function_start;
    size_t base_function = base_address(function_start);
    do
    {
        void* instruction_address = (void*)(base_function + bytes_to_overwrite);
        bytes_to_overwrite += nmd_x86_ldisasm(instruction_address, length, NMD_X86_MODE_32);

    }
    while (bytes_to_overwrite < 5);
    ASSERT(bytes_to_overwrite < length); // ensure the space we want to overwrite hasn't exceeded the bounds of the function

    // correct positive ebp offsets (typically function arguments) to account for the extra space we've pushed to the stack
    increase_positive_ebp_offsets(function_start, function_end, space_in_bytes);

    // make new hook flag to jump to raw assembly shellcode?
    hook::insert(function_start, function_start + bytes_to_overwrite, (void*)space_in_bytes, _hook_stack_frame_increase);
}

ulong patch::set_memory_protect(size_t address, ulong new_protect, size_t size)
{
    ulong old_protect;
    if (!VirtualProtect(base_address<void*>(address), size, new_protect, &old_protect))
    {
        VASSERT(0, c_string_builder("failed to set memory protection at baseless address 0x%08x!", address).get_string());
        return PAGE_NOACCESS;
    }
    return old_protect;
}

void patch::nop_region(size_t address, size_t length)
{
    // enable memory writing
    ulong old_protect = patch::set_memory_protect(address, PAGE_READWRITE, length);

    csmemset(base_address<void*>(address), 0x90, length);

    // restore memory protection
    patch::set_memory_protect(address, old_protect, length);
}

void patch::bytes(size_t address, std::initializer_list<byte> bytes)
{
    // enable memory writing
    ulong old_protect = patch::set_memory_protect(address, PAGE_READWRITE, bytes.size());

    csmemcpy(base_address<void*>(address), bytes.begin(), bytes.size());

    // restore memory protection
    patch::set_memory_protect(address, old_protect, bytes.size());
}

void patch::bytes(size_t address, byte* bytes, size_t size)
{
    // enable memory writing
    ulong old_protect = patch::set_memory_protect(address, PAGE_READWRITE, size);

    csmemcpy(base_address<void*>(address), bytes, size);

    // restore memory protection
    patch::set_memory_protect(address, old_protect, size);
}

void patch::function(size_t address, void* function_address)
{
    // enable memory writing
    ulong old_protect = patch::set_memory_protect(address, PAGE_READWRITE, sizeof(size_t));

    // convert function address to byte array
    ulong addr = reinterpret_cast<uintptr_t>(function_address);
    byte bytes[4];
    bytes[0] = (addr >> 0) & 0xFF;
    bytes[1] = (addr >> 8) & 0xFF;
    bytes[2] = (addr >> 16) & 0xFF;
    bytes[3] = (addr >> 24) & 0xFF;

    // copy bytes
    csmemcpy(base_address<void*>(address), bytes, sizeof(size_t));

    // restore memory protection
    patch::set_memory_protect(address, old_protect, sizeof(size_t));
}

void hook::function(size_t function_address, size_t length, void* function)
{
    ASSERT(function);

    byte jump_code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // jump w/ 4x placeholder bytes

    // ensure we have the space to write a jump
    ASSERT(length >= 5);

    // cleanup old function code
    patch::nop_region(function_address, length);

    // enable memory writing
    ulong old_protect = patch::set_memory_protect(function_address, PAGE_READWRITE, length);

    // write jump to hook function
    size_t jump_offset = ((size_t)(function)-base_address(function_address) - sizeof(jump_code));
    csmemcpy(&jump_code[1], &jump_offset, sizeof(jump_offset));
    csmemcpy(base_address<void*>(function_address), jump_code, sizeof(jump_code));

    // restore memory protection
    patch::set_memory_protect(function_address, old_protect, length);
}

void hook::call(size_t call_address, void* function)
{
    ASSERT(function);
    const bool k_target_is_call_opcode = base_address<byte*>(call_address)[0] == 0xE8;
    ASSERT(k_target_is_call_opcode == true); // assert if there's no call instruction at the supplied address

    byte call_code[5] = { 0xE8, 0x90, 0x90, 0x90, 0x90 }; // call w/ 4x placeholder bytes

    // enable memory writing
    ulong old_protect = patch::set_memory_protect(call_address, PAGE_READWRITE, sizeof(call_code));

    // overwrite call
    size_t call_offset = ((size_t)(function)-base_address(call_address) - sizeof(call_code));
    csmemcpy(&call_code[1], &call_offset, sizeof(call_offset));
    csmemcpy(base_address<void*>(call_address), call_code, sizeof(call_code));

    // restore memory protection
    patch::set_memory_protect(call_address, old_protect, sizeof(call_code));
}

//void patch_bytes(size_t address, char* bytes, size_t length)
//{
//    csmemcpy(base_address<void*>(address), bytes, length);
//}

void anvil_patches_apply()
{
    // enable tag edits
    patch::bytes(0x082DB4, { 0xEB });
    patch::nop_region(0x083120, 2);
    patch::nop_region(0x083AFC, 2);

    // contrail gpu freeze fix - twister
    patch::bytes(0x1D6B70, { 0xC3 });

    // enable netdebug
    //g_network_interface_show_latency_and_framerate_metrics_on_chud = true; // set this to true to enable
    //g_network_interface_fake_latency_and_framerate_metrics_on_chud = false;

    // test to replace tutorial map id with diamondback id in damage function
    //patch::bytes(0x40F290, { 0xC1, 0x02 }); // replace 0x36D with 0x2C1
    //patch::bytes(0x40F2E3, { 0xC1, 0x02 }); // replace 0x36D with 0x2C1
    //patch::bytes(0x411E02, { 0xC1, 0x02 }); // replace 0x36D with 0x2C1

    // english patch
    patch::bytes(0x2B923A, { _language_english });
}

void anvil_hooks_apply()
{
    anvil_hooks_miscellaneous_apply(); // MISCELLANEOUS
    anvil_hooks_effect_system_apply(); // EFFECTS
    anvil_hooks_debug_apply(); // DEBUG

    if (game_is_dedicated_server())
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
        anvil_hooks_simulation_events_apply(); // SIMULATION EVENTS
    }
}