#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QModelIndex>

#include "Standard.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace controls
{

Standard::Standard(QAbstractItemView *view, QWidget *parent)
    : QWidget(parent),
      m_view(view)
{
    QPushButton *addBtn    = new QPushButton("Add");
    QPushButton *editBtn   = new QPushButton("Edit");
    QPushButton *removeBtn = new QPushButton("Remove");

    QVBoxLayout *controlsLayout = new QVBoxLayout;
    controlsLayout->addWidget(new QLabel("Controls:"));
    controlsLayout->addWidget(addBtn);
    controlsLayout->addWidget(editBtn);
    controlsLayout->addWidget(removeBtn);
    controlsLayout->setAlignment(Qt::AlignTop);

    QFrame *separator = new QFrame;
    separator->setFrameShape(QFrame::VLine);

    QHBoxLayout *controlsLayoutWithSeparator = new QHBoxLayout;
    controlsLayoutWithSeparator->addWidget(separator);
    controlsLayoutWithSeparator->addLayout(controlsLayout);
    controlsLayoutWithSeparator->setMargin(0);

    setLayout(controlsLayoutWithSeparator);

    connect(addBtn, &QPushButton::clicked, [this]
    {
        add();
    });

    connect(editBtn, &QPushButton::clicked, [this]
    {
        edit();
    });

    connect(removeBtn, &QPushButton::clicked, [this]
    {
        remove();
    });

    connect(m_view, &QAbstractItemView::doubleClicked, [this](const QModelIndex &index)
    {
        m_view->edit(index);
    });
}

void Standard::add()
{
    auto model = m_view->model();
    int row = model->rowCount();

    model->insertRow(model->rowCount());

    QModelIndex index = model->index(row, 0);

    m_view->setCurrentIndex(index);
    m_view->edit(index);
}

void Standard::edit()
{
    const QModelIndex &index = m_view->currentIndex();
    if (index.isValid()) {
        m_view->edit(index);
    }
}

void Standard::remove()
{
    const QModelIndex &index = m_view->currentIndex();
    if (!index.isValid()) {
        return;
    }

    QString question = "Do you want remove " + index.sibling(index.row(), 0).data().toString() + '?';
    if (QMessageBox::question(m_view, "Confirm", question) == QMessageBox::StandardButton::Yes) {
        m_view->model()->removeRow(index.row());
    }
}

} // controls
} // form
} // package
} // composer
} // composer_gui
