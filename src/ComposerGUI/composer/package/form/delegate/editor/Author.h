#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QModelIndex>
#include <QDataWidgetMapper>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace delegate
{
namespace editor
{

class Author : public QDialog
{
public:
    Author(QWidget *parent = nullptr);
    void setModelIndex(const QModelIndex &index);

private:
    QLineEdit *m_name;
    QLineEdit *m_role;
    QLineEdit *m_email;
    QLineEdit *m_homepage;
    QDataWidgetMapper *m_mapper;
};

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
