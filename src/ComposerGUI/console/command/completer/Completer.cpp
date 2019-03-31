#include <QAbstractItemView>
#include <QTextCursor>
#include <QTextBlock>
#include <QRect>
#include <QSize>

#include "globals.h"
#include "Completer.h"

namespace composer_gui
{
namespace console
{
namespace command
{
namespace completer
{

Completer::Completer(const QStringList &completions, QObject *parent)
    : QCompleter(completions, parent)
{
    popup()->setStyleSheet(COMPOSERGUI_MAIN_WINDOW_BACKGROUND);
}

bool Completer::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);

        switch (e->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Tab:
            {
                QAbstractItemView *hints = popup();
                if (hints->isVisible()) {
                    hints->hide();
                    if (hints->currentIndex().isValid()) {
                        hint(hints->currentIndex().data(completionRole()).toString());
                        return true;
                    }
                }
            }
                break;
            case Qt::Key_Up:
            case Qt::Key_Down:
                break;
            default:
                Console *source = static_cast<Console *>(widget());
                QString prefix = createCompletionPrefix(source, e);

                if (prefix.length() > 1 && isHintNeeded(source)) {
                    setCompletionPrefix(prefix);
                    popup()->setCurrentIndex(completionModel()->index(0,0));

                    QRect rect(source->cursorRect().bottomLeft(), QSize(100, 5));
                    complete(rect);
                } else if (popup()->isVisible()) {
                    popup()->hide();
                }
        }
    }

    return QCompleter::eventFilter(watched, event);
}

void Completer::hint(const QString &completion)
{
    Console *source = static_cast<Console *>(widget());
    QTextCursor cursor = source->textCursor();

    cursor.movePosition(QTextCursor::StartOfWord);
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    cursor.insertText(completion + ' ');

    source->setTextCursor(cursor);
}

bool Completer::isHintNeeded(Console *source)
{
    QTextCursor cursor = source->textCursor();
    QString blockContent = cursor.block().text().trimmed();
    blockContent.replace(qobject_cast<Console*>(source)->getWhoami(), "");
    if (blockContent.count(' ') == 0) {
        return true;
    }

    return false;
}

QString Completer::createCompletionPrefix(Console *source, QKeyEvent *event)
{
    QTextCursor cursor = source->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

    QString prefix = cursor.selectedText() + event->text();

    if (event->key() == Qt::Key_Backspace) {
        prefix.chop(2); // remove \b and backspaced character from prefix
    }

    return prefix;
}

} // completer
} // command
} // console
} // composer_gui
