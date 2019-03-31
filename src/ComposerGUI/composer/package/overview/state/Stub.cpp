#include <QLabel>
#include <QHBoxLayout>

#include "Stub.h"
#include "globals.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace overview
{
namespace state
{

Stub::Stub(QWidget *parent) : QWidget(parent)
{
    QLabel *placeholder = new QLabel("No package selected");
    placeholder->setAlignment(Qt::AlignCenter);

    QHBoxLayout *stubLayout = new QHBoxLayout;
    stubLayout->addWidget(placeholder);

    setStyleSheet(COMPOSERGUI_WIDGET_BACKGROUND);
    setLayout(stubLayout);
}

} // state
} // overview
} // package
} // composer
} // composer_gui
