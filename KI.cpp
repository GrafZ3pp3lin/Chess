#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"
#include <iostream>
#include <thread>
#include <future>

int MAX_DEPTH = 5;

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

//TEMP
const char* temp(int8_t index)
{
    char* square = new char[3];
    square[0] = (char)((index % 10) + 64);
    square[1] = (char)(10 - (index / 10) + 48);
    square[2] = '\0';
    return square;
}

int calculate(ChessBoard &board, bool oponent, int depth);

RatedMove startCalculateMove(Move m, ChessBoard *board) {
    ChessBoard boardCopy{*board};
    boardCopy.move_piece(m, true);
    boardCopy.activePlayer = !boardCopy.activePlayer;
    return RatedMove {m, calculate(boardCopy, true, 1)};
}

Move getNextMove(ChessBoard *board) {
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    std::vector<std::future<RatedMove>> futures;
    std::vector<RatedMove> moves;
    int index;
    //Hier in Threads aufteilen
    for (int i = 0; i < moveset.size(); i++) {
        if (board->is_legal(moveset[i], board->activePlayer)) {
            futures.push_back(std::async(&startCalculateMove, moveset[i], board));
        }
        /*if (board->is_legal(moveset[i], board->activePlayer)) {
            ChessBoard boardCopy{*board};
            boardCopy.move_piece(moveset[i], true);
            //boardCopy.gameValue = evaluate_board(&boardCopy);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            moves.push_back(RatedMove {moveset[i], calculate(boardCopy, true, 1)});
        }*/
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
        throw "NoPossibleMoveError";
    }
    return bestMoves[rand() % bestMoves.size()].move;
}

int calculate(ChessBoard &board, bool oponent, int depth) {
    if (depth == MAX_DEPTH) {
        return evaluate_board(&board);
    }
    std::vector<Move> moveset = board.get_moveset_all(board.activePlayer);
    std::vector<ChessBoard> boards;
    std::vector<int> results;
    for (Move m : moveset) {
        if (board.is_legal(m, board.activePlayer)) {
            ChessBoard boardCopy{board};
            boardCopy.move_piece(m, true);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
    }
    if (oponent) {
        /*
        Value v;
        v.highest = 1;
        for (ChessBoard &b : boards) {
            b.gameValue = evaluate_board(&b);
            if (b.activePlayer != Color::WHITE) {
                if (b.gameValue > v.highest || v.highest == 1) {
                    v.highest = b.gameValue;
                }
            }
            else {
                if (b.gameValue < v.lowest || v.lowest == 1) {
                    v.lowest = b.gameValue;
                }
            }
        }*/
        for (ChessBoard &b : boards) {
            //Only calculate with best Moves from Oponent
            if ((b.activePlayer != Color::WHITE && b.gameValue >= 0) || (b.activePlayer != Color::BLACK && b.gameValue <= 0)) {
                //KI Moves
                int temp = calculate(b, false, depth+1);
                if (temp != 1) {
                    results.push_back(temp);
                }
            }
        }
    }
    else {
        for (ChessBoard &b : boards) {
            //Oponent Moves
            int temp = calculate(b, true, depth+1);
            if (temp != 1) {
                results.push_back(temp);
            }
        }
    }
    if (results.size() > 0) {
        // calculate best move for KI (also mind the best Move for Oponent!)
        if (board.activePlayer == Color::BLACK) {
            return getLowest(std::move(results));
        }
        else {
            return getHighest(std::move(results));
        }
    }
    else {
        return evaluate_board(&board);
    }
}