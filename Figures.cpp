#include "Figures.hpp"

Field::Field(int x, int y) : locx(x), locy(y) {};
Figure::Figure(int x, int y) : loc(Field(x,y)) {};

// Color ist in game gespeichert. über x und y Figur auslesen. Über Figur kann type ausgelesen werden.
std::vector<Field> getMoveSet(int x, int y, Game &game) {

}
