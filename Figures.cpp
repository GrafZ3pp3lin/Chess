#include "Figures.hpp"

Field::Field(int x, int y) : locx(x), locy(y) {};
Figure::Figure(int x, int y, Color color) : loc(Field(x,y)), c(color) {};
void Figure::move(Field f) {
    loc = f;
}
