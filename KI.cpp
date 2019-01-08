#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"

int MAX_DEPTH = 4;

Move getNextMove(ChessBoard *board) {
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    std::vector<ChessBoard> boards;
    int index;
    Value v;
    v.highest = 0;
    //Hier in Threads aufteilen
    for (int i = 0; i < moveset.size(); i++) {
        if (board->is_legal(moveset[i], board->activePlayer)) {
            ChessBoard boardCopy{*board};
            boardCopy.move_piece(moveset[i]);
            //boardCopy.gameValue = evaluate_board(&boardCopy);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            int temp = calculate(boardCopy, true, 1);
            if (boardCopy.activePlayer != Color::WHITE) {
                if (temp > v.highest) {
                    v.highest = temp;
                    index = i;
                }
            }
            else {
                if (temp < v.lowest) {
                    v.lowest = temp;
                    index = i;
                }
            }
        }
    }
    return moveset[index];
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
            boardCopy.move_piece(m);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
    }
    if (oponent) {
        Value v;
        v.highest = 0;
        for (ChessBoard &b : boards) {
            b.gameValue = evaluate_board(&b);
            if (b.activePlayer != Color::WHITE) {
                if (b.gameValue > v.highest) {
                    v.highest = b.gameValue;
                }
            }
            else {
                if (b.gameValue < v.lowest) {
                    v.lowest = b.gameValue;
                }
            }
        }
        for (ChessBoard &b : boards) {
            //Only calculate with best Moves from Oponent
            if (b.gameValue == v.highest) {
                //KI Moves
                results.push_back(calculate(b, false, depth+1));
            }
        }
    }
    else {
        for (ChessBoard &b : boards) {
            //Oponent Moves
            results.push_back(calculate(b, true, depth+1));
        }
    }
    // calculate best move for KI (also mind the best Move for Oponent!)
    if (board.activePlayer == Color::BLACK) {
        return getLowest(std::move(results));
    }
    else {
        return getHighest(std::move(results));
    }
}

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