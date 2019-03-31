#include <stdexcept>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

#include "Overview.h"
#include "composer/api/packagist/Packagist.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace overview
{

Overview::Overview(QWidget *parent) :
    QWidget(parent),
    m_stateStub(new state::Stub),
    m_stateViewPackage(new state::ViewPackage)
{

    QVBoxLayout *overviewLayout = new QVBoxLayout;
    overviewLayout->setMargin(0);
    overviewLayout->addWidget(m_stateStub);
    overviewLayout->addWidget(m_stateViewPackage);

    setLayout(overviewLayout);
    setState(State::STUB);
}

void Overview::setState(Overview::State state)
{
    switch (state)
    {
        case State::VIEW_PACKAGE:
            m_stateStub->hide();
            m_stateViewPackage->show();
        break;
        case State::STUB:
            m_stateStub->show();
            m_stateViewPackage->hide();
    }
}

QJsonObject Overview::createJsonObject(const QString &path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QString reason = path + " does not exists";
        throw std::runtime_error(reason.toStdString());
    }

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (document.isNull()) {
        QString reason = path + " does not contain valid JSON";
        throw std::runtime_error(reason.toStdString());
    }

    return document.object();
}

void Overview::dispatch(const QString &path, const QString &childName)
{
    QString childNameAsPath = QDir::toNativeSeparators(childName);

    QJsonObject rootPackage = createJsonObject(path + QDir::separator() + "composer.json");
    QString packageName = rootPackage.value("name").toString();

    QString vendorDir = rootPackage.value("vendor-dir").toString("vendor");
    QString vendorFullPath = path + QDir::separator() + vendorDir;
    QString childPackagePath = vendorFullPath + QDir::separator() + childNameAsPath;

    QJsonObject childPackage = createJsonObject(childPackagePath + QDir::separator() + "composer.json");

    try {
        QJsonObject composerLock = createJsonObject(path + QDir::separator() + "composer.lock");
        QJsonArray packages = composerLock.value("packages").toArray();

        for (int i = 0; i < packages.count(); i++)
        {
            QJsonObject package = packages[i].toObject();
            QString lockedName = package.value("name").toString();

            if (lockedName == childName) {
                QString version = package.value("version").toString();
                childPackage.insert("composergui_locked_version", version);
            }
        }
    } catch (...) {}

    populate(childPackage);
}

void Overview::dispatch(const QString &path)
{
    populate(createJsonObject(path + QDir::separator() + "composer.json"));
}

void Overview::populate(const QJsonObject &package)
{
    using Field = state::ViewPackage::Field;

    auto api = new composer::api::packagist::Packagist;
    auto signalAddr = QOverload<QJsonObject>::of(&composer::api::packagist::Packagist::found);

    connect(api, signalAddr, this, [this](QJsonObject package)
    {
        QStandardItemModel *model = m_stateViewPackage->getMappingModel();

        QString downloads = QString::number(package.value("downloads").toInt());
        QString favers = QString::number(package.value("favers").toInt());

        model->setItem(Field::DOWNLOADS, new QStandardItem(downloads));
        model->setItem(Field::FAVERS, new QStandardItem(favers));
    });

    api->find(package.value("name").toString());

    QStandardItemModel *model = m_stateViewPackage->getMappingModel();
    model->setItem(Field::DOWNLOADS, new QStandardItem("?"));
    model->setItem(Field::FAVERS, new QStandardItem("?"));

    QString locked = package.value("composergui_locked_version").toString();
    model->setItem(Field::LOCKED, new QStandardItem(locked));
    model->setItem(Field::MINIMUM_STABILITY, new QStandardItem(package.value("minimum-stability").toString()));
    model->setItem(Field::TYPE, new QStandardItem(package.value("type").toString()));
    model->setItem(Field::LICENSE, new QStandardItem(package.value("license").toString()));
    model->setItem(Field::ABANDONED, new QStandardItem((package.value("abandoned").toBool() ? "true" : "false")));
    model->setItem(Field::KEYWORDS, new QStandardItem(package.value("keywords").toVariant().toStringList().join(", ")));
    model->setItem(Field::TIME, new QStandardItem(package.value("time").toString()));
    model->setItem(Field::HOMEPAGE, new QStandardItem(package.value("homepage").toString()));
    model->setItem(Field::README, new QStandardItem(package.value("readme").toString()));
    model->setItem(Field::DESCRIPTION, new QStandardItem(package.value("description").toString()));

    m_stateViewPackage->showLocked(!locked.isEmpty());

    setState(State::VIEW_PACKAGE);
}

} // overview
} // package
} // composer
} // composer_gui
