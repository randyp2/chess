#include "../../include/chess/ui/input_controller.hpp"
#include "chess/core/Move.hpp"
#include "chess/core/MoveGenerator.hpp"
#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <cstdint>
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
        std::cout << "Target square: " << targetSquare << "\n";

        if (targetSquare != -1) {
            core::MoveList moves;
            moves.clear();
            core::MoveGenerator::generatePawnMoves(position, moves);

            // --- Testing pawn moves (QUIET AND PUSH)
            bool found = false;
            core::Move chosenMove;
            // Iterate until you find the move that matches the users move
            for (std::size_t i = 0; i < moves.size; ++i) {
                const chess::core::Move &m = moves.moves[i];

                if (m.getFrom() == drag.piece.squareIdx &&
                    m.getTo() == targetSquare) {
                    chosenMove = m;
                    found = true;
                    break;
                }
            }
            // --- Testing pawn moves (QUIET AND PUSH)

            if (found)
                position.makeMove(chosenMove, debugger);
            else
                std::cout << "Illegal pawn move!\n";

            // chess::core::Move move(
            //     static_cast<std::uint8_t>(drag.piece.squareIdx),
            //     static_cast<std::uint8_t>(targetSquare),
            //     chess::core::MoveFlag::QUEEN_CASTLE);
            // position.makeMove(move, debugger);
        }

        drag.active = false;
    }
}
}; // namespace chess::ui
