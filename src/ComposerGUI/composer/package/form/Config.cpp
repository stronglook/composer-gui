#include <limits>

#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QString>

#include "Config.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Config::Config(QWidget *parent) :
    QWidget(parent),
    m_mapper(new QDataWidgetMapper(this)),
    m_mapperSection(0)
{
    QStandardItemModel *model = new QStandardItemModel(30, 1, this);
    m_mapper->setModel(model);
    m_mapper->setOrientation(Qt::Vertical);
    m_mapper->toFirst();

    QFormLayout *configLayout = new QFormLayout;
    configLayout->setHorizontalSpacing(100);

    // integer options
    configLayout->addRow("process-timeout:", createSpinBox(-1, -1));
    configLayout->addRow("cache-files-ttl:", createSpinBox(-1, -1));

    // string options
    configLayout->addRow("cafile:", createFileLayout(false));
    configLayout->addRow("capath:", createFileLayout());
    configLayout->addRow("vendor-dir:", createFileLayout());
    configLayout->addRow("bin-dir:", createFileLayout());
    configLayout->addRow("data-dir:", createFileLayout());
    configLayout->addRow("cache-dir:", createFileLayout());
    configLayout->addRow("cache-files-dir:", createFileLayout());
    configLayout->addRow("cache-repo-dir:", createFileLayout());
    configLayout->addRow("cache-vcs-dir:", createFileLayout());
    configLayout->addRow("archive-dir:", createFileLayout());
    configLayout->addRow("archive-format:", createLineEdit());
    configLayout->addRow("cache-files-maxsize:", createLineEdit());
    configLayout->addRow("autoloader-suffix:", createLineEdit());
    configLayout->addRow("bin-compat:", createComboBox({"-", "full", "auto"}));
    configLayout->addRow("preferred-install:", createComboBox({"-", "source", "dist", "auto"}));

    // bool options
    configLayout->addRow("disable-tls:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("secure-http:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("optimize-autoloader:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("sort-packages:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("classmap-authoritative:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("apcu-autoloader:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("notify-on-install:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("prepend-autoloader:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("htaccess-protect:", createComboBox({"-", "true", "false"}));
    configLayout->addRow("use-include-path:", createComboBox({"-", "true", "false"}));

    // mixed options
    configLayout->addRow("store-auths:", createComboBox({"-", "true", "false", "prompt"}));
    configLayout->addRow("discard-changes:", createComboBox({"-", "true", "false", "stash"}));

    m_mapper->submit();

    connect(model, &QStandardItemModel::itemChanged, m_mapper, &QDataWidgetMapper::toFirst);
    setLayout(configLayout);
}

void Config::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *Config::getMappingModel()
{
    return static_cast<QStandardItemModel *>(m_mapper->model());
}

QStringList Config::getIntegerOptions()
{
    return {
        "process-timeout",
        "cache-files-ttl"
    };
}

QStringList Config::getStringOptions()
{
    return {
        "cafile", "capath", "vendor-dir", "bin-dir", "data-dir", "cache-dir", "cache-files-dir",
        "cache-repo-dir", "cache-vcs-dir", "archive-dir", "archive-format", "cache-files-maxsize",
        "autoloader-suffix", "bin-compat", "preferred-install"
    };
}

QStringList Config::getBooleanOptions()
{
    return {
        "disable-tls", "secure-http", "optimize-autoloader", "sort-packages", "classmap-authoritative",
        "apcu-autoloader", "notify-on-install", "prepend-autoloader", "htaccess-protect", "use-include-path"
    };
}

QStringList Config::getMixedOptions()
{
    return {
        "store-auths",
        "discard-changes"
    };
}

QSpinBox *Config::createSpinBox(int value, int minimum, int maximum)
{
    QSpinBox *spinBox = new QSpinBox;
    spinBox->setMinimum(minimum);
    spinBox->setMaximum(maximum);
    spinBox->setValue(value);

    m_mapper->addMapping(spinBox, m_mapperSection++);

    return spinBox;
}

QComboBox *Config::createComboBox(const QStringList &items)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->addItems(items);

    m_mapper->addMapping(comboBox, m_mapperSection++);

    return comboBox;
}

QLineEdit *Config::createLineEdit()
{
    QLineEdit *lineEdit = new QLineEdit;

    m_mapper->addMapping(lineEdit, m_mapperSection++);

    return lineEdit;
}

QHBoxLayout *Config::createFileLayout(bool onlyDirectories)
{
    QString placeholder = onlyDirectories ? "No directory selected" : "No file selected";
    QString dialogTitle = onlyDirectories ? "Select directory" : "Select file";

    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->setSpacing(5);

    QLineEdit *selectedPath = new QLineEdit;
    selectedPath->setReadOnly(true);
    selectedPath->setPlaceholderText(placeholder);

    m_mapper->addMapping(selectedPath, m_mapperSection++);

    QPushButton *browseButton = new QPushButton("Browse");

    fileLayout->addWidget(selectedPath);
    fileLayout->addWidget(browseButton);

    connect(browseButton, &QPushButton::clicked, [=]
    {
        QString path;

        if (onlyDirectories) {
            path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, dialogTitle));
        } else {
            path = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, dialogTitle));
        }

        selectedPath->setText(path);
        m_mapper->submit();
    });

    return fileLayout;
}


} // form
} // package
} // composer
} // composer_gui
