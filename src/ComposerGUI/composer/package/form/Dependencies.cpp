#include <QStringListModel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QtGlobal>

#include "Dependencies.h"
#include "globals.h"
#include "delegate/DependencyRow.h"
#include "controls/Standard.h"
#include "controls/Context.h"
#include "composer/package/Package.h"
#include "composer/package/layout/Search.h"
#include "composer/api/packagist/Packagist.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Dependencies::Dependencies(QWidget *parent)
    : QWidget(parent),
      m_settings(COMPOSERGUI_ORGANIZATION_NAME, COMPOSERGUI_SETTINGS_APPLICATION)
{
    QStringList headlines           = {"require", "require-dev", "conflict", "replace", "provide", "suggest"};
    QVBoxLayout *dependenciesLayout = new QVBoxLayout;

    for (int i = 0; i < headlines.count() ; i++)
    {
        QListView *listView = createListView(headlines[i]);

        auto delegate = new delegate::DependencyRow(headlines[i]);
        connect(delegate, &QListView::destroyed, delegate, &delegate::DependencyRow::deleteLater);
        listView->setItemDelegate(delegate);

        QLineEdit *searchInput = new QLineEdit;
        searchInput->setPlaceholderText("name");

        QFormLayout *searchInputLayout = new QFormLayout;
        searchInputLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
        searchInputLayout->addRow("Packagist search:", searchInput);

        QVBoxLayout *searchLayout = new QVBoxLayout;
        searchLayout->addLayout(searchInputLayout);

        layout::Search *searchResultsLayout = new layout::Search;
        searchLayout->addLayout(searchResultsLayout);

        api::packagist::Packagist *api = new api::packagist::Packagist(this);

        connect(api, QOverload<QJsonArray>::of(&api::packagist::Packagist::found), this, [=](QJsonArray results)
        {
            handleSearchResults(results, listView, searchResultsLayout);
        });

        connect(searchInput, &QLineEdit::textEdited, [=](const QString &text)
        {
            tryFind(api, text, searchResultsLayout);
        });

        QVBoxLayout *verticalGroupLayout = new QVBoxLayout;
        verticalGroupLayout->addLayout(searchLayout);
        verticalGroupLayout->addWidget(listView);

        QHBoxLayout *horizontalGroupLayout = new QHBoxLayout;
        horizontalGroupLayout->addLayout(verticalGroupLayout);
        horizontalGroupLayout->addWidget(new controls::Standard(listView, this));

        QGroupBox *dependenciesBox = new QGroupBox;
        dependenciesBox->setTitle(headlines[i]);
        dependenciesBox->setLayout(horizontalGroupLayout);

        dependenciesLayout->addWidget(dependenciesBox);

        connect(listView, &QListView::customContextMenuRequested, [listView](const QPoint &pos)
        {
            if (listView->currentIndex().isValid()) {
                controls::Context contextControls(listView);
                contextControls.show(pos);
            }
        });
    }

    setLayout(dependenciesLayout);
}

void Dependencies::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

void Dependencies::handleSearchResults(const QJsonArray &results, QListView *view, layout::Search *resultsLayout) const
{
    resultsLayout->clear(false);

    for (const QJsonValue &result : results)
    {
        package::Package *package = package::Package::fromJsonObject(result.toObject());
        package->setStyleSheet(COMPOSERGUI_PACKAGE_PACKGROUND);

        resultsLayout->addWidget(package);

        connect(package, &package::Package::added, [package, view]
        {
            auto model = view->model();
            int row = model->rowCount();
            model->insertRow(model->rowCount());
            QModelIndex index = model->index(row, 0);
            model->setData(index, package->getName());
            package->unhook();
        });
    }
}

void Dependencies::tryFind(api::packagist::Packagist *api, const QString &text, layout::Search *resultsLayout)
{
    bool ready = (text.size() > 2);
    resultsLayout->clear(ready);

    if (ready) {
        api->find(text, m_settings.value("packagist_search_limit").toInt());
    }
}

QListView *Dependencies::createListView(const QString &name)
{
    QListView *view = new QListView;

    view->setContextMenuPolicy(Qt::CustomContextMenu);
    view->setModel(new QStringListModel(view));
    view->setObjectName("dependencies_view");

    m_views[name] = view;

    return view;
}

QStringListModel *Dependencies::getMappingModel(const QString &name)
{
    return static_cast<QStringListModel *>(m_views[name]->model());
}

} // form
} // package
} // composer
} // composer_gui
