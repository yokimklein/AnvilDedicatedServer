#include "network_session_membership.h"
#include "..\..\dllmain.h"

int c_network_session_membership::get_first_peer()
{
    typedef int(__fastcall* get_first_peer_ptr)(c_network_session_membership* session_membership);
    auto get_first_peer = reinterpret_cast<get_first_peer_ptr>(module_base + 0x313F0);
    return get_first_peer(this);
}

int c_network_session_membership::get_next_peer(int peer_index) // UNTESTED!!
{
    int next_index = peer_index + 1;
    if (next_index >= k_network_maximum_machines_per_session)
        return -1;
    while (true)
    {
        if (_bittest((long*)&this->m_valid_peer_mask, next_index))
            break;
        if (++next_index >= k_network_maximum_machines_per_session)
            return -1;
    }
    return next_index;
}