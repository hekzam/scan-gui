#ifndef BARREPROGRESSION_H
#define BARREPROGRESSION_H

#include <QProgressBar>
#include <QTableWidgetItem>

class ProgressCell : public QProgressBar, public QTableWidgetItem
{
    Q_OBJECT
public:
    ProgressCell(int value, QWidget *parent = nullptr);
    bool operator<(const QTableWidgetItem &other) const;

private:
    int m_value;
};

#endif // BARREPROGRESSION_H
