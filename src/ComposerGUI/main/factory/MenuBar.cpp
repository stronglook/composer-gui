#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>
#include <QDir>

#include "MenuBar.h"
#include "globals.h"
#include "paginator/Paginator.h"
#include "composer/package/form/Path.h"
#include "composer/package/form/Form.h"
#include "settings/Settings.h"
#include "composer/package/form/visitor/DataExtractVisitor.h"
#include "composer/package/form/visitor/DataProviderVisitor.h"

namespace composer_gui
{
namespace main
{
namespace factory
{

QMenuBar *MenuBar::create(composer::project::explorer::Explorer *explorer)
{
    QMenuBar *menuBar = new QMenuBar;
    menuBar->setStyleSheet(COMPOSERGUI_WIDGET_BACKGROUND);

    menuBar->addMenu(createProjectMenu(explorer));
    menuBar->addMenu(createReferenceMenu());
    menuBar->addAction(createSettingsAction());

    return menuBar;
}

QMenu *MenuBar::createProjectMenu(composer::project::explorer::Explorer *explorer)
{
    QMenu *project = new QMenu("&Project");

    project->addAction(createProjectCreateAction(explorer));
    project->addSeparator();
    project->addAction(createProjectOpenAction(explorer));

    return project;
}

QAction *MenuBar::createSettingsAction()
{
    QAction *settings = new QAction("&Settings");

    QObject::connect(settings, &QAction::triggered, []
    {
        using namespace composer_gui::paginator;

        Paginator paginator;
        paginator.setWindowTitle("Application settings");
        paginator.setMinimumSize(600, 300);
        paginator.setStyleSheet(COMPOSERGUI_WIDGET_STYLESHEET);

        auto settings = new composer_gui::settings::Settings;
        paginator.addPage(settings);

        QObject::connect(&paginator, &Paginator::accepted, [settings]
        {
            settings->apply();
        });

        paginator.exec();
    });

    return settings;
}

QMenu *MenuBar::createReferenceMenu()
{
    QMenu *reference = new QMenu("&Reference");

    reference->addAction("Composer Homepage", []
    {
        QDesktopServices::openUrl(QUrl("https://getcomposer.org"));
    });

    reference->addSeparator();

    reference->addAction("Composer.json schema", []
    {
        QDesktopServices::openUrl(QUrl("https://getcomposer.org/doc/04-schema.md"));
    });

    reference->addSeparator();

    reference->addAction("Packagist", []
    {
        QDesktopServices::openUrl(QUrl("https://packagist.org/"));
    });

    return reference;
}

QAction *MenuBar::createProjectCreateAction(composer::project::explorer::Explorer *explorer)
{
    QAction *action = new QAction("Create");

    QObject::connect(action, &QAction::triggered, [explorer]
    {
        using namespace composer::package::form;
        using namespace composer_gui::paginator;

        Path *pathPage = new Path;
        Form *jsonDataPage = new Form;

        Paginator paginator;
        paginator.setWindowTitle("Composer project installer");
        paginator.setMinimumSize(687, 500);
        paginator.setStyleSheet(COMPOSERGUI_WIDGET_STYLESHEET);
        paginator.addPage(pathPage);
        paginator.addPage(jsonDataPage);
        paginator.block(true);

        QObject::connect(pathPage, &Path::pathChanged,[&paginator](bool isValid)
        {
            paginator.block(!isValid);
        });

        QObject::connect(&paginator, &Paginator::accepted, [=]
        {
            composer::package::form::visitor::DataExtractVisitor visitor;
            jsonDataPage->accept(visitor);

            QString installPath = pathPage->getSelectedPath() + QDir::separator() + "composer.json";

            try {
                QJsonObject &dumped = visitor.dump(installPath);
                explorer->addProject(installPath, dumped);
            } catch (std::runtime_error &e) {
                QString msg = e.what() + QString(" does not exists");
                QMessageBox::critical(explorer, "Error", msg);
            }
        });

        paginator.exec();
    });

    return action;
}

QAction *MenuBar::createProjectOpenAction(composer::project::explorer::Explorer *explorer)
{
    using namespace composer::package::form::visitor;
    using namespace composer_gui::paginator;

    QAction *action = new QAction("Open...");

    QObject::connect(action, &QAction::triggered, [explorer]
    {
        auto form = new composer::package::form::Form;

        try {
            QString filePath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(explorer, "Select project file"));
            if (filePath.isEmpty()) {
                return;
            }

            QString projectPath = filePath + QDir::separator() + "composer.json";

            if (explorer->exists(filePath)) {
                QString msg = "Project " + projectPath + " already exists";
                QMessageBox::critical(explorer, "Error", msg);
                return;
            }

            DataProviderVisitor dataProviderVisitor(projectPath);
            form->accept(dataProviderVisitor);

            Paginator paginator;
            paginator.setWindowTitle(projectPath);
            paginator.setStyleSheet(COMPOSERGUI_WIDGET_STYLESHEET);
            paginator.addPage(form);
            paginator.setMinimumSize(687, 500);

            QObject::connect(&paginator, &Paginator::accepted, [=, &dataProviderVisitor]
            {
                DataExtractVisitor dataExtractVisitor(dataProviderVisitor.getPackageJsonObject());
                form->accept(dataExtractVisitor);

                try {
                    QJsonObject dumped = dataExtractVisitor.dump(projectPath);
                    explorer->addProject(projectPath, dumped);
                } catch (std::runtime_error &e) {
                    QString msg = e.what() + QString(" does not exists");
                    QMessageBox::critical(explorer, "Error", msg);
                }
            });

            paginator.exec();
        } catch (std::runtime_error &e) {
            QMessageBox::critical(explorer, "Error", e.what());
        }
    });

    return action;
}

} // factory
} // main
} // composer_gui
