#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include <QSettings>

QT_BEGIN_NAMESPACE
class QTranslator;
QT_END_NAMESPACE


// andreybutov https://gist.github.com/andreybutov/33783bca1af9db8f9f36c463c77d7a86
QString windowsAppPath();

void setAppToStartAutomatically(bool startAutomatically);


class SettingsManager : public QObject
{
	Q_OBJECT

private:
    explicit SettingsManager(QObject* parent);

    QString confPath = qApp->applicationDirPath() + "/config.ini";

public:
    ~SettingsManager();
    static SettingsManager& getInstance();

public:
    void writeSetting(const QString& key, const QVariant& variant);
    void writeSettingsGroup(const QString& groupName, const QMap<QString, QVariant>& keys);

    void writeSettingInGroup(const QString& group, const QString& key, const QList<QVariant>& variant);

    void writeSettingInGroup(const QString& group, const QString& key, const QVariant& variant);

    QVariant readSetting(const QString& key);
    QVariant readSetting(const QString& groupKey, const QString& key);

};

class ApplicationManager : public QObject
{
    Q_OBJECT

private:
    QTranslator* translator = nullptr;

public:
    ApplicationManager(QObject* parent = nullptr);


public:
    void install_translator(QTranslator* tlator);
    void load_install_translator(QString language);
    
};