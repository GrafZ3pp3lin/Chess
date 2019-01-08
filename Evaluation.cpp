#include "Evaluation.hpp"

#include <cmath>

int evaluate_board(ChessBoard *board)
{
    int whiteKingPos = -1;
    int blackKingPos = -1;
    bool whiteQueenNotCaptured = false;
    bool blackQueenNotCaptured = false;
    // add up material values
    int whiteAdvantage = 0;
    int blackAdvantage = 0;
    for (int i = 21; i < 100; i += 10)
    {
        for (int k = i; k < i + 8; k++)
        {
            switch (board->board[k])
            {
                case 1:  { whiteAdvantage += (int)PieceValue::PAWN; break; }
                case 11: { whiteAdvantage += (int)PieceValue::ROOK; break; }
                case 12: { whiteAdvantage += (int)PieceValue::KNIGHT; break; }
                case 13: { whiteAdvantage += (int)PieceValue::BISHOP; break; }
                case 14: { whiteAdvantage += (int)PieceValue::QUEEN; whiteQueenNotCaptured = true; break; }
                case 10: { whiteAdvantage += (int)PieceValue::KING; whiteKingPos = k; break; }
                case 2:  { blackAdvantage += (int)PieceValue::PAWN; break; }
                case 21: { blackAdvantage += (int)PieceValue::ROOK; break; }
                case 22: { blackAdvantage += (int)PieceValue::KNIGHT; break; }
                case 23: { blackAdvantage += (int)PieceValue::BISHOP; break; }
                case 24: { blackAdvantage += (int)PieceValue::QUEEN; blackQueenNotCaptured = true; break; }
                case 20: { blackAdvantage += (int)PieceValue::KING; blackKingPos = k; break; }
            }
        }
    }
    /* add bonuses/penalties for for good/bad piece positions
     * criteria for when the endgame starts:
     *  - both sides have no queens or
     *  - 1500 (= queen + rook) or fewer points in material (not counting the king)
     */
    bool endgame = false;
    int minMaterial = (int)PieceValue::QUEEN + (int)PieceValue::ROOK;
    if (!whiteQueenNotCaptured && !blackQueenNotCaptured
    || whiteAdvantage < minMaterial && blackAdvantage < minMaterial)
    {
        endgame = true;
    }
    for (int i = 21; i < 100; i += 10)
    {
        for (int k = i; k < i + 8; k++)
        {
            switch (board->board[k])
            {
                case 1:  { whiteAdvantage += whitePawnTable[k]; break; }
                case 11: { whiteAdvantage += whiteRookTable[k]; break; }
                case 12: { whiteAdvantage += whiteKnightTable[k]; break; }
                case 13: { whiteAdvantage += whiteBishopTable[k]; break; }
                case 14: { whiteAdvantage += whiteQueenTable[k]; break; }
                case 10: { whiteAdvantage += endgame ? whiteKingTableEndGame[k] : whiteKingTableMiddleGame[k]; break; }
                case 2:  { blackAdvantage += blackPawnTable[k]; break; }
                case 21: { blackAdvantage += blackRookTable[k]; break; }
                case 22: { blackAdvantage += blackKnightTable[k]; break; }
                case 23: { blackAdvantage += blackBishopTable[k]; break; }
                case 24: { blackAdvantage += blackQueenTable[k]; break; }
                case 20: { blackAdvantage += endgame ? blackKingTableEndGame[k] : blackKingTableMiddleGame[k]; break; }
            }
        }
    }
    // consider king in check or check mate
    if (whiteKingPos == -1)
    {
        blackAdvantage += (int)PieceValue::KING;
    }
    else if (board->is_king_in_check(whiteKingPos, Color::WHITE))
    {
        blackAdvantage += checkBonus;
    }
    if (blackKingPos == -1)
    {
        whiteAdvantage += (int)PieceValue::KING;
    }
    else if (board->is_king_in_check(blackKingPos, Color::BLACK))
    {
        whiteAdvantage += checkBonus;
    }
    /* return:
     *  - positive value -> white is in advantage
     *  - negative value -> black is in advantage
     */
    return whiteAdvantage - blackAdvantage;
}
