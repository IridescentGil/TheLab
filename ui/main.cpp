#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--cli") == 0) {
        std::cout << "The Lab Cli\n";
        return 0;
    }

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Lab", "Main");

    return app.exec();
}
