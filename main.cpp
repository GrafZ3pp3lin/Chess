#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QObject>
#include <QDebug>
#include <QMetaObject>
#include <QQuickItem>
#include <QIcon>

#include "main.h"
#include "receiver.h"
#include "mediator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    // init window
    QQmlApplicationEngine *engine = new QQmlApplicationEngine(qApp);
    QQmlContext *context = engine->rootContext();
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine->rootObjects().isEmpty())
        return -1;

    // get root of main window
    QObject *root = engine->rootObjects().first();

    // setup connection between gui and chess engine through a mediator object
    std::shared_ptr<ChessBoard> chessBoard(new ChessBoard());
    std::shared_ptr<AI> ai(new AI());
    std::shared_ptr<Mediator> mediator(new Mediator(root, chessBoard, ai));
    Receiver *receiver = new Receiver(mediator);

    // connect qml to receiver
    context->setContextProperty("receiver", receiver);

    return app.exec();
}
