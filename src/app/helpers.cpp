#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QFont>

#include <QDebug>


#include <vector>
#include <Windows.h>
#include <type_traits>


QIcon createIconFromText(const QString& content, int iw, int ih, QColor color)
{
	QPixmap pixmap(iw, ih);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);

	QFont sansFont("Consolas", 45);

	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setFont(sansFont);

	painter.setPen(color);
	painter.drawText(QRect(0, 0, iw, ih), Qt::AlignCenter, content);

	return QIcon(pixmap);
}

QIcon createIconFromText(const QString& content, QColor color)
{
	int iwidth = 64, iheight = 64;
	
	QPixmap pixmap(iwidth, iheight);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);

	QFont sansFont("Consolas", 48);

	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setFont(sansFont);

	painter.setPen(color);
	painter.drawText(0, 0, iwidth, iheight, Qt::AlignCenter, content);

	return QIcon(pixmap);
}

QIcon createIconFromText(const QString& content, QColor color, QRect frame)
{
	int iwidth = 16, iheight = 16;

	QPixmap pixmap(iwidth, iheight);
	pixmap.fill(Qt::transparent);


	QPainter painter(&pixmap);
	QFont sansFont("Consolas", 13);

	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setFont(sansFont);
	painter.setPen(color);
	painter.drawText(0, 0, iwidth, iheight, Qt::AlignCenter, content);


	return QIcon(pixmap); //.scaledToWidth(sX)
}

bool bracket_pair_finder(std::string s, std::vector<int>& indices)
{
	// indices = vector containing the inices of paired brackets
	indices.clear();
	std::vector<int> oBs; // temporary vector with indices for open brackets

	for (size_t i = 0; i < s.size(); i++)
	{
		if (s[i] == '(') {
			oBs.push_back(i);
		}

		if (s[i] == ')' && not oBs.empty())
		{
			if (oBs.size() > 0)
			{
				indices.push_back(oBs.back());
				indices.push_back(i);

				oBs.pop_back();
			}
		}
	}

	if (!indices.empty()) { return true; }
	else { return false; }
}


std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}


	res.push_back(s.substr(pos_start));
	return res;
}


std::string charToString(unsigned char uChar)
{
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');

	oss << std::setw(2) << (unsigned int)uChar;
	std::string str_ = oss.str();

	return str_;
}

std::string uIntToString(uint16_t u)
{
	std::string str_ = std::format("{:#02}", u);
	return str_;
}


void get_screen_geometry(int& xWO_taskbar, int& yWO_taskbar)
{
	// size of screen (primary monitor) without taskbar or desktop toolbars
	xWO_taskbar = GetSystemMetrics(SM_CXFULLSCREEN);
	yWO_taskbar = GetSystemMetrics(SM_CYFULLSCREEN);

	//qDebug() << GetSystemMetrics(SM_CXFULLSCREEN) << " x " << GetSystemMetrics(SM_CYFULLSCREEN) << "\n";

	// size of screen (primary monitor) without just the taskbar
	RECT xy;
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
	//qDebug() << xy.right - xy.left << " x " << xy.bottom - xy.top << "\n";

	// the full width and height of the screen (primary monitor)
	//qDebug() << GetDeviceCaps(GetDC(NULL), HORZRES) << " x " << GetDeviceCaps(GetDC(NULL), VERTRES) << "\n";

}


