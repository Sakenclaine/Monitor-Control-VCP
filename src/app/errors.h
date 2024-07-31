#pragma once

#include <stdexcept>
#include <string>

#include <QMessageBox>


class NotImplemented : public std::logic_error
{
public:
    NotImplemented();
};


class FeatureWarning : public QMessageBox
{
    Q_OBJECT

public:
    FeatureWarning(uint16_t code);
};

