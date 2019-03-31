#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Php : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Php(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
