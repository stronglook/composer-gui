#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Ls : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Ls(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
