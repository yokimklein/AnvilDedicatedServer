#include "network_session_membership.h"
#include "..\..\dllmain.h"

long c_network_session_membership::get_first_peer()
{
    typedef long(__fastcall* get_first_peer_ptr)(c_network_session_membership* session_membership);
    auto get_first_peer = reinterpret_cast<get_first_peer_ptr>(module_base + 0x313F0);
    return get_first_peer(this);
}

long c_network_session_membership::get_next_peer(long peer_index) // UNTESTED!!
{
    long next_index = peer_index + 1;
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

long c_network_session_membership::get_peer_from_secure_address(s_transport_secure_address const* secure_address)
{
    typedef long(__fastcall* get_peer_from_secure_address_ptr)(c_network_session_membership* session_membership, s_transport_secure_address const* secure_address);
    auto get_peer_from_secure_address = reinterpret_cast<get_peer_from_secure_address_ptr>(module_base + 0x312B0);
    return get_peer_from_secure_address(this, secure_address);
}