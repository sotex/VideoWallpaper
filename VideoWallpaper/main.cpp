#include "playcontrolpanel.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayControlPanel w;
    w.show();
    return a.exec();
}
