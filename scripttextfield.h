#ifndef SCRIPTTEXTFIELD_H
#define SCRIPTTEXTFIELD_H
#include <QTextEdit>
#include <mainapplicationspace.h>
class ScriptTextField : public QTextEdit
{
    Q_OBJECT
public:
    ScriptTextField(QWidget * parent);
    void keyPressEvent(QKeyEvent *e) override;
    void subscribeApplicationSpace(MainApplicationSpace * space);
    void setHtml(QString text);

private:
    void deleteSystemMarks();
    bool holds_shift = false;

    MainApplicationSpace * space_subscribed;
    //[..)
    QString text_without_system_marks;

};

#endif // SCRIPTTEXTFIELD_H
