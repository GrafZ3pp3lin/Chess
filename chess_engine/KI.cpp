#include "Evaluation.hpp"
#include "MoveGenerator.hpp"
#include "KI.hpp"
#include <iostream>
#include <future>
#include <limits>
#include <mutex>
#include <ctime>
#include <algorithm>

AI::AI()
{
    MAX_DEPTH = 4;
    useDirectestCheckMate = true;
}

AI::~AI()
{
    delete [] opening.data();
}

void AI::set_difficulty(Difficulty dif) {
    switch (dif) 
    {
        case Difficulty::EASY:
            MAX_DEPTH = 3;
            gameValueOffset = 80;
            useDirectestCheckMate = false;
            break;
        case Difficulty::HARD:
            MAX_DEPTH = 5;
            gameValueOffset = 5;
            useDirectestCheckMate = true;
            break;
        default:
            MAX_DEPTH = 4;
            gameValueOffset = 40;
            useDirectestCheckMate = true;
    }
}

Move AI::get_next_move(std::shared_ptr<ChessBoard>& board)
{
    std::vector<Move> moveset = board->get_moveset_all(board->activePlayer);
    
    //Eröffnungen
    Move m = Move{static_cast<uint8_t>(0), static_cast<uint8_t>(0)};
    if(board->moveCounter < 4 && opening.size() > 0){
        m = opening_move(board);
        if (board->is_legal(m, board->activePlayer))
        {
            for(Move m2: moveset){
                if(m2 == m){
                    //return m2, da in den moves von moveset auch das flag gesetzt wurde, bei den moves von opening_move werden keine flags gesetzt
                    return m2;
                }
            }
        }
    }

    std::vector<std::future<RatedMove>> futures;
    std::vector<RatedMove> moves;
    for (size_t i = 0; i < moveset.size(); i++)
    {
        if (board->is_legal(moveset[i], board->activePlayer))
        {
            //start for each possible Move one Thread
            futures.push_back(std::async(std::launch::async, &AI::start_calculate_move, this, moveset[i], board.get()));
        }
    }
    Value v;
    v.highest = 1;
    std::vector<RatedMove> bestMoves;
    for (size_t i = 0; i < futures.size(); i++)
    {
        auto temp = futures[i].get();
        if (board->activePlayer == Color::WHITE)
        {
            if (temp.value != 1 && (temp.value > v.highest || v.highest == 1))
            {
                v.highest = temp.value;
            }
        }
        else
        {
            if (temp.value != 1 && (temp.value < v.lowest || v.lowest == 1))
            {
                v.lowest = temp.value;
            }
        }
        moves.push_back(temp);
    }
    for (RatedMove &r : moves)
    {
        if ((board->activePlayer == Color::BLACK && r.value <= (v.lowest + gameValueOffset))
            || (board->activePlayer == Color::WHITE && r.value >= (v.highest - gameValueOffset)))
        {
            bestMoves.push_back(r);
        }
    }
    if (bestMoves.size() == 0)
    {
        //KI is in CheckMate
        throw std::runtime_error("NoPossibleMoveError");
    }
    return bestMoves[static_cast<size_t>(rand()) % bestMoves.size()].move;
}

RatedMove AI::start_calculate_move(Move m, ChessBoard *board)
{
    ChessBoard boardCopy{*board};
    boardCopy.move_piece(m, true);
    boardCopy.activePlayer = !boardCopy.activePlayer;
    return RatedMove {m, calculate(boardCopy, 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())};
}

//calculate Moves recursive 
int AI::calculate(ChessBoard &board, int depth, int alpha, int beta)
{
    if (depth == MAX_DEPTH)
    {
        return evaluate_board(&board);
    }
    std::vector<Move> moveset = board.get_moveset_all(board.activePlayer);
    std::vector<ChessBoard> boards;
    for (Move m : moveset)
    {
        if (board.is_legal(m, board.activePlayer))
        {
            ChessBoard boardCopy{board};
            boardCopy.move_piece(m, true);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
    }
    //Alpha Beta Pruning
    Value v;
    v.highest = board.activePlayer == Color::BLACK ? beta : alpha;
    for (ChessBoard &b : boards)
    {
        int temp = calculate(b, depth+1, board.activePlayer == Color::BLACK ? alpha : v.highest, board.activePlayer == Color::WHITE ? beta : v.lowest);
        if (board.activePlayer == Color::BLACK)
        {
            if (useDirectestCheckMate && temp < -1000000)
            {
                temp += 10000;
            }
            if (temp < v.lowest || v.lowest == 1)
            {
                v.lowest = temp;
                if (v.lowest <= alpha)
                {
                    break;
                }
            }
        }
        else
        {
            if (useDirectestCheckMate && temp > 1000000)
            {
                temp -= 10000;
            }
            if (temp > v.highest || v.highest == 1)
            {
                v.highest = temp;
                if (v.highest >= beta)
                {
                    break;
                }
            }
        }
    }
    return v.highest;
}

Piece AI::get_promote_pawn(std::shared_ptr<ChessBoard>& board, uint8_t index)
{
    //Calculate for Queen and Knight
    int best_queen, best_knight;
    for (int i = 0; i < 2; i++) {
        ChessBoard boardCopy{*board.get()};
        boardCopy.promote_pawn(index, i == 0 ? Piece::QUEEN : Piece::KNIGHT);
        boardCopy.activePlayer = !boardCopy.activePlayer;
        (i == 0 ? best_queen : best_knight) = calculate(boardCopy, MAX_DEPTH - 2, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    }
    if ((board->activePlayer == Color::BLACK && best_knight < best_queen)
    || (board->activePlayer == Color::WHITE && best_knight > best_queen)) {
        return Piece::KNIGHT;
    }
    return Piece::QUEEN;
}

void AI::add_opening_move(Move m, int moveCounter)
{
    if(moveCounter == 0){
        opening.clear();
    }
    if(moveCounter < 3)
    {
        opening.push_back(m);
    }
}

Move AI::opening_move(std::shared_ptr<ChessBoard>& board)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    std::vector<Move> possible;

    switch(board->moveCounter)
    {
        //white
        case 0:
        // == Operator ist überschrieben -> Move.to == square_to_index("e4")
            possible.push_back(Move{"e2","e4"});
            possible.push_back(Move{"d2","d4"});
            possible.push_back(Move{"g1","f3"});
            break;
        //black
        case 1:
            if(opening[0] == "e4")
            {
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"e7", "e5"});
                possible.push_back(Move{"c7", "c5"});
            }
            else if(opening[0] == "d4")
            {
                possible.push_back(Move{"g8", "f6"});
                possible.push_back(Move{"d7", "d5"});
            }
            else if(opening[0] == "f3")
            {
                possible.push_back(Move{"g8", "f6"});
            }
            break;
        //white
        case 2:
            if(opening[0] == "e4" && (opening[1] == "c5" || opening[1] == "e5"))
            {
                possible.push_back(Move{"g1", "f3"});
            }
            else if(opening[0] == "e4" && opening[1] == "e6")
            {
                possible.push_back(Move{"d2", "d4"});
                possible.push_back(Move{"d2", "d3"});
            }

            else if(opening[0] == "d4" && (opening[1] == "f6" || opening[1] == "d5"))
            {
                possible.push_back(Move{"c2", "c4"});
                possible.push_back(Move{"g1","f3"});
            }

            else if(opening[0] == "f3" && opening[1] == "f6")
            {
                possible.push_back(Move{"c2", "c4"});
                possible.push_back(Move{"g2","g3"});
                possible.push_back(Move{"d2", "d4"});
            }
            break;
         //black
        case 3:
            if(opening[0] == "e4" && (opening[1] == "c5" || opening[1] == "e5") && opening[2] == "f3")
            {
                possible.push_back(Move{"d7", "d6"});
                possible.push_back(Move{"b8", "c6"});
            }
            else if(opening[0] == "e4" && opening[1] == "e6" && (opening[2] == "d4" || opening[2] == "d3"))
            {
                possible.push_back(Move{"d7", "d5"});
            }

            else if(opening[0] == "d4" && (opening[1] == "f6") && (opening[2] == "c4"))
            {
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"g7", "g6"});
            }
            else if(opening[0] == "d4" && (opening[1] == "d5") && (opening[2] == "c4"))
            {
                possible.push_back(Move{"c7", "c6"});
                possible.push_back(Move{"e7", "e6"});
            }
            else if(opening[0] == "d4" && (opening[1] == "f6") && (opening[2] == "f3"))
            {
                possible.push_back(Move{"d7", "d5"});
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"e7", "e6"});
            }
            else if(opening[0] == "d4" && (opening[1] == "d5") && (opening[2] == "f3"))
            {
                possible.push_back(Move{"g8", "f6"});
            }

            else if(opening[0] == "f3" && (opening[1] == "f6") && (opening[2] == "c4"))
            {
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"c7", "c5"});
            }
            else if(opening[0] == "f3" && (opening[1] == "f6") && (opening[2] == "d4"))
            {
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"e7", "e6"});
                possible.push_back(Move{"d7", "d5"});
            }
            else if(opening[0] == "f3" && (opening[1] == "f6") && (opening[2] == "g3"))
            {
                possible.push_back(Move{"g7", "g6"});
                possible.push_back(Move{"d7", "d5"});
            }
            break;
    }
    if(possible.size() > 0){
        return possible[static_cast<size_t>(rand()) % possible.size()];
    }
    return Move{static_cast<uint8_t>(0), static_cast<uint8_t>(0)};
}
