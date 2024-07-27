#pragma once
#include <QApplication>
#include <QMainwindow>
#include <QSystemTrayIcon>

#include <vector>

#include "TrayIconControlled.h"
#include "CustomSlider.h"
#include "MonitorHandler.h"


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
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void add_slider(QColor color, QString name, uint16_t code);

protected:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void writeSettings();
	void readSettings();

    void createTrayIcon();
	void createActions();

	void createMonitorGroupBox();
	void createPositionGroupBox();

	void updatePosLabel(const struct inSignal& input);

	bool autoStart = false;

	std::vector<TrayIconControlled*> trayIcons;
	std::vector<CustomSlider*> sliders;
	std::vector<Monitor*> registered_monitors;
	//std::vector<bool> chosen_monitors;

	QGroupBox* monitorGroupBox;
	QGroupBox* posGroupBox;


	QLabel* trayPos;
	QLabel* mousePosX;
	QLabel* mousePosY;


	QAction* minimizeAction;
	QAction* maximizeAction;
	QAction* restoreAction;
	QAction* quitAction;


	QMenu* trayIconMenu;
	QMenu* trayMonitorMenu;

	// Layouts
	QVBoxLayout* mainLayout;
	QHBoxLayout* sliderLayout;
};
