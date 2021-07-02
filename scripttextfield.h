#ifndef SCRIPTTEXTFIELD_H
#define SCRIPTTEXTFIELD_H
#include <QTextEdit>
#include <mainapplicationspace.h>
#include <QTextBrowser>
class ScriptTextField : public QTextEdit
{
    Q_OBJECT
public:
    ScriptTextField(QWidget * parent);
    void keyPressEvent(QKeyEvent *e) override;
    void subscribeApplicationSpace(MainApplicationSpace * space);
    void setHtml(QString text);

private:
    int countCursorPositionAtHtmlText() const;
    void deleteSystemMarks();
    bool holds_shift = false;

    MainApplicationSpace * space_subscribed;
    //[..)
    QString text_without_system_marks;
    QString html_tagged_text;

};

#endif // SCRIPTTEXTFIELD_H
