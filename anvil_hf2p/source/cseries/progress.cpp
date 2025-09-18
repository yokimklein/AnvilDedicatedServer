#include "progress.h"
#include "cseries\cseries.h"

REFERENCE_DECLARE(0x4EC4720, s_progress_globals, progress_globals);

void progress_set_default_callbacks(const progress_callbacks* callbacks)
{
	if (callbacks)
	{
		progress_globals.default_callbacks = *callbacks;
	}
	else
	{
		progress_globals.default_callbacks =
		{
			.progress_new_proc = NULL,
			.progress_update_proc = NULL,
			.progress_done_proc = NULL,
			.progress_data = NULL
		};
	}
}
