#include "progresscell.h"

ProgressCell::ProgressCell(int value, QWidget *parent) : QProgressBar(parent),QTableWidgetItem(),m_value(value) {
    setValue(value);
}

bool ProgressCell::operator<(const QTableWidgetItem &other) const{
    const ProgressCell *compareProg = dynamic_cast<const ProgressCell *>(&other);
    if(compareProg)
        return m_value < compareProg->m_value;
    return false;
}
