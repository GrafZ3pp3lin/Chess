#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"

int MAX_DEPTH = 4;

Move getNextMove(ChessBoard *board) {
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    std::vector<ChessBoard> boards;
    int index;
    int highest = 0;
    //Hier in Threads aufteilen
    for (int i = 0; i < moveset.size(); i++) {
        if (board->is_legal(moveset[i], board->activePlayer)) {
            ChessBoard boardCopy{*board};
            boardCopy.move_piece(moveset[i]);
            boardCopy.setGameValue(evaluate_board(boardCopy.board));
            boardCopy.activePlayer = !boardCopy.activePlayer;
            int temp = calculate(boardCopy, true, 1);
            if (highest < temp) {
                highest = temp;
                index = i;
            }
        }
    }
    return moveset[index];
}

int calculate(ChessBoard &board, bool enemy, int depth) {
    if (depth == MAX_DEPTH) {
        return evaluate_board(board.board);
    }
    std::vector<Move> moveset = board.get_moveset_all(board.activePlayer);
    std::vector<ChessBoard> boards;
    std::vector<int> results;
    for (Move m : moveset) {
        if (board.is_legal(m, board.activePlayer)) {
            ChessBoard boardCopy{board};
            boardCopy.move_piece(m);
            boardCopy.setGameValue(evaluate_board(boardCopy.board));
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
    }
    if (enemy) {
        int highest = 0;
        for (ChessBoard &b : boards) {
            if (b.getAbsGameValue() > highest) {
                highest = b.getAbsGameValue();
            }
        }
        for (ChessBoard &b : boards) {
            if (b.getAbsGameValue() == highest) {
                results.push_back(calculate(b, false, depth+1));
            }
        }
    }
    else {
        for (ChessBoard &b : boards) {
            results.push_back(calculate(b, true, depth+1));
        }
    }
    return getLowest(std::move(results));
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

int getAverage(std::vector<int> values) {
    int avg = 0;
    for (int value : values) {
        avg += value;
    }
    return avg / values.size();
}