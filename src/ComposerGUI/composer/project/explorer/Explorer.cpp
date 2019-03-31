#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

#include "Explorer.h"
#include "globals.h"
#include "composer/project/explorer/controls/ProjectExplorerContext.h"
#include "paginator/Paginator.h"
#include "composer/package/form/Form.h"
#include "composer/package/form/visitor/DataProviderVisitor.h"
#include "composer/package/form/visitor/DataExtractVisitor.h"

namespace composer_gui
{
namespace composer
{
namespace project
{
namespace explorer
{

Explorer::Explorer(QWidget *parent) :
    QTreeView(parent),
    m_contextMenu(this),
    m_settings(COMPOSERGUI_ORGANIZATION_NAME, COMPOSERGUI_SETTINGS_PROJECT)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setModel(new QStandardItemModel(0, 1, this));

    for (const QString &key : m_settings.allKeys())
    {
        QString path = m_settings.value(key).toString();

        if (QFileInfo::exists(path)) {
            QFile file(path);
            if (file.open(QFile::ReadOnly | QFile::Text)) {
                QJsonDocument document = QJsonDocument::fromJson(file.readAll());
                if (document.isNull()) {
                    continue;
                }

                addProject(path, document.object());
            }
        }
    }

    connect(this, &Explorer::customContextMenuRequested, [this](const QPoint &pos)
    {
        if (currentIndex().isValid() && !(currentIndex().parent().isValid())) {
            m_contextMenu.show(pos);
        }
    });

    connect(&m_contextMenu, &controls::ProjectExplorerContext::install, this, &Explorer::install);
    connect(&m_contextMenu, &controls::ProjectExplorerContext::update, this, &Explorer::update);
}

void Explorer::addProjectFromPath(const QString &path)
{
    QString filePath = path + QDir::separator() + "composer.json";

    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        if (!document.isNull()) {
            addProject(filePath, document.object());
        }
    }
}

void Explorer::addProject(const QString &path, const QJsonObject &project)
{
    QString fullname = project.value("name").toString();
    QString version = project.value("version").toString();
    if (!version.isEmpty()) {
        fullname += "@" + version;
    }

    QJsonObject required = project.value("require").toObject();

    insertIntoTree(fullname, required);
    m_projects[currentIndex()] = path;
}

bool Explorer::exists(const QString &path)
{
    QString needle = path + QDir::separator() + "composer.json";

    for (const QString &projectPath : m_projects)
    {
        if (projectPath == needle) {
            return true;
        }
    }

    return false;
}

int Explorer::count() const
{
    return m_projects.count();
}

void Explorer::removeProject(const QModelIndex &index)
{
    auto it = m_projects.find(index);
    m_projects.erase(it);

    QAbstractItemModel *m = model();
    m->removeRow(currentIndex().row());

    if (m->rowCount() == 0) {
        emit empty();
    }
}

QString Explorer::pathOf(const QPersistentModelIndex &index)
{
    QFileInfo info(m_projects[index]);
    return QDir::toNativeSeparators(info.path());
}

Explorer::~Explorer()
{
    m_settings.clear();

    int index = 0;

    for (const QString &path : m_projects)
    {
        m_settings.setValue(QString::number(index++), path);
    }
}

void Explorer::insertIntoTree(const QString &fullname, const QJsonObject &requiredSection)
{
    QStandardItem *root = new QStandardItem(fullname);

    for (const QString &key : requiredSection.keys())
    {
        QString childFullname = key + '@' + requiredSection.value(key).toString();
        QStandardItem *child = new QStandardItem(childFullname);
        root->setChild(root->rowCount(), 0, child);
    }

    QStandardItemModel *itemModel = static_cast<QStandardItemModel *>(model());

    itemModel->setItem(itemModel->rowCount(), 0, root);
    setCurrentIndex(itemModel->index(itemModel->rowCount() - 1, 0));

    itemModel->sort(0);
}

} // explorer
} // project
} // composer
} // composer_gui
