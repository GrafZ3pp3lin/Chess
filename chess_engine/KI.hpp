#ifndef DHBW_CHESS_KI
#define DHBW_CHESS_KI

#include "MoveGenerator.hpp"

#include <memory>

template <typename T>
bool contains(std::vector<T> vec, T val){
    for(T a: vec){
        if(a == val){
            return true;
        }
    }
    return false;
}

union Value {
    int highest;
    int lowest;
};

enum class Difficulty
{
    EASY,
    NORMAL,
    HARD
};

class RatedMove
{
    public:
    RatedMove(Move move, int value) : move(move), value(value) { }
    Move move;
    int value = 0;
};

class AI
{
    public:
        AI();
        ~AI();
        Move get_next_move(std::shared_ptr<ChessBoard>& board);
        void set_difficulty(Difficulty dif);
        void add_opening_move(Move,int);

    private:
        int MAX_DEPTH;
        int gameValueOffset = 75;
        bool useDirectestCheckMate;
        std::vector<Move> opening;

        RatedMove start_calculate_move(Move m, ChessBoard *board);
        int calculate(ChessBoard &board, bool oponent, int depth, int alpha, int beta);
        Move opening_move(std::shared_ptr<ChessBoard>& board);
};

#endif
