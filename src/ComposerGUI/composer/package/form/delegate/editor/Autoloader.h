#pragma once

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

class Autoloader : public QDialog
{
public:
    Autoloader(QWidget *parent = nullptr);
    void setModelIndex(const QModelIndex &index);

private:
    QComboBox *m_type;
    QLineEdit *m_path;
    QDataWidgetMapper *m_typeMapper;
    QDataWidgetMapper *m_pathMapper;
    bool m_isComplexPath;
};

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
