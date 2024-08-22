#include "MonitorTable.h"
#include "SignalLinker.h"

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
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

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

    //connect(this, &QTableWidget::itemSelectionChanged, this, &MonitorTable::monitor_selection_changed_);
    //connect(this, &MonitorTable::monitor_selection_changed, &Linker::getInstance(), &Linker::receive_monitor_selection);
    
    
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


void MonitorTable::monitor_selection_changed_()
{
    int id = currentRow();
    QString name = item(id, 1)->text();

    emit monitor_selection_changed(name, id);
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


