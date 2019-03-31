#pragma once

#include <QJsonObject>
#include <QString>

#include "VisitorInterface.h"

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

class DataProviderVisitor : public VisitorInterface
{
public:
    DataProviderVisitor(const QString &path);
    void visit(General &form) override;
    void visit(Dependencies &form) override;
    void visit(Autoload &form) override;
    void visit(Repositories &form) override;
    void visit(Scripts &form) override;
    void visit(Config &form) override;
    void visit(Authors &form) override;
    void visit(Support &form) override;
    const QJsonObject &getPackageJsonObject() const;

private:
    QJsonObject m_package;
};

} // visitor
} // form
} // package
} // composer
} // composer_gui
