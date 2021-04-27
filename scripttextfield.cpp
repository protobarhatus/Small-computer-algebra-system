#include "scripttextfield.h"
#include <QKeyEvent>
#include <QDebug>
ScriptTextField::ScriptTextField(QWidget * parent) : QTextEdit(parent)
{

    auto font = this->font();
    font.setPointSize(15);
    this->setFont(font);

}
bool isCursorChangingKey(QKeyEvent * e)
{
    return e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Up || e->key() == Qt::Key_Down || e->key() == Qt::Key_Home ||
                e->key() == Qt::Key_End || e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown;
}
void ScriptTextField::keyPressEvent(QKeyEvent *e)
{
    //qDebug() << "released";
    if (!isCursorChangingKey(e))
        this->deleteSystemMarks();
    if (e->modifiers().testFlag(Qt::ShiftModifier))
    {
        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        {
           // QTextEdit::keyPressEvent(e);
            space_subscribed->executeCurrentScript(this->toPlainText().split("\n"));
            return;
        }
    }
    QTextEdit::keyPressEvent(e);
}

void ScriptTextField::subscribeApplicationSpace(MainApplicationSpace *space)
{
    this->space_subscribed = space;
}

void ScriptTextField::setHtml(QString text)
{

    bool is_inside_mark = false;
    this->text_without_system_marks = "";
    for (int i = 0; i < text.size(); ++i)
    {
        if (text[i] == systemUnreadableSymbol())
        {
            if (is_inside_mark)
            {
                is_inside_mark = false;
            }
            else
            {
                is_inside_mark = true;
            }
            text.remove(i, 1);
            --i;
        }
        else if (!is_inside_mark)
            this->text_without_system_marks += text[i];
    }
    this->text_without_system_marks.replace("<br>", "\n");
    QTextEdit::setHtml(text);
    auto new_cursor = this->textCursor();
    new_cursor.movePosition(QTextCursor::End);
    this->setTextCursor(new_cursor);
    this->update();

}

void ScriptTextField::deleteSystemMarks()
{
    if (this->text_without_system_marks == "")
        return;
    this->space_subscribed->getWindow()->clearDifursSolvings();

    this->setPlainText(this->text_without_system_marks);
    auto new_cursor = this->textCursor();
    new_cursor.movePosition(QTextCursor::End);
    this->setTextCursor(new_cursor);
    this->update();
    this->text_without_system_marks = "";
}