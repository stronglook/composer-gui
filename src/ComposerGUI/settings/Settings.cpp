#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QThread>
#include <QDir>

#include "Settings.h"
#include "globals.h"

namespace composer_gui
{
namespace settings
{

Settings::Settings(QWidget *parent)
    : QWidget(parent),
      m_settings(COMPOSERGUI_ORGANIZATION_NAME, COMPOSERGUI_SETTINGS_APPLICATION),
      m_workerThreads(new QSpinBox),
      m_packagistSearchLimit(new QSpinBox),
      m_phpBinaryPath(new QLineEdit),
      m_composerBinaryPath(new QLineEdit)
{
    QVBoxLayout *settingsLayout = new QVBoxLayout;
    settingsLayout->setMargin(0);
    settingsLayout->addWidget(createGeneralSettingsBox());
    settingsLayout->addWidget(createBinariesSettingsBox());

    int workerThreads = m_settings.value("worker_threads", QThread::idealThreadCount()).toInt();
    int packagistSearchLimit = m_settings.value("packagist_search_limit", 2).toInt();
    QString phpBinaryPath = m_settings.value("php_binary_path").toString();
    QString composerBinaryPath = m_settings.value("composer_binary_path").toString();

    m_workerThreads->setValue(workerThreads);
    m_packagistSearchLimit->setValue(packagistSearchLimit);
    m_phpBinaryPath->setText(phpBinaryPath);
    m_composerBinaryPath->setText(composerBinaryPath);

    setLayout(settingsLayout);
}

void Settings::apply()
{
    m_settings.setValue("worker_threads", m_workerThreads->value());
    m_settings.setValue("packagist_search_limit", m_packagistSearchLimit->value());
    m_settings.setValue("php_binary_path", m_phpBinaryPath->text());
    m_settings.setValue("composer_binary_path", m_composerBinaryPath->text());
}

QHBoxLayout *Settings::createFileLayout(QLineEdit *pathHolder)
{
    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->setSpacing(5);

    pathHolder->setReadOnly(true);
    pathHolder->setPlaceholderText("No file selected");

    QPushButton *browseButton = new QPushButton("Browse");

    fileLayout->addWidget(pathHolder);
    fileLayout->addWidget(browseButton);

    connect(browseButton, &QPushButton::clicked, [=]
    {
        QString path = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, "Select file"));
        if (!path.isEmpty()) {
            pathHolder->setText(path);
        }
    });

    return fileLayout;
}

QGroupBox *Settings::createGeneralSettingsBox()
{
    QFormLayout *boxLayout = new QFormLayout;
    boxLayout->addRow("Worker threads: ", m_workerThreads);
    boxLayout->addRow("Packagist search limit", m_packagistSearchLimit);

    m_workerThreads->setMinimum(1);
    m_workerThreads->setMaximum(QThread::idealThreadCount());

    QGroupBox *box = new QGroupBox;
    box->setTitle("General");
    box->setLayout(boxLayout);

    return box;
}

QGroupBox *Settings::createBinariesSettingsBox()
{
    QFormLayout *boxLayout = new QFormLayout;
    boxLayout->addRow("PHP:", createFileLayout(m_phpBinaryPath));
    boxLayout->addRow("Composer:", createFileLayout(m_composerBinaryPath));

    QGroupBox *box = new QGroupBox;
    box->setTitle("Binaries");
    box->setLayout(boxLayout);

    return box;
}

} // settings
} // composer_gui

