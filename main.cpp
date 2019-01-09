#include "main.hpp"
#include "KI.hpp"
#include <string>
#include "MoveGenerator.hpp"


ChessBoard* chessBoard;

int main(int argc, char const *argv[])
{
    std::cout << " > Do you want to load an existing game? [N/y]" << std::endl;
    std::string in;
    std::getline(std::cin, in);

    ChessBoard* chessBoard;

    if(in == "y"){
        std::cout << " > Please type in your savegame path"  << std::endl;
        std::string path;
        std::cin >> path;
        chessBoard = load(path.c_str());
    }
    else{
        chessBoard = new ChessBoard{};
        chessBoard->init(true);
        chessBoard->convert_to_FEN();
    }

    // AI with random move selection
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

    delete chessBoard;
    
    return 0;
}

void save(std::string filename) {
    std::ofstream myfile;
    myfile.open (filename);
    myfile.write( chessBoard->convert_to_FEN().c_str());
    myfile.close();
}

ChessBoard* load(std::string filename){
    std::ifstream infile;
	infile.open( filename );
    std::string line;
    std::getline(infile, line)
    infile.close();
    return ptr;
}
