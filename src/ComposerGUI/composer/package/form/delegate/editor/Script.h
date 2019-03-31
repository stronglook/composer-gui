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

class Script : public QDialog
{
public:
    Script(QWidget *parent = nullptr);
    void setModelIndex(const QModelIndex &index);

private:
    QComboBox *m_event;
    QLineEdit *m_handler;
    QDataWidgetMapper *m_eventMapper;
    QDataWidgetMapper *m_handlerMapper;
};

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
