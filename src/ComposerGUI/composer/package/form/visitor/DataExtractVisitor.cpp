#include <stdexcept>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

#include "DataExtractVisitor.h"
#include "composer/package/form/General.h"
#include "composer/package/form/Dependencies.h"
#include "composer/package/form/Autoload.h"
#include "composer/package/form/Repositories.h"
#include "composer/package/form/Scripts.h"
#include "composer/package/form/Config.h"
#include "composer/package/form/Authors.h"
#include "composer/package/form/Support.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace visitor
{

DataExtractVisitor::DataExtractVisitor(const QJsonObject &source) : m_root(source)
{

}

void DataExtractVisitor::visit(General &form)
{
    using Field = General::Field;

    QStandardItemModel *model = form.getMappingModel();

    QString name = model->index(Field::NAME, 0).data().toString().trimmed();
    QString version = model->index(Field::VERSION, 0).data().toString().trimmed();
    QString type = model->index(Field::TYPE, 0).data().toString().trimmed();
    QString minimumStability = model->index(Field::MINIMUM_STABILITY, 0).data().toString().trimmed();
    QString preferStable = model->index(Field::PREFER_STABLE, 0).data().toString().trimmed();
    QString license = model->index(Field::LICENSE, 0).data().toString().trimmed();
    QString readme = model->index(Field::README, 0).data().toString().trimmed();
    QString homepage = model->index(Field::HOMEPAGE, 0).data().toString().trimmed();
    QString description = model->index(Field::DESCRIPTION, 0).data().toString().trimmed();

    handleElement("name", name, !name.isEmpty());
    handleElement("version", version, !version.isEmpty());
    handleElement("type", type, (type != '-'));
    handleElement("minimum-stability", minimumStability, (minimumStability != '-'));
    handleElement("prefer-stable", (preferStable == "true"), (preferStable != '-'));
    handleElement("license", license, !license.isEmpty());
    handleElement("readme", readme, !readme.isEmpty());
    handleElement("homepage", homepage, !homepage.isEmpty());
    handleElement("description", description, !description.isEmpty());

    m_root.insert("time", model->index(Field::TIME, 0).data().toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    QJsonArray keywords;
    QStringList splits = model->index(Field::KEYWORDS, 0).data().toString().split(',');

    for (QString &split : splits)
    {
        split = split.trimmed();
        if (!split.isEmpty()) {
            keywords.append(split);
        }
    }

    handleElement("keywords", keywords, (keywords.count() > 0));
}

void DataExtractVisitor::visit(Dependencies &form)
{
    QStringList headlines = {"require", "require-dev", "conflict", "replace", "provide", "suggest"};

    for (const QString &headline : headlines)
    {
        QStringListModel *model = form.getMappingModel(headline);
        QJsonObject dependencies;

        for (int i = 0; i < model->rowCount(); i++)
        {
            QStringList splits = model->index(i).data().toString().split('@');
            if (splits.count() != 2) {
                continue;
            }

            dependencies.insert(splits[0], splits[1]);
        }

        handleElement(headline, dependencies, (dependencies.count() > 0));
    }
}

void DataExtractVisitor::visit(Autoload &form)
{
    QStringList arrayItems = {"classmap", "files", "exclude-from-classmap"};

    for (const QString &name : {"autoload", "autoload-dev"})
    {
        QStandardItemModel *model = form.getMappingModel(name);
        QJsonObject autoloads;

        for (int i = 0; i < model->rowCount(); i++)
        {
            QStandardItem *typeRow = model->item(i);
            if (typeRow->text().isEmpty()) {
                continue;
            }

            if (arrayItems.contains(typeRow->text())) {
                QJsonArray paths;

                for (int j = 0; j < typeRow->rowCount(); j++)
                {
                    paths.append(typeRow->child(j)->text());
                }

                autoloads.insert(typeRow->text(), paths);

                continue;
            }

            QHash<QString, QJsonArray> pairs;

            for (int j = 0; j < typeRow->rowCount(); j++)
            {
                QStringList splits = typeRow->child(j)->text().split(':');
                if (splits.count() != 2) {
                    continue;
                }

                const QString &ns   = splits[0].trimmed();
                const QString &path = splits[1].trimmed();

                pairs[ns].append(path);
            }

            QJsonObject paths;

            for (auto it = pairs.keyBegin(); it != pairs.keyEnd(); it++)
            {
                const QString &ns = *it;
                const QJsonArray &jArr = pairs[ns];
                const QJsonValue &jVal = (jArr.count() > 1) ? jArr : jArr.first();

                paths.insert(ns, jVal);
            }

            autoloads.insert(typeRow->text(), paths);
        }

        handleElement(name, autoloads, (autoloads.count() > 0));
    }
}

void DataExtractVisitor::visit(Repositories &form)
{
    QJsonArray repositories;
    QStandardItemModel *model = form.getMappingModel();

    for (int i = 0; i < model->rowCount(); i++)
    {
        QStandardItem *typeRow = model->item(i);
        if (typeRow->text().isEmpty()) {
            continue;
        }

        QJsonObject typeChilds;

        for (int j = 0; j < typeRow->rowCount(); j++)
        {
            typeChilds.insert("type", typeRow->text());
            typeChilds.insert("url", typeRow->child(j)->text());
            repositories.append(typeChilds);
        }
    }

    handleElement("repositories", repositories, (repositories.count() > 0));
}

void DataExtractVisitor::visit(Scripts &form)
{
    QJsonObject scripts;
    QStandardItemModel *model = form.getMappingModel();

    for (int i = 0; i < model->rowCount(); i++)
    {
        QStandardItem *typeRow = model->item(i);
        if (typeRow->text().isEmpty()) {
            continue;
        }

        QHash<QString, QJsonArray> handlers;
        for (int j = 0; j < typeRow->rowCount(); j++)
        {
            handlers[typeRow->text()].append(typeRow->child(j)->text());
        }

        for (auto it = handlers.keyBegin(); it != handlers.keyEnd(); it++)
        {
            const QString &handler = *it;
            const QJsonArray &jArr = handlers[handler];
            const QJsonValue &jVal = (jArr.count() > 1) ? jArr : jArr.first();

            scripts.insert(handler, jVal);
        }
    }

    handleElement("scripts", scripts, (scripts.count() > 0));
}

void DataExtractVisitor::visit(Config &form)
{
    QJsonObject config;
    QStandardItemModel *model = form.getMappingModel();

    int i = 0;

    for (const QString &option : form.getIntegerOptions())
    {
        int value = model->item(i)->text().toInt();
        if (value != -1) {
            config.insert(option, value);
        }

        ++i;
    }

    for (const QString &option : form.getStringOptions())
    {
        QString value = model->item(i)->text().trimmed();
        if (!value.isEmpty() && value != '-') {
            config.insert(option, value);
        }

        ++i;
    }

    for (const QString &option : form.getBooleanOptions())
    {
        QString value = model->item(i)->text().trimmed();
        if (value != '-') {
            config.insert(option, (value == "true"));
        }

        ++i;
    }

    for (const QString &option : form.getMixedOptions())
    {
        QString value = model->item(i)->text().trimmed();

        if (value != '-') {
            bool isBool = (value == "true" || value == "false");
            if (isBool) {
                config.insert(option, (value == "true"));
            } else {
                config.insert(option, value);
            }
        }

        ++i;
    }

    handleElement("config", config, (config.count() > 0));
}

void DataExtractVisitor::visit(Authors &form)
{
    QJsonArray authors;
    QStandardItemModel *model = form.getMappingModel();
    QStringList columns = {"name", "role", "email", "homepage"};

    for (int i = 0; i < model->rowCount(); i++)
    {
        QJsonObject author;

        for (int j = 0; j < model->columnCount(); j++)
        {
            QString value = model->index(i, j).data().toString().trimmed();
            if (value.isEmpty()) {
                continue;
            }

            author.insert(columns[j], value);
        }

        if (author.count() > 0) {
            authors.append(author);
        }
    }

    handleElement("authors", authors, (authors.count() > 0));
}

void DataExtractVisitor::visit(Support &form)
{
    QStandardItemModel *model = form.getMappingModel();
    QStringList labels{"email", "issues", "forum", "wiki", "irc", "source", "docs", "chat"};

    QJsonObject support;
    for (int i = 0; i < model->rowCount(); i++)
    {
        QString value = model->index(i, 0).data().toString().trimmed();
        if (!value.isEmpty()) {
            support.insert(labels[i], value);
        }
    }

    handleElement("support", support, (support.count() > 0));
}

QJsonObject &DataExtractVisitor::dump(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QString reason = "Cannot open " + path + "for write";
        throw std::runtime_error(reason.toStdString());
    }

    QByteArray json = QJsonDocument(m_root).toJson();
    if (file.write(json.data(), json.size()) == -1) {
        QString reason = "Cannot write to " + path;
        throw std::runtime_error(reason.toStdString());
    }

    return m_root;
}

} // visitor
} // form
} // package
} // composer
} // composer_gui
