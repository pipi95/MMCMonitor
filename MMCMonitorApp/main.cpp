#include "mainwindow.h"
#include <QApplication>
#include <QDir>
//#include "Log4Qt/consoleappender.h"
#include "Log4Qt/logger.h"
//#include "Log4Qt/ttcclayout.h"
#include "Log4Qt/logmanager.h"
#include "Log4Qt/propertyconfigurator.h"
//#include "Log4Qt/basicconfigurator.h"

int main(int argc, char *argv[])
{
    using namespace Log4Qt;
    QApplication a(argc, argv);
    QDir path("./log.conf");
    PropertyConfigurator::configure(path.absolutePath());
    MainWindow w;
    w.show();

    return a.exec();
}
