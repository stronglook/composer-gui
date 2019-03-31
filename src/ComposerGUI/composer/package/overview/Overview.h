#pragma once

#include <QWidget>
#include <QString>
#include <QJsonObject>

#include "state/Stub.h"
#include "state/ViewPackage.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace overview
{

class Overview : public QWidget
{
public:
    enum State
    {
        VIEW_PACKAGE,
        STUB
    };

    Overview(QWidget *parent = nullptr);
    void setState(State state);
    void dispatch(const QString &path);
    void dispatch(const QString &path, const QString &childName);

private:
    state::Stub *m_stateStub;
    state::ViewPackage *m_stateViewPackage;

    QJsonObject createJsonObject(const QString &path);
    void populate(const QJsonObject &package);
};

} // overview
} // package
} // composer
} // composer_gui
