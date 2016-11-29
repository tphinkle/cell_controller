#include "mainview.h"
#include <QApplication>
#include <maincontroller.h>
#include <mainmodel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainView main_view;
    MainModel main_model;
    MainController main_controller(&main_model, &main_view);

    main_view.show();

    return a.exec();
}
