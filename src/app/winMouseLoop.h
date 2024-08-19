#pragma once
#include <Windows.h>


#ifndef INSIGNAL_H
#define INSIGNAL_H

// Structure to store out input data.
// Not necessary, I just find it neat.
struct mouseSignal {
	struct Mouse {
		// Keep in mind these are all deltas,
		// they'll change for one frame/cycle
		// before going back to zero.
		int x = 0;
		int y = 0;
		int wheel = 0;

		int xCoo = 0;
		int yCoo = 0;
	};

	Mouse mouse;
};

#endif


LRESULT CALLBACK EventHandler(HWND, unsigned, WPARAM, LPARAM);

bool CreateMessageWindow(HWND& hwindow);

bool HID_RegisterDevice(HWND hTarget, USHORT usage);

void HID_UnregisterDevice(USHORT usage);




