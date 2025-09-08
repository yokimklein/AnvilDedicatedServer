#pragma once
#include "cseries\cseries.h"

#define VERSION_PROJECT_VERSION STRINGIFY(VERSION_BUILD_NUMBER_VERSION) "." STRINGIFY(VERSION_BUILD_NUMBER)
#if VERSION_TRACKED_BUILD
#define VERSION_BUILD_STRING VERSION_PROJECT_VERSION " " VERSION_BRANCH_NAME
#else
#define VERSION_BUILD_STRING "untracked version"
#endif
#define VERSION_BUILD_DATE __DATE__
#define VERSION_BUILD_TIME __TIME__
#define VERSION_FULL_STRING VERSION_TARGET_APPLICATION " " VERSION_TARGET_VARIANT " " VERSION_TARGET_CONFIGURATION " " \
		VERSION_TARGET_PLATFORM " " VERSION_BUILD_STRING "  " VERSION_BUILD_DATE " " VERSION_BUILD_TIME
#define VERSION_TRACKED_STRING VERSION_PROJECT_VERSION "_" VERSION_BRANCH_NAME "___" VERSION_TARGET_CONFIGURATION
#define VERSION_UNTRACKED_STRING "untracked_" VERSION_BRANCH_NAME "__" VERSION_TARGET_CONFIGURATION
#if VERSION_TRACKED_BUILD
#define VERSION_WINDOW_VERSION VERSION_BRANCH_NAME "_" VERSION_TARGET_CONFIGURATION "_" VERSION_PROJECT_VERSION
#else
#define VERSION_WINDOW_VERSION VERSION_BRANCH_NAME "_" VERSION_TARGET_CONFIGURATION "_untracked"
#endif
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
extern long64 version_get_build_number_identifier();