#ifndef QIMAGEEDIT_H
#define QIMAGEEDIT_H
#include <QWidget>
#include <functional>
namespace MathExpressions {class AbstractExpression;}
class QImageEdit : public QWidget
{
    Q_OBJECT
public:
    QImageEdit(QWidget * parent = 0);
    void setExpressions(std::vector<std::unique_ptr<MathExpressions::AbstractExpression>>&& exprs);
protected:
    bool updated = false;
    void paintEvent(QPaintEvent * event);
    std::vector<std::unique_ptr<MathExpressions::AbstractExpression>> exprs;
};

#endif // QIMAGEEDIT_H
