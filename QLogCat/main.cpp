#include "qlogcat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qputenv("QLC","G");

    QLogCat w;
    w.show();    
    
    return a.exec();
}
