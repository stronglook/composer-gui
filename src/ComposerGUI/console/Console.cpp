#include <QApplication>
#include <QWidget>
#include <QTextBlock>
#include <QScrollBar>
#include <QClipboard>
#include <QDir>
#include <QPoint>
#include <QTextDocumentFragment>

#include "Console.h"

namespace composer_gui
{
namespace console
{

Console::Console(const QString &whoami, Invoker *invoker, QWidget *parent) :
    QPlainTextEdit(parent),
    m_commandInvoker(invoker),
    m_pwd(QDir::homePath()),
    m_whoami(whoami),
    m_historyPosition(0),
    m_active(true)
{
    m_commandInvoker->setParent(this);

    setContextMenuPolicy(Qt::CustomContextMenu);

    m_menu.addAction("Copy", [this]
    {
        QString selection = textCursor().selection().toPlainText();
        QApplication::clipboard()->setText(selection);

        cursorToLastPos(true);
    });

    m_menu.addSeparator();

    m_menu.addAction("Paste", [this]
    {
        auto commands = parseClipboard();
        historyWrite(commands);
        emit paste(commands);
    });

    connect(this, &Console::customContextMenuRequested, [this](const QPoint &pos)
    {
        m_menu.exec(mapToGlobal(pos));
    });
}

command::invoker::Invoker *Console::getCommandInvoker()
{
    return m_commandInvoker;
}

void Console::addWhoami(bool newBlock)
{
    addText(m_whoami, newBlock);
    scrollDown();
}

void Console::addWhoami()
{
    bool newBlock = blockCount() == 1 ? false : true;
    addWhoami(newBlock);
}

void Console::setTextColor(const char *color)
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    cursor.setCharFormat(format);
    setTextCursor(cursor);
}

void Console::setWorkingDirectory(const QString &directory)
{
    m_pwd = directory;
}

const QString &Console::getWorkingDirectory() const
{
    return m_pwd;
}

void Console::addText(const QString &text, bool newBlock)
{
    QTextCursor cursor = textCursor();
    if (newBlock) {
        cursor.insertBlock();
    }
    cursor.insertText(text);
    scrollDown();
}

void Console::setActive(bool active)
{
    m_active = active;
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void Console::historyBack()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(m_whoami + m_history.at(--m_historyPosition));
    setTextCursor(cursor);
}

void Console::historyForward()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();

    QString command = (m_historyPosition == m_history.length() - 1) ? "" : m_history.at(++m_historyPosition);
    cursor.insertText(m_whoami + command);
    setTextCursor(cursor);
}

QQueue<command::Command> Console::parseClipboard()
{
    QQueue<Command> commands;
    QString clipboard = QApplication::clipboard()->text();
    if (!clipboard.contains('\n')) {
        commands.enqueue(Command(clipboard, m_pwd));
    } else {
        for (const QString& cmd : clipboard.split('\n'))
        {
           commands.enqueue(Command(cmd.trimmed(), m_pwd));
        }
    }

    return commands;
}

const QString &Console::getWhoami() const
{
    return m_whoami;
}

void Console::execute(const QString &sCommand)
{
    addText(sCommand, false);

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
    QApplication::postEvent(this, event);
}

void Console::historyWrite(const Command &command)
{
    m_history.append(command.getFullname());
    m_historyPosition = m_history.length();
}

void Console::historyWrite(const QQueue<Command> &commands)
{
    for (const Command& cmd : commands)
    {
       historyWrite(cmd);
    }
}

void Console::cursorToLastPos(bool withClearSelection)
{
    QTextCursor cursor = textCursor();
    if (withClearSelection) {
        cursor.clearSelection();
    }

    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
}

bool Console::isCursorInLastBlock(const QTextCursor &cursor)
{
    return cursor.block().blockNumber() == blockCount() - 1;
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (!m_active || (event->key() == Qt::Key_Control)) {
        return;
    }

    QTextCursor cursor = textCursor();

    if (event->matches(QKeySequence::Copy) || event->matches(QKeySequence::Paste)) {
        return;
    } else if (!isCursorInLastBlock(cursor)) {
        cursorToLastPos();
    } else {

        switch (event->key())
        {
            case Qt::Key_Up:
                if (m_historyPosition > 0) {
                    historyBack();
                }
            break;
            case Qt::Key_Down:
                if(m_historyPosition < m_history.length()) {
                    historyForward();
                }
            break;
            case Qt::Key_Backspace:
            case Qt::Key_Left:
                if (cursor.positionInBlock() > m_whoami.size()) {
                    QPlainTextEdit::keyPressEvent(event);
                }
            break;
            case Qt::Key_Enter:
            case Qt::Key_Return:
            {
                QString sCmd = cursor.block().text().replace(m_whoami, "");
                Command cmd(sCmd, m_pwd);

                historyWrite(cmd);
                cursorToLastPos();

                emit command(cmd);
            }
            break;
            default:
                QPlainTextEdit::keyPressEvent(event);
        }
    }
}

void Console::contextMenuEvent(QContextMenuEvent *){}

} // console
} // composer_gui
