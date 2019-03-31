#pragma once

#include <QObject>
#include <QPersistentModelIndex>
#include <QAbstractItemView>
#include <QMenu>
#include <QAction>
#include <QPoint>

namespace composer_gui
{
namespace composer
{
namespace project
{
namespace explorer
{

class Explorer;

namespace controls
{

class ProjectExplorerContext : public QObject
{
    Q_OBJECT
public:
    ProjectExplorerContext(Explorer *explorer, QObject *parent = nullptr);
    void show(const QPoint &pos);

protected:
    void edit();
    void remove();
    void openContainingFolder();

private:
    QMenu m_menu;
    QAction *m_installAction;
    QAction *m_updateAction;
    Explorer *m_explorer;

signals:
    void install(QPersistentModelIndex);
    void update(QPersistentModelIndex);
};

} // controls
} // explorer
} // project
} // composer
} // composer_gui
