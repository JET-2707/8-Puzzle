import QtQuick

import Puzzle 1.0

Window {
    id: root_window
    width: 400
    height: 540
    visible: true
    title: "8-Puzzle"
    color: "#14161b"

    property int selected_difficulty: 0

    PuzzleController { id: puzzle_controller }

    Item {
        id: main_container
        anchors.fill: parent
        anchors.margins: 24

        TopBar {
            id: top_bar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            difficulty_index: root_window.selected_difficulty
            current_step: puzzle_controller.current_step
            total_steps: puzzle_controller.total_steps
            is_solved: puzzle_controller.is_solved

            onDifficultySelected: function(new_diff){ root_window.selected_difficulty = new_diff }
            onGenerateRequested: function(){ puzzle_controller.generateAndSolve(root_window.selected_difficulty) }
        }

        Board {
            id: board_view
            anchors.top: top_bar.bottom
            anchors.bottom: step_controls.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 16
            anchors.bottomMargin: 16
            tile_values: (puzzle_controller.tile_values) ? puzzle_controller.tile_values : []
            next_tile_index: (puzzle_controller.next_tile_index !== undefined) ? puzzle_controller.next_tile_index : -1
            is_solved: puzzle_controller.is_solved === true
        }

        StepControls {
            id: step_controls
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            current_step: puzzle_controller.current_step
            total_steps: puzzle_controller.total_steps

            onPreviousRequested: puzzle_controller.previousStep()
            onRestartRequested: puzzle_controller.restartSteps()
            onNextRequested: puzzle_controller.nextStep()
        }
    }
}
