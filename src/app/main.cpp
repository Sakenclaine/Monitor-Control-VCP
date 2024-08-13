#include <QApplication>
#include <QMessageBox>

#include "mainwindow.h"
#include "utilities.h"


int main(int argc, char* argv[])
{
	// Setup main application 
    QApplication a(argc, argv);


 	
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        auto choice = QMessageBox::critical(
            nullptr, 
            QObject::tr("Systray"),
            QObject::tr("I couldn't detect any system tray on this system."),
            QMessageBox::Close | QMessageBox::Ignore
        );

        if (choice == QMessageBox::Close)
            return 1;
        // Otherwise "lurk": if a system tray is started later, the icon will appear.
    }
    
    // Prevent program from closing, when closing main window without tray icon
    QApplication::setQuitOnLastWindowClosed(false);

    // Create main window and show it
	MainWindow w;
	w.show();

	return a.exec();
}



//void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
//{
//    QByteArray localMsg = msg.toLocal8Bit();
//    const char* file = context.file ? context.file : "";
//    const char* function = context.function ? context.function : "";
//
//    switch (type) {
//    case QtDebugMsg:        
//        //OutputDebugStringA(a);
//        OutputDebugStringA(qUtf8Printable(msg + "\n"));
//        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtInfoMsg:
//        OutputDebugStringA(qUtf8Printable(msg + "\n"));
//        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtWarningMsg:
//        OutputDebugStringA(qUtf8Printable(msg + "\n"));
//        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtCriticalMsg:
//        OutputDebugStringA(qUtf8Printable(msg + "\n"));
//        //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtFatalMsg:
//        OutputDebugStringA(qUtf8Printable(msg + "\n"));
//        //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    }
//}



//qInstallMessageHandler(myMessageOutput);


//   DISPLAY_DEVICE dd;
   //dd.cb = sizeof(DISPLAY_DEVICE);

   //DWORD deviceNum = 0;
   //while (EnumDisplayDevices(NULL, deviceNum, &dd, 0)) {
   //	DumpDevice(dd, 0);
   //	DISPLAY_DEVICE newdd = { 0 };
   //	newdd.cb = sizeof(DISPLAY_DEVICE);
   //	DWORD monitorNum = 0;
   //	while (EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
   //	{
   //		DumpDevice(newdd, 4);
   //		monitorNum++;
   //	}
   //	puts("");
   //	deviceNum++;
   //}