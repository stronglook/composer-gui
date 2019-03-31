#include "Search.h"
#include "composer/package/Package.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace layout
{

Search::Search()
{
    m_searchLabel = new QLabel("Search...");
    m_searchLabel->hide();
    addWidget(m_searchLabel);
}

void Search::clear(bool searchStarted)
{
    searchStarted ? m_searchLabel->show() : m_searchLabel->hide();

    QLayoutItem *child;
    while ((child = takeAt(1)) != 0)
    {
        static_cast<package::Package *>(child->widget())->unhook();
    }
}

} // layout
} // package
} // composer
} // composer_gui
