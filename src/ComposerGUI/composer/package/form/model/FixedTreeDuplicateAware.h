#pragma once

#include <QStandardItemModel>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace model
{

class FixedTreeDuplicateAware : public QStandardItemModel
{
public:
    FixedTreeDuplicateAware(QObject *parent = nullptr);
    void mergeDuplicates();
};

} // model
} // form
} // package
} // composer
} // composer_gui
