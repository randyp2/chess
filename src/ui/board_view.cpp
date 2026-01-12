#include "../../include/chess/ui/board_view.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <stdexcept>
#include <string>

namespace chess::ui {

BoardView::BoardView(sf::Vector2f topLeft, float squareSizePx)
    : topLeft(topLeft), squareSizePx(squareSizePx) {

    if (!pieceTexture.loadFromFile("assets/Pieces.png"))
        throw std::runtime_error("Failed to load piece texture");

    if (!font.loadFromFile("assets/Rubik-Regular.ttf"))
        throw std::runtime_error("Failed to load font");
}

void BoardView::draw(sf::RenderTarget &target) const {

    // {} enforces stricter rules such as no narrowing conversion, typically
    // preffered over ()
    const sf::Color light_square_color{240, 217, 181};
    const sf::Color dark_square_color{110, 73, 60};
    const sf::Color white{255, 255, 255};

    sf::RectangleShape square({squareSizePx, squareSizePx});

    sf::Text text;
    text.setFont(font);
    text.setString("Chess");
    text.setCharacterSize(32);
    text.setFillColor(white);
    // text.setPosition(0, 0);
    target.draw(text);

    /**
     * Chess game design:
     *      - Rows = Ranks
     *      - Columns = Files
     *      8x8 board -> 64 cells total
     */
    const char files[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {

            // -- Draw chess notation
            if (file == 0)
                drawRankNotation(target, text, rank, file);

            if (rank == 7)
                drawFileNotation(target, text, rank, file);
            //

            // -- Draw squares
            const bool isDarkSquare =
                (file + rank) % 2 == 0; // If even dark; else light

            // Initialize squares - color & pos
            square.setFillColor(!isDarkSquare ? dark_square_color
                                              : light_square_color);

            square.setPosition(topLeft.x + file * squareSizePx,
                               topLeft.y + rank * squareSizePx);

            // Draw square & Text
            target.draw(square);
        }
    }

    // Testing out pieces draw
    // sf::Sprite piece;
    // piece.setTexture(pieceTexture);
    //
    // constexpr int spriteW = 333; // Image width / 6
    // constexpr int spriteH = 334; // Image height / 2
    // const int spriteCol = 0;
    // const int spriteRow = 1;
    //
    // piece.setTextureRect(sf::IntRect(spriteCol * spriteW, spriteRow *
    // spriteH,
    //                                  spriteW, spriteH));
    //
    // // Scale sprite to fit square
    // const float scaleX = squareSizePx / static_cast<float>(spriteW);
    // const float scaleY = squareSizePx / static_cast<float>(spriteH);
    //
    // piece.setScale(scaleX, scaleY);
    //
    // const int file = 4;
    // const int rank = 6;
    //
    // piece.setPosition(topLeft.x + file * squareSizePx,
    //                   topLeft.y + rank * squareSizePx);
    //
    // std::cout << "Attempting to draw piece: " << std::endl;
    // target.draw(piece);

    // -- Private helpers
}

void BoardView::drawRankNotation(sf::RenderTarget &target, sf::Text text,
                                 int rank, int file) const {

    text.setString(std::to_string(8 - rank));

    // Measure text bound
    sf::FloatRect bounds = text.getLocalBounds();

    // Set origin to center intead of top-left
    text.setOrigin(bounds.left + bounds.width / 2,
                   bounds.top + bounds.height / 2);

    // Find center of square vertically
    float centerY = (topLeft.y + rank * squareSizePx) + (squareSizePx / 2.f);
    float centerX = (topLeft.x + file * squareSizePx) - (squareSizePx / 2.f);

    text.setPosition(centerX, centerY);

    target.draw(text);
}

void BoardView::drawFileNotation(sf::RenderTarget &target, sf::Text text,
                                 int rank, int file) const {

    const char files[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    std::string temp(1, files[file]);
    text.setString(temp);

    // Measure text bound
    sf::FloatRect bounds = text.getLocalBounds();

    // Set origin to center intead of top-left
    text.setOrigin(bounds.left + bounds.width / 2,
                   bounds.top + bounds.height / 2);

    // Find center of square vertically
    float centerY =
        (topLeft.y + rank * squareSizePx) + (squareSizePx + squareSizePx / 2.f);
    float centerX = (topLeft.x + file * squareSizePx) + (squareSizePx / 2.f);

    text.setPosition(centerX, centerY);

    target.draw(text);
}

} // namespace chess::ui
