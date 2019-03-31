#include <QDesktopServices>
#include <QMessageBox>
#include <QFileInfo>
#include <QUrl>
#include <QDir>

#include "ProjectExplorerContext.h"
#include "globals.h"
#include "paginator/Paginator.h"
#include "composer/package/form/Form.h"
#include "composer/package/form/visitor/DataProviderVisitor.h"
#include "composer/package/form/visitor/DataExtractVisitor.h"
#include "composer/project/explorer/Explorer.h"

namespace composer_gui
{
namespace composer
{
namespace project
{
namespace explorer
{
namespace controls
{

ProjectExplorerContext::ProjectExplorerContext(Explorer *explorer, QObject *parent)
    : QObject(parent),
      m_explorer(explorer)
{
    m_menu.addAction("Open containing folder", [this]
    {
        openContainingFolder();
    });

    m_menu.addSeparator();

    m_installAction = m_menu.addAction("Install", [this]
    {
        emit install(m_explorer->currentIndex());
    });

    m_updateAction = m_menu.addAction("Update", [this]
    {
        emit update(m_explorer->currentIndex());
    });

    m_menu.addSeparator();

    m_menu.addAction("Edit", [this]
    {
       edit();
    });

    m_menu.addSeparator();

    m_menu.addAction("Remove", [this]
    {
       remove();
    });


}

void ProjectExplorerContext::edit()
{
    using namespace composer::package::form::visitor;

    auto form = new composer::package::form::Form;

    try {
        QString filePath = m_explorer->pathOf(m_explorer->currentIndex()) + QDir::separator() + "composer.json";

        DataProviderVisitor dataProviderVisitor(filePath);
        form->accept(dataProviderVisitor);

        paginator::Paginator paginator;
        paginator.setWindowTitle("Edit " + m_explorer->currentIndex().data().toString());
        paginator.setStyleSheet(COMPOSERGUI_WIDGET_STYLESHEET);
        paginator.addPage(form);
        paginator.setMinimumSize(687, 500);

        QObject::connect(&paginator, &paginator::Paginator::finished, [=, &dataProviderVisitor]
        {
            DataExtractVisitor dataExtractVisitor(dataProviderVisitor.getPackageJsonObject());
            form->accept(dataExtractVisitor);

            try {
                QJsonObject dumped = dataExtractVisitor.dump(filePath);
                m_explorer->removeProject(m_explorer->currentIndex());
                m_explorer->addProject(filePath, dumped);
            } catch (std::runtime_error &e) {
                QString msg = e.what() + QString(" does not exists");
                QMessageBox::critical(m_explorer, "Error", msg);
            }
        });

        paginator.exec();
    } catch (std::runtime_error &e) {
        QMessageBox::critical(m_explorer, "Error", e.what());
    }
}

void ProjectExplorerContext::remove()
{
    QString fullname = m_explorer->currentIndex().data().toString();
    QString question = "Do you want remove " + fullname + '?';
    if (QMessageBox::question(m_explorer, "Confirm", question) == QMessageBox::StandardButton::Yes) {
        m_explorer->removeProject(m_explorer->currentIndex());
    }
}

void ProjectExplorerContext::openContainingFolder()
{
    QString path = m_explorer->pathOf(m_explorer->currentIndex());

    if (QFileInfo::exists(path)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    } else {
        QString msg = path + QDir::separator() + "composer.json does not exists";
        QMessageBox::information(m_explorer, "Information", msg);
    }
}

void ProjectExplorerContext::show(const QPoint &pos)
{
    bool locked = QFileInfo::exists(m_explorer->pathOf(m_explorer->currentIndex()) + QDir::separator() + "composer.lock");

    m_installAction->setVisible(!locked);
    m_updateAction->setVisible(locked);

    m_menu.exec(m_explorer->mapToGlobal(pos));
}

} // controls
} // explorer
} // project
} // composer
} // composer_gui
