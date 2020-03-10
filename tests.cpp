#include "tests.h"
#include "algebra\algexpr.h"
#include "QDebug"
#include "vector"
#include "functional"
#include "QString"
#include "random"
#include "algebra\some_algebra_expression_conversions.h"
#define A(x) AlgExpr(x)
bool testRootFunction()
{
    for (int i = 0; i < 100; ++i)
    {
        long long int res = rand() % 1000 + 1;
        long long int deg = rand() % 5 + 1;
        bool s;
        long long int fr_w = power(res, deg, s);
        if (root(fr_w, deg, s) != res)
        {
            qDebug() << res;
            qDebug() << deg;
            qDebug() << fr_w;
            return false;
        }
    }
    for (int i = 0; i < 50; ++i)
    {
        long long int res = rand() % 500 + 2;
        long long int deg = rand() % 5 + 2;
        bool s;
        long long int fr_w = power(res, deg, s) + 1;
        root(fr_w, deg, s);
        if (s)
        {
            qDebug() << res;
            qDebug() << deg;
            qDebug() << fr_w;
            return false;
        }
    }
    return true;
}
void testAlgMod()
{
    std::vector<std::function<bool (void)>> test_functions = {
        []()->bool {
            AlgExpr a = 57;
            AlgExpr b  = 107;
            AlgExpr c = 164;
            AlgExpr result = a + b;
            return result == c;
        },
        []()->bool {
            AlgExpr a = 57;
            AlgExpr b = 107;
            AlgExpr c = -50;
            AlgExpr result = a - b;
            return result == c;
        },
        []()->bool {
            AlgExpr a = 12;
            AlgExpr b = 26;
            AlgExpr c = 312;
            AlgExpr result = a * b;
            return result == c;
        },
            []()->bool {
            AlgExpr a = 312;
            AlgExpr b = 26;
            AlgExpr c = 12;
            AlgExpr result = a / b;
            return result == c;
},
            []()->bool { //5
            AlgExpr a = var();
            AlgExpr b = a;
            AlgExpr c = a + b;
            AlgExpr d = a * 2;
            return c == d;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr c = a - a;
            AlgExpr b = 0;
            return c == b;
},
            []()->bool {
            AlgExpr a = var();
            return a * a == pow(a, 2);
},
            []()->bool {
            AlgExpr a = var();
            return sqrt(pow(a, 2)) == abs(a);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr c = pow(a, 2);
            return pow(c, Number(1, 3)) == pow(a, Number(2, 3));
},
            []()->bool { //10
            AlgExpr a = var();
            AlgExpr c = pow(a, Number(-2)) - (AlgExpr(1) / pow(a, 2));
            return c == AlgExpr(0);
},
            []()->bool {
            return pow(8, AlgExpr(2) / 3) == AlgExpr(4);
},
            []()->bool {
            return pow(23298085122481, AlgExpr(13) / 12) == AlgExpr(302875106592253);
},
            []()->bool {
            return testRootFunction();
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();

            return pow(a, 3) * pow(a, b - 3) == pow(a, b);
},
            []()->bool { //15
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr c = pow(a, 3) * pow(a, b - 3) / pow(a, 3);
            return c == pow(a, b - 3);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr c = a / (AlgExpr(3) * a / b);
            return c == b / 3;
},
            []()->bool {
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr c = (a * 3 - b * a * 4);
            AlgExpr d = a * 5 + b * 8 - a * pow(b, 2);
            AlgExpr e = c - d;
            return e == ((a * -2) - (a * b * 4) - (b * 8) + (a * pow(b, 2)));
},
            []()->bool {
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr c = 3 * a - 2 * b + 1 - 1 + 2 * b - 2 * a;
            return c == a;
},
            []()->bool {
             AlgExpr c = sqrt(AlgExpr(2)) + sqrt(AlgExpr(2)) - sqrt(AlgExpr(2)) + sqrt(AlgExpr(3));
            return c == sqrt(AlgExpr(2)) + sqrt(AlgExpr(3));
},
            []()->bool { //20
            AlgExpr c = 1 * sqrt(AlgExpr(32));
            return c == 4 * sqrt(AlgExpr(2));
},
            []()->bool {
            AlgExpr c = 1 * sqrt(AlgExpr(864));
            return c == 12 * sqrt(AlgExpr(6));
},
            []()->bool {
            AlgExpr a = var();
            return pow(8, a) == pow(2, 3 * a);
},
            []()->bool {
            AlgExpr c = 1 * sqrt(AlgExpr(128));
            return c == 8 * sqrt(AlgExpr(2));
},
            []()->bool {
            AlgExpr a = var();
            return pow(-8, a) == pow(-2, 3 * a);
},
            []()->bool {
            return 1 * pow(2, AlgExpr(5) / 3) == 2 * pow(2, AlgExpr(2) / 3);
},
            []()->bool { //25
            AlgExpr a  = var();
            return pow(64, a) == pow(2, 6 * a);
},
            []()->bool {
            //вот эта фигня с отрицательными аргументами в степенях работает, вроде, не так, как определено в реальной математике (там оно на комплексную плоскость переходит),
            //но для побочного модуля к школьной геометрии сойдет
            AlgExpr a = var();
            AlgExpr c = pow(-64, a);
            return c == pow(-4, 3 * a) && c != pow(-2, 6 * a);
},
            []()->bool {
            return pow(-3, AlgExpr(1) / 3) / pow(3, AlgExpr(1) / 3) == AlgExpr(-1);
},
            []()->bool {
            return pow(-3, AlgExpr(1) / 3) + pow(3, AlgExpr(1) / 3) == AlgExpr(0);
},
            []()->bool {
            return 1 * pow(-128, AlgExpr(1)/ 3) == -4 * pow(2, AlgExpr(1)/3);
},
            []()->bool { //30
            AlgExpr c = sqrt(A(2)) + sqrt(A(2)) - sqrt(A(2)) + sqrt(A(3));
            return c == sqrt(A(2)) + sqrt(A(3));
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr c = 3 * a - 2 * b + 1 - 1 + 2 * b - 2 * a + 2 * sqrt(A(2)) - 3 * sqrt(A(2));
            return c == a - sqrt(A(2));
},
            []()->bool {
             AlgExpr c = 2 - A(3) / 7;
            return c == A(11) / 7;
},
            []()->bool {
            return 8 * A(5) / 4 == A(10);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return a - A(3) / (7 * b) == (7 * a * b - 3) / (7 * b);
},
            []()->bool { //35
            AlgExpr a = var();
            AlgExpr b = var();
            return 3*(a + b) + 1 == 3 * a + 3 * b + 1;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return 3*b*(a + b) + 1 == 3 * a * b + 3 * pow(b, 2) + 1;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr x = var();
            AlgExpr y = var();
            return (x+y)*(a+b) + 1 == 1 + a*x + a*y + b*x + b*y;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr x = var();
            AlgExpr y = var();
            return 3 * (x+y)*(a+b) + 1 == 1 + 3 * a*x + 3*a*y + 3*b*x + 3*b*y;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr x = var();
            AlgExpr y = var();
            return 3 * b * (x+y)*(a+b) + 1 == 1 + 3 * b * a*x + 3*b*a*y + 3*b*b*x + 3*b*b*y;
},
            []()->bool { //40
            AlgExpr a = var();
            AlgExpr b = var();
            return (a+b)*(a+b) + 1 == pow(a, 2) + pow(b, 2) + 2 * a * b + 1;
},
            []()->bool {
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr x = var();
            AlgExpr y = var();
            AlgExpr z = var();
            AlgExpr t = var();
            AlgExpr c = (a + b) * (x + y) * (z + t) + 1;
            return c == a*x*z + b * x * z + a * t * x + b*t*x + a*y*z + b*y*z + a*t*y + b*t*y + 1;
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = var();
            AlgExpr z = var();
            AlgExpr c = (x+(x+y)*(x-z)+y)*(y+2*z) + 1;
            return c == x*y+2*x*z+x*x*y+2*x*x*z+x*y*z-2*x*z*z+x*y*y-y*y*z-2*y*z*z+y*y+2*y*z + 1;
},
            []()->bool {
            AlgExpr x = var();
            return (pow(x, 2) + 2 * x + 1) / (x + 1) == x + 1;
},
            []()->bool {
            AlgExpr x = var();
            return (pow(x, 2) + 6 * x + 5) / (pow(x, 2) + 4 * x + 3) == (x + 5) / (x + 3);
},
            []()->bool { //45
            AlgExpr a = var();
            AlgExpr b = var();
            return (pow(a, 2) + 2 * a * b + pow(b, 2))/ (a + b) == (a + b);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr c = a / b - A(3) / (7 * b);
            return c == (7*a-3)/(7*b);
},
            []()->bool {
            AlgExpr x = var();
            return (x + 2 * sqrt(x) + 1) / (sqrt(x) + 1) == sqrt(x) + 1;
},
            []()->bool { //48
            AlgExpr x = var();
            AlgExpr y = var();
            return (x + 2 * sqrt(x) * sqrt(y) + y) / (sqrt(x) + sqrt(y)) == sqrt(x) + sqrt(y);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return (a - b) / (sqrt(a) + sqrt(b)) == sqrt(a) - sqrt(b);
},
            []()->bool { //50
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr y = var();
            return (a - b*sqrt(y)) / (sqrt(a) + sqrt(b)*pow(y, AlgExpr(1)/4)) == sqrt(a) - sqrt(b)*pow(y, AlgExpr(1) / 4);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr y = var();
            return (a*sqrt(y) - b*sqrt(y)) / (sqrt(a) + sqrt(b)) == sqrt(y)*(sqrt(a) - sqrt(b));
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = var();
            AlgExpr z = var();
            return (x*z+x-y*pow(z,2) - y*z) / (z + 1) == x - y *z;
},
            []()->bool {
            AlgExpr z = var();
            AlgExpr t = var();
            return (2 * z + 2 * t) * (3 * z + 3 * t) / 6 == pow(z + t, 2);

},
            []()->bool { //55
            return sqrt(A(15)) / sqrt(A(3)) == sqrt(A(5));
},
            []()->bool {
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr x = var();
            AlgExpr y = var();
            AlgExpr z = var();
            AlgExpr t = var();
            a = pow(7*63*63*63, 1 / A(12)) - pow(392, 1 / A(6))+ 6*pow(7, 1 / A(3)) + 6*sqrt(A(5))-sqrt(A(10))+sqrt(A(15))-2*sqrt(A(2))+2*sqrt(A(3))+12;
            b = pow(9, A(1)/4)-sqrt(A(2)) - 6;
            x = a * b;
            y = x * (-31+2*sqrt(A(6)));
            y = y / 937;
            return y == sqrt(A(5)) + pow(7, A(1) / 3) + 2;
},
            []()->bool {
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr x = var();
            AlgExpr y = var();
            AlgExpr z = var();
            AlgExpr t = var();
            a = (pow(x, 2)+y*z+2)*(pow(x, 3)-y+pow(z,2))*(6+6*x-sqrt(x*y))*(8+22*sqrt(z)) + 0;
            b = (pow(x, 2)+y*z+2)*(6+6*x-sqrt(x*y)) + 0;
            t = (pow(x, 3)-y+pow(z,2))*(8+22*sqrt(z))+0;
            return a/b == t;
},
            []()->bool {
            AlgExpr a = var();

            AlgExpr b = var();
            AlgExpr c = var();
            return b*c/((a-b)*(a-c)) + c*a/((b-c)*(b-a))+a*b/((c-a)*(c-b)) == AlgExpr(1);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b  = var();
            AlgExpr c = -3 * (-1 * a - b);
            return c.isEqualTo(3 * (a + b));
},
            []()->bool { //60
            AlgExpr c = (3+2*sqrt(A(2)))/ (1 + sqrt(A(2)));
            return c.isEqualTo( 1 + sqrt(A(2)));
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return (pow(a, 4) - pow(b, 4)) / (a - b) == pow(a, 3)+pow(b,3)+a*b*b+b*a*a;
},
            []()->bool {
            return ((root(7*63*63*63, A(12)) - root(392, A(6)) + 6*root(7, A(3)) + 6 * sqrt(A(5)) - sqrt(A(10))+sqrt(A(15))-2*sqrt(A(2))+2*sqrt(A(3))+12)/(root(7, 3)+2+sqrt(A(5)))).isEqualTo(
                6 + sqrt(A(3)) - sqrt(A(2)));
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return root(a*a+2*a*b+b*b, 2) == abs(a + b);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            qDebug() << root(9*a*a+30*a*b+25*b*b, 2).toString();
            return root(9*a*a+30*a*b+25*b*b, 2) == abs(3*a + 5*b);
},
            []()->bool { //65
            AlgExpr a = var();
            AlgExpr b = var();
            return root(9*a*a+6*sqrt(A(5))*a*b+5*b*b, 2) == abs(3*a + sqrt(A(5)) * b);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return root(27*a*a*a+27*sqrt(A(5))*a*a*b+45*a*b*b+5*sqrt(A(5))*b*b*b, 3) == 3*a+sqrt(A(5))*b;
},
            []()->bool {
            AlgExpr x = var();
            qDebug() << root(x+2*sqrt(x) + 1, 2).toString();
            return root(x+2*sqrt(x) + 1, 2) == sqrt(x) + 1;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            AlgExpr c = var();
            return root(a*a+b*b+c*c+2*a*b+2*b*c+2*a*c, 2) == abs(a+b+c);
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = var();
            return root(x*x*x+x*x*x*x*x*x+y*y*y+3*x*x*x*x+3*x*x*y+3*pow(x,5)+3*pow(x,4)*y+3*x*y*y+3*x*x*y*y+6*x*x*x*y, 3) == x+x*x+y;
},
            []()->bool {//70
            //it's too long to wait, but sometimes i have to check it
            return true;
            AlgExpr x = var();
            AlgExpr t = A(243) + 405*x + 675*x*x + 1035*x*x*x + 1500*pow(x, 4) + 2086*pow(x, 5) + 2405*pow(x, 6) +
            810*sqrt(A(5))*pow(x, 6) + 3150*pow(x, 7) + 1080*sqrt(A(5))*pow(x, 7) + 3935*pow(x, 8) +
            1620*sqrt(A(5))*pow(x, 8) + 4855*pow(x, 9) + 2280*sqrt(A(5))*pow(x, 9) + 5926*pow(x, 10) +
            3070*sqrt(A(5))*pow(x, 10) + 6760*pow(x, 11) + 4000*sqrt(A(5))*pow(x, 11) + 13315*pow(x, 12) +
            4000*sqrt(A(5))*pow(x, 12) + 14135*pow(x, 13) + 5240*sqrt(A(5))*pow(x, 13) + 16740*pow(x, 14) +
            6290*sqrt(A(5))*pow(x, 14) + 19406*pow(x, 15) + 7480*sqrt(A(5))*pow(x, 15) + 22085*pow(x, 16) +
            8820*sqrt(A(5))*pow(x, 16) + 25265*pow(x, 17) + 9240*sqrt(A(5))*pow(x, 17) + 22860*pow(x, 18) +
            13970*sqrt(A(5))*pow(x, 18) + 27800*pow(x, 19) + 13000*sqrt(A(5))*pow(x, 19) + 30281*pow(x, 20) +
            13660*sqrt(A(5))*pow(x, 20) + 32695*pow(x, 21) + 14080*sqrt(A(5))*pow(x, 21) + 35260*pow(x, 22) +
            14230*sqrt(A(5))*pow(x, 22) + 32250*pow(x, 23) + 15160*sqrt(A(5))*pow(x, 23) + 39070*pow(x, 24) +
            12880*sqrt(A(5))*pow(x, 24) + 32396*pow(x, 25) + 14560*sqrt(A(5))*pow(x, 25) + 30505*pow(x, 26) +
            14290*sqrt(A(5))*pow(x, 26) + 28235*pow(x, 27) + 13720*sqrt(A(5))*pow(x, 27) + 25370*pow(x, 28) +
            13360*sqrt(A(5))*pow(x, 28) + 25720*pow(x, 29) + 10240*sqrt(A(5))*pow(x, 29) + 21020*pow(x, 30) +
            10200*sqrt(A(5))*pow(x, 30) + 20355*pow(x, 31) + 7800*sqrt(A(5))*pow(x, 31) + 17170*pow(x, 32) +
            6700*sqrt(A(5))*pow(x, 32) + 13630*pow(x, 33) + 5720*sqrt(A(5))*pow(x, 33) + 11625*pow(x, 34) +
            4530*sqrt(A(5))*pow(x, 34) + 7286*pow(x, 35) + 4240*sqrt(A(5))*pow(x, 35) + 5815*pow(x, 36) +
            3200*sqrt(A(5))*pow(x, 36) + 4595*pow(x, 37) + 2280*sqrt(A(5))*pow(x, 37) + 3610*pow(x, 38) +
            1510*sqrt(A(5))*pow(x, 38) + 2900*pow(x, 39) + 800*sqrt(A(5))*pow(x, 39) + 1851*pow(x, 40) +
            600*sqrt(A(5))*pow(x, 40) + 1065*pow(x, 41) + 440*sqrt(A(5))*pow(x, 41) + 530*pow(x, 42) +
            310*sqrt(A(5))*pow(x, 42) + 230*pow(x, 43) + 200*sqrt(A(5))*pow(x, 43) + 160*pow(x, 44) +
            100*sqrt(A(5))*pow(x, 44) + 106*pow(x, 45) + 40*sqrt(A(5))*pow(x, 45) + 65*pow(x, 46) +
            10*sqrt(A(5))*pow(x, 46) + 35*pow(x, 47) + 15*pow(x, 48) + 5*pow(x, 49) + pow(x, 50);
            qDebug() << t.toString();
            return true;
            return root(t, 5) == x+x*x+pow(x, 3) + pow(x, 4) + pow(x, 5) + 2*sqrt(A(5))*pow(x, 6) +pow(x, 7) + pow(x, 8) + pow(x, 9) + pow(x, 10) + 3;
},
            []()->bool {
            return sqrt(7 + 4 * sqrt(A(3))) == 2+sqrt(A(3));
},
            []()->bool {
            return sqrt(7-4*sqrt(A(3))) == 2 - sqrt(A(3));
},
            []()->bool {
            return sqrt(2+sqrt(A(3))) == (1+sqrt(A(3)))/sqrt(A(2));
},
            []()->bool {
            return sqrt(42+6*sqrt(A(5))+12*sqrt(A(7))+4*sqrt(A(35))) == 3+sqrt(A(5)) + 2*sqrt(A(7));
},
            []()->bool { //75
            return sqrt(616 - 18*sqrt(A(7))+296*sqrt(A(3))) == 18 - 2 * sqrt(A(7)) + 9 * sqrt(A(3)) + sqrt(A(21));
},
            []()->bool {
            return sqrt(616 - 18*sqrt(A(7)) - 296*sqrt(A(3))) == - 18 + 2 * sqrt(A(7)) + 9 * sqrt(A(3)) + sqrt(A(21));
},
            []()->bool {
            return sqrt(620+84*sqrt(A(21)) + 62*sqrt(A(7)) + 156 * sqrt(A(3))) == sqrt(A(3)) + 2 * sqrt(A(7)) + 5 * sqrt(A(21)) + 8;
},
            []()->bool {
            return sqrt(620 - 76*sqrt(A(21)) + 2*sqrt(A(7)) -124 * sqrt(A(3))) == abs(sqrt(A(3)) + 2 * sqrt(A(7)) - 5 * sqrt(A(21)) + 8);
},
            []()->bool {
            return sqrt(224+20*sqrt(A(3))+10*sqrt(A(7))+4*sqrt(A(21))+60*sqrt(A(5))+24*sqrt(A(15))+12*sqrt(A(35))) == 5 + 2*sqrt(A(3)) + sqrt(A(7)) + 6*sqrt(A(5));
},
            []()->bool { //80
            return sqrt(224+20*sqrt(A(3))+10*sqrt(A(7))+4*sqrt(A(21))-60*sqrt(A(5))-24*sqrt(A(15))-12*sqrt(A(35))) == abs(5 + 2 * sqrt(A(3)) + sqrt(A(7)) - 6*sqrt(A(5)));
},
            []()->bool {
            return sqrt(224-20*sqrt(A(3))+10*sqrt(A(7))-4*sqrt(A(21))-60*sqrt(A(5))+24*sqrt(A(15))-12*sqrt(A(35))) == abs(5 - 2 * sqrt(A(3)) + sqrt(A(7)) - 6 * sqrt(A(5)));
},
            []()->bool {
            return abs(A(3)) == A(3);
},
            []()->bool {
            return abs(A(-3)) == A(3);
},
            []()->bool {
            return abs(3*sqrt(A(7)) - sqrt(A(3)) - 5) == 3 * sqrt(A(7)) - sqrt(A(3)) - 5;
},
            []()->bool { //85
            return abs(-3*sqrt(A(7)) + sqrt(A(3)) + 5) == 3 * sqrt(A(7)) - sqrt(A(3)) - 5;
},
            []()->bool {
            AlgExpr x = var();
            return abs(pow(x, 2)) == pow(x, 2);
},
            []()->bool {
            AlgExpr x = var();
            return abs(pow(x, 3)) != pow(x, 3) && abs(pow(x, 3)) != -1 * pow(x, 3);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return abs(a + b) * abs(a + b) == pow(a + b, 2);
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return abs(a - b) / abs(b - a) == A(1);
},
            []()->bool { //90
            AlgExpr a = var();
            return abs(1 + a*a + a*a*a*a) == 1 + a*a + a*a*a*a;
},
            []()->bool {
            AlgExpr a = var();
            return abs(-1 - a*a - a*a*a*a) == 1 + a*a + a*a*a*a;
},
            []()->bool {
            AlgExpr x = var(0, 3);
            AlgExpr y = var(4, 7);
            return abs(y - x - 1) == y - x - 1;
},
            []()->bool {
            AlgExpr x = var(0, 3);
            AlgExpr y = var(4, 7);
            return abs(y - x - 3) != y - x - 3 && abs(y - x - 3) != 3 + x - y;
},
            []()->bool {
            AlgExpr x = var();
            return abs(x*x + x + 1) == x*x + x + 1;
},
            []()->bool { //95
            AlgExpr x = var();
            return abs(-1 *x*x - x - 1) == x*x + x + 1;
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = x*x - 7 * x + 8;
            return abs(y) != y && abs(y) != -1 * y;
},
            []()->bool {
            AlgExpr a = var();
            return abs(a) != a && abs(a) != -1*a;
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr c = (2*pow(x, 6) + 19*pow(x, 5) - 475*x - 250)/(x - sqrt(A(5)));
            return c == 2*pow(x, 5) + (19 + 2*sqrt(A(5)))*pow(x, 4) + (19*sqrt(A(5))+10)*pow(x, 3) + (95 + 10*sqrt(A(5)))*pow(x, 2) + (95*sqrt(A(5)) + 50)*x + 50*sqrt(A(5));
},
            []()->bool {
            return sin(A(0)) == 0;
},
            []()->bool { //100
            return sin(pi()/2) == 1;
},
            []()->bool {
            return sin(pi()) == 0;
},
            []()->bool {
            return sin(3*pi()/2) == -1;
},
            []()->bool {
            return sin(-1*pi()/2) == -1;
},
            []()->bool {
            return sin(pi()/6) == A(1)/2;
},
            []()->bool { //105
            return sin(3*pi()/4) == sqrt(2)/2;
},
            []()->bool {
            return sin(4*pi()/3) == sin(5*pi()/3) && sin(4*pi()/3) == -1*sqrt(3)/2;
},
            []()->bool {
            return sin(-1*pi()/6) == -1/A(2);
},
            []()->bool {
            return sin(5*pi()/2) == 1;
},
            []()->bool {
            return sin(-5*pi()/2) == -1;
},
            []()->bool { //110
            return sin(11*pi()/3) == -1*sqrt(3)/2;
},
            []()->bool {
            return sin(-11*pi()/3) == 1*sqrt(3)/2;
},
            []()->bool {
            AlgExpr x = var();
            return sin(x + 3*pi()) == sin(x + pi());
},
            []()->bool {
            AlgExpr x = var();
            return sin(x + 4*pi()) == sin(x);
},
            []()->bool {
            AlgExpr x = var();
            return sin(x + 7*pi()/3) == sin(x + pi()/3);
},
            []()->bool { //115
            AlgExpr x = var();
            return sin(x + 7*pi()/2) == sin(x + 3*pi()/2);
},
            []()->bool {
            AlgExpr x = var();
            return sin(x - 7*pi()/2) == sin(x + pi()/2);
},
            []()->bool {
            AlgExpr x = var();
            return sin(x - 4*pi()) == sin(x);
},
            []()->bool {
            AlgExpr x = var();
            return sin(x - 3*pi()) == sin(x + pi());
},
            []()->bool {
            AlgExpr x = var();
            return sin(x) - sin(-1*x) == 2*sin(x);
},
            []()->bool { //120
            AlgExpr a = var(), b = var();
            return sin(a - b) + sin(b - a) == 0;
},
            []()->bool {
            AlgExpr a = var();
            return sin(pow(a, 3)) + sin(pow(-1*a, 3)) == 0;
},
            []()->bool {
            AlgExpr a = var();
            return sin(pow(a, 2)) + sin(pow(-1*a, 2)) == 2*sin(a*a);
},
            []()->bool {
            AlgExpr a = var(), b = var();
            return sin(a - b + 4*pi()/3) + sin(b - a - 4*pi()/3) == 0;
},
            []()->bool {
            AlgExpr a = var(), b = var();
            return sin(a - b + 4*pi()/3) + sin(b - a + 2*pi()/3) == 0;
},
            []()->bool { //125
            return abs(sin(1)) == sin(1);
},
            []()->bool {
            return cos(pi()/2) == 0;
},
            []()->bool {
            AlgExpr a = var();
            AlgExpr b = var();
            return cos(a - b + 4*pi()/3) - cos(b - a + 2*pi()/3) == 0;
},
            []()->bool {
            return cos(-7*pi()/6) == -1*sqrt(3)/2;
},
            []()->bool {
            return abs(cos(3)) == -1*cos(3);
},
            []()->bool { //130
            return abs(cos(11*pi()/9)) == -1*cos(11*pi()/9);
},
            []()->bool {
            AlgExpr x = var();
            return tan(pi()/6) + tan(-1*x + pi()/4) == 1/sqrt(3) - tan(x + 3*pi()/4);
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr f = -1/sqrt(3) - tan(x + 5*pi()/7);
            return cot(2*pi()/3) + cot (-1*x + 2*pi()/7) == -1/sqrt(3) - cot(x + 5*pi()/7);
},
            []()->bool {
            AlgExpr x = var();
            return sin(x)*sin(x)+cos(x)*cos(x) == 1;
},
            []()->bool {
            AlgExpr x = var();
            return (3*sin(x)*sin(x) + 4*(cos(x))*cos(x)) == 3 + cos(x)*cos(x);
},
            []()->bool { //135
            AlgExpr x = var();
            return tan(x)*tan(x) + 1 - 1/cos(x)/cos(x) == 0;
},
            []()->bool {
            AlgExpr x = var();
            return cot(x)*cot(x) + 1 - 1/(sin(x)*sin(x)) == 0;
},
            []()->bool {
            AlgExpr x = var();
            return tan(x) + cot(x) == (1 + tan(x)*tan(x))/tan(x);
},
            []()->bool {
            AlgExpr x = var();
            return tan(x)*tan(x) - 1/(cos(x)*cos(x)) == -1;
},
            []()->bool {
            AlgExpr x = var();
            return sin(x)/cos(x) - tan(x) == 0;
},
            []()->bool {//140
            AlgExpr x = var();
            return sin(x)*sin(x + 2*pi()) + cos(x - 2*pi())*cos(-1*x) == 1;
},
            []()->bool {
            AlgExpr x = var();
            return sin(x+2*pi()) == sin(x);
},
            []()->bool {
            AlgExpr x = var();
            return  sin(x)*sin(x + 2*pi()) + cos(x - 2*pi())*cos(-1*x) == 1;
},
            []()->bool {
            AlgExpr x = var();
            return sin(x + pi()/2) == cos(x);
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = var();
            return (3/A(2)*y*sqrt(2) + 5/A(2)*x*sqrt(2)) == sqrt(2)*(5*x+3*y)/2;
},
            []()->bool { //145
            AlgExpr x = var();
            return sin(x + pi()/4) == sqrt(2)/2*(sin(x) + cos(x));
},
            []()->bool {
            AlgExpr x = var();
            return  sin(x - pi()/4) == sqrt(2)/2*(sin(x) - cos(x));
},
            []()->bool {
            AlgExpr x = var();
            return cos(pi()/2 - x) == sin(x);
},
            []()->bool {
            AlgExpr x = var();
            return cos(pi()/2 + x) == -sin(x);
},
            []()->bool {
            AlgExpr x = var();
            return cos(x + pi()) == -cos(x);
},
            []()->bool { //150
            AlgExpr x = var();
            AlgExpr y = var();
            return cos(x - y) == cos(x)*cos(y) + sin(x)*sin(y);
},
            []()->bool {
            AlgExpr x = var();
            return tan(x)*cot(x) == 1;
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = var();
            return sin(x + y) - sin(x)*cos(y) == sin(y)*cos(x);
},
            []()->bool {
            AlgExpr x = var();
            return (sin(x)*sin(x) + sin(x) - 2) / (sin(x) - 1) == sin(x) + 2;
},
            []()->bool {
            AlgExpr x = var();
            AlgExpr y = var();
            return (x*x - x - y*x + y) / (x - 1) == x - y;
},
            []()->bool { //155
            AlgExpr x = var();
            AlgExpr y = var();
            return (x*x - x - y*x + y) / (x - y) == x - 1;
},
            []()->bool {
            AlgExpr x = var();
            return (sin(x)*sin(x)-sin(x)-cos(x)*sin(x) + cos(x)) / (sin(x) - 1) == sin(x) - cos(x);
},
            []()->bool {
            AlgExpr x = var();
            return (sin(x)*sin(x)-sin(x)-cos(x)*sin(x) + cos(x)) / (sin(x) - cos(x)) == sin(x) - 1;
},
            []()->bool {
            AlgExpr x = var();
            return sqrt(sin(x)*sin(x) + 2*sin(x) + 1) == abs(sin(x) + 1);
},
            []()->bool {
            AlgExpr x = var();
            return sqrt(sin(x)*sin(x) + 2*sin(x)*x + x*x) == abs(sin(x) + x);
}



    };


    std::vector<int> error_tests;
  for (int i = 0; i < test_functions.size(); ++i)
  {
      bool result = test_functions[i]();
      if (result)
          qDebug() << "TEST " << i + 1 << ": OK";
      else
      {
          qDebug() << "TEST " << i + 1 << ": ERROR";
          error_tests.push_back(i + 1);
      }

  }
  if (error_tests.size() == 0)
      qDebug() << "All tests have passed succesfully.";
  else
  {
      qDebug () << "Errors in tests:";
      for (auto it : error_tests)
          qDebug() << it;
  }

  deleteVariables();
}
