#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QTabWidget>
#include <QWidget>

#include "composer/project/explorer/Explorer.h"
#include "composer/package/overview/Overview.h"

namespace composer_gui
{
namespace main
{

class Window : public QMainWindow
{
    using TExplorer = composer::project::explorer::Explorer;
    using TOverview = composer::package::overview::Overview;
public:
    explicit Window(TExplorer *explorer,
                    TOverview *overview,
                    QTabWidget *consoleTabs,
                    QWidget *parent = nullptr);

private:
    composer::project::explorer::Explorer *m_explorer;
    composer::package::overview::Overview *m_overview;
    QTabWidget *m_consoleTabs;
};

} // main
} // composer_gui
