#include "utilities.h"

#include <QTranslator>

QString windowsAppPath()
{
#ifdef Q_OS_WIN
    return QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
#else
    return "";
#endif
}


void setAppToStartAutomatically(bool startAutomatically)
{
#if defined ( Q_OS_WIN )

    QString key = "Monitor-Control";

    QSettings registrySettings(
        "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat
    );

    registrySettings.remove(key);

    if (startAutomatically) {
        registrySettings.setValue(key, QString("\"" + windowsAppPath() + "\""));
    }

    registrySettings.sync();

#endif
}


SettingsManager::SettingsManager(QObject* parent = nullptr) : QObject(parent)
{
    QSettings settings(confPath, QSettings::IniFormat);
}


SettingsManager::~SettingsManager()
{

}

SettingsManager& SettingsManager::getInstance()
{
    static SettingsManager cntr;
    return cntr;
}

void SettingsManager::writeSetting(const QString& key, const QVariant& variant)
{
    QSettings settings(confPath, QSettings::IniFormat);

    settings.setValue(key, variant);
}


void SettingsManager::writeSettingsGroup(const QString& groupName, const QMap<QString, QVariant>& keyMap)
{
    QSettings settings(confPath, QSettings::IniFormat);

    settings.beginGroup(groupName);

    for (auto [key, value] : keyMap.asKeyValueRange())
    {
        settings.setValue(key, value);
    }

    settings.endGroup();

}


void SettingsManager::writeSettingInGroup(const QString& group, const QString& key, const QList<QVariant>& variant)
{
    QSettings settings(confPath, QSettings::IniFormat);

    settings.beginGroup(group);

    settings.setValue(key, variant);

    settings.endGroup();

}

void SettingsManager::writeSettingInGroup(const QString& group, const QString& key, const QVariant& variant)
{
    QSettings settings(confPath, QSettings::IniFormat);

    settings.beginGroup(group);

    settings.setValue(key, variant);

    settings.endGroup();

}

QVariant SettingsManager::readSetting(const QString& key)
{
    QSettings settings(confPath, QSettings::IniFormat);

    return settings.value(key);
}

QVariant SettingsManager::readSetting(const QString& groupKey, const QString& key)
{
    QSettings settings(confPath, QSettings::IniFormat);

    settings.beginGroup(groupKey);
    
    QVariant setting = settings.value(key);

    settings.endGroup();

    return setting;
}


ApplicationManager::ApplicationManager(QObject* parent) :
    QObject(parent)
{

}


void ApplicationManager::install_translator(QTranslator* tlator)
{
    qApp->removeTranslator(translator);

    translator = tlator;

    qApp->installTranslator(translator);
}


