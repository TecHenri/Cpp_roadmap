#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("Text Editor");
    app.setApplicationVersion("1.0");

    te::MainWindow window;
    window.show();

    return app.exec();
}