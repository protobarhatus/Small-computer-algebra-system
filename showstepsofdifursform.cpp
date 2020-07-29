#include "showstepsofdifursform.h"
#include "ui_showstepsofdifursform.h"

ShowStepsOfDifursForm::ShowStepsOfDifursForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowStepsOfDifursForm)
{
    ui->setupUi(this);
}

ShowStepsOfDifursForm::~ShowStepsOfDifursForm()
{
    delete ui;
}

void ShowStepsOfDifursForm::setText(const QString &text)
{
    this->ui->textEdit->setHtml(text);
}
