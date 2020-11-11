#include "showstepsofdifursform.h"
#include "ui_showstepsofdifursform.h"
#include <QVBoxLayout>
ShowStepsOfDifursForm::ShowStepsOfDifursForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowStepsOfDifursForm)
{
    ui->setupUi(this);

    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(this->ui->textEdit);

    this->setLayout(layout);

}

ShowStepsOfDifursForm::~ShowStepsOfDifursForm()
{
    delete ui;
}

void ShowStepsOfDifursForm::setText(const QString &text)
{
    this->ui->textEdit->setHtml(text);
}
