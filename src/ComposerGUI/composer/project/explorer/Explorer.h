#pragma once

#include <QTreeView>
#include <QJsonObject>
#include <QPersistentModelIndex>
#include <QSettings>
#include <QWidget>
#include <QPoint>
#include <QString>
#include <QHash>
#include <QMenu>

#include "composer/project/explorer/controls/ProjectExplorerContext.h"

namespace composer_gui
{
namespace composer
{
namespace project
{
namespace explorer
{

class Explorer : public QTreeView
{
    Q_OBJECT
public:
    Explorer(QWidget *parent = nullptr);
    void addProject(const QString &path, const QJsonObject &project);
    void addProjectFromPath(const QString &path);
    void removeProject(const QModelIndex &index);
    bool exists(const QString &path);
    int count() const;
    QString pathOf(const QPersistentModelIndex &index);
    ~Explorer();

private:
    QHash<QPersistentModelIndex, QString> m_projects;
    controls::ProjectExplorerContext m_contextMenu;
    QSettings m_settings;

    void insertIntoTree(const QString &fullname, const QJsonObject &requiredSection);

signals:
    void install(QPersistentModelIndex);
    void update(QPersistentModelIndex);
    void empty();
};

} // explorer
} // project
} // composer
} // composer_gui
