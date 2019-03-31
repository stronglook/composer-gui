#include <QVBoxLayout>
#include <QFrame>

#include "Paginator.h"

namespace composer_gui
{
namespace paginator
{

Paginator::Paginator(QWidget *parent)
    : QDialog(parent),
      m_workspace(new QHBoxLayout),
      m_currentPageIndex(0),
      m_back(new QPushButton("&Back")),
      m_next(new QPushButton("&Next")),
      m_finish(new QPushButton("&Finish")),
      m_cancel(new QPushButton("&Cancel"))
{

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_back);
    buttonsLayout->addWidget(m_next);
    buttonsLayout->addWidget(m_finish);
    buttonsLayout->addWidget(m_cancel);

    QFrame *border = new QFrame;
    border->setFrameShape(QFrame::HLine);

    QVBoxLayout *buttonsBorderedLayout = new QVBoxLayout;
    buttonsBorderedLayout->addWidget(border);
    buttonsBorderedLayout->addLayout(buttonsLayout);
    buttonsBorderedLayout->setAlignment(Qt::AlignRight);

    QVBoxLayout *paginatorLayout = new QVBoxLayout;
    paginatorLayout->addLayout(m_workspace);
    paginatorLayout->addWidget(border);
    paginatorLayout->addLayout(buttonsBorderedLayout);

    setLayout(paginatorLayout);

    connect(m_finish, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancel, &QPushButton::clicked, this, &QDialog::reject);

    connect(m_next, &QPushButton::clicked, [this]
    {
        setPage(m_currentPageIndex + 1);
        ++m_currentPageIndex;
    });

    connect(m_back, &QPushButton::clicked, [this]
    {
        setPage(m_currentPageIndex - 1);
        --m_currentPageIndex;
    });

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void Paginator::addPage(QWidget *page)
{
    m_workspace->addWidget(page);
    page->hide();
}

void Paginator::block(bool flag)
{
    flag = !flag;
    m_next->setEnabled(flag);
    m_finish->setEnabled(flag);
}

void Paginator::setPage(int num)
{
    QWidget *currentPage = m_workspace->itemAt(m_currentPageIndex)->widget();
    currentPage->hide();

    QWidget *newPage = m_workspace->itemAt(num)->widget();
    newPage->show();

    int lastPageIndex = (m_workspace->count() - 1);
    m_back->setVisible(num != 0);
    m_next->setVisible(num < lastPageIndex);
    m_finish->setVisible(num == lastPageIndex);
}

int Paginator::exec()
{
    setPage(0);
    return QDialog::exec();
}

} // paginator
} // composer_gui
