#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showstepsofdifursform.h"
#include <QVBoxLayout>
#include "script.h"
#include "scripttextfield.h"
#include "expressions_drawer/abstract_expression.h"
#include "expressions_drawer/expression_builder.h"
#include <QPainter>
#include <QScrollArea>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->show_equation_solving_steps->setVisible(false);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(this->ui->tabWidget);
    QHBoxLayout * hlt = new QHBoxLayout;
    hlt->addWidget(this->ui->label);
    hlt->addWidget(this->ui->render_button);
    hlt->addWidget(this->ui->wolfram_button);
    layout->addLayout(hlt);
   // layout->addWidget(this->ui->input_field);
    layout->addWidget(this->ui->output_field);
    scroll_area = new QScrollArea;
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(this->ui->output_image);
    layout->addWidget(scroll_area);
    //layout->addWidget(this->ui->output_image);
    layout->addWidget(this->ui->show_equation_solving_steps);
    this->ui->output_field->setVisible(false);
    this->ui->centralwidget->setLayout(layout);


    this->finished = true;

    //
    //this->addScript();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMainApplicationSpace(MainApplicationSpace *space)
{
    this->application_space = space;
    //this->ui->input_field->subscribeApplicationSpace(space);
    //this->current_input_field = this->ui->input_field;
    this->text_fields.push_back(current_input_field);
    this->addScript();

}

void MainWindow::outResult(const std::vector<CommandResponse> & answers, const std::vector<QString> & input_text)
{

    MathExpressions::ExpressionBuilder builder;
    builder.variableAutoIndex(true);

    builder.functionAutoIndex(true);
    builder.postSymbols(true);
    builder.wrapEmptyExpressionToFrame(true);
    std::vector<std::unique_ptr<MathExpressions::AbstractExpression>> expr_answ;
    for (auto &it : answers)
    {
        if (it.type() == CommandResponse::ACCEPTED_INPUT)
        {
            QString mes = it.getMessage()[OUTPUT_STRING_RENDER_STRING];
            mes.replace(" ", "");
            expr_answ.push_back( builder.exec(mes));
            QFont font;
            font.setFamily("Cambria Math");
            font.setPointSize(18);
            expr_answ.back()->setFont(font);
        }
    }

    this->ui->output_image->setExpressions(std::move(expr_answ));
    this->ui->output_image->repaint();
    //this->layout()->update();

    QString output_text, new_input_text;
    for (int i = 0; i < answers.size(); ++i)
    {

        if (answers[i].type() == CommandResponse::ACCEPTED_INPUT)
        {
            QString answer = answers[i].getMessage()[OUTPUT_STRING_WOLFRAM_STRING];
            QString input_n;
            input_n.setNum(answers[i].inputNumber());
            output_text += "<small><font color=\"gray\">out[" + input_n + "]</font></small>   " + answer + "<br>";
            new_input_text += QString(systemUnreadableSymbol())+"<small><font color=\"gray\">in[" + input_n + "]</font></small>   " +
                    QString(systemUnreadableSymbol()) + input_text[i] + "<br>";
        }
        else if (answers[i].type() == CommandResponse::ERROR)
        {
            new_input_text += input_text[i] + "<br>" + QString(systemUnreadableSymbol()) + "<small><font color=\"red\">" +
                    answers[i].getMessage()[0] + "</font></small><br>" + QString(systemUnreadableSymbol());
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

void MainWindow::on_render_button_toggled(bool checked)
{
    if (checked)
    {
        this->ui->output_image->setVisible(true);
        this->ui->output_field->setVisible(false);
        this->scroll_area->setVisible(true);
    }
}


void MainWindow::on_wolfram_button_toggled(bool checked)
{
    if (checked)
    {
        this->ui->output_image->setVisible(false);
        this->ui->output_field->setVisible(true);
        this->scroll_area->setVisible(false);
    }
}

