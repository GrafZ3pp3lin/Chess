#ifndef INCLUDE_Game
#define INCLUDE_Game
#include "Figures.hpp"
#include <memory>
#include <array>

class Game {
    public:
    std::array<std::unique_ptr<Figure>, 16> white;
    std::array<std::unique_ptr<Figure>, 16> black;
};

//Idee...
class SmallGame {
    public:
    uint8_t figures[32];
};


#endif