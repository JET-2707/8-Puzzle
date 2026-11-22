import QtQuick
import QtQuick.Controls

Item {
    id: top_bar_root
    height: top_column.height

    property int difficulty_index: 0
    property int current_step: 0
    property int total_steps: 0
    property bool is_solved: false

    signal difficultySelected(int new_difficulty)
    signal generateRequested()

    Column {
        id: top_column
        width: parent.width
        spacing: 12

        Item {
            id: controls_row
            width: parent.width
            height: 42

            Item {
                id: difficulty
                width: 240
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

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
                        x: 4 + top_bar_root.difficulty_index * width
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
                                    color: (top_bar_root.difficulty_index === index) ? "#ffffff" : "#94a3b8"

                                    Behavior on color { ColorAnimation { duration: 150 } }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        if(top_bar_root.difficulty_index !== index){ top_bar_root.difficultySelected(index) }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Button {
                id: generate_btn
                width: 90
                height: 34
                anchors.left: difficulty.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "New"
                onClicked: top_bar_root.generateRequested()

                contentItem: Text {
                    text: generate_btn.text
                    font.pixelSize: 13
                    font.bold: true
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    radius: 10
                    color: (generate_btn.down) ? "#2563eb" : ((generate_btn.hovered) ? "#4b8ff7" : "#3b82f6")

                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                HoverHandler { cursorShape: Qt.PointingHandCursor }
            }
        }

        Item {
            id: status_row
            width: parent.width
            height: 22

            Text {
                id: step_label
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "Step " + top_bar_root.current_step + " / " + top_bar_root.total_steps
                font.pixelSize: 14
                font.weight: Font.Medium
                color: "#cbd5e1"
            }

            Text {
                id: status_label
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                text: (top_bar_root.is_solved) ? "Solved" : "In progress"
                font.pixelSize: 13
                font.weight: Font.DemiBold
                color: (top_bar_root.is_solved) ? "#4ade80" : "#cbd5e1"
            }
        }
    }
}
