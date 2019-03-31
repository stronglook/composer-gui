#include <stdexcept>

#include <QStandardItemModel>
#include <QStringListModel>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QHash>

#include "DataProviderVisitor.h"
#include "composer/package/form/General.h"
#include "composer/package/form/Dependencies.h"
#include "composer/package/form/Autoload.h"
#include "composer/package/form/Repositories.h"
#include "composer/package/form/Scripts.h"
#include "composer/package/form/Config.h"
#include "composer/package/form/Support.h"
#include "composer/package/form/Authors.h"

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

DataProviderVisitor::DataProviderVisitor(const QString &path)
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

    m_package = document.object();
}

void DataProviderVisitor::visit(General &form)
{
    using Field = General::Field;

    QStandardItemModel *model = form.getMappingModel();
    model->setItem(Field::NAME, new QStandardItem(m_package.value("name").toString()));
    model->setItem(Field::VERSION, new QStandardItem(m_package.value("version").toString()));
    model->setItem(Field::TYPE, new QStandardItem(m_package.value("type").toString("-")));
    model->setItem(Field::MINIMUM_STABILITY, new QStandardItem(m_package.value("minimum-stability").toString("-")));

    if (m_package.contains("prefer-stable")) {
        model->setItem(Field::PREFER_STABLE, new QStandardItem(m_package.value("prefer-stable").toBool() ? "true" : "false"));
    }

    QString keywords = m_package.value("keywords").toVariant().toStringList().join(", ");
    model->setItem(Field::KEYWORDS, new QStandardItem(keywords));

    model->setItem(Field::LICENSE, new QStandardItem(m_package.value("license").toString()));
    model->setItem(Field::README, new QStandardItem(m_package.value("readme").toString()));
    model->setItem(Field::HOMEPAGE, new QStandardItem(m_package.value("homepage").toString()));
    model->setItem(Field::TIME, new QStandardItem(m_package.value("time").toString()));
    model->setItem(Field::DESCRIPTION, new QStandardItem(m_package.value("description").toString()));
}

void DataProviderVisitor::visit(Dependencies &form)
{
    QStringList headlines = {"require", "require-dev", "conflict", "replace", "provide", "suggest"};

    for (const QString &headline : headlines)
    {
        QJsonObject dependencies = m_package.value(headline).toObject();
        if (dependencies.count() > 0) {
            QStringList list;

            for (const QString &name : dependencies.keys())
            {
                list.append(name + '@' + dependencies.value(name).toString());
            }

            QStringListModel *model = form.getMappingModel(headline);
            model->setStringList(list);
        }
    }
}

void DataProviderVisitor::visit(Autoload &form)
{
    QStringList arrayItems = {"classmap", "files", "exclude-from-classmap"};

    for (const QString &name : {"autoload", "autoload-dev"})
    {
        QStandardItemModel *model = form.getMappingModel(name);
        QJsonObject autoloaders = m_package.value(name).toObject();

        for (const QString &item : arrayItems)
        {
            if (autoloaders.contains(item)) {
                QJsonArray autoloadersArray = autoloaders.value(item).toArray();
                if (autoloadersArray.count() > 0) {
                    QStandardItem *root = new QStandardItem(item);
                    for (auto autoloader : autoloadersArray)
                    {
                        root->setChild(root->rowCount(), new QStandardItem(autoloader.toString()));
                    }
                    model->setItem(model->rowCount(), root);
                }
            }
        }

        for (const QString &item : {"psr-0", "psr-4"})
        {
            if (autoloaders.contains(item)) {
                QJsonObject autoloadersObject = autoloaders.value(item).toObject();
                QStandardItem *root = new QStandardItem(item);

                for (auto key : autoloadersObject.keys())
                {
                    QString value = autoloadersObject.value(key).toString();
                    if (!value.isEmpty()) {
                        root->setChild(root->rowCount(), new QStandardItem(key + ": " + value));
                    } else {
                        QJsonObject autoloadersObject = autoloaders.value(item).toObject();

                        if (autoloadersObject.count() > 0) {
                            for (const QString &ns : autoloadersObject.keys())
                            {
                                for (auto autoloader : autoloadersObject.value(ns).toArray())
                                {
                                    root->setChild(root->rowCount(), new QStandardItem(key + ": " + autoloader.toString()));
                                }
                            }
                        }
                    }
                }

                if (root->rowCount() > 0) {
                    model->setItem(model->rowCount(), root);
                }
            }
        }
    }
}

void DataProviderVisitor::visit(Repositories &form)
{
    QJsonArray repositories = m_package.value("repositories").toArray();

    if (repositories.count() > 0) {
        QStringList supported = {
            "vcs",
            "git",
            "svn",
            "pear",
            "path",
            "composer",
            "artifact",
            "git-bitbucket",
            "hg-bitbucket",
            "github",
            "gitlab",
            "perforce",
            "fossil",
            "hg"
        };

        QStandardItemModel *model = form.getMappingModel();
        QHash<QString, QStringList> data;

        for (auto repository : repositories)
        {
            QJsonObject repositoryObject = repository.toObject();
            QString type = repositoryObject.value("type").toString().trimmed();
            QString url = repositoryObject.value("url").toString().trimmed();

            if (supported.contains(type) && !type.isEmpty() && !url.isEmpty()) {
                data[type].append(url);
            }
        }

        for (const QString &key : data.keys())
        {
            QStandardItem *root = new QStandardItem(key);

            for (const QString &value : data.value(key))
            {
                root->setChild(root->rowCount(), new QStandardItem(value));
            }

            model->setItem(model->rowCount(), 0, root);
        }
    }
}

void DataProviderVisitor::visit(Scripts &form)
{
    QJsonObject scripts = m_package.value("scripts").toObject();

    if (scripts.count() > 0) {
        QStringList supported = {
            "pre-install-cmd",
            "post-install-cmd",
            "pre-update-cmd",
            "post-update-cmd",
            "post-status-cmd",
            "pre-archive-cmd",
            "post-archive-cmd",
            "pre-autoload-dump",
            "post-autoload-dump",
            "post-root-package-install",
            "post-create-project-cmd",
            "pre-dependencies-solving",
            "post-dependencies-solving",
            "pre-package-install",
            "post-package-install",
            "pre-package-update",
            "post-package-update",
            "pre-package-uninstall",
            "post-package-uninstall",
            "init",
            "command",
            "pre-file-download",
            "pre-command-run",
        };

        QStandardItemModel *model = form.getMappingModel();

        for (const QString &key : scripts.keys())
        {
            if (!supported.contains(key)) {
                continue;
            }

            QString value = scripts[key].toString().trimmed();
            if (!value.isEmpty()) {
                QStandardItem *root = new QStandardItem(key);
                root->setChild(0, new QStandardItem(value));
                model->setItem(model->rowCount(), root);
            } else {
                QJsonArray scriptsArray = scripts.value(key).toArray();

                if (scriptsArray.count() > 0) {
                    QStandardItem *root = new QStandardItem(key);

                    for (auto script : scriptsArray)
                    {
                        root->setChild(root->rowCount(), new QStandardItem(script.toString()));
                    }

                    model->setItem(model->rowCount(), root);
                }
            }
        }
    }
}

void DataProviderVisitor::visit(Config &form)
{
    QJsonObject config = m_package.value("config").toObject();
    if (config.count() > 0) {
        QStandardItemModel *model = form.getMappingModel();

        int i = 0;

        for (const QString &option : form.getIntegerOptions())
        {
            if (config.contains(option)) {
                model->setItem(i, new QStandardItem(QString::number(config.value(option).toInt())));
            }

            ++i;
        }

        for (const QString &option : form.getStringOptions())
        {
            if (config.contains(option)) {
                model->setItem(i, new QStandardItem(config.value(option).toString()));
            }

            ++i;
        }

        for (const QString &option : form.getBooleanOptions())
        {
            if (config.contains(option)) {
                QString value = config.value(option).toBool() ? "true" : "false";
                model->setItem(i, new QStandardItem(value));
            }

            ++i;
        }

        for (const QString &option : form.getMixedOptions())
        {
            if (config.contains(option)) {
                QString sValue = config.value(option).toString().trimmed();
                if (!sValue.isEmpty()) {
                    model->setItem(i, new QStandardItem(sValue));
                } else {
                    model->setItem(i, new QStandardItem(config.value(option).toBool() ? "true" : "false"));
                }
            }

            ++i;
        }
    }
}

void DataProviderVisitor::visit(Authors &form)
{
    QStandardItemModel *model = form.getMappingModel();
    QJsonArray authors = m_package.value("authors").toArray();

    using Field = Authors::Field;

    for (int i = 0; i < authors.count(); i++)
    {
        QJsonObject author = authors[i].toObject();
        model->setItem(i, Field::NAME, new QStandardItem(author.value("name").toString()));
        model->setItem(i, Field::ROLE, new QStandardItem(author.value("role").toString()));
        model->setItem(i, Field::EMAIL, new QStandardItem(author.value("email").toString()));
        model->setItem(i, Field::HOMEPAGE, new QStandardItem(author.value("homepage").toString()));
    }
}

void DataProviderVisitor::visit(Support &form)
{
    QStandardItemModel *model = form.getMappingModel();
    QJsonObject support = m_package.value("support").toObject();

    using Field = Support::Field;

    model->setItem(Field::EMAIL, new QStandardItem(support.value("email").toString()));
    model->setItem(Field::ISSUES, new QStandardItem(support.value("issues").toString()));
    model->setItem(Field::FORUM, new QStandardItem(support.value("forum").toString()));
    model->setItem(Field::WIKI, new QStandardItem(support.value("wiki").toString()));
    model->setItem(Field::IRC, new QStandardItem(support.value("irc").toString()));
    model->setItem(Field::SOURCE, new QStandardItem(support.value("source").toString()));
    model->setItem(Field::DOCS, new QStandardItem(support.value("docs").toString()));
    model->setItem(Field::CHAT, new QStandardItem(support.value("chat").toString()));
}

const QJsonObject &DataProviderVisitor::getPackageJsonObject() const
{
    return m_package;
}

} // visitor
} // form
} // package
} // composer
} // composer_gui
