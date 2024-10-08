#include <QDebug>

#include "winController.h"

// Insights on coding of a thread in Qt from https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
// Credit for the idea for the process function goes again to luluco250: MouseRaw.cpp https://gist.github.com/luluco250/ac79d72a734295f167851ffdb36d77ee


Worker::Worker()
{
}

Worker::~Worker()
{
}

void Worker::process()
{
    qDebug("Do Work ...");
    
	extern mouseSignal input;

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
		Sleep(150);

		// Reset mouse data in case WM_INPUT isn't called:
		input.mouse.x = 0;
		input.mouse.y = 0;
		input.mouse.wheel = 0;
	}

	HID_UnregisterDevice(HID_MOUSE);

	qDebug() << "Windows Process Event Loop Finished.";
	
	emit finished();
}

void Worker::stop()
{
	quit = true;
}



Controller::Controller(QObject* parent) :
	QObject(parent)
{
    qDebug("\nInit Controller");
	thread = new QThread();

    Worker* worker = new Worker();
    worker->moveToThread(thread);

	connect(this, &Controller::startP, worker, &Worker::process);
	connect(this, &Controller::stopP, worker, &Worker::stop);

    connect(worker, &Worker::finished, thread, &QThread::quit);
	connect(worker, &Worker::finished, worker, &QObject::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	connect(worker, &Worker::update_mouse_pos, this, &Controller::receive_update);

    thread->start();
}



Controller::~Controller()
{
    qDebug("Controller Destructor.\n");
}

Controller& Controller::getInstance()
{
	static Controller cntr;
	return cntr;
}

void Controller::start_worker()
{
	qDebug() << "Starting Background Process...";
	emit startP();
}

void Controller::stop_worker()
{
	qDebug() << "Stopping Background Process...";
	emit stopP();
}


void Controller::receive_update(const struct mouseSignal& input)
{	
	//qDebug() << "Mouse Position " << input.mouse.xCoo << input.mouse.yCoo;

	emit updated(input);
}


