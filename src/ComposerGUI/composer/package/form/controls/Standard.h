#pragma once

#include <QWidget>
#include <QPushButton>
#include <QAbstractItemView>

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

class Standard : public QWidget
{
public:
    explicit Standard(QAbstractItemView *view, QWidget *parent = nullptr);

protected:
    virtual void add();
    virtual void edit();
    virtual void remove();

    QAbstractItemView *m_view;
};

} // controls
} // form
} // package
} // composer
} // composer_gui
