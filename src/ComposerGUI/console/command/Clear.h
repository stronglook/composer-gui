#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Clear : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Clear(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
