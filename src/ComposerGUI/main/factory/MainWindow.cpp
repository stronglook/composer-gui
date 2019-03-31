#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QPersistentModelIndex>

#include "MainWindow.h"
#include "globals.h"
#include "main/factory/MenuBar.h"
#include "console/factory/Factory.h"
#include "composer/package/overview/Overview.h"

namespace composer_gui
{
namespace main
{
namespace factory
{

std::unique_ptr<Window> MainWindow::create()
{
    using namespace composer::project::explorer;
    using namespace composer::package::overview;
    using namespace console::command;

    QTabWidget *consoleTabs = createConsoleeTabs();
    Explorer *explorer = createExplorer();
    Overview *overview = new composer::package::overview::Overview;

    QObject::connect(explorer, &Explorer::empty, [overview]
    {
        overview->setState(Overview::State::STUB);
    });

    QObject::connect(explorer, &Explorer::install, [=](QPersistentModelIndex index)
    {        
        console::Console *console = addConsoleTab(consoleTabs);
        console->setWorkingDirectory(explorer->pathOf(index));

        invoker::Invoker *invoker = console->getCommandInvoker();

        QObject::connect(invoker->getCommand("composer"), &AbstractCommand::finished, console, [=]
        {
            if (index.isValid()) {
                explorer->removeProject(index);
            }

            if (!explorer->exists(console->getWorkingDirectory())) {
                explorer->addProjectFromPath(console->getWorkingDirectory());
            }

        });

        console->execute("composer install");
    });

    QObject::connect(explorer, &Explorer::update, [=](QPersistentModelIndex index)
    {
        console::Console *console = addConsoleTab(consoleTabs);
        console->setWorkingDirectory(explorer->pathOf(index));
        console->execute("composer update");
    });

    QObject::connect(explorer, &Explorer::clicked, [=]
    {
        QModelIndex parent = explorer->currentIndex().parent();
        QModelIndex projectIndex = parent.isValid() ? parent : explorer->currentIndex();

        QString projectPath = explorer->pathOf(projectIndex);

        if (QFileInfo::exists(projectPath)) {
            try {
                if (parent.isValid()){
                    QString childName = explorer->currentIndex().data().toString();
                    overview->dispatch(projectPath, childName.section('@', 0, 0));
                } else {
                    overview->dispatch(projectPath);
                }
            } catch (std::runtime_error &e) {
                QMessageBox::critical(overview, "Error", e.what());
            }
        } else {
            QMessageBox::critical(overview, "Error", projectPath + " does not exists");
            explorer->removeProject(explorer->currentIndex());

            if (explorer->count() == 0) {
                overview->setState(Overview::State::STUB);
            }
        }
    });

    console::Console *console = console::factory::Factory::create();
    consoleTabs->addTab(console, "Terminal #1");

    invoker::Invoker *invoker = console->getCommandInvoker();
    QObject::connect(invoker->getCommand("composer"), &AbstractCommand::finished, console, [=](const Command &cmd)
    {
        QStringList args = cmd.getArguments();
        if (args.count() > 0 && args[0] == "install") {
            if (!explorer->exists(console->getWorkingDirectory())) {
                explorer->addProjectFromPath(console->getWorkingDirectory());
            }
        }
    });

    std::unique_ptr<Window> mainWindow(new Window(explorer, overview, consoleTabs));
    mainWindow->setMenuBar(MenuBar::create(explorer));

    return mainWindow;
}

QTabWidget *MainWindow::createConsoleeTabs()
{
    QTabWidget *consoleTabs = new QTabWidget;

    QObject::connect(consoleTabs, &QTabWidget::tabCloseRequested, [consoleTabs](int index)
    {
        consoleTabs->widget(index)->deleteLater();
        consoleTabs->removeTab(index);
        consoleTabs->setTabsClosable((consoleTabs->count() > 1));

        for (int i = 0; i < consoleTabs->count(); i++)
        {
            consoleTabs->setTabText(i, "Terminal #" + QString::number(i + 1));
        }
    });

    return consoleTabs;
}

console::Console *MainWindow::addConsoleTab(QTabWidget *consoleTabs)
{
    console::Console *console = console::factory::Factory::create();

    consoleTabs->setTabsClosable(true);
    consoleTabs->addTab(console, "Terminal #" + QString::number(consoleTabs->count() + 1));
    consoleTabs->setCurrentIndex(consoleTabs->count() - 1);

    return console;
}

composer::project::explorer::Explorer *MainWindow::createExplorer()
{
    auto *explorer = new composer::project::explorer::Explorer;
    explorer->setStyleSheet(COMPOSERGUI_WIDGET_BACKGROUND);

    QStandardItemModel *itemModel = static_cast<QStandardItemModel *>(explorer->model());
    itemModel->setHeaderData(0, Qt::Horizontal, "Project explorer");

    return explorer;
}

}
}
}
