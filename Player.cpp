#include "Player.hpp"
#include "main.hpp"

#include <limits>
#include <memory>

int8_t square_to_index(char square[2])
{
    return (10 - (int)(square[1] - 48)) * 10 // rank
        + ((int)(toupper(square[0])) - 64); // file
}

const char* index_to_square(int8_t index)
{
    char* square = new char[3];
    square[0] = (char)((index % 10) + 64);
    square[1] = (char)(10 - (index / 10) + 48);
    square[2] = '\0';
    return square;
}

Move get_move_of_player(std::shared_ptr<ChessBoard>& board)
{
    char buf[2] = { 0, 0 };
    uint8_t from;
    std::vector<Move> moveset;
    while (true)
    {
        while (buf[0] < 65 || buf[0] > 72 || buf[1] < 49 || buf[1] > 56)
        {
            std::cout << " > select a piece: ";
            std::cin >> buf;
            buf[0] = toupper(buf[0]);
            //quit Game
            if(buf[0] == 81 /*Q*/){
                std::cout << std::endl << "  > Do you want to save your game? [Y/n]" << std::endl;
                std::string in;
                std::getline(std::cin, in);

                if(in == "n"){
                
                }
                else if(in == "y" || in == ""){
                    std::cout << " > Please type in the name to save your game"  << std::endl;
                    std::string in;
                    std::getline(std::cin, in);
                    save(in);
                }
                exit(0);
            }
        }
        from = square_to_index(buf);
        if (!board->is_empty(board->board[from]))
        {
            if (board->get_color(from) == board->activePlayer)
            {
                int8_t piece = board->board[from];
                if (piece == 1 || piece == 2) moveset = board->get_legal_moveset(from, Piece::PAWN);
                else if (piece == 11 || piece == 21) moveset = board->get_legal_moveset(from, Piece::ROOK);
                else if (piece == 12 || piece == 22) moveset = board->get_legal_moveset(from, Piece::KNIGHT);
                else if (piece == 13 || piece == 23) moveset = board->get_legal_moveset(from, Piece::BISHOP);
                else if (piece == 14 || piece == 24) moveset = board->get_legal_moveset(from, Piece::QUEEN);
                else if (piece == 10 || piece == 20) moveset = board->get_legal_moveset(from, Piece::KING);
                if (moveset.empty())
                {
                    std::cout << "   no legal moves possible, choose another piece ..." << std::endl;
                    buf[0] = 0;
                    continue;
                }
                else break;
            }
            std::cout << "   wrong color, try again ..." << std::endl;
            buf[0] = 0;
            continue;
        }
        std::cout << "   empty square, try again ..." << std::endl;
        buf[0] = 0;
    }
    for (int i = 0; i < moveset.size(); i++)
    {
        std::cout << "   [" << i << "] " << index_to_square(moveset[i].to) << std::endl;
    }
    int selected_move = -1;
    while (selected_move < 0 || selected_move >= moveset.size())
    {
        std::cout << " > select a move: ";
        std::cin >> selected_move;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            selected_move = -1;
        }
    }
    return moveset[selected_move];
}
