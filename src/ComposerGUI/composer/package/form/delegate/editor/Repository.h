#pragma once

#include <QWidget>
#include <QDialog>
#include <QComboBox>
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

class Repository : public QDialog
{
public:
    Repository(QWidget *parent = nullptr);
    void setModelIndex(const QModelIndex &index);

private:
    QComboBox *m_type;
    QLineEdit *m_url;
    QDataWidgetMapper *m_typeMapper;
    QDataWidgetMapper *m_urlMapper;
};

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
