#pragma once

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace visitor
{

struct VisitorInterface;

struct AcceptorInterface
{
    virtual void accept(VisitorInterface &visitor) = 0;
    virtual ~AcceptorInterface() = default;
};

} // visitor
} // form
} // package
} // composer
} // composer_gui
