#pragma once

#include <QWidget>
#include <QJsonArray>
#include <QString>
#include <QStringListModel>
#include <QListView>
#include <QHash>
#include <QSettings>

#include "composer/package/form/visitor/VisitorInterface.h"
#include "composer/package/form/visitor/AcceptorInterface.h"
#include "composer/api/packagist/Packagist.h"
#include "composer/package/layout/Search.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

class Dependencies : public QWidget, public visitor::AcceptorInterface
{
    Q_OBJECT
public:
    explicit Dependencies(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStringListModel *getMappingModel(const QString &name);
    void handleSearchResults(const QJsonArray &results, QListView *view, layout::Search *resultsLayout) const;
    void tryFind(api::packagist::Packagist *api, const QString &text, layout::Search *resultsLayout);

private:
    QSettings m_settings;
    QHash<QString, QListView *> m_views;

    QListView *createListView(const QString &name);
};

} // form
} // package
} // composer
} // composer_gui
