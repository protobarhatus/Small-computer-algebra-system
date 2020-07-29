#ifndef SHOWSTEPSOFDIFURSFORM_H
#define SHOWSTEPSOFDIFURSFORM_H

#include <QWidget>
#include <QDialog>
namespace Ui {
class ShowStepsOfDifursForm;
}

class ShowStepsOfDifursForm : public QDialog
{
    Q_OBJECT

public:
    explicit ShowStepsOfDifursForm(QWidget *parent = nullptr);
    ~ShowStepsOfDifursForm();
    void setText(const QString & text);
private:
    Ui::ShowStepsOfDifursForm *ui;
};

#endif // SHOWSTEPSOFDIFURSFORM_H
