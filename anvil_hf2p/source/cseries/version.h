#pragma once
#include "cseries\cseries.h"

extern const bool k_tracked_build;

extern const char* version_get_branch_name();
extern long version_get_build_number();
extern bool version_is_tracked_build();
extern const char* version_get_build_string();
extern const char* version_get_target_configuration();
extern const char* version_get_target_variant();
extern const char* version_get_full_string();
extern const char* version_get_build_identifier();
extern long version_get_build_number_sequence_identifier();
extern const char* version_get_target_application();
extern const char* version_get_window_version();
extern const char* version_get_build_date();
extern const char* version_get_build_time();
extern long64 version_get_build_number_identifier();