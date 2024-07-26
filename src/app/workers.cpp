#include "workers.h"

// Insights on coding of a thread in Qt from https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
// Credit for the idea for the process function goes again to luluco250: MouseRaw.cpp https://gist.github.com/luluco250/ac79d72a734295f167851ffdb36d77ee


Worker::Worker()
{
}

Worker::~Worker()
{
}

struct inSignal Worker::get_mouse_data()
{
	//qDebug() << "Mouse Position " << input.mouse.xCoo << input.mouse.yCoo;
	
	return input;
}

void Worker::process()
{
    qDebug("Do Work ...");
    
	extern struct inSignal input;

	// Why even bother with WinMain?
	HINSTANCE instance = GetModuleHandle(0);

	HWND window;

	CreateMessageWindow(window);

	const USHORT HID_MOUSE = 0x02;
	const USHORT HID_KEYBOARD = 0x06;

	HID_RegisterDevice(window, HID_MOUSE);

	// Main loop:
	MSG event;

	while (!quit) {
		while (PeekMessage(&event, 0, 0, 0, PM_REMOVE)) {
			if (event.message == WM_QUIT) {
				quit = true;
				break;
			}

			// Does some Windows magic and sends the message to EventHandler()
			// because it's associated with the window we created.
			TranslateMessage(&event);
			DispatchMessage(&event);
		}



		emit update_mouse_pos(input);

		// Sleep a bit to retain reactivity
		Sleep(100);

		// Reset mouse data in case WM_INPUT isn't called:
		input.mouse.x = 0;
		input.mouse.y = 0;
		input.mouse.wheel = 0;
	}

	HID_UnregisterDevice(HID_MOUSE);


    emit finished();
}

void Worker::stop()
{
	quit = true;
}


Controller::Controller()
{
    qDebug("\nInit Controller");
    
    Worker* worker = new Worker();
    worker->moveToThread(&thread);

    connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &Controller::operate, worker, &Worker::process);
	connect(this, &Controller::get_data, worker, &Worker::get_mouse_data);
	connect(this, &Controller::stop, worker, &Worker::stop);

	
	connect(worker, &Worker::update_mouse_pos, this, &Controller::receive_update);
	

    thread.start();
}

Controller::~Controller()
{
    qDebug("Controller Obliterated.\n");

    thread.quit();
    thread.wait();
}


void Controller::handleResults()
{

}

void Controller::receive_update(const struct inSignal& input)
{
	//qDebug() << "Mouse Position " << input.mouse.xCoo << input.mouse.yCoo;

	emit updated(input);
}


