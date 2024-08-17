#pragma once
#include <QApplication>
#include <QMainwindow>
#include <QSystemTrayIcon>
#include <QTabWidget>

#include <vector>

#include "TrayIconControlled.h"
#include "CustomSlider.h"
#include "MonitorHandler.h"
#include "SettingsWidget.h"
#include "MonitorSettingsWidget.h"
#include "Dialogs.h"
#include "MonitorTable.h"


QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;
class QSettings;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();


protected:
	void closeEvent(QCloseEvent* event) override;

private:
	void setup();
	void init_monitors_WIN();
	void init_monitors_UNIX();

	void writeSettings();
	void readSettings();

	void createActions();
	void createMonitorGroupBox();
	void create_monitor_control_widget();


private:
	bool autoStart = false;
	QString config_path;

	Qt::WindowFlags flags = windowFlags();

	int screenSizeX, screenSizeY;

private:
	// Widgets
	QWidget* mainWidget;
	SettingsWidget* wSettings;
	MonitorWidget* wMonSet;
	MonitorTable* tableWidget;

	Dialog_AddSlider* dAddSlider;

	// Layouts
	QVBoxLayout* mainLayout;
	QHBoxLayout* sliderLayout;

	// Actions
	QAction* minimizeAction;
	QAction* maximizeAction;
	QAction* restoreAction;
	QAction* quitAction;


	QGroupBox* monitorGroupBox;

	QMenu* trayIconMenu;
	QMenu* trayMonitorMenu;


private slots:
	void restore();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void add_slider();

signals:
	void emit_add_slider(uint16_t&, QColor&, bool&);
};
