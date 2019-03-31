#pragma once

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

namespace composer_gui
{
namespace paginator
{

class Paginator : public QDialog
{
    Q_OBJECT
public:
    Paginator(QWidget *parent = nullptr);
    void addPage(QWidget *page);
    void block(bool flag);

private:
    QHBoxLayout *m_workspace;
    int m_currentPageIndex;

    QPushButton *m_back;
    QPushButton *m_next;
    QPushButton *m_finish;
    QPushButton *m_cancel;

    void setPage(int num);

    // QDialog interface
public slots:
    int exec();
};

} // paginator
} // composer_gui
