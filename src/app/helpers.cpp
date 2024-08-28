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

uint16_t hexstr2uint(std::string inpt)
{

	uint16_t ul = std::stoul("0x" + inpt, nullptr, 0);

	return ul;
}

uint16_t hexstr2uint(QString inpt)
{

	uint16_t ul = std::stoul("0x" + inpt.toStdString(), nullptr, 0);

	return ul;
}


void get_screen_geometry(int& xWO_TAB_TOB, int& yWO_TAB_TOB, int& xWO_TAB, int& yWO_TAB, int& x, int& y)
{
	// size of screen (primary monitor) without taskbar or desktop toolbars
	xWO_TAB_TOB = GetSystemMetrics(SM_CXFULLSCREEN);
	yWO_TAB_TOB = GetSystemMetrics(SM_CYFULLSCREEN);

	//qDebug() << GetSystemMetrics(SM_CXFULLSCREEN) << " x " << GetSystemMetrics(SM_CYFULLSCREEN) << "\n";

	// size of screen (primary monitor) without just the taskbar
	RECT xy;
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
	//qDebug() << xy.right - xy.left << " x " << xy.bottom - xy.top << "\n";
	xWO_TAB = xy.right - xy.left;
	yWO_TAB = xy.bottom - xy.top;

	// the full width and height of the screen (primary monitor)
	//qDebug() << GetDeviceCaps(GetDC(NULL), HORZRES) << " x " << GetDeviceCaps(GetDC(NULL), VERTRES) << "\n";
	x = GetDeviceCaps(GetDC(NULL), HORZRES);
	y = GetDeviceCaps(GetDC(NULL), VERTRES);
}





// Parse capability string sent from monitor
void parse_capability_string(std::string s, std::vector<std::string>& keywords, std::vector<std::string>& parsed_string, std::map<std::string, std::string>& vcp_dict)
{
	std::vector<int> inds; // vector containing the inices of paired brackets
	std::string vcps_with_options = "";
	bracket_pair_finder(s, inds);

	// Iterate through index positions of open brackets
	for (size_t i = 0; i < inds.size() - 2; i += 2)
	{
		// Get substrings of capability string corresponding to the content inbetween a pair of brackets
		std::string temp_value = s.substr(inds[i] + 1, (inds[i + 1] - inds[i]) - 1);
		parsed_string.push_back(temp_value);

		// Get term before the brackets
		std::string temp_string("");

		// Iterate from current index inds[i] - 1 (one position before an open bracket in the string) backwards through the string
		// until another bracket or a space appears in the string
		for (size_t k = inds[i] - 1; k >= 0; k--)
		{
			if (s[k] == ')' || s[k] == '(' || s[k] == ' ') { break; }
			temp_string += s[k];
		}

		std::reverse(temp_string.begin(), temp_string.end());
		keywords.push_back(temp_string);

		vcp_dict[temp_string] = temp_value;


		if (std::all_of(temp_string.begin(), temp_string.end(), ::isxdigit)) {
			//qDebug() << temp_string << " contains only hexadecimal digits";
			vcps_with_options += temp_string + " ";
		}
	}

	// Get only vcp codes
	std::string vcps = vcp_dict["vcp"];
	std::vector<int> inds_2;
	bool found_brackets = bracket_pair_finder(vcps, inds_2);
	int stop = 0;

	while (found_brackets)
	{
		vcps = vcps.erase(inds_2[0], (inds_2[1] - inds_2[0]) + 1);
		found_brackets = bracket_pair_finder(vcps, inds_2);

		stop++;

		if (stop > 1000) { break; }
	}

	vcp_dict["vcp_only"] = vcps;
	vcp_dict["vcps_with_options"] = vcps_with_options;
}