#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commandresponse.h"
#include <QScrollArea>
namespace MathExpressions {class AbstractExpression;}

class ScriptTextField;
class MainApplicationSpace;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMainApplicationSpace(MainApplicationSpace * space);
    void outResult(const std::vector<CommandResponse> & answers, const std::vector<QString> & input_text);
    void clearDifursSolvings();
    void addDifurSolving(const std::pair<QString, QString> & solving);
protected:

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_show_equation_solving_steps_clicked();

    void on_render_button_toggled(bool checked);

    void on_wolfram_button_toggled(bool checked);

private:
    std::vector<std::pair<QString, QString>> difurs_steps;
    QScrollArea * scroll_area;
    std::vector<ScriptTextField*> text_fields;
    bool render_as_image = true;
    bool on_add_tab = false;
    Ui::MainWindow *ui;
    MainApplicationSpace * application_space;
    void addScript();
    void changeScript(int index);
    bool finished = false;
    ScriptTextField * current_input_field;
};
#endif // MAINWINDOW_H
