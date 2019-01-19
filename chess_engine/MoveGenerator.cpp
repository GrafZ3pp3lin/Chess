#include "MoveGenerator.hpp"


uint8_t square_to_index(char square[2])
{
    return static_cast<uint8_t>((10 - (square[1] - 48)) * 10 // rank
            + ((toupper(square[0])) - 64)); // file
}

const char* index_to_square(int8_t index)
{
    char* square = new char[3];
    square[0] = static_cast<char>((index % 10) + 64);
    square[1] = static_cast<char>(10 - (index / 10) + 48);
    square[2] = '\0';
    return square;
}

char* ChessBoard::index_to_piece(uint8_t index)
{
    switch (board[index])
    {
    case 1:  return const_cast<char *>("white pawn");
    case 11: return const_cast<char *>("white rook");
    case 12: return const_cast<char *>("white knight");
    case 13: return const_cast<char *>("white bishop");
    case 14: return const_cast<char *>("white queen");
    case 10: return const_cast<char *>("white king");
    case 2:  return const_cast<char *>("black pawn");
    case 21: return const_cast<char *>("black rook");
    case 22: return const_cast<char *>("black knight");
    case 23: return const_cast<char *>("black bishop");
    case 24: return const_cast<char *>("black queen");
    case 20: return const_cast<char *>("black king");
    default: return const_cast<char *>("");
    }
}

std::string ChessBoard::convert_to_FEN()
{
    std::string fen = "";
    int jumped_fields = 0;
    for(int i = 21; i < 99; i++)
    {
        if((i % 10 < 9) && (i % 10 > 0))
        {
            if(board[i] > 0){
                if(jumped_fields > 0)
                {
                    fen += static_cast<char>(jumped_fields+48);
                    jumped_fields = 0;
                }
                fen += transfer(board[i]);
            }
            else if(board[i] == 0)
            {
                jumped_fields++;
            }
        }
        if(i % 10 == 0)
        {
            if(jumped_fields > 0)
            {
                fen += static_cast<char>(jumped_fields+48);
                jumped_fields = 0;
            }
            fen += "/";
        }
    }

    //active Player
    if(activePlayer == Color::BLACK)
    {
        fen += " b ";
    }
    else{
        fen += " w ";
    }

    //castling
    bool nocastling = true;

    if(white_castling_kingside)
    {
        nocastling = false;
        fen += "K";
    }
    if(white_castling_queenside)
    {
        nocastling = false;
        fen += "Q";
    }
    if(black_castling_kingside)
    {
        nocastling = false;
        fen += "k";
    }
    if(black_castling_queenside)
    {
        nocastling = false;
        fen += "q";
    }
    if(nocastling)
    {
        fen += "-";
    }

    //enPassant
    if(enPassant < 99 && enPassant > 21)
    {
        std::string field(index_to_square(static_cast<int8_t>(enPassant > 60 ? enPassant - 10 : enPassant + 10)));
        fen += " " + field;
    }
    else{
        fen += " -";
    }

    //Züge
    fen += " " + std::to_string(fiftyMovesRuleCounter);
    fen += " " + std::to_string(moveCounter);
    return fen;
}

bool ChessBoard::load_from_FEN(const char *fen)
{
    threefold.clear();
    //init empty board
    for(int i = 0; i < 120; i++)
    {
        if (i < 21 || i > 98 || i % 10 == 0 || i % 10 == 9)
        {
            board[i] = -1;
        }
        else
        {
            board[i] = 0;
        }
    }

    //piece positions
    int next = 0;
    int index = 21;
    pieceCounter = 0;
    int n = 0;
    while (fen[n] != '\0') n++;
    for(int i = 0; i < n; i++)
    {
        char c = fen[i];
        if ((c > 65 && c < 90) || (c > 97 && c < 122))
        {
            int rank = index / 10 - 1;
            int file = index % 10;
            if (rank < 1 || rank > 8 || file < 1 || file > 8) // outside of board
            {
                return false;
            }
            board[index] = transfer_back(c);
            index++;
            pieceCounter++;
        }
        else if (c > 48 && c < 57)
        {
            index += c - 48;
        }
        else if(c == 47)
        {
            index += 2;
        }
        else if(c == 32)
        {
            next = i + 1;
            break;
        }
        else
        {
            return false;
        }
    }
    if (next >= n) return false;
    //active Player
    if(fen[next] == 'w' || fen[next] == 'W')
    {
        activePlayer = Color::WHITE;
    }
    else if(fen[next] == 'b' || fen[next] == 'B')
    {
        activePlayer = Color::BLACK;
    }
    else
    {
        return false;
    }
    next++;
    if(next >= n || fen[next] != 32) return false;
    next++;
    if (next >= n) return false;
    //Castling
    black_castling_kingside = false;
    black_castling_queenside = false;
    white_castling_kingside = false;
    white_castling_queenside = false;
    if (fen[next] == '-')
    {
        next++;
    }
    else
    {
        if(fen[next] == 'K')
        {
            white_castling_kingside = true;
            next++;
        }
        if (next >= n) return false;
        if(fen[next] == 'Q')
        {
            white_castling_queenside = true;
            next++;
        }
        if (next >= n) return false;
        if(fen[next] == 'k')
        {
            black_castling_kingside = true;
            next++;
        }
        if (next >= n) return false;
        if(fen[next] == 'q')
        {
            black_castling_queenside = true;
            next++;
        }
    }
    if(next >= n || fen[next] != 32) return false;
    next++;
    if (next >= n) return false;
    // en passant
    if (fen[next] == '-')
    {
        enPassant = 0;
        next++;
    }
    else if (((fen[next] > 64 && fen[next] < 73) || (fen[next] > 96 && fen[next] < 105))
             && (fen[next+1] > 48 && fen[next+1] < 57))
    {
        char ep[3] =
        {
            fen[next],
            fen[next+1] > 52 ? char(fen[next+1] - 1) : char(fen[next+1] + 1),
            '\0'
        };
        enPassant = square_to_index(ep);
        next += 2;
    }
    else
    {
        return false;
    }
    next++;
    if (next >= n || fen[next] == 32) return false;
    if (fen[next] >= 48 && fen[next] < 58)
    {
        if (next + 1 < n && fen[next+1] != 32 && fen[next+1] >= 48 && fen[next+1] < 58)
        {
            fiftyMovesRuleCounter = static_cast<uint8_t>((fen[next] - 48) * 10 + (fen[next+1] - 48));
        }
        else
        {
            fiftyMovesRuleCounter = static_cast<uint8_t>(fen[next] - 48);
        }
    }
    else
    {
        return false;
    }
    return true;
}

void ChessBoard::init()
{
    // reset parameters
    activePlayer = Color::WHITE;
    endOfGame = 0;
    fiftyMovesRuleCounter = 0;
    moveCounter = 1;
    gameValue = 0;
    // off the board
    for (uint8_t i = 0; i < 120; i++)
    {
        board[i] = -1;
    }
    // black pieces
    board[21] = 21;
    board[22] = 22;
    board[23] = 23;
    board[24] = 24;
    board[25] = 20;
    board[26] = 23;
    board[27] = 22;
    board[28] = 21;
    for (uint8_t i = 31; i < 39; i++)
    {
        board[i] = 2;
    }
    // empty squares
    for (uint8_t i = 41; i < 100; i += 10)
    {
        for (uint8_t j = i; j < i + 8; j++)
        {
            board[j] = 0;
        }
    }
    // white pieces
    board[91] = 11;
    board[92] = 12;
    board[93] = 13;
    board[94] = 14;
    board[95] = 10;
    board[96] = 13;
    board[97] = 12;
    board[98] = 11;
    for (uint8_t i = 81; i < 89; i++)
    {
        board[i] = 1;
    }
    // white player starts the game
    activePlayer = Color::WHITE;
    //clear threefold from last game
    threefold.clear();
}

char transfer(int id)
{
    switch(id)
    {
    case 1: return 'P';
    case 2: return 'p';
    case 11: return 'R';
    case 21: return 'r';
    case 12: return 'N';
    case 22: return 'n';
    case 13: return 'B';
    case 23: return 'b';
    case 14: return 'Q';
    case 24: return 'q';
    case 10: return 'K';
    case 20: return 'k';
    default: return ' ';
    }
}

int8_t transfer_back(char piece)
{
    switch(piece)
    {
    case 'P': return 1;
    case 'p': return 2;
    case 'R': return 11;
    case 'r': return 21;
    case 'N': return 12;
    case 'n': return 22;
    case 'B': return 13;
    case 'b': return 23;
    case 'Q': return 14;
    case 'q': return 24;
    case 'K': return 10;
    case 'k': return 20;
    default: return 0;
    }
}

void ChessBoard::print()
{
    std::cout << "  ";
    for (int i = 0; i < 8; i++) {
        std::cout << "  " << char(65 + i) << " ";
    }
    std::cout << std::endl << "  " << char(0xDA);
    for (int i = 0; i < 7; i++) {
        std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xC2);
    }
    std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xBF) << std::endl;
    char c;
    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            //Felder schwarz/weiß
            c = (j + i) % 2 == 0 ? char(0xDB) : c = char(0xFF);
            char f = transfer(board[i * 10 + 21 + j]);
            f = f == ' ' ? c : f;
            std::cout << char(0xB3) << c << f << c;
            //Felder nicht schwarz weiß
            //std::cout << char(0xB3) << ' ' << transfer(board[i * 10 + 21 + j]) << ' ';
        }
        std::cout << char(0xB3) << std::endl;
        if (i < 7) {
            std::cout << "  " << char(0xC3);
            for (int i = 0; i < 7; i++) {
                std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xC5);
            }
            std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xB4) << std::endl;
        }
    }
    std::cout << "  " << char(0xC0);
    for (int i = 0; i < 7; i++) {
        std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xC1);
    }
    std::cout << char(0xC4) << char(0xC4) << char(0xC4) << char(0xD9) << std::endl;
}

void ChessBoard::print_moveset()
{
    std::vector<Move> moveset = get_moveset_all(activePlayer);
    for (size_t i = 0; i  < moveset.size(); i++)
    {
        std::cout
                << std::to_string(i + 1) << " : "
                << std::to_string(moveset[i].from) << " -> "
                << std::to_string(moveset[i].to)
                << (moveset[i].flag != 0 ? " (" + std::to_string(moveset[i].flag) + ") " : "")
                << std::endl;
    }
}

bool ChessBoard::is_white(int8_t p)
{
    return p == 1 || p == 10 || p == 11 || p == 12 || p == 13 || p == 14;
}

bool ChessBoard::is_black(int8_t p)
{
    return p == 2 || p == 20 || p == 21 || p == 22 || p == 23 || p == 24;
}

Color ChessBoard::get_color(uint8_t index)
{
    if (board[index] == 1 || board[index] == 10 || board[index] == 11 || board[index] == 12 || board[index] == 13 || board[index] == 14)
    {
        return Color::WHITE;
    }
    return Color::BLACK;
}

bool ChessBoard::is_empty(int8_t p)
{
    return p == 0;
}

std::vector<Move> ChessBoard::get_moveset_all(Color color)
{
    std::vector<Move> moveset;
    const uint8_t pawn = color == Color::WHITE ? 1 : 2;
    const uint8_t rook = color == Color::WHITE ? 11 : 21;
    const uint8_t knight = color == Color::WHITE ? 12 : 22;
    const uint8_t bishop = color == Color::WHITE ? 13 : 23;
    const uint8_t queen = color == Color::WHITE ? 14 : 24;
    const uint8_t king = color == Color::WHITE ? 10 : 20;
    // check for all pseudo-legal moves from board[i] to board[j]
    for (uint8_t i = 21; i < 99; i++)
    {
        if (board[i] == -1 || board[i] == 0) continue;
        if (board[i] == pawn)
        {
            std::vector<Move> ms = get_moveset_pawn(i, color);
            moveset.insert(moveset.end(), ms.begin(), ms.end());
        }
        else if (board[i] == bishop)
        {
            std::vector<Move> ms = get_moveset_bishop(i, color);
            moveset.insert(moveset.end(), ms.begin(), ms.end());
        }
        else if (board[i] == knight)
        {
            std::vector<Move> ms = get_moveset_knight(i, color);
            moveset.insert(moveset.end(), ms.begin(), ms.end());
        }
        else if (board[i] == rook)
        {
            std::vector<Move> ms = get_moveset_rook(i, color);
            moveset.insert(moveset.end(), ms.begin(), ms.end());
        }
        else if (board[i] == queen)
        {
            std::vector<Move> ms = get_moveset_queen(i, color);
            moveset.insert(moveset.end(), ms.begin(), ms.end());
        }
        else if (board[i] == king)
        {
            std::vector<Move> ms = get_moveset_king(i, color);
            moveset.insert(moveset.end(), ms.begin(), ms.end());
        }
    }
    return moveset;
}

const int8_t offset_bishop[4] = { -11, -9, 9, 11 };
const int8_t offset_knight[8] = { -21, -19, -12, -8, 8, 12, 19, 21 };
const int8_t offset_rook[4] = { -10, -1, 1, 10 };
const int8_t offset_queen[8] = { -11, -10, -9, -1, 1, 9, 10, 11 };
const int8_t offset_king[8] = { -11, -10, -9, -1, 1, 9, 10, 11 };

std::vector<Move> ChessBoard::get_moveset_king(uint8_t index)
{
    return get_moveset_king(index, activePlayer);
};

std::vector<Move> ChessBoard::get_moveset_queen(uint8_t index)
{
    return get_moveset_queen(index, activePlayer);
};

std::vector<Move> ChessBoard::get_moveset_rook(uint8_t index)
{
    return get_moveset_rook(index, activePlayer);
};

std::vector<Move> ChessBoard::get_moveset_knight(uint8_t index)
{
    return get_moveset_knight(index, activePlayer);
};

std::vector<Move> ChessBoard::get_moveset_bishop(uint8_t index)
{
    return get_moveset_bishop(index, activePlayer);
};

std::vector<Move> ChessBoard::get_moveset_pawn(uint8_t index)
{
    return get_moveset_pawn(index, activePlayer);
};

std::vector<Move> ChessBoard::get_moveset_king(uint8_t i, Color color)
{
    std::vector<Move> moveset;
    uint8_t j;
    for (int8_t o : offset_king)
    {
        j = static_cast<uint8_t>(i + o);
        // outside of board
        if (board[j] == -1) continue;
        // own piece
        if (color == Color::WHITE ? is_white(board[j]) : is_black(board[j])) continue;
        // empty or enemy piece
        moveset.push_back(Move{i, j});
    }
    // check for castling
    if (color == Color::WHITE)
    {
        // castling kingside
        if (white_castling_kingside)
        {
            if (is_empty(board[96]) && is_empty(board[97])
                    && !is_king_in_check(95, color) && !is_king_in_check(96, color) && !is_king_in_check(97, color))
            {
                moveset.push_back(Move{95, 97, 3});
            }
        }
        // castling queenside
        if (white_castling_queenside)
        {
            if (is_empty(board[94]) && is_empty(board[93]) && is_empty(board[92])
                    && !is_king_in_check(95, color) && !is_king_in_check(94, color) && !is_king_in_check(93, color))
            {
                moveset.push_back(Move{95, 93, 3});
            }
        }
    }
    else
    {
        // castling kingside
        if (black_castling_kingside)
        {
            if (is_empty(board[26]) && is_empty(board[27])
                    && !is_king_in_check(25, color) && !is_king_in_check(26, color) && !is_king_in_check(27, color))
            {
                moveset.push_back(Move{25, 27, 3});
            }
        }
        // castling queenside
        if (black_castling_queenside)
        {
            if (is_empty(board[24]) && is_empty(board[23]) && is_empty(board[22])
                    && !is_king_in_check(25, color) && !is_king_in_check(24, color) && !is_king_in_check(23, color))
            {
                moveset.push_back(Move{25, 23, 3});
            }
        }
    }
    return moveset;
}

std::vector<Move> ChessBoard::get_moveset_queen(uint8_t i, Color color)
{
    std::vector<Move> moveset;
    uint8_t j;
    for (int8_t o : offset_queen)
    {
        j = i;
        while (true)
        {
            j += o;
            // outside of board
            if (board[j] == -1) break;
            // own piece
            if (color == Color::WHITE ? is_white(board[j]) : is_black(board[j])) break;
            // empty
            if (is_empty(board[j]))
            {
                moveset.push_back(Move{i, j});
                continue;
            }
            // capture
            moveset.push_back(Move{i, j});
            break;
        }
    }
    return moveset;
}

std::vector<Move> ChessBoard::get_moveset_rook(uint8_t i, Color color)
{
    std::vector<Move> moveset;
    uint8_t j;
    for (int8_t o : offset_rook)
    {
        j = i;
        while (true)
        {
            j += o;
            // outside of board
            if (board[j] == -1) break;
            // own piece
            if (color == Color::WHITE ? is_white(board[j]) : is_black(board[j])) break;
            // empty
            if (is_empty(board[j]))
            {
                moveset.push_back(Move{i, j});
                continue;
            }
            // capture
            moveset.push_back(Move{i, j});
            break;
        }
    }
    return moveset;
}

std::vector<Move> ChessBoard::get_moveset_knight(uint8_t i, Color color)
{
    std::vector<Move> moveset;
    uint8_t j;
    for (int8_t o : offset_knight)
    {
        j = static_cast<uint8_t>(i + o);
        // outside of board
        if (board[j] == -1) continue;
        // own piece
        if (color == Color::WHITE ? is_white(board[j]) : is_black(board[j])) continue;
        // empty or enemy piece
        moveset.push_back(Move{i, j});
    }
    return moveset;
}

std::vector<Move> ChessBoard::get_moveset_bishop(uint8_t i, Color color)
{
    std::vector<Move> moveset;
    uint8_t j;
    for (int8_t o : offset_bishop)
    {
        j = i;
        while (true)
        {
            j += o;
            // outside of board
            if (board[j] == -1) break;
            // own piece
            if (color == Color::WHITE ? is_white(board[j]) : is_black(board[j])) break;
            // empty
            if (is_empty(board[j]))
            {
                moveset.push_back(Move{i, j});
                continue;
            }
            // capture
            moveset.push_back(Move{i, j});
            break;
        }
    }
    return moveset;
}

std::vector<Move> ChessBoard::get_moveset_pawn(uint8_t i, Color color)
{
    std::vector<Move> moveset;
    int8_t direction = color == Color::WHITE ? -1 : 1;
    // one square forward
    uint8_t j = static_cast<uint8_t>(i + direction * 10);
    if (board[j] == 0)
    {
        // check for promotion
        if ((color == Color::WHITE && j < 29) || (color == Color::BLACK && j > 90))
        {
            moveset.push_back(Move{i, j, 1});
        }
        else
        {
            moveset.push_back(Move{i, j});
        }
    }
    // two squares forward
    if (moveset.size() > 0)
    {
        if ((color == Color::WHITE && i > 80) || (color == Color::BLACK && i < 39))
        {
            j = static_cast<uint8_t>(i + direction * 20);
            if (board[j] == 0)
            {
                moveset.push_back(Move{i, j, 2});
            }
        }
    }
    // forward right
    j = static_cast<uint8_t>(i + direction * 9);
    // check en passant
    if (board[j] == 0)
    {
        uint8_t e = color == Color::WHITE ? i+1 : i-1;
        if ((color == Color::WHITE && board[e] == 2) || (color == Color::BLACK && board[e] == 1))
        {
            if (enPassant == e)
            {
                moveset.push_back(Move{i, j});
            }
        }
    }
    else if (color == Color::WHITE ? is_black(board[j]) : is_white(board[j]))
    {
        // check for promotion
        if ((color == Color::WHITE && j < 29) || (color == Color::BLACK && j > 90))
        {
            moveset.push_back(Move{i, j, 1});
        }
        else
        {
            moveset.push_back(Move{i, j});
        }
    }
    // forward left
    j = static_cast<uint8_t>(i + direction * 11);
    // check en passant
    if (board[j] == 0)
    {
        uint8_t e = color == Color::WHITE ? i-1 : i+1;
        if ((color == Color::WHITE && board[e] == 2) || (color == Color::BLACK && board[e] == 1))
        {
            if (enPassant == e)
            {
                moveset.push_back(Move{i, j});
            }
        }
    }
    else if (color == Color::WHITE ? is_black(board[j]) : is_white(board[j]))
    {
        // check for promotion
        if ((color == Color::WHITE && j < 29) || (color == Color::BLACK && j > 90))
        {
            moveset.push_back(Move{i, j, 1});
        }
        else
        {
            moveset.push_back(Move{i, j});
        }
    }
    return moveset;
}

std::vector<Move> ChessBoard::get_legal_moveset(uint8_t index, Piece type)
{
    std::vector<Move> moveset;
    switch (type)
    {
    case Piece::PAWN:
    {
        moveset = get_moveset_pawn(index, activePlayer);
        break;
    }
    case Piece::BISHOP:
    {
        moveset = get_moveset_bishop(index, activePlayer);
        break;
    }
    case Piece::KNIGHT:
    {
        moveset = get_moveset_knight(index, activePlayer);
        break;
    }
    case Piece::ROOK:
    {
        moveset = get_moveset_rook(index, activePlayer);
        break;
    }
    case Piece::QUEEN:
    {
        moveset = get_moveset_queen(index, activePlayer);
        break;
    }
    case Piece::KING:
    {
        moveset = get_moveset_king(index, activePlayer);
        break;
    }
    }
    std::vector<Move> legalMoveset;
    for (size_t i = 0; i < moveset.size(); i++)
    {
        if (is_legal(moveset[i], activePlayer))
        {
            legalMoveset.push_back(moveset[i]);
        }
    }
    return legalMoveset;
}

uint8_t ChessBoard::is_king_in_check(uint8_t i, Color color)
{
    uint8_t checkCount = 0;
    uint8_t j;
    // horizontal and vertical moves
    for (int8_t o : offset_rook)
    {
        j = i;
        while (true)
        {
            j += o;
            int8_t piece = board[j];
            // empty
            if (is_empty(piece)) continue;
            // outside of board
            if (piece == -1) break;
            // own piece
            if (color == Color::WHITE ? is_white(piece) : is_black(piece)) break;
            else if (color == Color::WHITE ? is_black(piece) : is_white(piece))
            {
                if ((color == Color::WHITE && (piece == 21 || piece == 24))
                        || (color == Color::BLACK && (piece == 11 || piece == 14))) // rook or queen
                {
                    checkCount++;
                }
                break;
            }
        }
    }
    // diagonal moves
    for (int8_t o : offset_bishop)
    {
        j = i;
        while (true)
        {
            j += o;
            int8_t piece = board[j];
            // empty
            if (is_empty(piece)) continue;
            // outside of board
            if (piece == -1) break;
            // own piece
            if (color == Color::WHITE ? is_white(piece) : is_black(piece)) break;
            else if (color == Color::WHITE ? is_black(piece) : is_white(piece))
            {
                if ((color == Color::WHITE && (piece == 23 || piece == 24))
                        || (color == Color::BLACK && (piece == 13 || piece == 14))) // bishop or queen
                {
                    checkCount++;
                }
                break;
            }
        }
    }
    // knight moves
    for (int8_t o : offset_knight)
    {
        j = static_cast<uint8_t>(i + o);
        int8_t piece = board[j];
        // empty
        if (is_empty(piece)) continue;
        // outside of board
        if (piece == -1) continue;
        // own piece
        if (color == Color::WHITE ? is_white(piece) : is_black(piece)) continue;
        else if (color == Color::WHITE ? is_black(piece) : is_white(piece))
        {
            if ((color == Color::WHITE && piece == 22)
                    || (color == Color::BLACK && piece == 12)) //knight
            {
                checkCount++;
            }
            continue;
        }
    }
    // pawns
    if (color == Color::WHITE)
    {
        if (board[i - 9] == 2 || board[i - 11] == 2)
        {
            checkCount++;
        }
    }
    else
    {
        if (board[i + 9] == 1 || board[i + 11] == 1)
        {
            checkCount++;
        }
    }
    // other king: check all squares in a distance of 2
    int k = color == Color::WHITE ? 20 : 10;
    if (board[i + 10] == k || board[i - 10] == k
            || board[i + 11] == k || board[i - 11] == k
            || board[i + 1] == k || board[i - 1] == k
            || board[i + 9] == k || board[i - 9] == k)
    {
        checkCount++;
    }
    return checkCount;
}

bool ChessBoard::is_legal(Move m, Color color)
{
    int8_t king = color == Color::WHITE ? 10 : 20;
    uint8_t kingPos = 0;
    ChessBoard boardCopy{*this};
    // get position of king
    for (uint8_t i = 21; i < 99; i++)
    {
        if (boardCopy.board[i] == king)
        {
            kingPos = i;
            break;
        }
    }
    if (kingPos != 0)
    {
        uint8_t checkBefore = boardCopy.is_king_in_check(kingPos, color);
        // in case of double checking only king moves are legal
        if (checkBefore > 1 && boardCopy.board[m.from] != king) return false;
        if (boardCopy.board[m.from] == king) kingPos = m.to;
        boardCopy.move_piece(m, true);
        uint8_t checkAfter = boardCopy.is_king_in_check(kingPos, color);
        // in case of (single) check only moves that remove the check are legal
        if (checkBefore == 1 && checkAfter >= checkBefore) return false;
        // moves where the king is in check afterwards are illegal
        if (checkAfter > checkBefore) return false;
    }
    return true;
}

bool ChessBoard::is_move_possible() {
    std::vector<Move> moveset = get_moveset_all(activePlayer);
    for (Move m : moveset)
    {
        if (is_legal(m, activePlayer))
        {
            return true;
        }
    }
    int8_t king = activePlayer == Color::WHITE ? 10 : 20;
    uint8_t kingPos = 0;
    // get position of king
    for (uint8_t i = 21; i < 99; i++)
    {
        if (board[i] == king)
        {
            kingPos = i;
            break;
        }
    }
    if (kingPos != 0)
    {
        if (is_king_in_check(kingPos, activePlayer))
        {
            endOfGame = activePlayer == Color::WHITE ? 2 : 1;
        }
        else
        {
            endOfGame = 3;
        }
    }
    else
    {
        endOfGame = activePlayer == Color::WHITE ? 2 : 1;
    }
    return false;
}

void ChessBoard::move_piece(Move m)
{
    move_piece(m, false);
}

void ChessBoard::move_piece(Move m, bool ignoreFlag)
{
    if (activePlayer == Color::BLACK)
    {
        moveCounter++;
    }
    // count moves for fifty-moves rule
    if (is_empty(board[m.to])) fiftyMovesRuleCounter++;
    else // capture
    {
        fiftyMovesRuleCounter = 0;
        pieceCounter--;
    }
    // check for checkmate
    if (board[m.to] == 20) endOfGame = 1; // white wins
    else if (board[m.to] == 10) endOfGame = 2; // black wins
    // check en passant
    if (!ignoreFlag)
    {
        uint8_t e = activePlayer == Color::WHITE ? m.from-1 : m.from+1;
        if ((activePlayer == Color::WHITE && board[e] == 2)
                || (activePlayer == Color::BLACK && board[e] == 1))
        {
            if (enPassant == e)
            {
                board[enPassant] = 0;
                fiftyMovesRuleCounter = 0;
            }
        }
    }
    // move piece
    board[m.to] = board[m.from];
    board[m.from] = 0;
    //KI promote Pawn temporaly to Queen
    if (ignoreFlag) 
    {
        if (activePlayer == Color::BLACK && (int)(m.to / 10) == 9)
        {
            board[m.to] = 24;
        }
    }
    // handle flag
    if (!ignoreFlag)
    {
        // as passant
        if (m.flag == 2)
        {
            enPassant = m.to;
        }
        // castling
        // aditionially to the king that gets moved by default (due to the above)
        // the involved rook has to be moved
        else if (m.flag == 3)
        {
            // white castling
            if (m.from == 95)
            {
                // kingside
                if (m.to == 97)
                {
                    // move kingsided rook
                    board[96] = board[98];
                    board[98] = 0;
                }
                // queenside
                else
                {
                    board[94] = board[91];
                    board[91] = 0;
                }
                
            }
            // black castling
            else
            {
                // kingside
                if (m.to == 27)
                {
                    // move kingsided rook
                    board[26] = board[28];
                    board[28] = 0;
                }
                // queenside
                else
                {
                    board[24] = board[21];
                    board[21] = 0;
                }
            }
        }
    }
    // further castling is denied if either the involved rook or king have ever been moved
    if (m.from == 95 || m.from == 98) white_castling_kingside = false;
    if (m.from == 95 || m.from == 91) white_castling_queenside = false;
    if (m.from == 25 || m.from == 28) black_castling_kingside = false;
    if (m.from == 25 || m.from == 21) black_castling_queenside = false;
    // check for a draw due to fifty-moves rule, king-against-king, etc.
    if (!ignoreFlag)
    {
        check_draw();
    }
}

void ChessBoard::check_draw()
{

    // fifty moves rule
    if (fiftyMovesRuleCounter >= 50) endOfGame = 3;

    //king vs king
    if(pieceCounter == 2) endOfGame = 3;

    //king vs king/bishop
    //king vs king/knight
    if(pieceCounter == 3){
        for(int i = 21; i < 100; i++){
            if(board[i] == 23 ||board[i] == 13 || board[i] == 22 ||board[i] == 12){
                endOfGame = 3;
            }
        }
    }

    //king/bishop vs king/bishop
    if(pieceCounter == 4){
        int8_t black = 0;
        int8_t white = 0;
        for(int8_t i = 21; i < 100; i++){
            if(board[i] == 23 && black == 0){
                black = i;
            }
            else if(board[i] == 13 && white == 0){
                white = i;
            }
            if(is_black(black) == is_black(white) && black != 0 && white != 0){
                endOfGame = 3;
            }
        }
    }

    //Threefoldrepetition
    ThreefoldData data;
    for(int i = 0; i < 120; i++){
        data.board[i] = board[i];
    }
    data.enPassant = enPassant;
    data.black_castling_kingside = black_castling_kingside;
    data.white_castling_kingside = white_castling_kingside;
    data.black_castling_queenside = data.black_castling_queenside;
    data.white_castling_queenside = data.white_castling_queenside;

    threefold.push_back(data);


    for(auto t: threefold){
        int count = 0;
        for(auto t2: threefold){
            if(t2 == t){
                count++;
            }
            if(count == 3){
                endOfGame = 3;
                break;
            }
        }
        if(count == 3){
            break;
        }
    }
}

void ChessBoard::promote_pawn(uint8_t index, Piece promotion)
{
    Color c = is_white(board[index]) ? Color::WHITE : Color::BLACK;
    switch (promotion)
    {
    case Piece::QUEEN: // promote to queen
    {
        if (c == Color::WHITE) board[index] = 14;
        else board[index] = 24;
        break;
    }
    case Piece::ROOK: // promote to rook
    {
        if (c == Color::WHITE) board[index] = 11;
        else board[index] = 21;
        break;
    }
    case Piece::KNIGHT: // promote to knight
    {
        if (c == Color::WHITE) board[index] = 12;
        else board[index] = 22;
        break;
    }
    case Piece::BISHOP: // promote to bishop
    {
        if (c == Color::WHITE) board[index] = 13;
        else board[index] = 23;
        break;
    }
    default:
    {
        break;
    }
    }
}
