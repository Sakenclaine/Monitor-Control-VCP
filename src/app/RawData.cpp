#include "RawData.h"


struct inSignal input;

// Event handler for the hidden/invisible window getting the raw mouse input
// Adapted from original code from luluco250: MouseRaw.cpp https://gist.github.com/luluco250/ac79d72a734295f167851ffdb36d77ee
LRESULT CALLBACK EventHandler(
	HWND hwnd, // the handle of the window
	unsigned event, // the event
	WPARAM wparam,
	LPARAM lparam
) {
	switch (event) {

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_INPUT: {
		// The official Microsoft examples are pretty terrible about this.
		// Size needs to be non-constant because GetRawInputData() can return the
		// size necessary for the RAWINPUT data, which is a weird feature.
		unsigned size = sizeof(RAWINPUT);
		static RAWINPUT raw[sizeof(RAWINPUT)];
		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

		if (raw->header.dwType == RIM_TYPEMOUSE) {
			input.mouse.x = raw->data.mouse.lLastX;
			input.mouse.y = raw->data.mouse.lLastY;

			POINT mouseCoo;

			bool bmousePos = GetCursorPos(&mouseCoo);

			if (bmousePos)
			{
				input.mouse.xCoo = mouseCoo.x;
				input.mouse.yCoo = mouseCoo.y;
			}

			// Wheel data needs to be pointer casted to interpret an unsigned short as a short, with no conversion
			// otherwise it'll overflow when going negative.
			// Didn't happen before some minor changes in the code, doesn't seem to go away
			// so it's going to have to be like this.
			if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
				input.mouse.wheel = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
		}
	} return 0;
	}

	// Run default message processor for any missed events:
	return DefWindowProc(hwnd, event, wparam, lparam);
}

bool CreateMessageWindow(HWND& hwindow)
{
	HINSTANCE instance = GetModuleHandle(0);

	// "{}" is necessary, otherwise we have to use ZeroMemory() (which is just memset).
	WNDCLASS window_class = {};
	window_class.lpfnWndProc = EventHandler;
	window_class.hInstance = instance;
	window_class.lpszClassName = L"SimpleEngine Class";

	if (!RegisterClass(&window_class))
		return -1;

	hwindow = CreateWindow(L"SimpleEngine Class", L"SimpleEngine", 0, 0, 0, 0, 0, HWND_MESSAGE, 0, 0, 0);

	if (hwindow == nullptr)
		return false;

	else { return true; }
	// End of creating window.
}


// Function to register a device to receive raw input from it, e.g. a mouse usage = 0x02
// dwFlags can be found here: https://learn.microsoft.com/de-de/windows/win32/api/winuser/ns-winuser-rawinputdevice
bool HID_RegisterDevice(HWND hTarget, USHORT usage)
{
	RAWINPUTDEVICE hid;

	hid.usUsagePage = 0x01;
	hid.usUsage = usage;
	hid.dwFlags = RIDEV_DEVNOTIFY | RIDEV_INPUTSINK;
	hid.hwndTarget = hTarget;

	bool bregister = RegisterRawInputDevices(&hid, 1, sizeof(hid));

	return bregister;
}

// Function to UNregister a device to receive raw input from it, e.g. a mouse usage = 0x02
void HID_UnregisterDevice(USHORT usage)
{
	RAWINPUTDEVICE hid;
	hid.usUsagePage = 0x01;
	hid.usUsage = usage;
	hid.dwFlags = RIDEV_REMOVE;
	hid.hwndTarget = NULL;

	RegisterRawInputDevices(&hid, 1, sizeof(hid));
}





