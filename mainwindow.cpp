#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showstepsofdifursform.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->show_equation_solving_steps->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMainApplicationSpace(MainApplicationSpace *space)
{
    this->application_space = space;
    this->ui->input_field->subscribeApplicationSpace(space);
    this->current_input_field = this->ui->input_field;
    this->text_fields.push_back(current_input_field);
}

void MainWindow::outResult(const std::vector<CommandResponse> & answers, const std::vector<QString> & input_text)
{
    QString output_text, new_input_text;
    for (int i = 0; i < answers.size(); ++i)
    {
        if (answers[i].type() == CommandResponse::ACCEPTED_INPUT)
        {
            QString input_n;
            input_n.setNum(answers[i].inputNumber());
            output_text += "<small><font color=\"gray\">out[" + input_n + "]</font></small>   " + answers[i].getMessage() + "<br>";
            new_input_text += QString(systemUnreadableSymbol())+"<small><font color=\"gray\">in[" + input_n + "]</font></small>   " +
                    QString(systemUnreadableSymbol()) + input_text[i] + "<br>";
        }
        else if (answers[i].type() == CommandResponse::ERROR)
        {
            new_input_text += input_text[i] + "<br>" + QString(systemUnreadableSymbol()) + "<small><font color=\"red\">" +
                    answers[i].getMessage() + "</font></small><br>" + QString(systemUnreadableSymbol());
        }
        else
            new_input_text += input_text[i] + "<br>";
    }
    int take_steps_index = -1;
    while ((take_steps_index = output_text.indexOf("#^")) >= 0)
    {
        int ind_of_end_eq = output_text.indexOf("#>");
        QString equation = output_text.mid(take_steps_index + 3,  ind_of_end_eq - (take_steps_index + 3));
        int ind_of_end_steps = output_text.indexOf("#$");
        QString steps = output_text.mid(ind_of_end_eq + 2, ind_of_end_steps - (ind_of_end_eq + 2));
        this->addDifurSolving({equation, steps});
        output_text.remove(take_steps_index, ind_of_end_steps + 2 - take_steps_index);
    }

    this->ui->output_field->setHtml(output_text);
    this->current_input_field->setHtml(new_input_text);
    this->update();
}

void MainWindow::clearDifursSolvings()
{
    this->difurs_steps.clear();
    this->ui->show_equation_solving_steps->setVisible(false);
}

void MainWindow::addDifurSolving(const std::pair<QString, QString> &solving)
{
    this->ui->show_equation_solving_steps->setVisible(true);
    this->difurs_steps.push_back(solving);
}



void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (on_add_tab)
        return;
    if (index == this->ui->tabWidget->count() - 1)
        addScript();
    else
        changeScript(index);
}

void MainWindow::addScript()
{
    on_add_tab = true;
    int num = this->ui->tabWidget->count();
    QString tnum;
    tnum.setNum(num);
    ScriptTextField* new_field = new ScriptTextField(this->ui->tabWidget);
    new_field->subscribeApplicationSpace(this->application_space);
    this->ui->tabWidget->removeTab(this->ui->tabWidget->count() - 1);
    this->ui->tabWidget->addTab(new_field, "Script" + tnum);
    this->ui->tabWidget->addTab(new ScriptTextField(this->ui->tabWidget), "New List");
    this->ui->tabWidget->setCurrentIndex(this->ui->tabWidget->count() - 2);
    this->current_input_field = new_field;
    this->text_fields.push_back(new_field);

    this->application_space->addScript();
    on_add_tab = false;

    this->clearDifursSolvings();
}

void MainWindow::changeScript(int index)
{
    this->current_input_field = text_fields[index];
    this->application_space->changeScript(index);

    this->clearDifursSolvings();
}

void MainWindow::on_show_equation_solving_steps_clicked()
{
    if (this->difurs_steps.size() == 0)
        return;
    ShowStepsOfDifursForm form;
    form.setText(this->difurs_steps[0].second);
    form.exec();

}
