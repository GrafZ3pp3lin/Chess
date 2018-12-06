#ifndef INCLUDE_Field
#define INCLUDE_Field
#include <vector>
#include "Field.hpp"

enum class Color {BLACK, WHITE};
enum class Piece { King, Queen, Rook, Knight, Bishop, Pawn };

class Figure {
public:
    Figure(int x, int y, Color color);
    virtual ~Figure();
    Field loc;
    Color c;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b) = 0;
    virtual Piece getName() = 0;
    void move(int x, int y);
};

class King : public Figure {
public:
    using Figure::Figure;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b);
    virtual Piece getName() { return Piece::King; }
};

class Queen : public Figure {
public:
    using Figure::Figure;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b);
    virtual Piece getName() { return Piece::Queen; }
};

class Rook : public Figure {
public:
    using Figure::Figure;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b);
    virtual Piece getName() { return Piece::Rook; }
};

class Bishop : public Figure {
public:
    using Figure::Figure;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b);
    virtual Piece getName() { return Piece::Bishop; }
};

class Knight : public Figure {
public:
    using Figure::Figure;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b);
    virtual Piece getName() { return Piece::Knight; }
};

class Pawn : public Figure {
public:
    using Figure::Figure;
    virtual std::vector<Field> getMoveSet(std::vector<Figure*> &w, std::vector<Figure*> &b);
    virtual Piece getName() { return Piece::Pawn; }
};
#endif
