import QtQuick 2.0

Item {
    id: chessBoard

    property string lightColor: "#f0d9b2"
    property string darkColor: "#b58860"
    property string greenColor: "#9922bb22"

    // light and dark quares

    Repeater {
        model: 64

        Rectangle {
            width: chessBoard.width / 8
            height: chessBoard.height / 8
            x: width * (index % 8)
            y: height * Math.floor(index / 8)
            color: Math.floor((index / 8) + (index % 8)) % 2 ? chessBoard.darkColor : chessBoard.lightColor
        }
    }

    // green squares

    Repeater {
        objectName: "greenSquares"
        model: 64

        Rectangle {
            visible: false
            width: chessBoard.width / 8
            height: chessBoard.height / 8
            x: width * (index % 8)
            y: height * Math.floor(index / 8)
            color: greenColor
        }
    }

    // black pieces

    Piece {
        pos: 0
        type: "r"
        objectName: "blackRook"
    }

    Piece {
        pos: 1
        type: "n"
        objectName: "blackKnight"
    }

    Piece {
        pos: 2
        type: "b"
        objectName: "blackBishop"
    }

    Piece {
        pos: 3
        type: "q"
        objectName: "blackQueen"
    }

    Piece {
        pos: 4
        type: "k"
        objectName: "blackKing"
    }

    Piece {
        pos: 5
        type: "b"
        objectName: "blackBishop"
    }

    Piece {
        pos: 6
        type: "n"
        objectName: "blackKnight"
    }

    Piece {
        pos: 7
        type: "r"
        objectName: "blackRook"
    }

    Piece {
        pos: 8
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 9
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 10
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 11
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 12
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 13
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 14
        type: "p"
        objectName: "blackPawn"
    }

    Piece {
        pos: 15
        type: "p"
        objectName: "blackPawn"
    }

    // white pieces

    Piece {
        pos: 48
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 49
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 50
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 51
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 52
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 53
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 54
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 55
        type: "P"
        objectName: "whitePawn"
    }

    Piece {
        pos: 56
        type: "R"
        objectName: "whiteRook"
    }

    Piece {
        pos: 57
        type: "N"
        objectName: "whiteKnight"
    }

    Piece {
        pos: 58
        type: "B"
        objectName: "whiteBishop"
    }

    Piece {
        pos: 59
        type: "Q"
        objectName: "whiteQueen"
    }

    Piece {
        pos: 60
        type: "K"
        objectName: "whiteKing"
    }

    Piece {
        pos: 61
        type: "B"
        objectName: "whiteBishop"
    }

    Piece {
        pos: 62
        type: "N"
        objectName: "whiteKnight"
    }

    Piece {
        pos: 63
        type: "R"
        objectName: "whiteRook"
    }
}
