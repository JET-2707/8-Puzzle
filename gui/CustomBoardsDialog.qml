import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Rectangle {
    id: custom_panel_root
    visible: false
    radius: 12
    color: "#1E1E2E"
    border.width: 1
    border.color: "#45475A"
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
                color: "#CDD6F4"
            }

            Button {
                id: close_btn
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 56
                height: 24
                text: "Close"
                onClicked: custom_panel_root.closedRequested()

                contentItem: Text {
                    text: close_btn.text
                    font.pixelSize: 11
                    font.bold: true
                    color: "#CDD6F4"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    radius: 7
                    color: (close_btn.down) ? "#585B70" : ((close_btn.hovered) ? "#45475A" : "#313244")

                Behavior on color { ColorAnimation { duration: 120 } }
                }
                
                HoverHandler { cursorShape: Qt.PointingHandCursor }
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
                    color: "#A6ADC8"
                }

                Rectangle {
                    width: boards_row.mini_board_size
                    height: boards_row.mini_board_size
                    radius: 8
                    color: "#181825"

                    Grid {
                        id: start_grid
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
                                required property int index
                                width: start_grid.mini_cell
                                height: start_grid.mini_cell
                                radius: 5
                                color: "#313244"
                                border.width: 1
                                border.color: "#45475A"

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
                                    color: "#CDD6F4"
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
                        color: "#A6ACD8"
                    }

                    Text {
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        text: "1...8"
                        font.pixelSize: 11
                        color: "#6857B3"

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
                    color: "#181825"

                    Grid {
                        id: goal_grid
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
                                required property int index
                                width: goal_grid.mini_cell
                                height: goal_grid.mini_cell
                                radius: 5
                                color: "#313244"
                                border.width: 1
                                border.color: "#45475A"

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
                                    color: "#CDD6F4"
                                    selectionColor: "#6857B3"
                                    selectedTextColor: "#1E1E2E"
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
            color: (custom_panel_root.is_valid_status) ? "#A6E3A1" : "#F38BA8"
        }

        Button {
            id: confirm_btn
            // width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            width: 300
            height: 36
            text: "Confirm and solve"

            onClicked: {
                var start_vals = custom_panel_root.collectValues(start_repeater)
                var goal_vals = custom_panel_root.collectValues(goal_repeater)
                custom_panel_root.solveRequested(start_vals, goal_vals)
            }
    
            contentItem: Text {
                text: confirm_btn.text
                font.pixelSize: 13
                font.bold: true
                color: "#CDD6F4"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                radius: 10
                color: (confirm_btn.down) ? "#585B70" : ((confirm_btn.hovered) ? "#45475A" : "#313244")

                Behavior on color { ColorAnimation { duration: 120 } }
            }

            HoverHandler { cursorShape: Qt.PointingHandCursor }
        }
    }
}
