#pragma once

#include <QString>

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Cd : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Cd(QObject *parent = nullptr);
    void run();

signals:
    void directory(QString);
};

} // command
} // console
} // composer_gui
