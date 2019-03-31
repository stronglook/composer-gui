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

class DataExtractVisitor : public VisitorInterface
{
public:
    DataExtractVisitor() = default;
    DataExtractVisitor(const QJsonObject &source);

    void visit(General &form) override;
    void visit(Dependencies &form) override;
    void visit(Autoload &form) override;
    void visit(Repositories &form) override;
    void visit(Scripts &form) override;
    void visit(Config &form) override;
    void visit(Authors &form) override;
    void visit(Support &form) override;
    QJsonObject &dump(const QString &path);

private:
    QJsonObject m_root;

    template<typename T>
    void handleElement(const QString &key, const T &value, bool condition)
    {
        if (condition) {
            m_root.insert(key, value);
        } else {
            m_root.remove(key);
        }
    }
};

} // visitor
} // form
} // package
} // composer
} // composer_gui
