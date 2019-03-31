#pragma once

#include "AbstractCommand.h"

namespace composer_gui
{
namespace console
{
namespace command
{

class Help : public AbstractCommand
{
    Q_OBJECT
public:
    explicit Help(QObject *parent = nullptr);
    void run() override;
};

} // command
} // console
} // composer_gui
