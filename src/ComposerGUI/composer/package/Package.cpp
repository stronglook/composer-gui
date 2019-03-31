#include <QPushButton>
#include <QDesktopServices>
#include <QPixmap>
#include <QUrl>
#include <QJsonArray>
#include <QStyleOption>
#include <QPainter>

#include "Package.h"
#include "globals.h"

namespace composer_gui
{
namespace composer
{
namespace package
{

Package::Package(QWidget *parent)
    : QWidget(parent),
      m_mapper(new QDataWidgetMapper(this))
{
    QVBoxLayout *packageLayout = new QVBoxLayout;
    packageLayout->addLayout(createInfoLineLayout());

    QFrame *separator = new QFrame;
    separator->setFrameStyle(QFrame::HLine);

    packageLayout->addWidget(separator);
    packageLayout->addLayout(createControlsLayout());

    setLayout(packageLayout);

    m_mapper->setModel(new QStandardItemModel(5, 1, this));
    m_mapper->setOrientation(Qt::Vertical);
    m_mapper->addMapping(m_name, 0, "text");
    m_mapper->addMapping(m_description, 1, "text");
    m_mapper->addMapping(m_versions, 2);
    m_mapper->addMapping(m_downloads, 3, "text");
    m_mapper->addMapping(m_rate, 4, "text");
    m_mapper->toFirst();
}

QStandardItemModel *Package::getMappingModel()
{
    return static_cast<QStandardItemModel *>(m_mapper->model());
}

QStringListModel *Package::getVersionsWidgetModel()
{
    return static_cast<QStringListModel *>(m_versions->model());
}

Package *Package::fromJsonObject(const QJsonObject &json)
{
    Package *package = new Package;

    QStringList versions = json.value("versions").toVariant().toStringList();
    QStringListModel *versionsModel = package->getVersionsWidgetModel();
    versionsModel->setStringList(versions);

    QStandardItemModel *mappingModel  = package->getMappingModel();
    mappingModel->setItem(Field::NAME, new QStandardItem(json.value("name").toString()));
    mappingModel->setItem(Field::DESCRIPTION, new QStandardItem(json.value("description").toString()));
    mappingModel->setItem(Field::VERSIONS, new QStandardItem(versions.last()));
    mappingModel->setItem(Field::DOWNLOADS, new QStandardItem(QString::number(json.value("downloads").toInt())));
    mappingModel->setItem(Field::FAVERS, new QStandardItem(QString::number(json.value("favers").toInt())));

    return package;
}

QString Package::getName()
{
    QStandardItemModel *mappingModel = getMappingModel();
    const QString name = mappingModel->item(0)->text() + '@' + mappingModel->item(2)->text();

    return name;
}

void Package::unhook()
{
    hide();
    deleteLater();
}

QHBoxLayout *Package::createStatisticsLayout()
{
    m_downloads = new QLabel;
    m_rate = new QLabel;

    QLabel *downloadsIcon = new QLabel;
    downloadsIcon->setPixmap(QPixmap(":/icons/down.png"));

    QLabel *rateIcon = new QLabel;
    rateIcon->setPixmap(QPixmap(":/icons/rate.png"));

    QHBoxLayout *statisticsLayout = new QHBoxLayout;
    statisticsLayout->addWidget(downloadsIcon);
    statisticsLayout->addWidget(m_downloads);
    statisticsLayout->addWidget(rateIcon);
    statisticsLayout->addWidget(m_rate);

    return statisticsLayout;
}

QVBoxLayout *Package::createInfoLineLayout()
{
    m_name = new QLabel;
    m_description = new QLabel;
    m_description->setStyleSheet(COMPOSERGUI_PACKAGE_DESCRIPTION_STYLESHEET);
    m_description->setWordWrap(true);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->setAlignment(Qt::AlignLeft);
    nameLayout->addWidget(m_name);

    QHBoxLayout *infoLayout = new QHBoxLayout;
    infoLayout->addLayout(nameLayout);

    QHBoxLayout *statisticsLayout = createStatisticsLayout();
    statisticsLayout->setAlignment(Qt::AlignRight);
    infoLayout->addLayout(statisticsLayout);

    QVBoxLayout *infoLineLayout = new QVBoxLayout;
    infoLineLayout->addLayout(infoLayout);
    infoLineLayout->addWidget(m_description);

    return infoLineLayout;
}

QHBoxLayout *Package::createControlsLayout()
{
    QStringListModel *versionsModel = new QStringListModel(this);
    m_versions = new QComboBox;
    m_versions->setModel(versionsModel);
    m_versions->setMinimumWidth(150);
    m_versions->setMaximumWidth(150);

    QPushButton *detailsButton = new QPushButton("Details");
    QPushButton *addButton = new QPushButton("Add");
    connect(addButton, &QPushButton::clicked, [this]
    {
       emit added(this);
    });

    QHBoxLayout *leftControlsLayout = new QHBoxLayout;
    leftControlsLayout->addWidget(m_versions);
    leftControlsLayout->setAlignment(Qt::AlignLeft);

    QHBoxLayout *rightControlsLayout = new QHBoxLayout;
    rightControlsLayout->addWidget(detailsButton);
    rightControlsLayout->addWidget(addButton);
    rightControlsLayout->setAlignment(Qt::AlignRight);

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addLayout(leftControlsLayout);
    controlsLayout->addLayout(rightControlsLayout);

    connect(detailsButton, &QPushButton::clicked, [this]
    {
       QDesktopServices::openUrl(QUrl("https://packagist.org/packages/" + m_name->text()));
    });

    return controlsLayout;
}

void Package::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opts;
    opts.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opts, &painter, this);
}

} // package
} // composer
} // composer_gui
