#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"
#include <iostream>
#include <future>
#include <limits>
#include <mutex>

int MAX_DEPTH = 4;
int gameValueOffset = 75;
bool useDirectestCheckMate = true;

void setDifficulty(Difficulty dif) {
    switch (dif) 
    {
        case Difficulty::EASY:
            MAX_DEPTH = 3;
            gameValueOffset = 150;
            useDirectestCheckMate = false;
            break;
        case Difficulty::HARD:
            MAX_DEPTH = 5;
            gameValueOffset = 10;
            useDirectestCheckMate = true;
            break;
        default:
            MAX_DEPTH = 4;
            gameValueOffset = 75;
            useDirectestCheckMate = true;
    }
}

//------------------------------------DEBUG------------------------------------------------
const char* convert(int8_t index)
{
    char* square = new char[3];
    square[0] = (char)((index % 10) + 64);
    square[1] = (char)(10 - (index / 10) + 48);
    square[2] = '\0';
    return square;
}
//------------------------------------DEBUG------------------------------------------------

int calculate(ChessBoard &board, bool oponent, int depth, int alpha, int beta);
RatedMove startCalculateMove(Move m, ChessBoard *board);

Move getNextAIMove(ChessBoard *board) {
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    std::vector<std::future<RatedMove>> futures;
    std::vector<RatedMove> moves;
    int index;
    for (int i = 0; i < moveset.size(); i++) {
        if (board->is_legal(moveset[i], board->activePlayer)) {
            //start for each possible Move one Thread
            futures.push_back(std::async(std::launch::async, &startCalculateMove, moveset[i], board));
        }
    }
    Value v;
    v.highest = 1;
    std::vector<RatedMove> bestMoves;
    for (int i = 0; i < futures.size(); i++) {
        auto temp = futures[i].get();
        //DEBUG std::cout << "Move: from " << convert(temp.move.from) << " to " << convert(temp.move.to) << " has value of " << temp.value << std::endl;
        if (board->activePlayer == Color::WHITE) {
            if (temp.value != 1 && (temp.value > v.highest || v.highest == 1)) {
                v.highest = temp.value;
            }
        }
        else {
            if (temp.value != 1 && (temp.value < v.lowest || v.lowest == 1)) {
                v.lowest = temp.value;
            }
        }
        moves.push_back(temp);
    }
    for (RatedMove &r : moves) {
        if ((board->activePlayer == Color::BLACK && r.value <= (v.lowest + gameValueOffset))
            || (board->activePlayer == Color::WHITE && r.value >= (v.highest - gameValueOffset))) {
            bestMoves.push_back(r);
        }
    }
    if (bestMoves.size() == 0) {
        //KI is in CheckMate
        throw std::runtime_error("NoPossibleMoveError");
    }
    return bestMoves[rand() % bestMoves.size()].move;
}

RatedMove startCalculateMove(Move m, ChessBoard *board) {
    ChessBoard boardCopy{*board};
    boardCopy.move_piece(m, true);
    boardCopy.activePlayer = !boardCopy.activePlayer;
    return RatedMove {m, calculate(boardCopy, true, 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())};
}

//calculate Moves recursive 
int calculate(ChessBoard &board, bool oponent, int depth, int alpha, int beta) {
    if (depth == MAX_DEPTH) {
        return evaluate_board(&board);
    }
    std::vector<Move> moveset = board.get_moveset_all(board.activePlayer);
    std::vector<ChessBoard> boards;
    for (Move m : moveset) {
        if (board.is_legal(m, board.activePlayer)) {
            ChessBoard boardCopy{board};
            boardCopy.move_piece(m, true);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
    }
    //Alpha Beta Pruning
    Value v;
    v.highest = board.activePlayer == Color::BLACK ? beta : alpha;
    for (ChessBoard &b : boards) {
        int temp = calculate(b, !oponent, depth+1, board.activePlayer == Color::BLACK ? alpha : v.highest, board.activePlayer == Color::WHITE ? beta : v.lowest);
        if (board.activePlayer == Color::BLACK) {
            if (useDirectestCheckMate && temp < -1000000) {
                temp += 10000;
            }
            if (temp < v.lowest || v.lowest == 1) {
                v.lowest = temp;
                if (v.lowest <= alpha) {
                    break;
                }
            }
        }
        else {
            if (useDirectestCheckMate && temp > 1000000) {
                temp -= 10000;
            }
            if (temp > v.highest || v.highest == 1) {
                v.highest = temp;
                if (v.highest >= beta) {
                    break;
                }
            }
        }
    }
    return v.highest;
}