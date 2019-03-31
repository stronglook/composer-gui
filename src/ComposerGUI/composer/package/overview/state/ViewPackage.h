#pragma once

#include <QWidget>
#include <QLabel>
#include <QStringList>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace overview
{
namespace state
{

class ViewPackage : public QWidget
{
public:
    enum Field {DOWNLOADS, FAVERS, LOCKED, MINIMUM_STABILITY, TYPE, LICENSE,
               ABANDONED, KEYWORDS, TIME, HOMEPAGE, README, DESCRIPTION};

    ViewPackage(QWidget *parent = nullptr);

    QFormLayout *createFormLayout(const QStringList &labels);
    QFormLayout *createDescriptionFormLayout();
    QHBoxLayout *createStatisticsLayout();

    QStandardItemModel *getMappingModel();
    void showLocked(bool flag);

private:
    QDataWidgetMapper *m_mapper;
    int m_mapperSection;
    QLabel *m_locked;
};

} // state
} // overview
} // package
} // composer
} // composer_gui
