#include "main.hpp"
#include "KI.hpp"
#include <string>
#include <cstring>
#include "MoveGenerator.hpp"
#include <memory>

bool multiplayermode = false;
auto chessBoard = std::make_shared<ChessBoard>();

int main(int argc, char const *argv[])
{
    std::cout << std::endl << std::endl;
    std::cout << "  .oooooo.   oooo                                               .o.       ooooo" << std::endl;
    std::cout <<" d8P'  `Y8b  `888                                              .888.      `888'" << std::endl;
    std::cout <<"888           888 .oo.    .ooooo.   .oooo.o  .oooo.o          .8\"888.      888"  << std::endl;
    std::cout <<"888           888P\"Y88b  d88' `88b d88(  \"8 d88(  \"8         .8' `888.     888"  << std::endl;
    std::cout <<"888           888   888  888ooo888 `\"Y88b.  `\"Y88b.         .88ooo8888.    888"  << std::endl;
    std::cout <<"`88b    ooo   888   888  888    .o o.  )88b o.  )88b       .8'     `888.   888"  << std::endl;
    std::cout <<" `Y8bood8P'  o888o o888o `Y8bod8P' `Y8888P' `Y8888P'      o88o     o8888o o888o"  << std::endl;
    
    std::cout << std::endl << "you can quit the game with \"q\" or get help with\"h\"" << std::endl;                                                   
    std::string in;

    chessBoard->init();
    setDifficulty(Difficulty::NORMAL);

    std::cout << " > If you want to play a multiplayergame type in [m] else press enter" << std::endl;
    std::getline(std::cin, in);
    multiplayermode = in == "m" ? true : false;

    if(multiplayermode){
        std::cout << "you are now playing in multiplayermode" << std::endl;
    }
    else{
        std::cout << "you are now playing in singleplayermode" << std::endl << std::endl;
    }

    std::cout << " > To load an existing game type in [l] else press enter" << std::endl;
    std::getline(std::cin, in);

    if(in == "l"){
        std::cout << " > Please type in path or FEN-notation"  << std::endl;
        std::string path;
        std::cin >> path;
        
        if(!chessBoard->load_from_FEN(path.c_str())) { std::cout << "ja"; }
        else if(!chessBoard->load_from_FEN(load(path.c_str())));
        else{
            std::cout << "No valid Chess-game, you start with a normal game " << std::endl;
            chessBoard->init();
        }
    }

    // AI with random move selection
    while (!chessBoard->endOfGame)
    {
        if (!chessBoard->is_move_possible()) {
            continue;
        }
        std::cout << std::endl << "*******************************************************************" << std::endl;
        if(multiplayermode){
            std::cout << "Player 1 is on the line:" << std::endl;
        }
        chessBoard->disable_ai_move();
        chessBoard->print();
        Move player_move = get_move_of_player(chessBoard);
        chessBoard->move_piece(player_move);
        chessBoard->activePlayer = !chessBoard->activePlayer;
        if (!chessBoard->is_move_possible()) {
            continue;
        }
        std::cout << std::endl << "*******************************************************************" << std::endl;
        //multiplayer
        if(multiplayermode){
            std::cout << "Player 2 is on the line:" << std::endl;
            chessBoard->print();
            Move player_move = get_move_of_player(chessBoard);
            chessBoard->move_piece(player_move);
        }
        else{
            chessBoard->enable_ai_move();
            add_opening_move(player_move, chessBoard->moveCounter-1);
            //AI
            chessBoard->print();
            std::cout << " < AI calculates Move..." << std::endl;

            Move ai_move = getNextAIMove(chessBoard);
            std::cout << " < AI move: " << index_to_square(ai_move.from) << " -> " << index_to_square(ai_move.to) << std::endl;
            chessBoard->move_piece(ai_move);
            add_opening_move(ai_move, chessBoard->moveCounter);
        }
        chessBoard->activePlayer = !chessBoard->activePlayer;
    }
    chessBoard->print();
    chessBoard->end_game();

    
    return 0;
}

void save(std::string filename) {
    std::ofstream myfile;
    myfile.open (filename);
    char* fen = (char*) chessBoard->convert_to_FEN().c_str();
    myfile.write( fen, strlen(fen));
    std::cout << chessBoard->convert_to_FEN() << std::endl;
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

void quit(){
    std::cout << std::endl << " > Do you want to save your game? [Y/n]" << std::endl;
    std::string in;
    std::getline(std::cin, in);
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

void help(){
    std::cout << "In CHESS AI it is possible to play Chess with another person in multiplayer or you can try to beat our AI (you will loose :) )" << std::endl
        << "If you want you can load a game by entering it in FEN-notation (input directly or path to FEN-file)" << std::endl << std::endl
        //about
        << "This Game was developed in a C++ project of the DHBW Stuttgart Campus Horb in 2018/19" << std::endl 
        << "programmers: Johannes Gaiser, Peter Würth, Joel Schmid" << std::endl << "Thank you for playing our game :)" << std::endl;
}