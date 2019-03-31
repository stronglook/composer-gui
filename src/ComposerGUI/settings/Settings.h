#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QSettings>
#include <QHBoxLayout>
#include <QGroupBox>

namespace composer_gui
{
namespace settings
{

class Settings : public QWidget
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

    QHBoxLayout *createFileLayout(QLineEdit *pathHolder);
    QGroupBox *createGeneralSettingsBox();
    QGroupBox *createBinariesSettingsBox();
};

} // settings
} // composer_gui
