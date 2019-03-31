#include <QApplication>
#include <QStyleFactory>
#include <QFile>

#include "globals.h"
#include "main/factory/MainWindow.h"

int main(int argc, char *argv[])
{
    QFile appStyles(":/styles/app.qss");
    appStyles.open(QFile::ReadOnly | QFile::Text);

    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setStyleSheet(appStyles.readAll());

    auto window = composer_gui::main::factory::MainWindow::create();
    window->setStyleSheet(COMPOSERGUI_MAIN_WINDOW_STYLESHEET);
    window->setMinimumSize(877, 600);
    window->show();

    return app.exec();
}
