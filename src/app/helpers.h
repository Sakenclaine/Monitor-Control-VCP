#pragma once
#include <QWidget>
#include <QString>

#include <map>
#include <string>



QIcon createIconFromText(const QString& content, int iw, int ih, QColor color);

QIcon createIconFromText(const QString& content, QColor color);

QIcon createIconFromText(const QString& content, QColor color, QRect frame);


bool bracket_pair_finder(std::string s, std::vector<int>& indices);


std::vector<std::string> split(std::string s, std::string delimiter);


std::string charToString(unsigned char uChar);

std::string uIntToString(uint16_t u);

// https://www.lonecpluspluscoder.com/2015/08/13/an-elegant-way-to-extract-keys-from-a-c-map/
template<typename TK, typename TV>
std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
    std::vector<TK> retval;
    for (auto const& element : input_map) {
        retval.push_back(element.first);
    }
    return retval;
}

// Credit to AndreyS Scherbakov
// https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
template <typename I> QString n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
{
    static const char* digits = "0123456789ABCDEF";
    std::string str_(hex_len, '0');

    for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
    {
        str_[i] = digits[(w >> j) & 0x0f];
    }

    //qDebug() << "n2hexstr(): " << str_;

    return QString::fromStdString(str_);
}

void get_screen_geometry(int& xWO_taskbar, int& yWO_taskbar);