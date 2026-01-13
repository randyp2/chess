#include "../../include/chess/ui/board_view.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <string>

namespace chess::ui {

/* ========= CONSTRUCTORS =========*/
BoardView::BoardView(sf::Vector2f topLeft, float squareSizePx)
    : topLeft(topLeft), squareSizePx(squareSizePx) {

    if (!pieceTexture.loadFromFile("assets/Pieces.png"))
        throw std::runtime_error("Failed to load piece texture");

    if (!font.loadFromFile("assets/Rubik-Regular.ttf"))
        throw std::runtime_error("Failed to load font");
}

/* ========= UI HELPERS =========*/
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

// Convert square index to {x_pixel, y_pixel}
sf::Vector2f BoardView::squareToPixel(int square) const {
    int rank = 7 - (square / 8);
    int file = 7 - (square % 8);

    float x = topLeft.x + file * squareSizePx;
    float y = topLeft.y + rank * squareSizePx;

    return {x, y};
}

int BoardView::pixelToSquare(sf::Vector2f mouse) const {

    // Convert to board coords
    float curr_x = mouse.x - topLeft.x;
    float curr_y = mouse.y - topLeft.y;

    // Conversion outside of board world
    if (curr_x < 0 || curr_y < 0)
        return -1;

    int file = static_cast<int>(curr_x / squareSizePx);
    int rank = static_cast<int>(curr_y / squareSizePx);

    if (file < 0 || file > 7 || rank < 0 || rank > 7)
        return -1;

    int engineRank = 7 - rank;
    int engineFile = 7 - file;

    return engineRank * 8 + engineFile;
}
// Convert piece to textured rect to draw
sf::IntRect BoardView::pieceToTextureRect(chess::core::Color color,
                                          chess::core::PieceType piece) const {

    constexpr int spriteW = 333; // Image width / 6
    constexpr int spriteH = 334; // Image height / 2

    const int spriteCol = static_cast<int>(piece);
    const int spriteRow = (color == chess::core::Color::White) ? 0 : 1;

    return {spriteCol * spriteW, spriteRow * spriteH, spriteW, spriteH};
}

/* ========= METHOD IMPLEMENTATIONS =========*/
void BoardView::draw(sf::RenderTarget &target,
                     const chess::core::Position &position) const {

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
    // --- Draw chess board
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

    // --- Draw chess pieces
    constexpr float SPRITE_W = 333.f; // Image width / 6
    constexpr float SPRITE_H = 334.f; // Image height / 2

    sf::Sprite sprite;
    sprite.setTexture(pieceTexture);
    sprite.setScale(squareSizePx / SPRITE_W, squareSizePx / SPRITE_H);

    for (const auto &piece : position.getAllPieces()) {
        sprite.setTextureRect(pieceToTextureRect(piece.color, piece.piece));
        sprite.setPosition(squareToPixel(piece.squareIdx));
        target.draw(sprite);
    }
}

} // namespace chess::ui
