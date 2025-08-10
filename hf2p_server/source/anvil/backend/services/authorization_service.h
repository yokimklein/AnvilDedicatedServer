#pragma once
#include <anvil\backend\backend.h>

class c_backend::authorization_service
{
public:
	static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

	// $TODO: encapsulate these
	inline static ulong m_last_token_refresh = NONE;
	inline static std::string m_authorisation_token;
	inline static std::string m_diagnostics_token;
	
private:
	inline static resolved_endpoint* m_endpoint;
};
