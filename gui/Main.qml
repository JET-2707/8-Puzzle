import QtQuick
import Puzzle

Window {
    id: root_window
    width: 420
    height: 560
    minimumWidth: 380
    minimumHeight: 500
    visible: true
    title: "8-Puzzle"
    color: "#F6F7F9"

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
            z: 20
            difficulty_index: root_window.selected_difficulty
            current_step: puzzle_controller.current_step
            total_steps: puzzle_controller.total_steps
            is_solved: puzzle_controller.is_solved
            is_manual_mode: puzzle_controller.is_manual_mode
            status_message: puzzle_controller.status_message

            onModeSelected: function(mode_name){
                custom_panel.visible = false

                if(mode_name !== "manual"){ scores_menu.scores_menu_open = false }

                puzzle_controller.setMode(mode_name)
            }
            onDifficultySelected: function(new_difficulty){ root_window.selected_difficulty = new_difficulty }
            onGenerateRequested: function(){ puzzle_controller.generateAndSolve(root_window.selected_difficulty) }
            onCustomConfigRequested: function(){ custom_panel.visible = !custom_panel.visible }
            onScoresMenuRequested: function(){
                if(puzzle_controller.is_manual_mode){ scores_menu.scores_menu_open = !scores_menu.scores_menu_open }
            }
        }

        Board {
            id: board_view
            anchors.top: top_bar.bottom
            anchors.bottom: step_controls.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 16
            anchors.bottomMargin: (step_controls.visible) ? 16 : 4
            tile_values: (puzzle_controller.tile_values) ? puzzle_controller.tile_values : []
            next_tile_index: (puzzle_controller.next_tile_index !== undefined) ? puzzle_controller.next_tile_index : -1
            is_solved: puzzle_controller.is_solved === true
            interactive_tiles: puzzle_controller.is_manual_mode === true

            onTileClicked: function(tile_idx){ puzzle_controller.clickTile(tile_idx) }
        }

        CustomBoardsDialog {
            id: custom_panel
            anchors.top: top_bar.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
            height: 295
            status_text: puzzle_controller.status_message
            is_valid_status: puzzle_controller.is_solvable

            onClosedRequested: custom_panel.visible = false

            onSolveRequested: function(start_list, goal_list){
                var ok_result = puzzle_controller.solveCustomBoards(start_list, goal_list)
                if(ok_result){ custom_panel.visible = false }
            }
        }

        ScoresMenu {
            id: scores_menu
            property bool scores_menu_open: false
            visible: scores_menu_open && puzzle_controller.is_manual_mode
            anchors.top: top_bar.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            scores_list: (puzzle_controller.best_scores) ? puzzle_controller.best_scores : []

            onClosedRequested: scores_menu_open = false
        }

        StepControls {
            id: step_controls
            visible: !puzzle_controller.is_manual_mode
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
