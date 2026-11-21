import QtQuick

Item {
    id: board
    width: 310
    height: 310

    property var tiles: ["1", "2", "3", "4", "5", "6", "7", "8", "0"]
    property int next_tile_index: -1

    readonly property int columns: 3
    readonly property int rows: 3
    readonly property real pad: 12
    readonly property real gap: 10
    readonly property real inner_width: width - pad * 2
    readonly property real inner_height: height - pad * 2
    readonly property real cell_width: (inner_width - gap * (columns - 1)) / columns
    readonly property real cell_height: (inner_height - gap * (rows - 1)) / rows

    function cellX(column){ return pad + column * (cell_width + gap) }
    function cellY(row){ return pad + row * (cell_height + gap) }
    function trySwap(index){
        const zero_index = tiles.indexOf("0")
        if(zero_index === -1 || index === zero_index){ return }

        const row1 = Math.floor(index / columns)
        const column1 = index % columns
        const row0 = Math.floor(zero_index / columns)
        const column0 = zero_index % columns

        const adjacent = (Math.abs(row1 - row0) + Math.abs(column1 - column0)) === 1

        if(!adjacent){ return }

        const next = tiles.slice()
        const temporal = next[index]
        next[index] = next[zero_index]
        next[zero_index] = temporal
        board.tiles = next
    }

    Rectangle {
        anchors.fill: parent
        radius: 16
        color: "#1e222b"
        border.color: "#2e3440"
        border.width: 2

        Repeater {
            model: board.tiles

            Rectangle {
                id: tileRect
                readonly property int r: Math.floor(index / board.columns)
                readonly property int c: index % board.columns
                readonly property bool isZero: modelData === "0"

                x: board.cellX(c)
                y: board.cellY(r)
                width: board.cell_width
                height: board.cell_height
                radius: 12

                readonly property bool is_next_tile: index === board.next_tile_index && !isZero

                visible: !isZero
                opacity: isZero ? 0 : 1
                color: is_next_tile ? "#1e3a5f" : "#2a303c"
                border.color: is_next_tile ? "#60a5fa" : "#3b4354"
                border.width: is_next_tile ? 2 : 1

                Behavior on x { NumberAnimation { duration: 160; easing.type: Easing.OutCubic } }
                Behavior on y { NumberAnimation { duration: 160; easing.type: Easing.OutCubic } }

                Text {
                    anchors.centerIn: parent
                    text: modelData
                    color: "#f1f5f9"
                    font.pixelSize: Math.min(parent.width, parent.height) * 0.42
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: board.trySwap(index)
                }
            }
        }
    }
}
