#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Unknown : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Unknown(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
