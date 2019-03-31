#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Composer : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Composer(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
