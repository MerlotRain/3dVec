#include <QApplication>
#include <SARibbonBar/SARibbonBar.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    SARibbonBar* bar = new SARibbonBar();
    return app.exec();
}