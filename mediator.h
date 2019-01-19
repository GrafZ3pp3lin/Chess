#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include <QQuickItem>
#include <QRect>
#include <QTimer>

#include <vector>
#include <algorithm>
#include <map>

#include "chess_engine/MoveGenerator.hpp"
#include "chess_engine/Evaluation.hpp"
#include "chess_engine/KI.hpp"

void delay(int ms);

class Mediator
{
    public:
        Mediator(QObject *root, std::shared_ptr<ChessBoard>& chessBoard, std::shared_ptr<AI>& ai);
        ~Mediator();

        void evaluateClick(double x, double y, double w, double h);
        bool checkFEN(const char* fen);
        bool saveFEN(QString location);
        void refreshGUI();
        void setMode(bool singleplayer);
        void setAIDifficulty(Difficulty d);
        void newGame();
        void promotePawn(Piece promotion, bool ai);

    private:
        QObject *root;
        std::shared_ptr<AI> ai;
        std::shared_ptr<ChessBoard> chessBoard;
        bool singleplayer = true;
        QQuickItem *selectedPiece;
        QQuickItem *promotablePawn;
        QMap<QQuickItem *, Move> *validMoveSquares;
        std::vector<Move> *moveset;
        int moveAnimationDuration = 250; // duration in milliseconds
        bool evaluationRunning;

        void startGame();
        QQuickItem* getGreenSquareAt(int index);
        QList<QQuickItem *> getPieces(char piece);
        QQuickItem* getPieceByIndex(int index_10x12);
        uint8_t index_8x8_to_10x12(int index_8x8);
        int index_10x12_to_8x8(int index_10_12);
        void showMoveSet(uint8_t pieceIndex, QRect &square);
        void movePiece(Move m, bool guiOnly);
        void makeAIMove();
        void hideGreenSquares();
        void endGame();
};

#endif // MEDIATOR_H
