import QtQuick
import QtQuick.Controls

import Puzzle 1.0

ApplicationWindow {
    width: 440
    height: 540
    visible: true
    title: "8-Puzzle"
    color: "#14161b"

    PuzzleController {
        id: puzzle
    }

    Item { // Difficulty
        id: difficulty
        x: (parent.width - width) / 2
        y: 36
        width: 240
        height: 42

        property int select_index: 0 // 0 / 1
        readonly property var options: ["Easy", "Hard"]

        Rectangle {
            anchors.fill: parent
            radius: 12
            color: "#1e222b"
            border.color: "#2e3440"
            border.width: 2

            Rectangle {
                id: pill
                width: (parent.width - 8) / 2
                height: parent.height - 8
                y: 4
                x: 4 + (difficulty.select_index * width)
                radius: 9
                color: "#3b82f6"

                Behavior on x { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
                }

            Row {
                anchors.fill: parent
                anchors.margins: 4

                Repeater {
                    model: difficulty.options

                    Item {
                        width: (difficulty.width - 8) / 2
                        height: difficulty.height - 8

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            font.pixelSize: 14
                            font.bold: true
                            color: difficulty.select_index === index ? "#ffffff" : "#94a3b8"

                            Behavior on color { ColorAnimation { duration: 150 } }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: difficulty.select_index = index

                        }
                    }
                }
            }
        }
    }  


    Board {
        id: puzzle_board
        x: (parent.width - width) / 2
        y: 100
        tiles: puzzle.tiles
        next_tile_index: puzzle.next_tile_index
    }
}
