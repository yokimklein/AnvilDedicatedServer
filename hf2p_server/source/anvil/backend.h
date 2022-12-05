#pragma once
#include <unordered_map>

extern std::unordered_map<std::string, const char*> backend_hashmap;

void backend_deobfuscate_uri(char* uri, long uri_size);