#pragma once

#include <QObject>
#include <QString>
#include <QJsonArray>

#include "composer/package/Package.h"

namespace composer_gui
{
namespace composer
{
namespace api
{
namespace packagist
{

class Packagist : public QObject
{
    Q_OBJECT
public:
    Packagist(QObject *parent = nullptr);
    void find(const QString &name, int limit);
    void find(const QString &name);

signals:
    void found(QJsonArray);
    void found(QJsonObject);
};

} // packagist
} // api
} // composer
} // composer_gui
