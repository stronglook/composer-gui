#pragma once

#include <QCompleter>
#include <QStringList>
#include <QObject>
#include <QKeyEvent>
#include <QEvent>

#include "console/Console.h"

namespace composer_gui
{
namespace console
{
namespace command
{
namespace completer
{

class Completer : public QCompleter
{
    Q_OBJECT
public:
    Completer(const QStringList &completions, QObject *parent = nullptr);

public:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void hint(const QString &completion);
    bool isHintNeeded(Console *source);
    QString createCompletionPrefix(Console *source, QKeyEvent *event);
};

} // completer
} // command
} // console
} // composer_gui
