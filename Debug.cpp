#include "main.hpp"
#include "KI.hpp"
#include <string>
#include "MoveGenerator.hpp"

int main(int argc, char const *argv[])
{
    ChessBoard* chessBoard;
    chessBoard = new ChessBoard{};
    chessBoard->load_from_FEN((char*)"8/8/5k1K/8/8/8/8/6q1 b - - 0 1");
    chessBoard->activePlayer = Color::BLACK;
    setDifficulty(Difficulty::EASY);
    // AI with random move selection
    while (!chessBoard->endOfGame)
    {
        if (!chessBoard->is_move_possible()) {
            continue; //continue?
        }
        std::cout << std::endl << "*******************************************************************" << std::endl;
        chessBoard->print();
        std::cout << " < AI calculates Move..." << std::endl;

        Move ai_move = getNextAIMove(chessBoard);
        std::cout << " < AI move: " << index_to_square(ai_move.from) << " -> " << index_to_square(ai_move.to) << std::endl;
        chessBoard->move_piece(ai_move);
        chessBoard->activePlayer = !chessBoard->activePlayer;

        if (!chessBoard->is_move_possible()) {
            break; //continue?
        }

        std::cout << std::endl << "*******************************************************************" << std::endl;
        chessBoard->print();
        Move player_move = get_move_of_player(chessBoard);
        chessBoard->move_piece(player_move);
        chessBoard->activePlayer = !chessBoard->activePlayer;

    }
    chessBoard->print();
    chessBoard->end_game();

    printf_s("%s", "BEFORE DELETE\n");
    
    delete chessBoard;

    printf_s("%s", "FINISH");
    
    return 0;
}

void save(std::string filename)
{
    std::ofstream myfile;
    myfile.open (filename);
    //myfile.write( chessBoard->convert_to_FEN().c_str());
    myfile.close();
}

const char* load(std::string filename){
    std::ifstream infile;
	infile.open( filename );
    std::string line;
    std::getline(infile, line);
    infile.close();
    return line.c_str();
}
