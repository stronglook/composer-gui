#pragma once

#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QTextCursor>
#include <QStringList>
#include <QString>
#include <QQueue>
#include <QMenu>

#include "command/Command.h"
#include "command/invoker/Invoker.h"

namespace composer_gui
{
namespace console
{

class Console : public QPlainTextEdit
{
    using Command = command::Command;
    using Invoker = command::invoker::Invoker;

    Q_OBJECT
public:
    Console(const QString &whoami, Invoker *invoker, QWidget *parent = nullptr);
    Invoker *getCommandInvoker();
    void setTextColor(const char *color);
    void setWorkingDirectory(const QString &directory);
    const QString &getWorkingDirectory() const;
    void addText(const QString &text, bool newBlock = true);
    void addWhoami();
    void setActive(bool active);
    void scrollDown();
    const QString &getWhoami() const;
    void execute(const QString &sCommand);

protected:
    void keyPressEvent(QKeyEvent *);
    void contextMenuEvent(QContextMenuEvent *);

private:
    QMenu m_menu;
    Invoker *m_commandInvoker;
    QString m_pwd;
    QString m_whoami;
    QStringList m_history;
    int m_historyPosition;
    bool m_active;

    void addWhoami(bool newBlock);
    void historyWrite(const Command &command);
    void historyWrite(const QQueue<Command> &commands);
    void historyBack();
    void historyForward();
    void cursorToLastPos(bool withClearSelection = false);
    bool isCursorInLastBlock(const QTextCursor &cursor);
    QQueue<Command> parseClipboard();

signals:
    void command(composer_gui::console::command::Command command);
    void paste(QQueue<composer_gui::console::command::Command> commands);
};

} // console
} // composer_gui
