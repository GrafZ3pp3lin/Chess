#ifndef INCLUDE_Game
#define INCLUDE_Game
#include "Figures.hpp"
#include <memory>
#include <array>

/*

Figuren direkt in array speichern klappt nicht, wegen abstracter Klasse.
-> über Pointer und in Heap speichern?
-> array mit einfachen Positionen und jede Figure hat festen Wert im Array. (White King = 0)

*/

class Game {
    public:
    std::array<std::unique_ptr<Figure>, 16> white; //white Figures
    std::array<std::unique_ptr<Figure>, 16> black; //black Figures
    Color active; //active Color
    bool castleWhiteK; //Castle White King side
    bool castleWhiteQ; //Castle White Queen side
    bool castleBlackK; //Castle Black King side
    bool castleBlackQ; //Castle Black Queen side
    Field enPassant; //position behind the pawn(after moved two squares). Regardless if enPassant possible.
    int halvmoveClock; //check for the fifty-move rule
    int Fullmove; //increase after Black Moves
};

//Idee...
class SmallGame {
    public:
    uint8_t figures[32];
};


#endif