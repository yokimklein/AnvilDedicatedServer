#include "version.h"

const bool k_tracked_build = VERSION_TRACKED_BUILD;

char const k_version_target_application[] = VERSION_TARGET_APPLICATION;
char const k_version_target_variant[] = VERSION_TARGET_VARIANT;
char const k_version_target_configuration[] = VERSION_TARGET_CONFIGURATION;
char const k_version_target_platform[] = VERSION_TARGET_PLATFORM;

long const k_version_build_number_version = VERSION_BUILD_NUMBER_VERSION;
long const k_version_build_number = VERSION_BUILD_NUMBER;

char const k_version_branch_name[] = VERSION_BRANCH_NAME;
char const k_version_project_version[] = VERSION_PROJECT_VERSION;

char const k_version_build_string[] = VERSION_BUILD_STRING;
char const k_version_build_name[] = VERSION_BUILD_NAME;
char const k_version_full_string[] = VERSION_FULL_STRING;

char const k_version_tracked_string[] = VERSION_TRACKED_STRING;
char const k_version_untracked_string[] = VERSION_UNTRACKED_STRING;

long const k_version_build_number_sequence_identifier = VERSION_BUILD_NUMBER_SEQUENCE_IDENTIFIER;

const char* version_get_branch_name()
{
    return k_version_branch_name;
}

long version_get_build_number()
{
    return k_version_build_number;
}

bool version_is_tracked_build()
{
    return k_tracked_build;
}

const char* version_get_build_string()
{
    return k_version_build_string;
}

const char* version_get_target_configuration()
{
    return k_version_target_configuration;
}

const char* version_get_target_variant()
{
    return k_version_target_variant;
}

const char* version_get_full_string()
{
    return k_version_full_string;
}

const char* version_get_build_identifier()
{
    return version_is_tracked_build() ? k_version_tracked_string : k_version_untracked_string;
}

long version_get_build_number_sequence_identifier()
{
    return k_version_build_number_sequence_identifier;
}

const char* version_get_target_application()
{
    return k_version_target_application;
}

long64 version_get_build_number_identifier()
{
    return make_int64(version_get_build_number(), version_get_build_number_sequence_identifier());
}