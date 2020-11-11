#include "mainwindow.h"
#include <QApplication>
#include "algebra\algexpr.h"
#include "tests.h"
#include "algebra\some_algebra_expression_conversions.h"
#include "random"
#include "QDataStream"
#include <mainapplicationspace.h>
#define IN_MOSCOW
#ifdef IN_MOSCOW
#include <G:\QTProjects\mo2\ExprMake\texpr_builder.h>
#include <G:\QTProjects\mo2\testpaintwidget.h>

#endif
#define A(x) AlgExpr(x)
#define x2 pow(x, 2)
#define x3 pow(x, 3)
#define x4 pow(x, 4)
#define y2 pow(y, 2)
#define y3 pow(y, 3)
#define y4 pow(y, 4)

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
#include "algebra/exception.h"
#include "form.h"
#include "parser.h"
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
#include <QException>
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
    AlgExpr k =var();
    AlgExpr l = var();

    AlgExpr t = var();

    AlgExpr dx = D(x);
    AlgExpr dy = D(y);



 //   try {
 //       c = (root(7*63*63*63, A(12))/(root(7, 3)+2));
//
  //  } catch (Exception ) {
  //      qDebug() << "Fuck";
  //  }












    //GaluaField::initialize(997);
    //Polynom pol({112, 58, -31, 107, -66});
   // Polynom pol({1, 12, -22, -163, 309, -119});
   // Polynom pol({1, -7, 10, 26, -60});

//integral(pow(3, a*x + b), x);

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
    /*eqs[0] = copy((2*x/7 + y/b + 3*z - 5).getExpr());
    eqs[1] = copy((5*x + 2*y - 7*z - 13).getExpr());
    eqs[2] = copy((2*x -5*y  - 1*z +22).getExpr());
    auto res = solveSystemOfEquations(eqs, {1, 2, 3});
    qDebug() << res[0][0]->makeStringOfExpression();
    qDebug() << res[1][0]->makeStringOfExpression();
    qDebug() << res[2][0]->makeStringOfExpression();*/

//auto res = factorizePolynom((a*a*a + 3*a*a*b + 3*a*a + 3*a*b*b + 6*a*b + b*b*b + 3*b*b).getExpr()).first;

    //c = (a + b) / 2;
   // TExpr_Builder builder;
   // auto expr = builder.BuildExpr("x^2+x+1");

  //  c = integral(1/(3-5*cos(x)), x);

    //!!разобраться с этой хренью. то же самое с логарифмами
    //c = sqrt(-x*y) / sqrt(-y); c = sqrt(-x*y) + sqrt(-y)

//TODO: разобраться с ситуациями по типу
    //2log(cos(x)) - log(1-sin(x)^2)
    //ТАКЖЕ, если  синусы и косинусы раскрываются по сумме всегда сразу, то мы лишаемся
    //возможности взять интегралы вида 1/sin(ax+b),  1/(sin(ax+b)cos(ax+d)), 1/(sin(ax+b)^2cos(ax+d)) и т. д.
    //это надо исправить

    //c = (sin(2*x)/y + x)*dx + (y - sin(x)*sin(x)/y/y)*dy;

   // c = integral(x*x*sqrt(1+x2), x);
   // c = (1/x2) * abs(x);

   //c = integral(pow(cos(x), 3)/cbrt(pow(sin(x), 4)), x);
    //c = sin(acos(sqrt(1 - sin(x)*sin(x))));
  //  c = integral(sin(x)/pow(cos(x), 2), x) - (2/((-1*tan((1*x/2)))+(1))/((1)+(tan((1*x/2)))));

   // auto res = solveEquation(ln(abs(x)) + ln(abs(y)) + c, y);
   // auto res = solveEquation((2*pow(x, 4) - 3*pow(x, 3) - x*x -3*x + 2), x);
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
 //  auto res = solveEquation(-ln(abs(-y+x)) - ln(abs(x)) + ln(abs(y)) + integratingConstantExpr(), y);
//res.begin()->getExpr()->setSimplified(false);
//res.begin()->getExpr()->simplify();

    // auto res = solveDifur(dx*y - dy*x, x, y);
    //auto res = solveDifur(y - dy*x/dx, x, y);
    // auto  res = solveDifur(x * derivObj(y, x, 1) - y, x, y);

   // auto res = solveDifur(dy/dx + 2*y, x, y);                       //y = C/e^(2x),  C in R
   // auto res = solveDifur(dy/dx + (2*y + 1)*cot(x), x, y);             // y = -1/2 (sin(x)^2 + C)/sin(x)^2,  C in R
    //auto res = solveDifur(y*ln(y) + x*dy/dx, x, y);                // y = 1/C^(1/|x|), C in (1; inf), y = C^(1/|x|), C in (1; inf), y = 1
   // auto res = solveDifur(pow(euler(),(y - x*x))*dy -2*x*dx, x, y);   //ln(e^x^2 + C), C in R
   // auto res = solveDifur(sqrt(x*y - 2*x)*dy/dx + x*y*y + 5*y*y, x, y);
   // auto res = solveDifur(2*(x*y + y)*dy/dx + x*(pow(y, 4) + 1), x, y);  //-1*ln(|1 + x|) + x + atan(y^2) + C3  = 0  and  C3 is in  R
    //auto res = solveDifur(2*dy/dx *sin(y) *cos(y)*pow(sin(x), 2) + cos(x), x, y);
    //auto res = solveDifur((1 + pow(euler(), x))*y*dy - pow(euler(), y)*dx, x, y);               //(-1 - y - x*e^y + ln(1 + e^x)*e^y + e^y*C) /e^y  = 0  and  C is in  R
    //auto res = solveDifur(y - x*dy/dx - 3*(1 + x*x*dy/dx), x, y);      //y = -1 (-3*|1 + 3*x| + x*C28) /|1 + 3*x|  and  C28 is in  R



   // auto res = solveDifur(x*dy/dx - (y - x*pow(euler(), y/x)), x, y);      //y = -1*x*ln(ln(|x|) + C10)  and  C10 is in  R
   // auto res = solveDifur(x*dy/dx - 2*sqrt(x*x + y*y) - y, x, y);
 //  auto res = solveDifur(x*dy/dx + 2*sqrt(x*y) - y, x, y);    //НЕ хватает ограничения в системе, нужно будет дополнить позже
  //  auto res = solveDifur((y*y - 2*x*y)*dx + x*x*dy, x, y);
   // auto res = solveDifur(y*y + x2*dy/dx - x*y*dy/dx, x, y);


  //  auto res = solveDifur(dy/dx - y - pow(euler(), x), x, y);
   // auto res = solveDifur(dy/dx + y*tan(x)-1/cos(x), x, y);  //y = -1 (-1*sin(x) + cos(x)*C3)   and  C3 is in  R
  //  auto res = solveDifur(dy/dx + y/x - 2*pow(euler(), x2), x, y);   //y = -1 (-1*e^x^2 + C5) /x  and  C5 is in  R
   // auto res = solveDifur(dy/dx - 2*y/(x + 1) - pow(x + 1, 3), x, y);    //y = -1/2 (-5*x^2 - 4*x^3 - 2*x - x^4 + C5 + x^2*C5 + 2*x*C5)   and  C5 is in  R
   // auto res = solveDifur(x2*dy/dx - 2*x*y - 3, x, y);            //y = -1 (1 + x^3*C6) /x  and  C6 is in  R
    //auto res = solveDifur(x*dy/dx + (x + 1)*y - 3*x2*pow(euler(), -x), x, y);  //y = -1 (-1*x^3 + C5) /x/e^x  and  C5 is in  R

    //почти на каждом здесь должно быть y = 0 в качестве корня
    //auto res = solveDifur(dy/dx - 2*x*y - 2*x3*y2, x, y);  //y = -1*e^x^2/(-1*e^x^2 + e^x^2*x^2 + C5)  and  C5 is in  R
    //auto res = solveDifur(dy/dx + y/(x + 1) + y2, x, y);   //y = 1/(ln(|1 + x|) + C3 + x*ln(|1 + x|) + x*C3)  and  C3 is in  R
   // auto res = solveDifur(dy/dx - 2*y/x - 2*x*sqrt(y), x, y);    //y = -1/4*x^2 (-4*x^2 - C4^2 + 4*x*C4)   and  C4 is in  R
    //auto res = solveDifur(x*dy/dx - 4*y -x2*sqrt(y), x, y);         //y = -1/4*x^4 (-1*ln(|x|)^2 - C4^2 + 2*ln(|x|)*C4)   and  C4 is in  R
    //auto res = solveDifur(dy/dx + 3*y/x - x3*y3, x, y);          //y = -1/sqrt(1 + x^2*C5)/x^2  and  C5 is in  R
    //auto res = solveDifur(2*dy/dx - 3*y*cos(x) + pow(euler(), -2*x) *(2+ 3*cos(x))/y, x, y); //y = e^(3/2*sin(x))*sqrt(e^(-3*sin(x) - 2*x) + C8)  and  C8 is in  R


   // auto res = solveDifur((2*x - y + 1)*dx + (2*y - x - 1)*dy , x, y);  //y = 1/2 (1 + x + sqrt(-3*x^2 - 2*x + C3))   and  C3 is in  R"   "y = 1/2 (-1*sqrt(-3*x^2 - 2*x + C5) + 1 + x)   and  C5 is in  R
  //  auto res = solveDifur((3*x2 - 3*y2 + 4*x)*dx - (6*x*y + 4*y)*dy, x, y); //y = sqrt(2*C2 + 3*x^4 + 3*x*C2 + 4*x^2 + 8*x^3)*C7/(2 + 3*x)  and  C2 is in  R  and  C7 is in  { -1 } U { 1 }
   // auto res = solveDifur((6*y - 3*x2 + 3*y2)*dx + (6*x + 6*x*y)*dy, x, y);    //y =  (sqrt(x)*C7 + sqrt(x^3 + 3*x + C3)*C9) *C11/sqrt(x)  and  C3 is in  R  and  C7 is in  { -3 } U { sqrt(3) }  and  C9 is in  { sqrt(3) } U { 3 }  and  C11 is in  { -1/3 } U { 1/3 }
    //auto res = solveDifur(2*x*(1 - pow(euler(), y))*dx/pow(1 + x2, 2) + pow(euler(), y)*dy/(1 + x2), x, y);  //"y = ln(C13 + x^2*C13 + C14)  and  C13 is in  (-inf; 0)  and  C14 is in  { -1 } U { 1 }"  "y = ln(-1*C11 - x^2*C11 + 1)  and  C11 is in  (-inf; 0)"  "y = 0"
  //  auto res = solveDifur((sin(2*x)/y + x)*dx + (y - sin(x)*sin(x)/y/y)*dy, x, y);  //(-1*cos((2*x)) + 1 + y^3 + y*x^2 + y*C) /y  = 0  and  C is in  R


   // auto res = solveDifur(x * derivObj(y, x, 1) - y, x, y);

   // auto res = solveDifur(derivObj(y, x, 5) - 1, x, y);  //y = -1/120 (-1*x^5 + x*C17 + x^2*C15 + x^3*C13 + x^4*C11 + C18)   and  C11 is in  R  and  C13 is in  R  and  C15 is in  R  and  C17 is in  R  and  C18 is in  R
   // auto res = solveDifur(derivObj(y, x, 2) + derivObj(y, x, 1)/(x + 1) - 9*(x + 1), x, y);     //y = -1 (-3*x - 3*x^2 - x^3 + ln(|1 + x|)*C4 + 3*ln(|1 + x|) + C8)   and  C4 is in  R  and  C8 is in  R
   // auto res = solveDifur((y - 1)*derivObj(y, x, 2) - 2*pow(derivObj(y, x, 1), 2), x, y);
   // auto res = solveDifur(derivObj(y, x, 2) + derivObj(y, x, 1) - 2, x, y); //y = -1 (-2*x*e^x + e^x*C36 + C37) /e^x  and  C36 is in  R  and  C37 is in  R


   // auto res = solveDifur(derivObj(y, x, 2) + derivObj(y, x, 1) - 2*y, x, y);
  //  auto res = solveDifur(derivObj(y, x, 2) - 6*derivObj(y, x, 1) + 9*y, x, y);  //y = e^(3*x) (x*C1 + C)   and  C is in  R  and  C1 is in  R
   // auto res = solveDifur(derivObj(y, x, 2) - 2*derivObj(y, x, 1) + 10*y, x, y);

    //auto res = solveDifur(derivObj(y, x, 2) + derivObj(y, x, 1) + y - 2, x, y);
  //  auto res = solveDifur(derivObj(y, x, 2) - 4*derivObj(y, x, 1) + 5*y - pow(euler(), 2*x)/cos(x), x, y);  //y = e^(2*x) (x*sin(x) + cos(x)*C2 + sin(x)*C3 + ln(|cos(x)|)*cos(x))   and  C2 is in  R  and  C3 is in  R
    //auto res = solveDifur(derivObj(y, x, 2) - 4*derivObj(y, x, 1) - 8 + 16*x, x, y);  //y = -1/4 (-8*x^2 + e^(4*x)*C11 + 4*x + C12)   and  C11 is in  R  and  C12 is in  R
    //auto res = solveDifur(derivObj(y, x, 2) - 6*derivObj(y, x, 1) + 10*y - 51*pow(euler(), -x), x, y); //y = -1 (-3 + cos(x)*e^(4*x)*C8 + sin(x)*e^(4*x)*C9) /e^x  and  C8 is in  R  and  C9 is in  R
   // auto res = solveDifur(derivObj(y, x, 2) - 6 * derivObj(y, x, 1) + 9*y - pow(euler(), 3*x), x, y);  //y = -1/2*e^(3*x) (-1*x^2 + x*C9 + C8)   and  C8 is in  R  and  C9 is in  R
    //auto res = solveDifur(derivObj(y, x, 2) - 2*derivObj(y, x, 1) + 5*y - 21*cos(2*x) +sin(2*x) ,x, y);  //y = -1/4 (-4*cos(2*x) + cos(2*x)*e^x*C8 + sin(2*x)*e^x*C9 + 20*sin(2*x))   and  C8 is in  R  and  C9 is in  R
   // auto res = solveDifur(derivObj(y, x, 2) + y - 2*cos(x), x, y); //y = -1/2 (-2*x*sin(x) - cos(x) + cos(x)*C8 + sin(x)*C9)   and  C8 is in  R  and  C9 is in  R
    //auto res = solveDifur(derivObj(y, x, 2) + 9*y - 2*x*sin(3*x), x, y);  //y = -1/11664 (-648*x*sin(3*x) - 108*cos(3*x) + cos(3*x)*C8 + sin(3*x)*C9 + 1944*cos(3*x)*x^2)   and  C8 is in  R  and  C9 is in  R
    //auto res = solveDifur(derivObj(y, x, 2) - 2*derivObj(y, x, 1) + 5*y - pow(euler(), x)*sin(2*x), x, y);  //y = -1/64*e^x (-4*sin(2*x) + cos(2*x)*C8 + sin(2*x)*C9 + 16*x*cos(2*x))   and  C8 is in  R  and  C9 is in  R
    //auto res = solveDifur(derivObj(y, x, 2) + 9*y + 18*sin(3*x) + 18*pow(euler(), 3*x), x, y);   //y = -1/2 (-6*x*cos(3*x) + sin(3*x) + cos(3*x)*C8 + sin(3*x)*C9 + 2*e^(3*x))   and  C8 is in  R  and  C9 is in  R

  //  auto res = solveDifur(derivObj(y, x, 3) + derivObj(y, x, 1), x, y);  //y = cos(x)*C1 + sin(x)*C2 + C  and  C is in  R  and  C1 is in  R  and  C2 is in  R
    //qDebug() << "RES: ";
   // for (auto &it : res.first)
   //     qDebug() << it.toString();

//ScriptsNameSpace sp;
 //   c = parseAndComplete("2 + 3", sp);
  //  c = parseAndComplete("x + x", sp);
 //   c = parseAndComplete("2*x + 3y - 3x + 3", sp);
 //   c = parseAndComplete("2x*y + 3^3*x", sp);
    //c = parseAndComplete("x(2 + 3) + (3+4)*7",sp);
  //  c = parseAndComplete("(x + 3)(x - 2) + 2", sp);
    // c = parseAndComplete("((x + 3)(x - 2)) + 2", sp);
    //c = parseAndComplete("2pow(x, y + 3) + 3x*sqrt(5)", sp);
  //  c = parseAndComplete("x*y/a + b/c - 2", sp);
    //c = parseAndComplete("Integrate[x*D(x)]", sp);

    //c = parseAndComplete("Integrate[x, x]", sp);
    qDebug();
    qDebug() << "######################################################";
    c = x2 + x + 1;
    qDebug() << c.toString();


    QApplication sdsd(argc, argv);
#ifdef IN_MOSCOW
   /* Form window;

    TestPaintWidget widg;

    widg.setFontSize(18);

    widg.setExpr(c.toString());
    widg.show();

    window.show();*/
  //  TestPaintWidget widg;
      //  widg.setFontSize(18);

     //   widg.setExpr("a + b");
     //   widg.show();
#endif
   MainApplicationSpace appl;
   appl.launch();

    return sdsd.exec();
}
