import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

Window {
    property int settingsWidth: 225
    property int settingsHeight: 225
    property int settingsSpace: 25
    property int edgeWidth: width < height ? width/15 : height/15
    property int boardSize: width < height ? width - 2*edgeWidth : height - 2*edgeWidth
    property string selectionColor: "#eeeeee"
    property string selectionColorHover: "#228822"
    property string selectionBorderColor: "#555555"
    property string overlayColor: "#ccffffff"
    property int selectionBorderWidth: 10
    property bool allowClosing: false

    function indexToFile(index) {
        switch (index) {
            case 0: return "A";
            case 1: return "B";
            case 2: return "C";
            case 3: return "D";
            case 4: return "E";
            case 5: return "F";
            case 6: return "G";
            case 7: return "H";
        }
    }

    function indexToRank(index) {
        switch (index) {
            case 0: return "1";
            case 1: return "2";
            case 2: return "3";
            case 3: return "4";
            case 4: return "5";
            case 5: return "6";
            case 6: return "7";
            case 7: return "8";
        }
    }

    id: root
    objectName: "root"
    title: qsTr("Chess")
    visible: true
    color: "#ffffff"
    width: 800
    height: 800
    minimumWidth: 500
    minimumHeight: 500
    onClosing: {
        if (!allowClosing && !saveFEN.visible) {
            close.accepted = false
            saveFEN.visible = true
        }
    }

    /* **************************************************
     *
     * Chess Board
     *
     * **************************************************/

    Rectangle {
        anchors.centerIn: parent
        width: boardSize
        height: boardSize

        ChessBoard {
            objectName: "chessBoard"
            id: chessBoard
            anchors.fill: parent
        }

        DropShadow {
            source: chessBoard
            anchors.fill: source
            horizontalOffset: 0
            verticalOffset: 0
            radius: 16
            samples: 20
            color: "#88000000"
        }

        MouseArea {
            objectName: "moveArea"
            id: moveArea
            enabled: (selectionMode.visible || selectionDifficulty.visible || selectionNewGame.visible || inputFEN.visible || selectionPromotion.visible || saveFEN.visible || gameOver.visible) ? false : true
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.LeftButton) {
                    receiver.receive_click(mouseX, mouseY, this.width, this.height)
                }
                else if (mouse.button === Qt.RightButton) {
                    menu.popup()
                }
            }

            Menu {
                id: menu
                width: 200

                Action {
                    text: "New Game (discards current one)"
                    onTriggered: {
                        selectionMode.visible = true
                    }
                }

                Action {
                    text: "Load Game (discards current one)"
                    onTriggered: {
                        inputFEN.visible = true
                    }
                }

                Action {
                    text: "Exit and Save"
                    onTriggered: {
                        saveFEN.visible = true
                        saveDialog.visible = true
                    }
                }

                Action {
                    text: "Exit without saving"
                    onTriggered: {
                        allowClosing = true
                        root.close()
                    }
                }

                delegate: MenuItem {
                    id: menuItem
                    implicitHeight: 40

                    contentItem: Text {
                        leftPadding: 10
                        rightPadding: 10
                        text: menuItem.text
                        font: menuItem.font
                        opacity: enabled ? 1.0 : 0.3
                        color: menuItem.highlighted ? "#000000" : "#888888"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        implicitHeight: 40
                        opacity: enabled ? 1 : 0.3
                        border.color: "#228822"
                        border.width: menuItem.highlighted ? 2 : 0
                        color: menuItem.highlighted ? "#11228822" : "transparent"
                    }
                }
            }
        }
    }

    // files top
    Rectangle {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: edgeWidth
        width: boardSize
        color: "#00000000"

        Repeater {
            model: 8

            Text {
                anchors.left: parent.left
                anchors.leftMargin: index * parent.width/8
                text: qsTr(indexToFile(index))
                width: parent.width/8
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: "#888888"
            }
        }
    }

    // files bottom
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: edgeWidth
        width: boardSize
        color: "#00000000"

        Repeater {
            model: 8

            Text {
                anchors.left: parent.left
                anchors.leftMargin: index * parent.width/8
                text: qsTr(indexToFile(index))
                width: parent.width/8
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: "#888888"
            }
        }
    }

    // ranks left
    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: parent.width/2 - boardSize/2 - edgeWidth
        anchors.verticalCenter: parent.verticalCenter
        height: boardSize
        width: edgeWidth
        color: "#00000000"

        Repeater {
            model: 8

            Text {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: index * parent.height/8
                text: qsTr(indexToRank(index))
                width: edgeWidth
                height: parent.height/8
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: "#888888"
            }
        }
    }

    // ranks right
    Rectangle {
        anchors.right: parent.right
        anchors.rightMargin: parent.width/2 - boardSize/2 - edgeWidth
        anchors.verticalCenter: parent.verticalCenter
        height: boardSize
        width: edgeWidth
        color: "#00000000"

        Repeater {
            model: 8

            Text {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: index * parent.height/8
                text: qsTr(indexToRank(index))
                width: edgeWidth
                height: parent.height/8
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: "#888888"
            }
        }
    }

    /* **************************************************
     *
     * Game settings
     *
     * **************************************************/

    // single- or multiplayer
    Rectangle {
        objectName: "seletionMode"
        id: selectionMode
        anchors.fill: parent
        color: overlayColor
        visible: true

        GridLayout {
            anchors.centerIn: parent
            columnSpacing: settingsSpace
            flow: GridLayout.LeftToRight

            Selection {
                Layout.column: 0
                text: "Singleplayer"
                w: settingsWidth
                onEvent: {
                    receiver.receive_mode(true)
                    selectionMode.visible = false
                    selectionDifficulty.visible = true
                }
            }

            Selection {
                Layout.column: 1
                text: "Multiplayer"
                w: settingsWidth
                onEvent: {
                    receiver.receive_mode(false)
                    selectionMode.visible = false
                    selectionNewGame.visible = true
                }
            }
        }
    }

    // difficulty
    Rectangle {
        objectName: "seletionDifficulty"
        id: selectionDifficulty
        anchors.fill: parent
        color: overlayColor
        visible: false

        GridLayout {
            anchors.centerIn: parent
            rowSpacing: settingsSpace
            columnSpacing: settingsSpace
            flow: GridLayout.TopToBottom

            Selection {
                Layout.row: 0
                text: "easy"
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2

                onEvent: {
                    receiver.receice_difficulty("easy")
                    selectionDifficulty.visible = false
                    selectionNewGame.visible = true
                }
            }

            Selection {
                Layout.row: 1
                text: "normal"
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2

                onEvent: {
                    receiver.receice_difficulty("normal")
                    selectionDifficulty.visible = false
                    selectionNewGame.visible = true
                }
            }

            Selection {
                Layout.row: 2
                text: "hard"
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2

                onEvent: {
                    receiver.receice_difficulty("hard")
                    selectionDifficulty.visible = false
                    selectionNewGame.visible = true
                }
            }
        }
    }

    // start new game or load game
    Rectangle {
        objectName: "seletionNewGame"
        id: selectionNewGame
        anchors.fill: parent
        color: overlayColor
        visible: false

        GridLayout {
            anchors.centerIn: parent
            columnSpacing: settingsSpace
            flow: GridLayout.LeftToRight

            Selection {
                Layout.column: 0
                text: "New Game"
                w: settingsWidth
                onEvent: {
                    receiver.receive_newgame(true)
                    selectionNewGame.visible = false
                }
            }

            Selection {
                Layout.column: 1
                text: "Load Game"
                w: settingsWidth
                onEvent: {
                    receiver.receive_newgame(false)
                    selectionNewGame.visible = false
                    inputFEN.visible = true
                }
            }
        }
    }

    // load game by typing in fen or loading file with fen from disk
    Rectangle {
        objectName: "inputFEN"
        id: inputFEN
        anchors.fill: parent
        color: overlayColor
        visible: false

        GridLayout {
            anchors.centerIn: parent
            columnSpacing: settingsSpace
            rowSpacing: settingsSpace
            flow: GridLayout.LeftToRight

            Selection {
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.row: 0
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2
                cursor: Qt.IBeamCursor

                Text {
                    id: fenHint
                    anchors.fill: parent
                    anchors.margins: settingsSpace
                    font.pixelSize: .75 * parent.fontsize
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    text: "FEN notation (confirm with ENTER)"
                    color: "#999999"
                    visible: fenInput.text.length > 0 ? false : true
                }

                TextInput {
                    id: fenInput
                    anchors.fill: parent
                    anchors.margins: settingsSpace
                    font.pixelSize: .75 * parent.fontsize
                    horizontalAlignment: TextInput.AlignLeft
                    verticalAlignment: TextInput.AlignVCenter
                    clip: true
                    onAccepted: {
                        receiver.receive_FEN(text, false, inputFEN)
                    }
                }
            }

            Selection {
                Layout.column: 0
                Layout.row: 1
                text: "Load from Disk"
                w: settingsWidth
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    fileDialog.visible = true
                }
            }

            FileDialog {
                id: fileDialog
                title: "Load from Disk"
                selectMultiple: false
                selectFolder: false
                folder: shortcuts.documents
                nameFilters: [ "Textfiles (*.txt)" ]
                onAccepted: {
                    receiver.receive_FEN(fileDialog.fileUrl, true, inputFEN)
                    inputFEN.visible = false
                }
            }

            Selection {
                Layout.column: 1
                Layout.row: 1
                text: "New Game"
                w: settingsWidth
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    receiver.receive_newgame(true)
                    inputFEN.visible = false
                }
            }
        }
    }

    // promotion
    Rectangle {
        objectName: "selectionPromotion"
        id: selectionPromotion
        anchors.fill: parent
        color: overlayColor
        visible: false

        GridLayout {
            anchors.centerIn: parent
            rowSpacing: settingsSpace
            columnSpacing: settingsWidth
            flow: GridLayout.TopToBottom

            Selection {
                Layout.row: 0
                text: "Queen"
                w: 2*settingsWidth + parent.columnSpacing/2
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    selectionPromotion.visible = false
                    receiver.receive_promotion("Queen")
                }
            }

            Selection {
                Layout.row: 1
                text: "Rook"
                w: 2*settingsWidth + parent.columnSpacing/2
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    selectionPromotion.visible = false
                    receiver.receive_promotion("Rook")
                }
            }

            Selection {
                Layout.row: 2
                text: "Knight"
                w: 2*settingsWidth + parent.columnSpacing/2
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    selectionPromotion.visible = false
                    receiver.receive_promotion("Knight")
                }
            }

            Selection {
                Layout.row: 3
                text: "Bishop"
                w: 2*settingsWidth + parent.columnSpacing/2
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    selectionPromotion.visible = false
                    receiver.receive_promotion("Bishop")
                }
            }
        }
    }

    // game over
    Rectangle {
        objectName: "gameOver"
        id: gameOver
        anchors.fill: parent
        color: overlayColor
        opacity: 1
        visible: false

        GridLayout {
            anchors.centerIn: parent
            rowSpacing: settingsSpace
            columnSpacing: settingsSpace
            flow: GridLayout.LeftToRight

            Selection {
                objectName: "gameOverMessage"
                id: gameOverMessage
                Layout.row: 0
                Layout.column: 0
                Layout.columnSpan: 2
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2
                text: "Game Over: white wins"
                onEvent: {
                    gameOver.opacity = (gameOver.opacity == 1) ? .1 : 1
                }
            }

            Selection {
                Layout.row: 1
                Layout.column: 0
                text: "New Game"
                w: settingsWidth
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    gameOver.visible = false
                    selectionMode.visible = true
                }
            }

            Selection {
                Layout.row: 1
                Layout.column: 1
                text: "Exit"
                w: settingsWidth
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    allowClosing = true
                    root.close()
                }
            }
        }
    }

    // save game
    Rectangle {
        objectName: "saveFEN"
        id: saveFEN
        anchors.fill: parent
        color: (selectionMode.visible || selectionDifficulty.visible || selectionNewGame.visible || inputFEN.visible || selectionPromotion.visible || gameOver.visible) ? "#ffffff" : overlayColor
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        visible: false

        GridLayout {
            anchors.centerIn: parent
            rowSpacing: settingsSpace
            columnSpacing: settingsSpace
            flow: GridLayout.TopToBottom

            Selection {
                Layout.row: 0
                text: "Save"
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    saveDialog.visible = true
                }
            }

            FileDialog {
                id: saveDialog
                title: "Load from Disk"
                selectExisting: false
                selectMultiple: false
                selectFolder: false
                folder: shortcuts.documents
                nameFilters: [ "Textfiles (*.txt)" ]
                onAccepted: {
                    allowClosing = true
                    if (receiver.receice_save(saveDialog.fileUrl)) {
                        root.close()
                    }
                }
            }

            Selection {
                Layout.row: 1
                text: "Exit without Saving"
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    allowClosing = true
                    root.close()
                }
            }

            Selection {
                Layout.row: 2
                text: "Keep Playing"
                w: 2*settingsWidth + parent.columnSpacing
                h: settingsHeight/2 - parent.rowSpacing/2
                onEvent: {
                    saveFEN.visible = false
                }
            }
        }
    }
}
