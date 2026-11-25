import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

Rectangle {
    id: scores_menu_root
    visible: false
    width: 220
    height: 230
    radius: 12
    color: "#1E1E2E"
    border.color: "#45475A"
    border.width: 2
    z: 35

    property var scores_list: []

    signal closedRequested()

    MouseArea { anchors.fill: parent }

    Column {
        id: scores_column
        x: 14
        y: 14
        width: parent.width - 28
        spacing: 10

        Item {
            width: parent.width
            height: 24

            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "Best Scores"
                font.pixelSize: 14
                font.bold: true
                color: "#CDD6F4"
            }

            Button {
                id: close_scores_btn
                width: 56
                height: 24
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                text: "Close"
                onClicked: scores_menu_root.closedRequested()

                contentItem: Text {
                    text: close_scores_btn.text
                    font.pixelSize: 11
                    font.bold: true
                    color: "#1E1E2E"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    width: close_scores_btn.width
                    height: close_scores_btn.height
                    radius: 7
                    color: (close_scores_btn.down) ? "#89B4FA" : "#6857B3"
                    
                    Behavior on color { ColorAnimation {duration: 120 } }
                }

                HoverHandler { cursorShape: Qt.PointingHandCursor }
            }
        }

        Column {
            id: score_rows
            width: parent.width
            spacing: 6

            Repeater {
                model: 5

                Rectangle {
                    id: score_row
                    required property int index
                    width: scores_column.width
                    height: 28
                    radius: 7
                    color: "#313244"
                    border.color: "#45475A"
                    border.width: 1

                    readonly property var safe_list: (scores_menu_root.scores_list) ? scores_menu_root.scores_list : []
                    readonly property int pts_val: (index < safe_list.length) ? Number(safe_list[index]) : 0

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: "#" + (score_row.index + 1)
                        font.pixelSize: 12
                        font.bold: true
                        color: "#A6ADC8"
                    }

                    Text {
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: score_row.pts_val + " pts"
                        font.pixelSize: 12
                        font.bold: true
                        color: (score_row.pts_val > 0) ? "#A6E3A1" : "#CDD6F4"
                    }
                }
            }
        }
    }
}
