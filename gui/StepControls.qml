import QtQuick
import QtQuick.Controls

Item {
    id: controls_root
    height: (visible) ? 38 : 0

    property int current_step: 0
    property int total_steps: 0

    signal previousRequested()
    signal restartRequested()
    signal nextRequested()

    readonly property real btn_spacing: 10
    readonly property real btn_width: 96
    readonly property real btn_height: 34

    Row {
        id: controls_row
        anchors.centerIn: parent
        spacing: controls_root.btn_spacing

        Button {
            id: prev_btn
            width: controls_root.btn_width
            height: controls_root.btn_height
            text: "Last"
            enabled: controls_root.current_step > 0
            onClicked: controls_root.previousRequested()

            contentItem: Text {
                text: prev_btn.text
                font.pixelSize: 13
                font.bold: true
                color: (prev_btn.enabled) ? "#ffffff" : "#64748b"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                width: prev_btn.width
                height: prev_btn.height
                radius: 10
                color: (!prev_btn.enabled) ? "#1e222b" : ((prev_btn.down) ? "#2563eb" : ((prev_btn.hovered) ? "#4b8ff7" : "#3b82f6"))
                border.color: "#2e3440"
                border.width: (prev_btn.enabled) ? 0 : 2

                Behavior on color { ColorAnimation { duration: 120 } }
            }

            HoverHandler { cursorShape: Qt.PointingHandCursor }
        }

        Button {
            id: restart_btn
            width: controls_root.btn_width
            height: controls_root.btn_height
            text: "Reset"
            enabled: controls_root.current_step > 0
            onClicked: controls_root.restartRequested()

            contentItem: Text {
                text: restart_btn.text
                font.pixelSize: 13
                font.bold: true
                color: (restart_btn.enabled) ? "#ffffff" : "#64748b"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                width: restart_btn.width
                height: restart_btn.height
                radius: 10
                color: (!restart_btn.enabled) ? "#1e222b" : ((restart_btn.down) ? "#2563eb" : ((restart_btn.hovered) ? "#4b8ff7" : "#3b82f6"))
                border.color: "#2e3440"
                border.width: (restart_btn.enabled) ? 0 : 2

                Behavior on color { ColorAnimation { duration: 120 } }
            }

            HoverHandler { cursorShape: Qt.PointingHandCursor }
        }

        Button {
            id: next_btn
            width: controls_root.btn_width
            height: controls_root.btn_height
            text: "Next"
            enabled: controls_root.total_steps > 0 && controls_root.current_step < controls_root.total_steps
            onClicked: controls_root.nextRequested()

            contentItem: Text {
                text: next_btn.text
                font.pixelSize: 13
                font.bold: true
                color: (next_btn.enabled) ? "#ffffff" : "#64748b"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                width: next_btn.width
                height: next_btn.height
                radius: 10
                color: (!next_btn.enabled) ? "#1e222b" : ((next_btn.down) ? "#2563eb" : ((next_btn.hovered) ? "#4b8ff7" : "#3b82f6"))
                border.color: "#2e3440"
                border.width: (next_btn.enabled) ? 0 : 2

                Behavior on color { ColorAnimation { duration: 120 } }
            }

            HoverHandler { cursorShape: Qt.PointingHandCursor }
        }
    }
}
