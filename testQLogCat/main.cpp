#include "tester.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tester w;
    w.show();
    
    qputenv("QLC","G");
    qputenv("QLOGCAT","");
    // Эта штука пишется не здесь а в настройках QtCreator
    // Сейчас просто для удобства вам

    return a.exec();
}
