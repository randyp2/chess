#include <SFML/Graphics.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "../include/chess/core/Position.hpp"
#include "../include/chess/ui/board_view.hpp"
#include <iostream>

int main() {

    std::cout << "Humble beginnings..." << std::endl;

    // ------ Init Window ------
    constexpr unsigned windowWidth = 800;
    constexpr unsigned windowHeight = 800;

    // SFML uses sf namespace
    // Window: SFML Class that renders what you "draw" into it
    // Video Mode: Class explaining dimensions of window
    // sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
    // "Chess");

    // Synchronize drawing with refresh rate
    // window.setVerticalSyncEnabled(true);

    // ------ Init board ------
    constexpr float squareSize = 80.0f;
    // chess::ui::BoardView boardView({80.0f, 80.0f}, // top-left corner of
    // board squareSize);

    // ------ Main Event Loop ------
    // while (window.isOpen()) {
    //
    //     sf::Event event{};
    //
    //     // Insert pending events and store in event
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     }
    //
    //     // Erase frame and replace it with the following color
    //     window.clear(sf::Color(30, 30, 30));
    //     boardView.draw(window);
    //     window.display();
    // }

    // Testing positions
    chess::core::Position position;

    position.print_bitboard(position.pieces(chess::core::PieceType::Knight));
    // position.print_bitboard(position.pieces(chess::core::Color::White,
    //                                         chess::core::PieceType::Knight));
    //
    // position.print_bitboard(position.pieces(chess::core::Color::Black,
    //                                         chess::core::PieceType::Knight));

    std::cout << "Goodbye..." << std::endl;

    return 0;
}
