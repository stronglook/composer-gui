#pragma once

#include "Standard.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace controls
{

class FixedTree : public Standard
{
public:
    FixedTree(QAbstractItemView *view, QWidget *parent = nullptr);

protected:
    virtual void add();
    virtual void edit();
    virtual void remove();
};

} // controls
} // form
} // package
} // composer
} // composer_gui
