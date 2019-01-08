#include "MoveGenerator.hpp"
#include "Player.hpp"

int main(int argc, char const *argv[])
{
    ChessBoard* chessBoard = new ChessBoard{};
    chessBoard->init();

    // AI with random move selection
    while (!chessBoard->endOfGame)
    {
        chessBoard->print();
        Move player_move = get_move_of_player(chessBoard);
        chessBoard->move_piece(player_move);
        chessBoard->activePlayer = Color::BLACK;
        std::vector<Move> moveset = chessBoard->get_moveset_all(chessBoard->activePlayer);
        if (moveset.size() > 0 && !chessBoard->endOfGame)
        {
            int i = rand() % moveset.size();
            while (!chessBoard->is_legal(moveset[i], chessBoard->activePlayer))
            {
                i++;
                if (i >= moveset.size()) i = 0;
            }
            Move ai_move = moveset[i];
            chessBoard->move_piece(ai_move);
            std::cout << " < AI move: " << index_to_square(ai_move.from) << " -> " << index_to_square(ai_move.to) << std::endl;
            chessBoard->activePlayer = Color::WHITE;
        }
    }
    chessBoard->print();
    chessBoard->end_game();
    
    return 0;
}
