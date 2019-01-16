import QtQuick 2.0

Item {
    property int fontsize: 20
    property string text: ""
    property string color: "#ffffff"
    property string colorHover: "#11228822"
    property string borderColor: "#555555"
    property string borderColorHover: "#228822"
    property string borderColorInvalid: "#882222"
    property int selectionBorderWidth: 10
    property int w: 0
    property int h: w
    property int cursor: Qt.PointingHandCursor
    property bool valid: true
    signal event()

    id: selection
    width: w
    height: h

    Rectangle {
        anchors.fill: parent
        color: selection.color
        border.color: selection.valid ? selection.borderColor : selection.borderColorInvalid
        border.width: root.selectionBorderWidth

        Rectangle {
            id: layer
            anchors.fill: parent
            anchors.margins: selection.selectionBorderWidth
            color: selection.color
        }

        Text {
            anchors.centerIn: parent
            text: selection.text
            font.pixelSize: selection.fontsize
            font.weight: Font.Bold
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: selection.cursor
            hoverEnabled: true
            onEntered: {
                parent.border.color = selection.valid ? selection.borderColorHover : selection.borderColorInvalid
                layer.color = selection.colorHover
            }
            onExited: {
                parent.border.color = selection.valid ? selection.borderColor : selection.borderColorInvalid
                layer.color = selection.color
            }
            onClicked: {
                selection.event()
            }
        }
    }
}

