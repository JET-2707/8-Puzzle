import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Item {
    id: top_bar_root
    height: top_column.height

    property int difficulty_index: 0
    property int current_step: 0
    property int total_steps: 0
    property bool is_solved: false
    property bool is_manual_mode: false
    property string status_message: ""
    property bool has_timer: false
    property string remaining_time_txt: ""

    readonly property int mode_index: top_bar_root.is_manual_mode ? 1 : 0

    signal modeSelected(string mode_name)
    signal difficultySelected(int new_difficulty)
    signal generateRequested()
    signal customConfigRequested()
    signal scoresMenuRequested()
    signal quitRequested()

    Column {
        id: top_column
        width: parent.width
        spacing: 12

        Item {
            id: controls_row
            width: parent.width
            height: 42

            Button {
                id: exit_btn
                width: 68
                height: 42
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "Exit"
                onClicked: top_bar_root.quitRequested()

                contentItem: Text {
                    text: exit_btn.text
                    font.pixelSize: 13
                    font.bold: true
                    color: "#CDD6F4"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    width: exit_btn.width
                    height: exit_btn.height
                    radius: 12
                    color: (exit_btn.down) ? "#6857B3" : ((exit_btn.hovered) ? "#45475A" : "#313244")
                    border.color: (exit_btn.down || exit_btn.hovered) ? "#6857B3" : "#45475A"
                    border.width: 2

                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                HoverHandler { cursorShape: Qt.PointingHandCursor }
            }

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
                    color: "#1E1E2E"
                    border.color: "#45475A"
                    border.width: 2

                    Rectangle {
                        id: mode_pill
                        width: (parent.width - 8) / 2
                        height: parent.height - 8
                        y: 4
                        x: 4 + top_bar_root.mode_index * width
                        radius: 9
                        color: "#6857B3"

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
                                id: mode_option
                                required property int index
                                required property string modelData

                                width: (mode_selector.width - 8) / 2
                                height: mode_selector.height - 8

                                Text {
                                    anchors.centerIn: parent
                                    text: mode_option.modelData
                                    font.pixelSize: 13
                                    font.bold: true
                                    color: (top_bar_root.mode_index === mode_option.index) ? "#1E1E2E" : "#A6ADC8"

                                    Behavior on color { ColorAnimation { duration: 150 } }
                                }

                                MouseArea {
                                    width: parent.width
                                    height: parent.height
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        if(top_bar_root.mode_index !== mode_option.index){ top_bar_root.modeSelected(mode_option.index === 1 ? "manual" : "smart") }
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
                    color: "#1E1E2E"
                    border.color: "#45475A"
                    border.width: 2

                    Rectangle {
                        id: difficulty_pill
                        width: (parent.width - 8) / 2
                        height: parent.height - 8
                        y: 4
                        x: 4 + top_bar_root.difficulty_index * width
                        radius: 9
                        color: "#6857B3"

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
                                id: difficulty_option
                                required property int index
                                required property string modelData

                                width: (difficulty_selector.width - 8) / 2
                                height: difficulty_selector.height - 8

                                Text {
                                    anchors.centerIn: parent
                                    text: difficulty_option.modelData
                                    font.pixelSize: 13
                                    font.bold: true
                                    color: (top_bar_root.difficulty_index === difficulty_option.index) ? "#1E1E2E" : "#A6ADC8"

                                    Behavior on color { ColorAnimation { duration: 150 } }
                                }

                                MouseArea {
                                    width: parent.width
                                    height: parent.height
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        if(top_bar_root.difficulty_index !== difficulty_option.index){ top_bar_root.difficultySelected(difficulty_option.index) }
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
                    color: "#1E1E2E"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    width: generate_btn.width
                    height: generate_btn.height
                    radius: 10
                    color: (generate_btn.down) ? "#89B4FA" : ((generate_btn.hovered) ? "#CBA6F7" : "#6857B3")

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
                    color: "#1E1E2E"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    width: custom_btn.width
                    height: custom_btn.height
                    radius: 10
                    color: (custom_btn.down) ? "#89B4FA" : ((custom_btn.hovered) ? "#CBA6F7" : "#6857B3")

                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                HoverHandler { cursorShape: Qt.PointingHandCursor }
            }

            Button {
                id: scores_btn
                width: 82
                height: 36
                anchors.left: generate_btn.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                visible: top_bar_root.is_manual_mode
                text: "Scores"
                onClicked: top_bar_root.scoresMenuRequested()

                contentItem: Text {
                    text: scores_btn.text
                    font.pixelSize: 13
                    font.bold: true
                    color: "#1E1E2E"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    width: scores_btn.width
                    height: scores_btn.height
                    radius: 10
                    color: (scores_btn.down) ? "#89B4FA" : ((scores_btn.hovered) ? "#CBA6F7" : "#6857B3")

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
                text: (top_bar_root.is_manual_mode) ? ("Moves: " + top_bar_root.current_step) : ("Step " + top_bar_root.current_step + " / " + top_bar_root.total_steps)
                font.pixelSize: 14
                font.weight: Font.Medium
                color: "#CDD6F4"
            }

            Text {
                id: timer_label
                visible: top_bar_root.has_timer
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "Time: " + top_bar_root.remaining_time_txt
                font.pixelSize: 14
                font.bold: true
                color: "#FAB387"
            }

            Text {
                id: status_label
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                text: (top_bar_root.is_solved) ? "Solved" : "In progress"
                font.pixelSize: 13
                font.weight: Font.DemiBold
                color: (top_bar_root.is_solved) ? "#A6E3A1" : "#CDD6F4"
            }
        }

        Text {
            id: info_banner
            width: parent.width
            visible: top_bar_root.status_message !== ""
            text: top_bar_root.status_message
            font.pixelSize: 12
            color: "#A6ADC8"
            elide: Text.ElideRight
        }
    }
}
