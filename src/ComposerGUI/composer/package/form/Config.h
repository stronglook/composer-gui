#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QStringList>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

#include "composer/package/form/visitor/VisitorInterface.h"
#include "composer/package/form/visitor/AcceptorInterface.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

class Config : public QWidget, public visitor::AcceptorInterface
{
public:
    explicit Config(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStandardItemModel *getMappingModel();

    QStringList getIntegerOptions();
    QStringList getStringOptions();
    QStringList getBooleanOptions();
    QStringList getMixedOptions();

private:
    QDataWidgetMapper *m_mapper;
    int m_mapperSection;

    QSpinBox *createSpinBox(int value, int minimum = 0, int maximum = std::numeric_limits<int>::max());
    QComboBox *createComboBox(const QStringList &items);
    QLineEdit *createLineEdit();
    QHBoxLayout *createFileLayout(bool onlyDirectories = true);
};

} // form
} // package
} // composer
} // composer_gui
