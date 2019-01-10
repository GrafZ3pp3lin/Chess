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
    chessBoard->print();
    chessBoard->activePlayer = Color::BLACK;

    std::vector<Move> moveset = chessBoard->get_moveset_all(chessBoard->activePlayer);
    //printf_s("%s %i (id: %lu)\n", "moveset size", moveset.size(), std::this_thread::get_id());
    //std::vector<ChessBoard> boards;
    for (Move m : moveset) {
        printf_s("%s from: %i to: %i\n", "Move: ", m.from, m.to);
        std::cout << chessBoard->is_legal(m, chessBoard->activePlayer) << std::endl;
        //if (chessBoard->is_legal(m, chessBoard->activePlayer)) {
            /*ChessBoard boardCopy{*chessBoard};
            boardCopy.move_piece(m, true);
            boardCopy.activePlayer = !boardCopy.activePlayer;
            boards.push_back(boardCopy);*/
        //}
        //printf_s("%s from: %i to: %i\n", "finish check Move: ", m.from, m.to);
    }

    printf_s("%s", "BEFORE DELETE");
    
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
