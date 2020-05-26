#include "mainwindow.h"
#include <QApplication>
#include "algebra\algexpr.h"
#include "tests.h"
#include "algebra\some_algebra_expression_conversions.h"
#include "random"
#include "QDataStream"
//#define IN_MOSCOW
#ifdef IN_MOSCOW
#include <G:\QTProjects\mo2\ExprMake\texpr_builder.h>
#include <G:\QTProjects\mo2\testpaintwidget.h>
#endif
#define A(x) AlgExpr(x)
#define x2 pow(x, 2)
#define x3 pow(x, 3)
#define x4 pow(x, 4)
int rand(int min, int max)
{
   static std::random_device rt;
     static std::mt19937_64 gen(rt());
     std::uniform_int_distribution<int> dist(min, max);

     return dist(rt);
}
#include "algebra/solving_equations.h"
#include "algebra/abstractexpression.h"
#include "algebra/some_algebra_expression_conversions.h"
#include "algebra/polynom.h"
#include "algebra/polynomials_factorization.h"
void out(const Polynom & p)
{
    auto debug = qDebug();
    for (int i = p.deg();i >= 0; --i)
    {
        if (i != 0)
            debug << " + " << p[i].toInt() << "x^" << i;
        else
            debug << " + " << p[i].toInt();
    }
}
int main(int argc, char *argv[])
{
  //  testAlgMod();
    qDebug() << "#########";
   // GaluaField::initialize(11);
  //  Polynom p1({7, 0, 4, 0, 2, 1});
  //  Polynom p2({5, 0, 0, 2});
  //  auto xres = xea(p1, p2);
  //  out(xres.second.first);
  //  out(xres.second.second);

    AlgExpr x = var();
    AlgExpr y = var();
    AlgExpr z = var();

    std::vector<abs_ex> eqs(3);

    AlgExpr a = var();
    AlgExpr b = var();
    AlgExpr c = var();
    AlgExpr d = var();
    AlgExpr e = var();
    AlgExpr f = var();
    AlgExpr g = var();
    AlgExpr h = var();
    AlgExpr i = var();
    AlgExpr j = var();
    AlgExpr  k =var();
    AlgExpr l = var();

    AlgExpr t = var();
    //GaluaField::initialize(997);
    Polynom pol({112, 58, -31, 107, -66});
    auto res = factorize(pol);
    for (auto &it : res)
        out(it);
    /*eqs[0] = copy((a*x + 2*y + 3*z - 5).getExpr());
    eqs[1] = copy((5*x + a*y - 7*z - 13).getExpr());
    eqs[2] = copy((2*x -5*y  - 1*z - a).getExpr());
    auto res = solveSystemOfEquations(eqs, {1, 2, 3});
    qDebug() << res[0][0]->makeStringOfExpression();
    qDebug() << res[1][0]->makeStringOfExpression();
    qDebug() << res[2][0]->makeStringOfExpression();*/


    /*GaluaField::initialize(13);
    for (int i = 0; i < 13; ++i)
        qDebug() << GaluaField::reverse(i);
    qDebug() << "EINF";
    Polynom dividend({1, 0, 4, 12, 11, 6}), divider({1, 5, 7});
    auto res = divide(dividend, divider);
    for (int i = 0; i < res.first.size(); ++i)
       qDebug() << res.first[i].toInt();
    qDebug() << "-------";
    for (int i = 0; i < res.second.size(); ++i)
       qDebug() << res.second[i].toInt();
    Polynom mult_res = res.first * divider + res.second;
    if (mult_res == dividend)
        qDebug() << "Right";
    else
        qDebug() << "Hui";*/



    //c = (a + b) / 2;
   // TExpr_Builder builder;
   // auto expr = builder.BuildExpr("x^2+x+1");




//TODO: разобраться с ситуациями по типу
    //log(tan(x)) - (log(sin(x)) - log(cos(x)))
    //2log(cos(x)) - log(1-sin(x)^2)
    //ТАКЖЕ, если  синусы и косинусы раскрываются по сумме всегда сразу, то мы лишаемся
    //возможности взять интегралы вида 1/(sin(ax+b)cos(ax+d)), 1/(sin(ax+b)^2cos(ax+d)) и т. д.
    //это надо исправить

   //c = (13*a-25)/(a*a-10) - (a*a*a+45*a-73)/(a*a+41*a+93)*(7*a+15)/(a*a-10);

   // c.out();


// + 5*pow(x, 4) - pow(cos(x), 3)
  //  c = (3*sin(x)*((-1*
    //                pow(sin(x), 2)) + (1))) + ((4*((-1*sin(2*x)*sin(3)) + (cos(2*x)*
     //                cos(3)))*((cos(2*x)*sin(3)) + (cos(3)*sin(2*x)))));

    c = derivative(ln(3+4*x), x);
    c = sqrt(1-x*x);
    qDebug();
    qDebug() << "######################################################";
    qDebug() << c.toString();


    QApplication sdsd(argc, argv);
#ifdef IN_MOSCOW
    TestPaintWidget widg;
    widg.setFontSize(18);

    widg.setExpr(c.toString());
    widg.show();
#endif
    MainWindow w;


    return sdsd.exec();
}
