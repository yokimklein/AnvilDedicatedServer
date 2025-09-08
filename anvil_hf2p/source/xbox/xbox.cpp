#include "xbox.h"
#include "cseries\cseries.h"
#include "shell\shell_windows.h"

bool get_clipboard_as_text(char* buf, long len)
{
	if (!IsClipboardFormatAvailable(CF_TEXT) || !OpenClipboard(g_windows_params.game_window_handle))
	{
		return false;
	}

	bool result = false;

	HANDLE clipboard_data = GetClipboardData(CF_TEXT);
	if (!clipboard_data)
	{
		CloseClipboard();
		return false;
	}

	if (const char* clipboard_text = (const char*)GlobalLock(clipboard_data))
	{
		csstrnzcpy(buf, clipboard_text, len);
		GlobalUnlock(clipboard_data);
	}

	CloseClipboard();
	return true;
}
