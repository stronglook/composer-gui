#include <QScrollArea>

#include "Form.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Form::Form(QWidget *parent)
    : QTabWidget(parent),
      m_general(new General),
      m_dependencies(new Dependencies),
      m_autoload(new Autoload),
      m_repositories(new Repositories),
      m_scripts(new Scripts),
      m_config(new Config),
      m_authors(new Authors),
      m_support(new Support)
{
    addTab(m_general, "General");
    addTab(createScrollForWidget(m_dependencies), "Dependencies");
    addTab(m_autoload, "Autoload");
    addTab(m_repositories, "Repositories");
    addTab(m_scripts, "Scripts");
    addTab(createScrollForWidget(m_config), "Config");
    addTab(m_authors, "Authors");
    addTab(m_support, "Support");
}

void Form::accept(visitor::VisitorInterface &visitor)
{
    m_general->accept(visitor);
    m_dependencies->accept(visitor);
    m_autoload->accept(visitor);
    m_repositories->accept(visitor);
    m_scripts->accept(visitor);
    m_config->accept(visitor);
    m_authors->accept(visitor);
    m_support->accept(visitor);
}

QScrollArea *Form::createScrollForWidget(QWidget *widget)
{
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidget(widget);
    scroll->setWidgetResizable(true);

    return scroll;
}

} // form
} // package
} // composer
} // composer_gui
