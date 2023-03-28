#include "mainwindow.h"
#include <QApplication>
#include "algebra\algexpr.h"
#include "tests.h"
#include <mainapplicationspace.h>

int main(int argc, char *argv[])
{




    QApplication sdsd(argc, argv);

    //необходимо вызывать чтобы он создался
   VariablesDistributor::get();
   MainApplicationSpace appl;
   appl.launch();

    return sdsd.exec();
}
