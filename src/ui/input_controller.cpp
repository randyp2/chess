#include "../../include/chess/ui/input_controller.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
namespace chess::ui {

/* ========= METHOD IMPLEMENTATIONS =========*/
void InputController::handleEvent(const sf::Event &event,
                                  const sf::RenderWindow &window,
                                  const chess::ui::BoardView &board,
                                  chess::core::Position &position,
                                  const chess::config::DebugConfig &debugger) {
    // --- Mouse press = Pick up piece
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mouse =
            window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        // Find pixel square
        int squareIdx = board.pixelToSquare(mouse);
        if (squareIdx == -1)
            return;

        for (const auto &p : position.getAllPieces()) {
            // Check for matching squares
            if (p.squareIdx == squareIdx) {
                drag.active = true;
                drag.piece = p;
                drag.mousePos = mouse;
                break;
            }
        }
    }

    // --- Mouse move = Update drag and sprite
    else if (event.type == sf::Event::MouseMoved && drag.active) {

        // Convert window pixels to coordinates used to draw
        drag.mousePos =
            window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
    }

    // --- Mouse release = update piece to new position
    else if (event.type == sf::Event::MouseButtonReleased &&
             event.mouseButton.button == sf::Mouse::Left && drag.active) {
        int targetSquare = board.pixelToSquare(drag.mousePos);

        if (targetSquare != -1) {
            position.makeMove(drag.piece.squareIdx, targetSquare, debugger);
        }

        drag.active = false;
    }
}
}; // namespace chess::ui
