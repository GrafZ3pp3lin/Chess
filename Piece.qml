import QtQuick 2.0

Item {
    property string type: ""
    property int pos: 0

    id: piece
    width: parent.width / 8
    height: parent.height / 8
    x: (pos % 8) * (parent.width / 8)
    y: Math.floor(pos / 8) * (parent.height / 8)

    function piece2path(piece) {
        switch (piece) {
            case "p": return "img/pawn_black.png";
            case "r": return "img/rook_black.png";
            case "n": return "img/knight_black.png";
            case "b": return "img/bishop_black.png";
            case "q": return "img/queen_black.png";
            case "k": return "img/king_black.png";
            case "P": return "img/pawn_white.png";
            case "R": return "img/rook_white.png";
            case "N": return "img/knight_white.png";
            case "B": return "img/bishop_white.png";
            case "Q": return "img/queen_white.png";
            case "K": return "img/king_white.png";
            default: return "";
        }
    }

    Image {
        anchors.centerIn: parent
        width: 0.75 * parent.width
        height: 0.75 * parent.height
        source: piece2path(piece.type)
        fillMode: Image.PreserveAspectFit
    }

    PropertyAnimation {
        id: moveAnimation
        duration: 1000
        target: piece
        from: piece.x
        to: piece.x + 200
    }
}
