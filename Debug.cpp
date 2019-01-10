#include "main.hpp"
#include "KI.hpp"
#include <string>
#include "MoveGenerator.hpp"

int main(int argc, char const *argv[])
{
    ChessBoard* chessBoard;
    chessBoard = new ChessBoard{};
    //chessBoard->init(true);
    chessBoard->load_from_FEN("8/8/5k1K/8/8/8/8/6q1 b - - 0 1");
    chessBoard->singleplayergame = true;

    std::vector<Move> moveset = chessBoard->get_moveset_all(chessBoard->activePlayer);
    //printf_s("%s %i (id: %lu)\n", "moveset size", moveset.size(), std::this_thread::get_id());
    std::vector<ChessBoard> boards;
    for (Move m : moveset) {
        printf_s("%s from: %i to: %i\n", "check Move: ", m.from, m.to);
        if (chessBoard->is_legal(m, chessBoard->activePlayer)) {
            ChessBoard boardCopy{*chessBoard};
            boardCopy.move_piece(m, true);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);
        }
        printf_s("%s from: %i to: %i\n", "finish check Move: ", m.from, m.to);
    }
    //7R/8/8/8/5K1k/8/8/8 b - - 0 1
    //std::cout << chessBoard->load_from_FEN("7K/8/k1P5/7p/8/8/8/8 w KQkq - 0 1") << std::endl; // so könnte momentan ein Spiel in FEN-Notation geladen werden

    /*
    while (!chessBoard->endOfGame)
    {
        chessBoard->print();
        Move player_move = get_move_of_player(chessBoard);
        chessBoard->move_piece(player_move);
        chessBoard->activePlayer = Color::BLACK;

        //AI move
        if(chessBoard->singleplayergame){
            chessBoard->print();
            std::cout << " < AI calculates Move..." << std::endl;

            Move ai_move = getNextMove(chessBoard);
            chessBoard->move_piece(ai_move);
            std::cout << " < AI move: " << index_to_square(ai_move.from) << " -> " << index_to_square(ai_move.to) << std::endl;
            chessBoard->activePlayer = Color::WHITE;
        }
        else{

        }
    }
    chessBoard->print();
    chessBoard->end_game();
    */
    delete chessBoard;

    printf_s("%s", "FINISH");
    
    return 0;
}

void save(std::string filename) {
    std::ofstream myfile;
    myfile.open (filename);
    //myfile.write( chessBoard->convert_to_FEN().c_str());
    myfile.close();
}

ChessBoard* load(std::string filename){
    std::ifstream infile;
	infile.open( filename );
    std::string line;
    std::getline(infile, line);
    infile.close();
    return nullptr;
}
