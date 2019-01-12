#ifndef DHBW_CHESS_MOVEGENERATOR
#define DHBW_CHESS_MOVEGENERATOR

#include <vector>
#include <cstdint>
#include <string>

enum class Color : uint8_t
{
    BLACK,
    WHITE
};

enum class Piece
{
    KING,
    QUEEN,
    ROOK,
    KNIGHT,
    BISHOP,
    PAWN
};

/**
 * overload operator '!' so that
 * 
 *      !Color::WHITE equals Color::BLACK 
 * 
 * and vice versa
 */
inline Color operator !(Color c)
{
    if (c == Color::WHITE) return Color::BLACK;
    return Color::WHITE;
};

class Move
{
    public:
    Move(const char from[2], const char to[2]) { 
        this->from  =  (10 - (int)(from[1] - 48)) * 10 + ((int)(toupper(from[0])) - 64); 
        this->to    =  (10 - (int)  (to[1] - 48)) * 10 + ((int)(toupper(  to[0])) - 64);  
    };

    Move(uint8_t from, uint8_t to) : from(from), to (to) { };
    Move(uint8_t from, uint8_t to, uint8_t flag) : from(from), to (to), flag(flag) {};
    uint8_t from;       // index
    uint8_t to;         // index
    uint8_t flag = 0;   // 1: promotion, 2: en passant, 3: castling
    friend bool operator==(const Move& m1, const Move& m2){
        if(m1.from == m2.from && m1.to == m2.to && m1.flag == m2.flag){
            return true;
        }
        return false;
    }
    //es wird nur auf move.to abgeglichen
    friend bool operator==(const Move& m1, const char square[2]){
        if(m1.to == (10 - (int)(square[1] - 48)) * 10 + ((int)(toupper(square[0])) - 64)){
            return true;
        }
        return false;
    }
};


    

class ChessBoard
{
    public:
        int8_t board[120];
        Color activePlayer;
        uint8_t endOfGame = 0; // 0: running, 1: checkmate - white wins, 2: checkmate - black wins, 3: draw
        uint8_t moveCounter = 0;
        int gameValue = 0;
        void init();
        void end_game();
        void print();
        void print_moveset();
        bool is_empty(int8_t p);
        Color get_color(uint8_t index);
        void move_piece(Move m, bool aiMove);
        void move_piece(Move m, bool ingoreFlag, bool aiMove);
        bool is_legal(Move m, Color color);
        bool is_move_possible();
        std::vector<Move> get_moveset_all(Color color);
        std::vector<Move> get_legal_moveset(uint8_t index, Piece type);
        std::string convert_to_FEN();
        bool load_from_FEN(const char*);
        uint8_t is_king_in_check(uint8_t index, Color color);
    private:
        uint8_t pieceCounter = 32;
        uint8_t enPassant;
        bool white_castling_kingside = true;    //
        bool white_castling_queenside = true;   // only check, whether king or rook have move during the game;
        bool black_castling_kingside = true;    // other castling-rules are checked during move generation
        bool black_castling_queenside = true;   //
        std::vector<Move> get_moveset_king(uint8_t index);
        std::vector<Move> get_moveset_king(uint8_t index, Color color);
        std::vector<Move> get_moveset_queen(uint8_t index);
        std::vector<Move> get_moveset_queen(uint8_t index, Color color);
        std::vector<Move> get_moveset_rook(uint8_t index);
        std::vector<Move> get_moveset_rook(uint8_t index, Color color);
        std::vector<Move> get_moveset_knight(uint8_t index);
        std::vector<Move> get_moveset_knight(uint8_t index, Color color);
        std::vector<Move> get_moveset_bishop(uint8_t index);
        std::vector<Move> get_moveset_bishop(uint8_t index, Color color);
        std::vector<Move> get_moveset_pawn(uint8_t index);
        std::vector<Move> get_moveset_pawn(uint8_t index, Color color);
        bool is_white(int8_t p);
        bool is_black(int8_t p);
        void promote_pawn(uint8_t index, bool aiMove);
        void check_draw();
        char* index_to_piece(uint8_t index);
};

#endif
