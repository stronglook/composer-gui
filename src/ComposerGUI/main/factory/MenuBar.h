#pragma once

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "composer/project/explorer/Explorer.h"

namespace composer_gui
{
namespace main
{
namespace factory
{

class MenuBar
{
public:
    static QMenuBar *create(composer::project::explorer::Explorer *explorer);

private:
    static QMenu *createProjectMenu(composer::project::explorer::Explorer *explorer);
    static QMenu *createReferenceMenu();
    static QAction *createSettingsAction();

    static QAction *createProjectCreateAction(composer::project::explorer::Explorer *explorer);
    static QAction *createProjectOpenAction(composer::project::explorer::Explorer *explorer);
};

} // factory
} // main
} // composer_gui
