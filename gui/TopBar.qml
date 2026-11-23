import QtQuick
import QtQuick.Controls

Item {
    id: top_bar_root
    height: top_column.height

    property int difficulty_index: 0
    property int current_step: 0
    property int total_steps: 0
    property bool is_solved: false
    property bool is_manual_mode: false
    property string status_message: ""

    readonly property int mode_index: top_bar_root.is_manual_mode ? 1 : 0

    signal modeSelected(string mode_name)
    signal difficultySelected(int new_difficulty)
    signal generateRequested()
    signal customConfigRequested()

    Column {
        id: top_column
        width: parent.width
        spacing: 12

        Item {
            id: controls_row
            width: parent.width
            height: 42

            Item {
                id: mode_selector
                width: 148
                height: parent.height
                anchors.right: difficulty_selector.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                readonly property var mode_options: ["Smart", "Manual"]

                Rectangle {
                    width: parent.width
                    height: parent.height
                    radius: 12
                    color: "#1e222b"
                    border.color: "#2e3440"
                    border.width: 2

                    Rectangle {
                        id: mode_pill
                        width: (parent.width - 8) / 2
                        height: parent.height - 8
                        y: 4
                        x: 4 + top_bar_root.mode_index * width
                        radius: 9
                        color: "#3b82f6"

                        Behavior on x { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
                    }

                    Row {
                        x: 4
                        y: 4
                        width: parent.width - 8
                        height: parent.height - 8

                        Repeater {
                            model: mode_selector.mode_options

                            Item {
                                width: (mode_selector.width - 8) / 2
                                height: mode_selector.height - 8

                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    font.pixelSize: 13
                                    font.bold: true
                                    color: (top_bar_root.mode_index === index) ? "#ffffff" : "#94a3b8"

                                    Behavior on color { ColorAnimation { duration: 150 } }
                                }

                                MouseArea {
                                    width: parent.width
                                    height: parent.height
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        if(top_bar_root.mode_index !== index){ top_bar_root.modeSelected(index === 1 ? "manual" : "smart") }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Item {
                id: difficulty_selector
                width: 148
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                readonly property var difficulty_options: ["Easy", "Hard"]

                Rectangle {
                    width: parent.width
                    height: parent.height
                    radius: 12
                    color: "#1e222b"
                    border.color: "#2e3440"
                    border.width: 2

                    Rectangle {
                        id: difficulty_pill
                        width: (parent.width - 8) / 2
                        height: parent.height - 8
                        y: 4
                        x: 4 + top_bar_root.difficulty_index * width
                        radius: 9
                        color: "#3b82f6"

                        Behavior on x { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
                    }

                    Row {
                        x: 4
                        y: 4
                        width: parent.width - 8
                        height: parent.height - 8

                        Repeater {
                            model: difficulty_selector.difficulty_options

                            Item {
                                width: (difficulty_selector.width - 8) / 2
                                height: difficulty_selector.height - 8

                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    font.pixelSize: 13
                                    font.bold: true
                                    color: (top_bar_root.difficulty_index === index) ? "#ffffff" : "#94a3b8"

                                    Behavior on color { ColorAnimation { duration: 150 } }
                                }

                                MouseArea {
                                    width: parent.width
                                    height: parent.height
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
                width: 76
                height: 36
                anchors.left: difficulty_selector.right
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
                    width: generate_btn.width
                    height: generate_btn.height
                    radius: 10
                    color: (generate_btn.down) ? "#2563eb" : ((generate_btn.hovered) ? "#4b8ff7" : "#3b82f6")

                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                HoverHandler { cursorShape: Qt.PointingHandCursor }
            }

            Button {
                id: custom_btn
                width: 88
                height: 36
                anchors.left: generate_btn.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                visible: !top_bar_root.is_manual_mode
                text: "Start/Goal"
                onClicked: top_bar_root.customConfigRequested()

                contentItem: Text {
                    text: custom_btn.text
                    font.pixelSize: 13
                    font.bold: true
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    width: custom_btn.width
                    height: custom_btn.height
                    radius: 10
                    color: (custom_btn.down) ? "#2563eb" : ((custom_btn.hovered) ? "#4b8ff7" : "#3b82f6")

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
                text: top_bar_root.is_manual_mode ? ("Moves: " + top_bar_root.current_step) : ("Step " + top_bar_root.current_step + " / " + top_bar_root.total_steps)
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

        Text {
            id: info_banner
            width: parent.width
            visible: top_bar_root.status_message !== ""
            text: top_bar_root.status_message
            font.pixelSize: 12
            color: "#94a3b8"
            elide: Text.ElideRight
        }
    }
}
