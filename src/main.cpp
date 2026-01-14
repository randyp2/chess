#include <SFML/Graphics.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "../include/chess/config/DebugConfig.hpp"
#include "../include/chess/core/Position.hpp"
#include "../include/chess/ui/board_view.hpp"
#include "../include/chess/ui/input_controller.hpp"
#include <iostream>

using chess::config::DebugConfig;
DebugConfig parse_debug_config(int argc, char **argv) {
    DebugConfig debugger = DebugConfig::disabled();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--debug-bitboard")
            debugger.enable_bitboards();
    }

    return debugger;
}

int main(int argc, char **argv) {

    DebugConfig debugger = parse_debug_config(argc, argv);

    std::cout << "Humble beginnings..." << std::endl;

    // ------ Init Window ------
    constexpr unsigned windowWidth = 800;
    constexpr unsigned windowHeight = 800;

    // SFML uses sf namespace
    // Window: SFML Class that renders what you "draw" into it
    // Video Mode: Class explaining dimensions of window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Chess");

    // Synchronize drawing with refresh rate
    window.setVerticalSyncEnabled(true);

    // ------ Init board & Position ------
    constexpr float squareSize = 80.0f;
    chess::ui::BoardView boardView({80.0f, 80.0f}, // top-left corner of board
                                   squareSize);
    chess::core::Position position;
    position.print_bitboard(position.getPieces(chess::core::PieceType::Pawn));

    chess::ui::InputController input_controller;

    // ------ Main Event Loop ------
    // int counter = 0;
    while (window.isOpen() /*&& counter != 1*/) {

        // counter++;
        sf::Event event{};

        // Insert pending events and store in event
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            input_controller.handleEvent(event, window, boardView, position,
                                         debugger);
        }

        // Erase frame and replace it with the following color
        window.clear(sf::Color(30, 30, 30));
        boardView.draw(window, position, input_controller.dragState());
        window.display();
    }

    // Testing positions
    // chess::core::Position position;
    //
    // position.print_bitboard(position.pieces(chess::core::PieceType::Knight));
    // position.print_bitboard(position.pieces(chess::core::Color::White,
    //                                         chess::core::PieceType::Knight));
    //
    // position.print_bitboard(position.pieces(chess::core::Color::Black,
    //                                         chess::core::PieceType::Knight));

    std::cout << "Goodbye..." << std::endl;

    return 0;
}
