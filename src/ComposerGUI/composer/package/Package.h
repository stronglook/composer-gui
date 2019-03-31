#pragma once

#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDataWidgetMapper>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QJsonObject>

namespace composer_gui
{
namespace composer
{
namespace package
{

class Package : public QWidget
{
    Q_OBJECT
public:
    Package(QWidget *parent = nullptr);
    static Package *fromJsonObject(const QJsonObject &json);
    QString getName();
    void unhook();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QDataWidgetMapper *m_mapper;
    QComboBox *m_versions;
    QLabel *m_downloads;
    QLabel *m_rate;
    QLabel *m_name;
    QLabel *m_description;

    QHBoxLayout *createStatisticsLayout();
    QVBoxLayout *createInfoLineLayout();
    QHBoxLayout *createControlsLayout();

    QStandardItemModel *getMappingModel();
    QStringListModel *getVersionsWidgetModel();

    enum Field {NAME, DESCRIPTION, VERSIONS, DOWNLOADS, FAVERS};

signals:
    void added(Package *);
};

} // package
} // composer
} // composer_gui
