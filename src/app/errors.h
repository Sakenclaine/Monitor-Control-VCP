#pragma once

#include <stdexcept>
#include <string>


class NotImplemented : public std::logic_error
{
public:
    NotImplemented();
};

