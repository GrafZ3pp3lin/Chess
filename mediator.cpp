#include "mediator.h"

Mediator::Mediator(QObject *r, std::shared_ptr<ChessBoard>& c, std::shared_ptr<AI>& a) : root(r), ai(a), chessBoard(c)
{
    chessBoard->init();
    validMoveSquares = new QMap<QQuickItem *, Move>();
    evaluationRunning = false;
    selectedPiece = nullptr;
    moveset = nullptr;
    promotablePawn = nullptr;
}

Mediator::~Mediator()
{
    delete validMoveSquares;
}

QQuickItem* Mediator::getGreenSquareAt(int index)
{
    QObject *greenSquares = root->findChild<QObject *>("greenSquares");
    QQuickItem *child;
    QMetaObject::invokeMethod(greenSquares,
                                          "itemAt",
                                          Qt::DirectConnection,
                                          Q_RETURN_ARG(QQuickItem *, child),
                                          Q_ARG(int, index));
    return child;
}

QList<QQuickItem *> Mediator::getPieces(char piece)
{
    QString pieceName = "";
    switch (piece)
    {
    case 'p':
    {
        pieceName = "blackPawn";
        break;
    }
    case 'r':
    {
        pieceName = "blackRook";
        break;
    }
    case 'n':
    {
        pieceName = "blackKnight";
        break;
    }
    case 'b':
    {
        pieceName = "blackBishop";
        break;
    }
    case 'q':
    {
        pieceName = "blackQueen";
        break;
    }
    case 'k':
    {
        pieceName = "blackKing";
        break;
    }
    case 'P':
    {
        pieceName = "whitePawn";
        break;
    }
    case 'R':
    {
        pieceName = "whiteRook";
        break;
    }
    case 'N':
    {
        pieceName = "whiteKnight";
        break;
    }
    case 'B':
    {
        pieceName = "whiteBishop";
        break;
    }
    case 'Q':
    {
        pieceName = "whiteQueen";
        break;
    }
    case 'K':
    {
        pieceName = "whiteKing";
        break;
    }
    }
    return root->findChildren<QQuickItem *>(pieceName);
}

QQuickItem* Mediator::getPieceByIndex(int index_10x12)
{
    int8_t pieceNr = chessBoard->board[index_10x12];
    char piece;
    if (pieceNr == 1) piece = 'P';
    else if (pieceNr == 11) piece = 'R';
    else if (pieceNr == 12) piece = 'N';
    else if (pieceNr == 13) piece = 'B';
    else if (pieceNr == 14) piece = 'Q';
    else if (pieceNr == 10) piece = 'K';
    else if (pieceNr == 2) piece = 'p';
    else if (pieceNr == 21) piece = 'r';
    else if (pieceNr == 22) piece = 'n';
    else if (pieceNr == 23) piece = 'b';
    else if (pieceNr == 24) piece = 'q';
    else if (pieceNr == 20) piece = 'k';
    else piece = '\0';
    // if the clicked square isn't empty
    if (piece != '\0')
    {
        QQuickItem *guiPiece = nullptr;
        QList<QQuickItem *> guiPieces = getPieces(piece);
        double squareWidth = root->findChild<QQuickItem *>("chessBoard")->width() / 8;
        double squareHeight = root->findChild<QQuickItem *>("chessBoard")->height() / 8;
        int row = index_10x12_to_8x8(index_10x12) / 8;
        int col = index_10x12_to_8x8(index_10x12) % 8;
        double pieceX = squareWidth * col;
        double pieceY = squareHeight * row;
        if (guiPieces.count() == 0) return nullptr;
        if (guiPieces.count() == 1) guiPiece = guiPieces[0];
        else
        {
            // search the clicked piece if there are more than one of the same kind one the board
            for (QQuickItem *p : guiPieces)
            {
                if (p->x() >= pieceX && p->x() < (pieceX + squareWidth)
                    && p->y() >= pieceY && p->y() < (pieceY + squareHeight))
                {
                    guiPiece = p;
                    break;
                }
            }
        }
        return guiPiece;
    }
    return nullptr;
}

uint8_t Mediator::index_8x8_to_10x12(int index_8x8)
{
    return static_cast<uint8_t>(
           index_8x8                    // index of 8x8 board
           + 21                         // simple offset: the actual chess board in a 10x12 board starts at 21 => 8x8(0) == 10x12(21)
           + ((index_8x8 / 8) * 2)      // additional increase of 2 every 8 squares due to the left and right edge columns
    );
}

int Mediator::index_10x12_to_8x8(int index_10_12)
{
    int row = (index_10_12 / 10) - 2;
    int col = (index_10_12 % 10) - 1;
    return row * 8 + col;
}

void delay(int ms)
{
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, SLOT(quit()));
    loop.exec();
}

void Mediator::evaluateClick(double x, double y, double w, double h)
{
    if (evaluationRunning)
    {
        return;
    }
    evaluationRunning = true;
    // get index of clicked square
    int col = static_cast<int>((x * 8) / w);
    int row = static_cast<int>((y * 8) / h);
    int clickIndex = row * 8 + col;
    uint8_t pieceIndex = index_8x8_to_10x12(clickIndex);
    // find out what piece that was clicked
    QRect clickedSquare(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));

    if (validMoveSquares->empty()) // select piece
    {
        hideGreenSquares();
        showMoveSet(pieceIndex, clickedSquare);
    }
    else // select move
    {
        QQuickItem *greenSquare = nullptr;
        Move move(static_cast<int>(0), static_cast<int>(0));
        for (QQuickItem *s : validMoveSquares->keys())
        {
            if (x > s->x() && x < (s->x() + s->width()) && y > s->y() && y < (s->y() + s->height()))
            {
                greenSquare = s;
                move = validMoveSquares->value(s, Move(static_cast<int>(0), static_cast<int>(0)));
                break;
            }
        }
        if (greenSquare != nullptr) // clicked on green square
        {
            movePiece(move, false);
            chessBoard->activePlayer = !chessBoard->activePlayer;
            ai->add_opening_move(move, chessBoard->moveCounter - 1);
            hideGreenSquares();
            if (chessBoard->endOfGame == 0)
            {
                if (!chessBoard->is_move_possible())
                {
                    endGame();
                }
                // make AI move
                if (singleplayer && promotablePawn == nullptr)
                {
                    makeAIMove();
                }
            }
            else
            {
                endGame();
            }
        }
        hideGreenSquares();
    }
    evaluationRunning = false;
}

void Mediator::hideGreenSquares()
{
    // get all green squares
    QQuickItem *greenSquares[64];
    // hide all green squares
    for (int i = 0; i < 64; i++)
    {
        greenSquares[i] = getGreenSquareAt(i);
        if (greenSquares[i])
        {
            greenSquares[i]->setVisible(false);
        }
    }
    validMoveSquares->clear();
    QCoreApplication::processEvents();
    delay(100);
}

void Mediator::showMoveSet(uint8_t pieceIndex, QRect &square)
{
    int8_t pieceNr = chessBoard->board[pieceIndex];
    char piece;
    if (pieceNr == 1) piece = 'P';
    else if (pieceNr == 11) piece = 'R';
    else if (pieceNr == 12) piece = 'N';
    else if (pieceNr == 13) piece = 'B';
    else if (pieceNr == 14) piece = 'Q';
    else if (pieceNr == 10) piece = 'K';
    else if (pieceNr == 2) piece = 'p';
    else if (pieceNr == 21) piece = 'r';
    else if (pieceNr == 22) piece = 'n';
    else if (pieceNr == 23) piece = 'b';
    else if (pieceNr == 24) piece = 'q';
    else if (pieceNr == 20) piece = 'k';
    else piece = '\0';
    // if the clicked square isn't empty
    if (piece != '\0')
    {
        QQuickItem *guiPiece = nullptr;
        QList<QQuickItem *> guiPieces = getPieces(piece);
        if (guiPieces.count() == 0) return;
        if (guiPieces.count() == 1) guiPiece = guiPieces[0];
        else
        {
            // search the clicked piece if there are more than one of the same kind one the board
            for (QQuickItem *p : guiPieces)
            {
                if (square.x() > p->x() && square.x() < (p->x() + p->width())
                    && square.y() > p->y() && square.y() < (p->y() + p->height()))
                {
                    guiPiece = p;
                    break;
                }
            }
        }
        if (guiPiece != nullptr)
        {
            selectedPiece = guiPiece;
            bool white = chessBoard->activePlayer == Color::WHITE;
            char pawn = white ? 'P' : 'p';
            char rook = white ? 'R' : 'r';
            char knight = white ? 'N' : 'n';
            char bishop = white ? 'B' : 'b';
            char queen = white ? 'Q' : 'q';
            char king = white ? 'K' : 'k';
            std::vector<Move> moveset;
            if (piece == pawn) moveset = chessBoard->get_legal_moveset(pieceIndex, Piece::PAWN);
            else if (piece == rook) moveset = chessBoard->get_legal_moveset(pieceIndex, Piece::ROOK);
            else if (piece == knight) moveset = chessBoard->get_legal_moveset(pieceIndex, Piece::KNIGHT);
            else if (piece == bishop) moveset = chessBoard->get_legal_moveset(pieceIndex, Piece::BISHOP);
            else if (piece == queen) moveset = chessBoard->get_legal_moveset(pieceIndex, Piece::QUEEN);
            else if (piece == king) moveset = chessBoard->get_legal_moveset(pieceIndex, Piece::KING);
            if (!moveset.empty())
            {
                validMoveSquares->clear();
                for (Move m : moveset)
                {
                    int squareIndex = index_10x12_to_8x8(m.to);
                    QQuickItem *greenSquare = getGreenSquareAt(squareIndex);
                    greenSquare->setVisible(true);
                    validMoveSquares->insert(greenSquare, m);
                }
            }
        }
        else
        {
            selectedPiece = nullptr;
        }
    }
}

void Mediator::movePiece(Move m, bool guiOnly)
{
    // animate move
    double currentX = selectedPiece->x();
    double currentY = selectedPiece->y();
    double squareWidth = root->findChild<QQuickItem *>("chessBoard")->width() / 8;
    double squareHeight = root->findChild<QQuickItem *>("chessBoard")->height() / 8;
    int destinationPos = index_10x12_to_8x8(m.to);
    double destinationX = (destinationPos % 8) * squareWidth;
    double destinationY = (destinationPos / 8) * squareHeight;
    int steps = 5;
    if (chessBoard->board[m.from] == 22 || chessBoard->board[m.from] == 12) // knights
    {
        int ms = static_cast<int>(steps * moveAnimationDuration / (std::abs(currentX - destinationX) + std::abs(currentY - destinationY)));
        if (m.to == m.from + 12)
        {
            while (currentX < destinationX)
            {
                currentX += steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
            while (currentY < destinationY)
            {
                currentY += steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
        if (m.to == m.from + 21)
        {
            while (currentY < destinationY)
            {
                currentY += steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
            while (currentX < destinationX)
            {
                currentX += steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
        }
        if (m.to == m.from + 19)
        {
            while (currentY < destinationY)
            {
                currentY += steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
            while (currentX > destinationX)
            {
                currentX -= steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
        }
        if (m.to == m.from + 8)
        {
            while (currentX > destinationX)
            {
                currentX -= steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
            while (currentY < destinationY)
            {
                currentY += steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
        if (m.to == m.from - 12)
        {
            while (currentX > destinationX)
            {
                currentX -= steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
            while (currentY > destinationY)
            {
                currentY -= steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
        if (m.to == m.from - 21)
        {
            while (currentY > destinationY)
            {
                currentY -= steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
            while (currentX > destinationX)
            {
                currentX -= steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
        }
        if (m.to == m.from - 19)
        {
            while (currentY > destinationY)
            {
                currentY -= steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
            while (currentX < destinationX)
            {
                currentX += steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
        }
        if (m.to == m.from - 8)
        {
            while (currentX < destinationX)
            {
                currentX += steps;
                selectedPiece->setX(currentX);
                delay(ms);
            }
            while (currentY > destinationY)
            {
                currentY -= steps;
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
    }
    else
    {
        int ms = std::min(
                    static_cast<int>(currentX - destinationX) != 0 ? std::abs(static_cast<int>(steps * moveAnimationDuration / (currentX - destinationX))) : 2 * moveAnimationDuration,
                    static_cast<int>(currentY - destinationY) != 0 ? std::abs(static_cast<int>(steps * moveAnimationDuration / (currentY - destinationY))) : 2 * moveAnimationDuration
        );
        if (currentX <= destinationX && currentY <= destinationY)
        {
            while (currentX < destinationX || currentY < destinationY)
            {
                if (currentX < destinationX) currentX += steps;
                if (currentY < destinationY) currentY += steps;
                selectedPiece->setX(currentX);
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
        else if (currentX <= destinationX && currentY > destinationY)
        {
            while (currentX < destinationX || currentY > destinationY)
            {
                if (currentX < destinationX) currentX += steps;
                if (currentY > destinationY) currentY -= steps;
                selectedPiece->setX(currentX);
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
        else if (currentX > destinationX && currentY <= destinationY)
        {
            while (currentX > destinationX || currentY < destinationY)
            {
                if (currentX > destinationX) currentX -= steps;
                if (currentY < destinationY) currentY += steps;
                selectedPiece->setX(currentX);
                selectedPiece->setY(currentY);
                delay(ms);
            }
        }
        else if (currentX > destinationX && currentY > destinationY)
    {
        while (currentX > destinationX || currentY > destinationY)
        {
            if (currentX > destinationX) currentX -= steps;
            if (currentY > destinationY) currentY -= steps;
            selectedPiece->setX(currentX);
            selectedPiece->setY(currentY);
            delay(ms);
        }
    }
    }
    // set final location through pos property
    selectedPiece->setProperty("pos", index_10x12_to_8x8(m.to));
    if (!chessBoard->is_empty(chessBoard->board[m.to]))
    {
        QQuickItem *guiPiece = getPieceByIndex(m.to);
        if (guiPiece != nullptr)
        {
            guiPiece->setVisible(false);
        }
    }
    if (!guiOnly)
    {
        chessBoard->move_piece(m);
    }
    if (m.flag == 1) //promotion
    {
        promotablePawn = selectedPiece;
        if (!singleplayer || chessBoard->activePlayer == Color::WHITE) // player promotion
        {
            root->findChild<QQuickItem *>("selectionPromotion")->setVisible(true);
        }
        else // AI promotion
        {
            uint8_t index_10x12 = index_8x8_to_10x12(promotablePawn->property("pos").toInt());
            Piece pawn_to_piece = ai->get_promote_pawn(chessBoard, index_10x12);
            promotePawn(pawn_to_piece, true);
        }
    }
    else if(m.flag == 3) // castling
    {
        // white castling
        if (m.from == 95)
        {
            QList<QQuickItem *> rooks = getPieces('R');
            QQuickItem *rookQueenside = nullptr;
            QQuickItem *rookKingside = nullptr;
            for (QQuickItem *r : rooks)
            {
                if (static_cast<int>(r->x()) == 0) rookQueenside = r;
                else if (static_cast<int>(r->x()) == static_cast<int>(7 * squareWidth)) rookKingside = r;
            }
            // kingside
            if (m.to == 97 && rookKingside != nullptr)
            {
                selectedPiece = rookKingside;
                movePiece(Move(98, 96), true);
                rookKingside->setVisible(true);
            }
            // queenside
            else if (rookQueenside != nullptr)
            {
                selectedPiece = rookQueenside;
                movePiece(Move(91, 94), true);
                rookQueenside->setVisible(true);
            }
        }
        // black castling
        else
        {
            QList<QQuickItem *> rooks = getPieces('r');
            QQuickItem *rookQueenside = nullptr;
            QQuickItem *rookKingside = nullptr;
            for (QQuickItem *r : rooks)
            {
                if (static_cast<int>(r->x()) == 0) rookQueenside = r;
                else if (static_cast<int>(r->x()) == static_cast<int>(7 * squareWidth)) rookKingside = r;
            }
            // kingside
            if (m.to == 27 && rookKingside != nullptr)
            {
                selectedPiece = rookKingside;
                movePiece(Move(28, 26), true);
                rookKingside->setVisible(true);
            }
            // queenside
            else if (rookQueenside != nullptr)
            {
                selectedPiece = rookQueenside;
                movePiece(Move(21, 24), true);
                rookQueenside->setVisible(true);
            }
        }
    }
    // sync the GUI with the underlying board-array to remove pawns that got captured through en passant
    refreshGUI();
    chessBoard->is_move_possible();
}

void Mediator::makeAIMove()
{
    if (!chessBoard->is_move_possible())
    {
        endGame();
        return;
    }
    try
    {
        Move m = ai->get_next_move(chessBoard);
        QQuickItem *guiPiece = getPieceByIndex(m.from);
        if (guiPiece != nullptr)
        {
            selectedPiece = guiPiece;
            movePiece(m, false);
        }
        chessBoard->activePlayer = !chessBoard->activePlayer;
        if (!chessBoard->is_move_possible())
        {
            endGame();
        }
    }
    catch (...)
    {
        endGame();
    }
}

bool Mediator::checkFEN(const char* fen)
{
    bool valid = chessBoard->load_from_FEN(fen);
    chessBoard->init();
    if (valid)
    {
        chessBoard->load_from_FEN(fen);
        refreshGUI();
        if (chessBoard->activePlayer == Color::BLACK)
        {
            delay(1000);
            makeAIMove();
        }
    }
    return valid;
}

bool Mediator::saveFEN(QString location)
{
    QFile file(location.replace("file:///", ""));
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream out(&file);
        out << chessBoard->convert_to_FEN().c_str();
        file.close();
        return true;
    }
    return false;
}

void Mediator::refreshGUI()
{
    // get all (GUI-) pieces
    QList<QQuickItem *> whitePawns = getPieces('P');
    QList<QQuickItem *> whiteRooks = getPieces('R');
    QList<QQuickItem *> whiteKnights = getPieces('N');
    QList<QQuickItem *> whiteBishops = getPieces('B');
    QQuickItem *whiteQueen = getPieces('Q').at(0);
    QQuickItem *whiteKing = getPieces('K').at(0);
    QList<QQuickItem *> blackPawns = getPieces('p');
    QList<QQuickItem *> blackRooks = getPieces('r');
    QList<QQuickItem *> blackKnights = getPieces('n');
    QList<QQuickItem *> blackBishops = getPieces('b');
    QQuickItem *blackQueen = getPieces('q').at(0);
    QQuickItem *blackKing = getPieces('k').at(0);
    // remember the already positioned pieces
    int wP = 0;
    int wR = 0;
    int wN = 0;
    int wB = 0;
    bool wQ = false;
    bool wK = false;
    int bP = 0;
    int bR = 0;
    int bN = 0;
    int bB = 0;
    bool bQ = false;
    bool bK = false;
    // move all pieces to their correct position
    for (int i = 0; i < 120; i++)
    {
        QQuickItem *piece = nullptr;
        switch (chessBoard->board[i])
        {
        case 1:  { piece = wP < whitePawns.count() ? whitePawns.at(wP++) : nullptr; break; }
        case 2:  { piece = bP < blackPawns.count() ? blackPawns.at(bP++) : nullptr; break; }
        case 11: { piece = wR < whiteRooks.count() ? whiteRooks.at(wR++) : nullptr; break; }
        case 21: { piece = bR < blackRooks.count() ? blackRooks.at(bR++) : nullptr; break; }
        case 12: { piece = wN < whiteKnights.count() ? whiteKnights.at(wN++) : nullptr; break; }
        case 22: { piece = bN < blackKnights.count() ? blackKnights.at(bN++) : nullptr; break; }
        case 13: { piece = wB < whiteBishops.count() ? whiteBishops.at(wB++) : nullptr; break; }
        case 23: { piece = bB < blackBishops.count() ? blackBishops.at(bB++) : nullptr; break; }
        case 14: { piece = whiteQueen; wQ = true; break; }
        case 24: { piece = blackQueen; bQ = true; break; }
        case 10: { piece = whiteKing; wK = true; break; }
        case 20: { piece = blackKing; bK = true; break; }
        }

        if (piece != nullptr)
        {
            piece->setProperty("pos", index_10x12_to_8x8(i));
            piece->setVisible(true);
        }
    }
    // hide all captured pieces
    for (int i = wP; i < whitePawns.count(); i++) whitePawns.at(i)->setVisible(false);
    for (int i = wR; i < whiteRooks.count(); i++) whiteRooks.at(i)->setVisible(false);
    for (int i = wN; i < whiteKnights.count(); i++) whiteKnights.at(i)->setVisible(false);
    for (int i = wB; i < whiteBishops.count(); i++) whiteBishops.at(i)->setVisible(false);
    for (int i = bP; i < blackPawns.count(); i++) blackPawns.at(i)->setVisible(false);
    for (int i = bR; i < blackRooks.count(); i++) blackRooks.at(i)->setVisible(false);
    for (int i = bN; i < blackKnights.count(); i++) blackKnights.at(i)->setVisible(false);
    for (int i = bB; i < blackBishops.count(); i++) blackBishops.at(i)->setVisible(false);
    whiteQueen->setVisible(wQ);
    whiteKing->setVisible(wK);
    blackQueen->setVisible(bQ);
    blackKing->setVisible(bK);
    QCoreApplication::processEvents();
    delay(100);
}

void Mediator::setMode(bool singleplayer)
{
    this->singleplayer = singleplayer;
}

void Mediator::setAIDifficulty(Difficulty d)
{
    ai->set_difficulty(d);
}

void Mediator::newGame()
{
    chessBoard->init();
    refreshGUI();
}

void Mediator::promotePawn(Piece promotion, bool ai)
{
    uint8_t index_10x12 = index_8x8_to_10x12(promotablePawn->property("pos").toInt());
    chessBoard->promote_pawn(index_10x12, promotion);
    promotablePawn = nullptr;
    refreshGUI();
    //Fehler: nach black promotion
    if (!ai)
    {
        delay(250);
        makeAIMove();
    }
}

void Mediator::endGame()
{
    QString msg = "Game Over: ";
    switch (chessBoard->endOfGame)
    {
    case 1:
        msg += "white wins";
        break;
    case 2:
        msg += "black wins";
        break;
    case 3:
        msg += "draw";
        break;
    }
    root->findChild<QQuickItem *>("gameOverMessage")->setProperty("text", msg);
    delay(1000);
    root->findChild<QQuickItem *>("gameOver")->setVisible(true);
}
