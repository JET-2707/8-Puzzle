import QtQuick

pragma ComponentBehavior: Bound

Item {
    id: board_root

    property var tile_values: []
    property int next_tile_index: -1
    property bool is_solved: false
    property bool interactive_tiles: false
    signal tileClicked(int tile_index)
    readonly property int board_size: Math.max(0, Math.min(width, height))

    Rectangle {
        id: board_frame
        width: board_root.board_size
        height: board_root.board_size
        anchors.centerIn: parent
        radius: 14
        color: "#1E1E2E"

        Grid {
            id: tile_grid
            anchors.fill: parent
            anchors.margins: 10
            columns: 3
            rows: 3
            spacing: 10

            readonly property real cell_size: Math.max(0, (width - spacing * 2) / 3)

            Repeater {
                model: 9

                Rectangle {
                    id: tile_card
                    required property int index
                    width: tile_grid.cell_size
                    height: tile_grid.cell_size
                    radius: 10

                    readonly property var current_list: (board_root.tile_values) ? board_root.tile_values : []
                    readonly property string tile_label: (index < current_list.length) ? String(current_list[index]) : ""
                    readonly property bool is_empty: tile_label === "" || tile_label === "0"
                    readonly property bool is_hint: (!board_root.is_solved && !board_root.interactive_tiles && index === board_root.next_tile_index)

                    color: (is_empty) ? "transparent" : ((tile_mouse.containsMouse && board_root.interactive_tiles && !board_root.is_solved) ? "#45475A" : ((is_hint) ? "#45475A" : "#313244"))
                    border.width: (is_empty) ? 0 : ((is_hint) ? 2 : 1)
                    border.color: (is_hint) ? "#6857B3" : "#45475A"

                    Behavior on color { ColorAnimation { duration: 120 } }

                    Text {
                        anchors.centerIn: parent
                        visible: !tile_card.is_empty
                        text: tile_card.tile_label
                        font.pixelSize: Math.max(16, tile_grid.cell_size * 0.34)
                        font.weight: Font.Bold
                        color: (tile_card.is_hint) ? "#6857B3" : "#CDD6F4"
                    }

                    MouseArea {
                        id: tile_mouse
                        anchors.fill: parent
                        enabled: board_root.interactive_tiles && !tile_card.is_empty && !board_root.is_solved
                        hoverEnabled: true
                        cursorShape: (enabled) ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: board_root.tileClicked(tile_card.index)
                    }
                }
            }
        }
    }
}
