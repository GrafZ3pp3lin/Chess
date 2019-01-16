#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QFile>

#include "mediator.h"

class Receiver : public QObject
{
    Q_OBJECT
    private:
        std::shared_ptr<Mediator> mediator;

    public:
        explicit Receiver(QObject *parent = nullptr);
        Receiver(std::shared_ptr<Mediator>& m);

    signals:

    public slots:
        void receive_click(double x, double y, double w, double h);
        void receive_mode(bool singlerplayer);
        void receice_difficulty(QString difficulty);
        void receive_newgame(bool newgame);
        void receive_FEN(QString fen, bool file, QQuickItem *selection);
        void receive_promotion(QString promotion);
        bool receice_save(QString location);
};

#endif // RECEIVER_H
