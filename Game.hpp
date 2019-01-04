#ifndef INCLUDE_Game
#define INCLUDE_Game

enum class Color {BLACK, WHITE};

/*

Figuren direkt in array speichern klappt nicht, wegen abstracter Klasse.
-> über Pointer und in Heap speichern?
-> array mit einfachen Positionen und jede Figur hat festen Wert im Array. (White King = 0)

*/

class Game {
    public:
    char board[120];
    Color active; //active Color
    bool castleWhiteK; //Castle White King side
    bool castleWhiteQ; //Castle White Queen side
    bool castleBlackK; //Castle Black King side
    bool castleBlackQ; //Castle Black Queen side
    int enPassant; //position behind the pawn(after moved two squares). Regardless if enPassant possible. (Index of Field)
    int halvmoveClock; //check for the fifty-move rule
    int Fullmove; //increase after Black Moves
};

#endif