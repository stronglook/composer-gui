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
    : QTabWidget(parent),
      m_settings(COMPOSERGUI_ORGANIZATION_NAME, COMPOSERGUI_SETTINGS_APPLICATION),
      m_workerThreads(new QSpinBox),
      m_packagistSearchLimit(new QSpinBox),
      m_phpBinaryPath(new QLineEdit),
      m_composerBinaryPath(new QLineEdit),
      m_httpProxy(new QLineEdit),
      m_httpsProxy(new QLineEdit)
{
    addTab(createGeneralSettings(), "General");
    addTab(createBinariesSettings(), "Binaries");
    addTab(createProxySettings(), "Proxy");
}

void Settings::apply()
{
    m_settings.setValue("worker_threads", m_workerThreads->value());
    m_settings.setValue("packagist_search_limit", m_packagistSearchLimit->value());
    m_settings.setValue("php_binary_path", m_phpBinaryPath->text());
    m_settings.setValue("composer_binary_path", m_composerBinaryPath->text());
    m_settings.setValue("var_http_proxy", m_httpProxy->text());
    m_settings.setValue("var_https_proxy", m_httpsProxy->text());
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

QWidget *Settings::createProxySettings()
{
    QFormLayout *proxyLayout = new QFormLayout;
    proxyLayout->addRow("HTTP Proxy: ", m_httpProxy);
    proxyLayout->addRow("HTTPS Proxy: ", m_httpsProxy);

    QString httpProxy = m_settings.value("var_http_proxy").toString();
    QString httpsProxy = m_settings.value("var_https_proxy").toString();

    m_httpProxy->setPlaceholderText("<scheme>:[//[<username>[:<password>]@]<host>[:<port>]]");
    m_httpProxy->setText(httpProxy);

    m_httpsProxy->setPlaceholderText("<scheme>:[//[<username>[:<password>]@]<host>[:<port>]]");
    m_httpsProxy->setText(httpsProxy);

    QWidget *proxy = new QWidget;
    proxy->setLayout(proxyLayout);

    return proxy;
}

QWidget *Settings::createGeneralSettings()
{
    QFormLayout *settingsLayout = new QFormLayout;
    settingsLayout->addRow("Worker threads: ", m_workerThreads);
    settingsLayout->addRow("Packagist search limit", m_packagistSearchLimit);

    m_workerThreads->setMinimum(1);
    m_workerThreads->setMaximum(QThread::idealThreadCount());

    int workerThreads = m_settings.value("worker_threads", QThread::idealThreadCount()).toInt();
    int packagistSearchLimit = m_settings.value("packagist_search_limit", 2).toInt();

    m_workerThreads->setValue(workerThreads);
    m_packagistSearchLimit->setValue(packagistSearchLimit);

    QWidget *general = new QWidget;
    general->setLayout(settingsLayout);

    return general;
}

QWidget *Settings::createBinariesSettings()
{
    QFormLayout *binariesLayout = new QFormLayout;
    binariesLayout->addRow("PHP:", createFileLayout(m_phpBinaryPath));
    binariesLayout->addRow("Composer:", createFileLayout(m_composerBinaryPath));

    QString phpBinaryPath = m_settings.value("php_binary_path").toString();
    QString composerBinaryPath = m_settings.value("composer_binary_path").toString();

    m_phpBinaryPath->setText(phpBinaryPath);
    m_composerBinaryPath->setText(composerBinaryPath);

    QWidget *binaries = new QWidget;
    binaries->setLayout(binariesLayout);

    return binaries;
}

} // settings
} // composer_gui

