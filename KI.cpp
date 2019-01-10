#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"
#include <iostream>
#include <future>
#include <limits>
#include <mutex>

int MAX_DEPTH = 5;
std::mutex mu;

int getLowest(std::vector<int> values) {
    int lowest = values[0];
    for (int value : values) {
        if (lowest > value) {
            lowest = value;
        }
    }
    return lowest;
}

int getHighest(std::vector<int> values) {
    int highest = values[0];
    for (int value : values) {
        if (highest < value) {
            highest = value;
        }
    }
    return highest;
}

int getAverage(std::vector<int> values) {
    int avg = 0;
    for (int value : values) {
        avg += value;
    }
    return avg / values.size();
}

//------------------------------------DEBUG------------------------------------------------
const char* temp(int8_t index)
{
    char* square = new char[3];
    square[0] = (char)((index % 10) + 64);
    square[1] = (char)(10 - (index / 10) + 48);
    square[2] = '\0';
    return square;
}
//------------------------------------DEBUG------------------------------------------------

int calculate(ChessBoard &board, bool oponent, int depth, int alpha, int beta);

RatedMove startCalculateMove(Move m, ChessBoard *board) {
    ChessBoard boardCopy{*board};
    boardCopy.move_piece(m, true);
    boardCopy.activePlayer = !boardCopy.activePlayer;
    return RatedMove {m, calculate(boardCopy, true, 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())};
}

Move getNextMove(ChessBoard *board) {
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    std::vector<std::future<RatedMove>> futures;
    std::vector<RatedMove> moves;
    int index;
    for (int i = 0; i < moveset.size(); i++) {
        if (board->is_legal(moveset[i], board->activePlayer)) {
            futures.push_back(std::async(std::launch::async, &startCalculateMove, moveset[i], board));
        }
    }
    for (int i = 0; i < futures.size(); i++) {
        moves.push_back(futures[i].get());
    }
    std::vector<RatedMove> bestMoves;
    Value v;
    v.highest = 1;
    for (RatedMove &r : moves) {
        std::cout << "Move: from " << temp(r.move.from) << " to " << temp(r.move.to) << " has value of " << r.value << std::endl;
        if (board->activePlayer == Color::WHITE) {
            if (r.value != 1 && (r.value > v.highest || v.highest == 1)) {
                v.highest = r.value;
            }
        }
        else {
            if (r.value != 1 && (r.value < v.lowest || v.lowest == 1)) {
                v.lowest = r.value;
            }
        }
    }
    for (RatedMove &r : moves) {
        if (r.value == v.highest) {
            bestMoves.push_back(r);
        }
    }
    if (bestMoves.size() == 0) {
        throw std::runtime_error("NoPossibleMoveError");
    }
    return bestMoves[rand() % bestMoves.size()].move;
}

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
            if (temp < v.lowest || v.lowest == 1) {
                v.lowest = temp;
                if (v.lowest <= alpha) {
                    break;
                }
            }
        }
        else {
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