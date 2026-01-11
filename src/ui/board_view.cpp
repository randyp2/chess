#include "../../include/chess/ui/board_view.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

namespace chess::ui {

BoardView::BoardView(sf::Vector2f topLeft, float squareSizePx)
    : topLeft(topLeft), squareSizePx(squareSizePx) {}

void BoardView::draw(sf::RenderTarget &target) const {

    // {} enforces stricter rules such as no narrowing conversion, typically
    // preffered over ()
    const sf::Color light_square_color{240, 217, 181};
    const sf::Color dark_square_color{110, 73, 60};

    sf::RectangleShape square({squareSizePx, squareSizePx});

    /**
     * Chess game design:
     *      - Rows = Ranks
     *      - Columns = Files
     *      8x8 board -> 64 cells total
     */
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            const bool isDarkSquare =
                (file + rank) % 2 == 0; // If even dark; else light

            // Initialize squares - color & pos
            square.setFillColor(isDarkSquare ? dark_square_color
                                             : light_square_color);

            square.setPosition(topLeft.x + file * squareSizePx,
                               topLeft.y + rank * squareSizePx);

            // Draw square
            target.draw(square);
        }
    }
}

} // namespace chess::ui
