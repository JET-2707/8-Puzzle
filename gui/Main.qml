import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 440
    height: 540
    visible: true
    title: "8-Puzzle"
    color: "#14161b"


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

                Behavior on x {
                        NumberAnimation { duration: 180; easing.type: Easing.OutCubic }
                    }
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

    Item { // Board
        id: board
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 28
        width: 310
        height: 310

        property var tiles: ["1", "2", "3", "4", "5", "6", "7", "8", " "]

        Rectangle {
            anchors.fill: parent
            radius: 16
            color: "#1e222b"
            border.color: "#2e3440"
            border.width: 2

            GridLayout {
                anchors.fill: parent
                anchors.margins: 12
                columns: 3
                rows: 3
                columnSpacing: 10
                rowSpacing: 10

                Repeater {
                    model: board.tiles

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        radius: 12
                        color: "#2a303c"
                        border.color: "#3b4354"
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            color: "#f1f5f9"
                            font.pixelSize: Math.min(parent.width, parent.height) * 0.42
                            font.bold: true
                        }
                    }
                }
            }
        }
    }
}
