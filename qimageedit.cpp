#include "qimageedit.h"
#include <QPainter>
#include <QDebug>
#include "expressions_drawer/abstract_expression.h"
#include "expressions_drawer/expression_builder.h"
QImageEdit::QImageEdit(QWidget * parent) : QWidget(parent)
{

}

void QImageEdit::setExpressions(std::vector<std::unique_ptr<MathExpressions::AbstractExpression> > &&exprs)
{
    this->exprs = std::move(exprs);
    updated = true;
}


void QImageEdit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    int act_width = 0, act_height = 0;
    int y = 5;
    for (int i = 0; i < this->exprs.size(); ++i)
    {
        if (updated)
            this->setMinimumSize(act_width + 3000, act_height + 1000);
        MathExpressions::ExpressionBuilder builder;
        auto num_expr = builder.exec("String(\"out [" + QString::number(i) + "]\")");
        QFont font;
        font.setFamily("Computer Modern");
        font.setPointSize(14);
        num_expr->setFont(font);
        num_expr->setColor(QColor::fromRgb(169, 169, 169));
        num_expr->setPaintDevice(painter.device());
        num_expr->draw(5, y+2, Qt::AlignRight);
        exprs[i]->setPaintDevice(painter.device());
        exprs[i]->draw(10 + num_expr->width(), y, Qt::AlignRight);
        y += exprs[i]->height() + 5;

        act_height = y;
        act_width = std::max(act_width, num_expr->width() + exprs[i]->width() + 10);
        if (updated)
            this->setMinimumSize(act_width, act_height);

    }
    updated = false;
}
