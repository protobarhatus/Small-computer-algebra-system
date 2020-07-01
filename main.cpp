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
#include "algebra/solving_differential_equations.h"
#include "algebra/solving_equations.h"
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

   // testAlgMod();
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

    //AlgExpr p1 = pow(x, 3)-2*pow(x, 2)-15*x+36;
    //AlgExpr p2 = pow(x,2)*3 - 4*x-15;
  //  auto gcf = gcd(static_cast<Polynomial*>(p1.getExpr().get()), static_cast<Polynomial*>(p2.getExpr().get()));
  //  qDebug() << gcf->makeStringOfExpression();
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

    AlgExpr dx = D(x);
    AlgExpr dy = D(y);
    //GaluaField::initialize(997);
    //Polynom pol({112, 58, -31, 107, -66});
   // Polynom pol({1, 12, -22, -163, 309, -119});
   // Polynom pol({1, -7, 10, 26, -60});


   // Polynom pol({1, -3, -7, 12, -69, 502, -727, 1615, -4683, 2754, -5535, 8100});
    //Polynom pol({1, -2, -15, 36});
   /* Polynom pol(256);
    pol[256] = 1;
    pol[1] = -1;
    pol[0] = 1;*/

    //Polynom pol({1, 0, 0, 0, -1, -15});
  /*  auto res = factorize(pol);
    for (auto &it : res)
        out(it);*/
/*
    AlgExpr p = 112*pow(x, 4) + 58 * pow(x, 3) - 31*x*x + 107 * x - 66;
    c = (x-3)*(x+2)/p;
    static_cast<Fractal*>(c.getExpr().get())->bringRationalFunctionIntoFormToDecay();
    auto sres = static_cast<Fractal*>(c.getExpr().get())->splitIntoSumOfElementaryFractals();
    for (auto &it : sres)
        qDebug() << it->makeStringOfExpression();*/
    /*eqs[0] = copy((a*x + 2*y + 3*z - 5).getExpr());
    eqs[1] = copy((5*x + a*y - 7*z - 13).getExpr());
    eqs[2] = copy((2*x -5*y  - 1*z - a).getExpr());
    auto res = solveSystemOfEquations(eqs, {1, 2, 3});
    qDebug() << res[0][0]->makeStringOfExpression();
    qDebug() << res[1][0]->makeStringOfExpression();
    qDebug() << res[2][0]->makeStringOfExpression();*/

//auto res = factorizePolynom((a*a*a + 3*a*a*b + 3*a*a + 3*a*b*b + 6*a*b + b*b*b + 3*b*b).getExpr()).first;

    //c = (a + b) / 2;
   // TExpr_Builder builder;
   // auto expr = builder.BuildExpr("x^2+x+1");

    //!!разобраться с этой хренью. то же самое с логарифмами
    //c = sqrt(-x*y) / sqrt(-y); c = sqrt(-x*y) + sqrt(-y)


//TODO: разобраться с ситуациями по типу
    //2log(cos(x)) - log(1-sin(x)^2)
    //ТАКЖЕ, если  синусы и косинусы раскрываются по сумме всегда сразу, то мы лишаемся
    //возможности взять интегралы вида 1/sin(ax+b),  1/(sin(ax+b)cos(ax+d)), 1/(sin(ax+b)^2cos(ax+d)) и т. д.
    //это надо исправить




    //auto res = solveEquation(ln(abs(x)) + ln(abs(y)) + c, y);
    //auto res = solveEquation((2*pow(x, 4) - 3*pow(x, 3) - x*x -3*x + 2), x);
    //auto res = solveEquation((2*x4 - 15*x3 + 35*x2 - 30* x + 8 ), x);
   // auto res = solveEquation(x + 2*sqrt(x)-15, x);
    //auto res = solveEquation(x4 - 4*x2 + 3, x);
   // auto res = solveEquation(pow(2, 2*x) - 8, x);
    //auto res = solveEquation(pow(2, 2*x) + pow(2, x) + 2 - a, x);
   // auto res = solveEquation(2*ln(y) - ln(3) + ln(x), x);
   // auto res = solveEquation(x4 - 4*x3 + 6*x2 - 4*x - 2, x);
   // auto res = solveEquation(abs(2*x + 1) + abs(x - 3) - 5, x);
   // auto res = solveEquation(sin(2*x) - A(1)/2, x);
   // auto res = solveEquation(sin(2*x + 1) - sin(x - 3), x);
   // auto res = solveEquation(ln(x - 3) + ln(x + 3), x);
    //auto res = solveEquation(pow(euler(), a)/abs(x) - abs(y), y);
    //auto res = solveEquation(ln(abs(x)) - ln(abs(x+1)) - ln(abs(y - 3)) + integratingConstantExpr(), y);
//auto res = solveEquation(ln(abs(ln(y))) + ln(abs(x)) + std::move(integratingConstantExpr()), y);
//res.begin()->getExpr()->setSimplified(false);
//res.begin()->getExpr()->simplify();

  //   auto res = solveDifur(dx*y - dy*x, x, y);
   // auto res = solveDifur(y - dy*x/dx, x, y);
   // auto res = solveDifur(dy/dx + 2*y, x, y);                       //y = C/e^(2x),  C in R
   // auto res = solveDifur(dy/dx + (2*y + 1)*cot(x), x, y);             // y = -1/2 (sin(x)^2 + C)/sin(x)^2,  C in R
   // auto res = solveDifur(y*ln(y) + x*dy/dx, x, y);                // y = 1/C^(1/|x|), C in (1; inf), y = C^(1/|x|), C in (1; inf), y = 1
   // auto res = solveDifur(pow(euler(),(y - x*x))*dy -2*x*dx, x, y);   //ln(e^x^2 + C), C in R
   // auto res = solveDifur(sqrt(x*y - 2*x)*dy/dx + x*y*y + 5*y*y, x, y);
   // auto res = solveDifur(2*(x*y + y)*dy/dx + x*(pow(y, 4) + 1), x, y);  //-1*ln(|1 + x|) + x + atan(y^2) + C3  = 0  and  C3 is in  R
    //auto res = solveDifur(2*dy/dx *sin(y) *cos(y)*pow(sin(x), 2) + cos(x), x, y);
    //auto res = solveDifur((1 + pow(euler(), x))*y*dy - pow(euler(), y)*dx, x, y);               //(-1 - y - x*e^y + ln(1 + e^x)*e^y + e^y*C) /e^y  = 0  and  C is in  R
    auto res = solveDifur(y - x*dy/dx - 3*(1 + x*x*dy/dx), x, y);
    //auto res = solveDifur(x*dy/dx - (y - x*pow(euler(), y/x)), x, y);

  //  auto res = solveDifur(x*dy/dx - 2*sqrt(x*x + y*y) - y, x, y);
   /* y = (pow(x, 4) - 1)/2/x;
    dy = D(y);
    c = x*dy/dx - 2*sqrt(x*x + y*y) - y;*/
    qDebug() << "RES: ";
    for (auto &it : res)
        qDebug() << it.toString();

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
