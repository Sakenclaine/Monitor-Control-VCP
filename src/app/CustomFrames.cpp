#include "CustomFrames.h"

#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include <QScrollBar>
#include <QComboBox>

ScrollFrame::ScrollFrame(QWidget* parent) : QFrame(parent)
{
	//setFrameStyle(QFrame::Panel);

	innerContent = new QWidget(this);

	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidgetResizable(true);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->verticalScrollBar()->setEnabled(false);
	scrollArea->setWidget(innerContent);

	scrollArea->resize(QSize(width() - rimX, height() - rimY));
	scrollArea->move(rimX, rimY);

	QFont btnFont = QFont("Arial", 15);
	btnFont.setBold(true);

	addButton = new QPushButton("+", this);
	addButton->setFont(btnFont);
	addButton->setStyleSheet(QString("QPushButton{background-color: rgb(150, 150, 150); padding: 0px; min-width: 1.25em; min-height: 0.5em; border-radius: 0.4em;} QPushButton::hover{background-color: rgb(128, 128, 128); padding: 0px; min-width: 1.5em; min-height: 1em; border-radius: 0.65em;}"));
	addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	addButton->move(0, 0);

	addButton->show();
}

void ScrollFrame::resizeEvent(QResizeEvent* evnt)
{
	scrollArea->resize(QSize(evnt->size().width() - rimX, evnt->size().height() - rimY));

	updateGeometry();
	innerContent->updateGeometry();
	scrollArea->updateGeometry();
}

void ScrollFrame::setLayout(QLayout* layout)
{
	layout->setContentsMargins(0, 4, 10, 0);
	innerContent->setLayout(layout);
}

QSize ScrollFrame::sizeHint() const
{
	return innerContent->sizeHint();
}

QSize ScrollFrame::minimumSizeHint() const
{
	return QSize(innerContent->minimumSizeHint().width(), innerContent->minimumSizeHint().height() + 20);
}

QPushButton* ScrollFrame::get_button()
{
	return addButton;
}



ComboBoxFrame::ComboBoxFrame(QWidget* parent) : QFrame(parent)
{
	//setFrameStyle(QFrame::Panel);

	innerContent = new QFrame(this);
	innerContent->setFrameStyle(QFrame::Panel);
	innerContent->resize(QSize(width() - rimX, height() - rimY));
	innerContent->move(rimX, rimY);
	innerContent->show();

	monitorBox = new QComboBox(this);
	monitorBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	monitorBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	monitorBox->move(0, 0);
	monitorBox->setWindowOpacity(0);

	monitorBox->show();

	connect(monitorBox, &QComboBox::currentIndexChanged, this, &ComboBoxFrame::comboBoxItemChanged_);
}

void ComboBoxFrame::resizeEvent(QResizeEvent* evnt)
{
	innerContent->resize(QSize(evnt->size().width() - rimX, evnt->size().height() - rimY));

	updateGeometry();
	innerContent->updateGeometry();
}

void ComboBoxFrame::setLayout(QLayout* layout)
{
	layout->setContentsMargins(0, 4, 10, 0);
	innerContent->setLayout(layout);
	monitorBox->adjustSize();
}

QSize ComboBoxFrame::sizeHint() const
{
	return innerContent->sizeHint();
}

QSize ComboBoxFrame::minimumSizeHint() const
{
	return QSize(innerContent->minimumSizeHint().width(), innerContent->minimumSizeHint().height() + 20);
}

void ComboBoxFrame::comboBoxItemChanged_(int itemID)
{
	//qDebug() << "Item Changed: " << itemID << " --> " << monitorBox->currentText();

	QString temp = monitorBox->currentText();
	emit comboBoxItemChanged(temp, itemID);
}

void ComboBoxFrame::addItem(QString key, QVariant value)
{
	monitorBox->addItem(key, value);
}