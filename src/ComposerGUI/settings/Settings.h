#pragma once

#include <QTabWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QSettings>
#include <QHBoxLayout>
#include <QGroupBox>

namespace composer_gui
{
namespace settings
{

class Settings : public QTabWidget
{
public:
    Settings(QWidget *parent = nullptr);
    void apply();

private:
    QSettings m_settings;

    QSpinBox *m_workerThreads;
    QSpinBox *m_packagistSearchLimit;
    QLineEdit *m_phpBinaryPath;
    QLineEdit *m_composerBinaryPath;
    QLineEdit *m_httpProxy;
    QLineEdit *m_httpsProxy;

    QHBoxLayout *createFileLayout(QLineEdit *pathHolder);
    QWidget *createGeneralSettings();
    QWidget *createBinariesSettings();
    QWidget *createProxySettings();
};

} // settings
} // composer_gui
