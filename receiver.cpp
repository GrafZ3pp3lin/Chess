#include "receiver.h"

Receiver::Receiver(QObject *parent) : QObject(parent) { }

Receiver::Receiver(std::shared_ptr<Mediator>& m) : mediator(m) { }

void Receiver::receive_click(double x, double y, double w, double h)
{
    mediator->evaluateClick(x, y, w, h);
}

void Receiver::receive_mode(bool singleplayer)
{
    mediator->setMode(singleplayer);
}

void Receiver::receice_difficulty(QString difficulty)
{
    if (difficulty == "easy")
    {
        mediator->setAIDifficulty(Difficulty::EASY);
    }
    else if (difficulty == "hard")
    {
        mediator->setAIDifficulty(Difficulty::HARD);
    }
    else
    {
        mediator->setAIDifficulty(Difficulty::NORMAL);
    }
}

void Receiver::receive_newgame(bool newgame)
{
    if (newgame)
    {
        mediator->newGame();
    }
}

void Receiver::receive_FEN(QString fen, bool file, QQuickItem *selection)
{
    selection->setVisible(false);
    const char *fenString = fen.toStdString().c_str();
    QString fenFileContent;
    if (file)
    {
        QFile fenFile(fen.replace("file:///", ""));
        if (fenFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&fenFile);
            fenFileContent = in.readAll();
        }
        else
        {
            return;
        }
    }
    if (mediator->checkFEN(file ? fenFileContent.toStdString().c_str() : fenString)) { }
    else
    {
        selection->setVisible(true);
    }
}

void Receiver::receive_promotion(QString promotion)
{
    promotion = promotion.toLower();
    if (promotion == "queen") mediator->promotePawn(Piece::QUEEN);
    else if (promotion == "rook") mediator->promotePawn(Piece::ROOK);
    else if (promotion == "knight") mediator->promotePawn(Piece::KNIGHT);
    else if (promotion == "bishop") mediator->promotePawn(Piece::BISHOP);
    else mediator->promotePawn(Piece::QUEEN);
}

bool Receiver::receice_save(QString location)
{
    return mediator->saveFEN(location);
}
