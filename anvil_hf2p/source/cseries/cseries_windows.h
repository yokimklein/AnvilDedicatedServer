#pragma once

ulong system_seconds();
ulong system_milliseconds();

void display_debug_string(const char* format, ...);
void system_get_date_and_time(char* buffer, short buffer_size, bool short_date_and_time);
void system_abort();
void system_exit();