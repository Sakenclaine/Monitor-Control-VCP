#include "MonitorTable.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QHeaderView>

MonitorTable::MonitorTable(QWidget* parent, QStringList tableHeader) :
    QTableWidget(parent),
    tableHeader(tableHeader)
{
    if (tableHeader.size() < 2) { setColumnCount(2); }
    else { setColumnCount(tableHeader.size()); }
    
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setHorizontalHeaderLabels(tableHeader);
    verticalHeader()->setVisible(false);
    
    QSize qCheckBoxSize;
    QWidget checkBoxWidget = QWidget(); //create QWidget
    QCheckBox checkBox = QCheckBox();   //create QCheckBox
    QHBoxLayout layoutCheckBox; //create QHBoxLayout 
    layoutCheckBox.addWidget(&checkBox);     //add QCheckBox to layout
    layoutCheckBox.setAlignment(Qt::AlignCenter); //set Alignment layout
    layoutCheckBox.setContentsMargins(0, 0, 0, 0);

    qCheckBoxSize = checkBoxWidget.sizeHint();

    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    // https://forum.qt.io/topic/78350/qtableview-give-columnwidth-a-percentage-and-keep-percentage-when-table-gets-wider/3
    setColumnWidth(0, qCheckBoxSize.width());
    resizeColumnToContents(0);
}


void MonitorTable::add_monitor()
{
    insertRow(rowCount());
    setCellWidget(rowCount()-1, 0, create_checkbox());
}

void MonitorTable::add_monitor(Monitor* monitor)
{
    insertRow(rowCount());
    setCellWidget(rowCount() - 1, 0, create_checkbox());

    QTableWidgetItem* nameItem = new QTableWidgetItem(monitor->get_name());
    nameItem->setTextAlignment(Qt::AlignCenter);

    setItem(rowCount() - 1, 1, nameItem);

    checkBoxes.back()->setObjectName("cb_" + monitor->get_name() + "_" + QString::number(monitor->get_ID()));
    connect(checkBoxes.back(), &QCheckBox::checkStateChanged, monitor, &Monitor::set_status);
    connect(monitor, &Monitor::send_status, checkBoxes.back(), &QCheckBox::setChecked);


    resizeColumnsToContents();
}

QWidget* MonitorTable::create_checkbox()
{
    QWidget* checkBoxWidget = new QWidget(); //create QWidget
    QCheckBox* checkBox = new QCheckBox();   //create QCheckBox
    QHBoxLayout* layoutCheckBox = new QHBoxLayout(checkBoxWidget); //create QHBoxLayout 
    layoutCheckBox->addWidget(checkBox);     //add QCheckBox to layout
    layoutCheckBox->setAlignment(Qt::AlignCenter); //set Alignment layout
    layoutCheckBox->setContentsMargins(0, 0, 0, 0);

    checkBoxes.append(checkBox);

    return checkBoxWidget;
}






//MonitorTable::MonitorTable(QWidget* parent, std::vector<Monitor*> monitors) :
//    QWidget(parent)
//{
//    QStringList tableHeaders = { "", "Monitor" }; // , "Brightness", "Contrast", "R", "G", "B"
//    tableWidget = new QTableWidget(monitors.size(), tableHeaders.size(), this);
//
//    tableWidget->setHorizontalHeaderLabels(tableHeaders);
//    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    tableWidget->verticalHeader()->setVisible(false);
//    tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
//    tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
//
//    QSize qCheckBoxSize;
//
//    for (int row = 0; row < monitors.size(); row++)
//    {
//        QWidget* checkBoxWidget = new QWidget(); //create QWidget
//        QCheckBox* checkBox = new QCheckBox();   //create QCheckBox
//        QHBoxLayout* layoutCheckBox = new QHBoxLayout(checkBoxWidget); //create QHBoxLayout 
//        layoutCheckBox->addWidget(checkBox);     //add QCheckBox to layout
//        layoutCheckBox->setAlignment(Qt::AlignCenter); //set Alignment layout
//        layoutCheckBox->setContentsMargins(0, 0, 0, 0);
//
//        qCheckBoxSize = checkBoxWidget->sizeHint();
//
//        tableWidget->setCellWidget(row, 0, checkBoxWidget);
//
//        connect(checkBox, &QCheckBox::checkStateChanged, monitors[row], &Monitor::set_status);
//        connect(monitors[row], &Monitor::send_status, checkBox, &QCheckBox::setChecked);
//
//        for (int column = 1; column < tableHeaders.size(); column++)
//        {
//            QString cell_content;
//
//            if (column == 1)
//            {
//                cell_content = monitors[row]->get_name();
//
//                tableWidget->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
//            }
//
//            QTableWidgetItem* newItem = new QTableWidgetItem(cell_content);
//            newItem->setTextAlignment(Qt::AlignCenter);
//
//            tableWidget->setItem(row, column, newItem);
//        }
//    }
//
//    // https://forum.qt.io/topic/78350/qtableview-give-columnwidth-a-percentage-and-keep-percentage-when-table-gets-wider/3
//    tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
//    tableWidget->setColumnWidth(0, qCheckBoxSize.width());
//
//    tableWidget->resizeColumnsToContents();
//
//}