#pragma once
#include <QApplication>
#include <QMainwindow>

#include <QSystemTrayIcon>
#include <QWidget>
#include <QGroupBox>
#include <QMenu>
#include <QSlider>
#include <QTranslator>




QT_BEGIN_NAMESPACE
class QAction;
class QHBoxLayout;
class QVBoxLayout;

class TrayIconControlled;
class MonitorTable;
class CustomSlider;
class MonitorWidget;
class SettingsDialog;
class ApplicationManager;
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(ApplicationManager* appMngr, QWidget* parent = nullptr);
	~MainWindow();


private:
	void setup();

	void createActions();
	void createMonitorGroupBox();
	void createTrayMenu();

	void closeEvent(QCloseEvent* event) override;
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void restore();

	void writeSettings();

	void openSettings();


private:
	ApplicationManager* appMngr;

	bool autoStart = false;
	QString config_path;
	Qt::WindowFlags flags = windowFlags();



private:
	// Widgets
	QWidget* mainWidget;
	QGroupBox* monitorGroupBox;
	MonitorTable* tableWidget;

	MonitorWidget* monitorSettings;
	SettingsDialog* applicationSettings;


	QMenu* trayIconMenu;
	QMenu* trayMonitorMenu;

	TrayIconControlled* tray;
	CustomSlider* slider;


	// Layouts
	QVBoxLayout* mainLayout;
	QHBoxLayout* sliderLayout;

	// Actions
	QAction* minimizeAction;
	QAction* maximizeAction;
	QAction* restoreAction;
	QAction* quitAction;
	QAction* openSettingsAction;



};
