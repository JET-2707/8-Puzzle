import QtQuick
import QtQuick.Controls

Rectangle {
    id: custom_panel_root
    visible: false
    radius: 12
    color: "#FFFFFF"
    border.width: 1
    border.color: "#CBD5E1"
    z: 20

    property string status_text: ""
    property bool is_valid_status: true

    signal closedRequested()
    signal solveRequested(var start_list, var goal_list)

    readonly property var default_start: ["1", "2", "3", "4", "5", "6", "7", "", "8"]
    readonly property var default_goal: ["1", "2", "3", "4", "5", "6", "7", "8", ""]

    function collectValues(repeater_obj){
        var collected = []
        for(var index = 0; index < repeater_obj.count; index++){
            var item_obj = repeater_obj.itemAt(index)
            collected.push((item_obj) ? item_obj.cell_text : "")
        }
        return collected
    }

    function applyPreset(repeater_obj, preset_arr){
        for(var index = 0; index < repeater_obj.count; index++){
            var item_obj = repeater_obj.itemAt(index)
            if(item_obj){ item_obj.cell_text = preset_arr[index] }
        }
    }

    MouseArea { anchors.fill: parent }

    Column {
        id: panel_column
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Item {
            width: parent.width
            height: 24

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "Start/Goal configuration"
                font.pixelSize: 15
                font.weight: Font.Bold
                color: "#0F172A"
            }

            Button {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: 24
                text: "Close"
                onClicked: custom_panel_root.closedRequested()
            }
        }

        Item {
            id: boards_row
            width: parent.width
            height: 156

            readonly property real mini_board_size: Math.min(156, (width - 16) / 2)

            Column {
                id: start_column
                width: boards_row.mini_board_size
                anchors.left: parent.left
                anchors.top: parent.top
                spacing: 6

                Text {
                    text: "Start"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    color: "#334155"
                }

                Rectangle {
                    width: boards_row.mini_board_size
                    height: boards_row.mini_board_size
                    radius: 8
                    color: "#E2E8F0"

                    Grid {
                        anchors.fill: parent
                        anchors.margins: 6
                        columns: 3
                        rows: 3
                        spacing: 4

                        readonly property real mini_cell: (width - spacing * 2) / 3

                        Repeater {
                            id: start_repeater
                            model: 9

                            Rectangle {
                                id: start_cell
                                width: parent.mini_cell
                                height: parent.mini_cell
                                radius: 5
                                color: "#FFFFFF"
                                border.width: 1
                                border.color: "#CBD5E1"

                                property string cell_text: custom_panel_root.default_start[index]

                                TextInput {
                                    anchors.centerIn: parent
                                    width: parent.width - 4
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: start_cell.cell_text
                                    maximumLength: 1
                                    font.pixelSize: 14
                                    font.weight: Font.Bold
                                    color: "#0F172A"
                                    onTextChanged: start_cell.cell_text = text
                                }
                            }
                        }
                    }
                }
            }

            Column {
                id: goal_column
                width: boards_row.mini_board_size
                anchors.right: parent.right
                anchors.top: parent.top
                spacing: 6

                Item {
                    width: parent.width
                    height: 16

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Goal"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        color: "#334155"
                    }

                    Text {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        text: "1..8"
                        font.pixelSize: 11
                        color: "#2563EB"

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: custom_panel_root.applyPreset(goal_repeater, custom_panel_root.default_goal)
                        }
                    }
                }

                Rectangle {
                    width: boards_row.mini_board_size
                    height: boards_row.mini_board_size
                    radius: 8
                    color: "#E2E8F0"

                    Grid {
                        anchors.fill: parent
                        anchors.margins: 6
                        columns: 3
                        rows: 3
                        spacing: 4

                        readonly property real mini_cell: (width - spacing * 2) / 3

                        Repeater {
                            id: goal_repeater
                            model: 9

                            Rectangle {
                                id: goal_cell
                                width: parent.mini_cell
                                height: parent.mini_cell
                                radius: 5
                                color: "#FFFFFF"
                                border.width: 1
                                border.color: "#CBD5E1"

                                property string cell_text: custom_panel_root.default_goal[index]

                                TextInput {
                                    anchors.centerIn: parent
                                    width: parent.width - 4
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: goal_cell.cell_text
                                    maximumLength: 1
                                    font.pixelSize: 14
                                    font.weight: Font.Bold
                                    color: "#0F172A"
                                    onTextChanged: goal_cell.cell_text = text
                                }
                            }
                        }
                    }
                }
            }
        }

        Text {
            width: parent.width
            wrapMode: Text.WordWrap
            text: custom_panel_root.status_text
            font.pixelSize: 12
            font.weight: Font.DemiBold
            color: (custom_panel_root.is_valid_status) ? "#15803D" : "#B91C1C"
        }

        Button {
            width: parent.width
            height: 36
            text: "Confirm and solve"
            onClicked: {
                var start_vals = custom_panel_root.collectValues(start_repeater)
                var goal_vals = custom_panel_root.collectValues(goal_repeater)
                custom_panel_root.solveRequested(start_vals, goal_vals)
            }
        }
    }
}
