// #include <iostream>
// #include <vector>
//  // #include "core.hpp"
// #include "algorithms.hpp"
// #include "core.hpp"
// #include "modes.hpp"
//
// void printBoard(const Board& board) {
//     std::cout << "-------------\n";
//     for (int i = 0; i < 3; ++i) {
//         std::cout << "| ";
//         for (int j = 0; j < 3; ++j) {
//             if (board[i][j] == 0) {
//                 std::cout << "  | ";
//             } else {
//                 std::cout << board[i][j] << " | ";
//             }
//         }
//         std::cout << "\n-------------\n";
//     }
// }
//
//
// //
// // int main() {
// //     Node goal;
// //     goal.board = {
// //         {1, 2, 3},
// //         {4, 5, 6},
// //         {7, 8, 0}
// //     };
// //
// //     Node start;
// //     start.board = Game::generatePuzzle(goal.board, MOVES_DIFFICULTY::NO_EASY);
// //
// //     std::cout << "=== ESTADO INICIAL (Generado Aleatoriamente) ===" << std::endl;
// //     printBoard(start.board);
// //
// //     std::cout << "\n=== ESTADO OBJETIVO ===" << std::endl;
// //     printBoard(goal.board);
// //
// //     Graph solver;
// //     std::cout << "\nCalculando solucion con A*...\n" << std::endl;
// //
// //     std::vector<Node> path = solver.AStar(start, goal);
// //
// //     if (path.empty()) {
// //         std::cout << "No se encontro ninguna solucion." << std::endl;
// //     } else {
// //         std::cout << "¡Solucion encontrada en " << path.size() - 1 << " movimientos!\n" << std::endl;
// //
// //         for (size_t i = 0; i < path.size(); ++i) {
// //             std::cout << "Paso " << i << ":" << std::endl;
// //             printBoard(path[i].board);
// //             std::cout << std::endl;
// //         }
// //     }
// //
// //     return 0;
// // }
// //
//
//
// int main(){
//     // Board s({4, 1, 3, 7, 2, 6, 0, 5, 8}), 
//     //       g({1, 2, 3, 4, 5, 6, 7, 8, 0});
//     //
//     // Node start, goal;
//     // start.board = s;
//     // goal.board = g;
//     //
//     // Graph gr;
//     // std::vector<Node> solution = gr.AStar(start, goal);
//
//     SmartMode sm;
//     sm.generateOptions(MOVES_DIFFICULTY::EASY);
//
//     std::vector<Board> options_boards = sm.getOptions();
//
//     for(auto &brd : options_boards){ printBoard(brd); std::cout << '\n'; }
//
//     sm.choiceOption(1);
//     sm.obtainPath();
//
//     std::vector<Node> solution = sm.getPath();
//
//     // Depuración del tamaño
//     std::cout << "Tamaño del vector solution: " << solution.size() << std::endl;
//
//     if (solution.empty()) {
//         std::cout << "ERROR: El algoritmo A* no encontró solución o devolvió un vector vacío.\n";
//     } else {
//         std::cout << "Solution (" << solution.size() - 1 << " movimientos):\n";
//
//         for(size_t i = 0; i < solution.size(); ++i){
//             std::cout << "Paso " << i << ":" << std::endl;
//             printBoard(solution[i].board);
//             std::cout << '\n'; 
//         }
//     }
//
//     // std::cout << "Solution: \n";
//     // for(auto &s : solution){
//     //     printBoard(s.board);
//     // }
//
//     return 0;
// }



#include <iostream>
#include <vector>

#include "core.hpp"
#include "files.hpp"

int main() {
    // // 1. Creamos 5 puntajes de prueba
    // std::vector<Score> puntajesOriginales = { Score(1500), Score(3200), Score(800), Score(4500), Score(2100) };
    //
    // Score::sortScores(puntajesOriginales);
    //
    // // 3. Guardamos en el archivo binario
    // if(Files::saveFile(puntajesOriginales)){ std::cout << "Archivo guardado con exito.\n\n"; } 
    // else { std::cout << "Error al guardar el archivo.\n"; return 1; }

    // 4. Leemos desde el archivo a un vector nuevo
    std::vector<Score> puntajesLeidos = Files::loadFile();

    if(puntajesLeidos.empty()){ std::cout << "Error al leer el archivo.\n"; return 1; }

    // 5. Mostramos los resultados recuperados
    std::cout << "--- Puntajes leidos de data.bin ---\n";
    for(const auto& s : puntajesLeidos){
        std::cout << s.getPoints() << " pts\n";
    }

    return 0;
}

