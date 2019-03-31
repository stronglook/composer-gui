#pragma once

#include <memory>

#include <QTabWidget>

#include "main/Window.h"
#include "console/Console.h"
#include "composer/project/explorer/Explorer.h"

namespace composer_gui
{
namespace main
{
namespace factory
{

class MainWindow
{
public:
    static std::unique_ptr<Window> create();

private:
    static QTabWidget *createConsoleeTabs();
    static console::Console *addConsoleTab(QTabWidget *consoleTabs);
    static composer::project::explorer::Explorer *createExplorer();
};

} // factory
} // main
} // composer_gui
