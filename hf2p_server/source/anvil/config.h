#pragma once
#include <fstream>
#include <string>
#include <map>

void anvil_load_configuration();
extern std::map<std::string, std::string> g_anvil_configuration;