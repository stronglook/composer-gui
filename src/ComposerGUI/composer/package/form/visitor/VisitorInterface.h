#pragma once

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
    class General;
    class Dependencies;
    class Autoload;
    class Repositories;
    class Scripts;
    class Config;
    class Authors;
    class Support;

namespace visitor
{
    struct VisitorInterface
    {
        virtual void visit(General &form) = 0;
        virtual void visit(Dependencies &form) = 0;
        virtual void visit(Autoload &form) = 0;
        virtual void visit(Repositories &form) = 0;
        virtual void visit(Scripts &form) = 0;
        virtual void visit(Config &form) = 0;
        virtual void visit(Authors &form) = 0;
        virtual void visit(Support &form) = 0;

        virtual ~VisitorInterface() = default;
    };
} // visitor

} // form
} // package
} // composer
} // composer_gui
