#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Pwd : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Pwd(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
