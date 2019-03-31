#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QEventLoop>

#include "Packagist.h"
#include "concurrent/thread/Thread.h"

namespace composer_gui
{
namespace composer
{
namespace api
{
namespace packagist
{

Packagist::Packagist(QObject *parent) : QObject(parent)
{

}

void Packagist::find(const QString &name, int limit)
{
    concurrent::thread::Thread([this, name, limit]
    {
        QEventLoop threadLoop;
        QNetworkAccessManager *networkManager = new QNetworkAccessManager;
        QUrl url("https://packagist.org/search.json?per_page=" + QString::number(limit) + "&q=" + name);
        QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::finished, [this, networkManager, &threadLoop, reply]
        {
            if (!reply->error()) {
                QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
                QJsonArray packages = document.object().value("results").toArray();
                QJsonArray results;

                for (auto package : packages)
                {
                    QEventLoop loop;

                    const QJsonObject &root = package.toObject();
                    QJsonObject result;
                    result.insert("name", root.value("name"));
                    result.insert("description", root.value("description"));
                    result.insert("downloads", root.value("downloads"));
                    result.insert("favers", root.value("favers"));

                    QUrl url("https://packagist.org/packages/" + root.value("name").toString() + ".json");
                    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));
                    connect(reply, &QNetworkReply::finished, [reply, &result, &results, &loop]
                    {
                        if (!reply->error()) {
                            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
                            QJsonObject versions = document.object().value("package").toObject().value("versions").toObject();
                            result.insert("versions", QJsonArray::fromStringList(versions.keys()));
                            results.append(result);
                        }
                        loop.quit();
                    });

                    loop.exec();
                    reply->deleteLater();
                }

                emit found(results);

                reply->deleteLater();
                networkManager->deleteLater();

                threadLoop.quit();
            }
        });

        threadLoop.exec();
    });
}

void Packagist::find(const QString &name)
{
    concurrent::thread::Thread([this, name]
    {
        QEventLoop threadLoop;
        QNetworkAccessManager *networkManager = new QNetworkAccessManager;
        QUrl url("https://packagist.org/packages/" + name + ".json");
        QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::finished, [this, networkManager, &threadLoop, reply]
        {
            if (!reply->error()) {
                QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
                if (!document.isNull()) {
                    QJsonObject package = document.object().value("package").toObject();
                    QJsonObject result;
                    result.insert("downloads", package.value("downloads").toObject().value("total"));
                    result.insert("favers", package.value("favers"));
                    emit found(result);
                }
            }

            networkManager->deleteLater();
            reply->deleteLater();
            threadLoop.quit();
        });

        threadLoop.exec();
    });
}

} // packagist
} // api
} // composer
} // composer_gui
