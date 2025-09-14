#include "version.h"
#include "cseries\build_number_internal.h"

constexpr long stoi(const char* str)
{
    long value = 0;
    long i = 0;
    for (i = 0; str[i] != 0; i++)
    {
        // return -1 if the character isn't a number
        if (str[i] < '0' || str[i] > '9')
        {
            return NONE;
        }

        value = value * 10 + (str[i] - '0');
    }
    return i != 0 ? value : NONE;
}

#define VERSION_BUILD_DATE __DATE__
#define VERSION_BUILD_TIME __TIME__

#if !VERSION_TRACKED_BUILD
#define VERSION_BUILD_STRING "untracked version " VERSION_BRANCH_NAME
#define VERSION_BUILD_IDENTIFIER "untracked_" VERSION_BRANCH_NAME "__" VERSION_TARGET_CONFIGURATION
#define VERSION_PROJECT_VERSION "NONE"
#if defined(VERSION_BUILD_NUMBER_VERSION)
#undef VERSION_BUILD_NUMBER_VERSION
#endif
#define VERSION_BUILD_NUMBER_VERSION NONE
#if defined(VERSION_BUILD_NUMBER)
#undef VERSION_BUILD_NUMBER
#endif
#define VERSION_BUILD_NUMBER NONE
#else
#define VERSION_PROJECT_VERSION STRINGIFY(VERSION_BUILD_NUMBER_VERSION) "." STRINGIFY(VERSION_BUILD_NUMBER)
#define VERSION_BUILD_STRING VERSION_PROJECT_VERSION " " VERSION_BRANCH_NAME "-" VERSION_COMMIT_HASH
#define VERSION_WINDOW_VERSION VERSION_BRANCH_NAME "_" VERSION_TARGET_CONFIGURATION "_" VERSION_PROJECT_VERSION
#define VERSION_BUILD_IDENTIFIER VERSION_PROJECT_VERSION "_" VERSION_BRANCH_NAME "___" VERSION_TARGET_CONFIGURATION
#endif

#define VERSION_WINDOW_VERSION VERSION_BRANCH_NAME "_" VERSION_TARGET_CONFIGURATION "_" VERSION_PROJECT_VERSION

#define VERSION_FULL_STRING VERSION_TARGET_APPLICATION " " VERSION_TARGET_VARIANT " " VERSION_TARGET_CONFIGURATION " " \
		VERSION_TARGET_PLATFORM " " VERSION_BUILD_STRING "  " VERSION_BUILD_DATE " " VERSION_BUILD_TIME

const bool k_tracked_build = VERSION_TRACKED_BUILD;

char const k_version_target_application[] = VERSION_TARGET_APPLICATION;
char const k_version_target_variant[] = VERSION_TARGET_VARIANT;
char const k_version_target_configuration[] = VERSION_TARGET_CONFIGURATION;
char const k_version_target_platform[] = VERSION_TARGET_PLATFORM;

long const k_version_build_number_version = VERSION_BUILD_NUMBER_VERSION;
long const k_version_build_number = stoi(STRINGIFY(VERSION_BUILD_NUMBER));

char const k_version_branch_name[] = VERSION_BRANCH_NAME;
char const k_version_project_version[] = VERSION_PROJECT_VERSION;

char const k_version_build_string[] = VERSION_BUILD_STRING;
char const k_version_build_name[] = VERSION_BUILD_NAME;
char const k_version_full_string[] = VERSION_FULL_STRING;

char const k_version_build_identifier[] = VERSION_BUILD_IDENTIFIER;

// No idea what this is but it's 2 in ms23 & its crash reporter
long const k_version_build_number_sequence_identifier = VERSION_BUILD_NUMBER_SEQUENCE_IDENTIFIER;

char const k_version_window_version[] = VERSION_WINDOW_VERSION;
char const k_version_build_date[] = VERSION_BUILD_DATE;
char const k_version_build_time[] = VERSION_BUILD_TIME;

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
    return k_version_build_identifier;
}

long version_get_build_number_sequence_identifier()
{
    return k_version_build_number_sequence_identifier;
}

const char* version_get_target_application()
{
    return k_version_target_application;
}

const char* version_get_window_version()
{
    return k_version_window_version;
}

const char* version_get_build_date()
{
    return k_version_build_date;
}

const char* version_get_build_time()
{
    return k_version_build_time;
}

long64 version_get_build_number_identifier()
{
#if (VERSION_TRACKED_BUILD)
    return make_int64(version_get_build_number(), version_get_build_number_sequence_identifier());
#else
    return NONE;
#endif
}