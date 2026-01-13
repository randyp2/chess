#pragma once // Define header once

#include "../core/Position.hpp"
// #include "input_controller.hpp" // ERROR: CREATES CIRCULAR DEPENDENCIES

// Forward reference
//  - There exists a type called chess::ui::DragState and will be defined
//  somewhere else
namespace chess::ui {
struct DragState;
}
// namespace is good practice here to show ownership
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
namespace chess::ui {

class BoardView {
  public:
    static constexpr float LABEL_OFFSET_X = 10;
    /**
     * Parameterized constructor
     *
     * @params topLeft - pixel initial position of top-left corner of board
     *         squareSize - size of one square in pixels
     */
    BoardView(sf::Vector2f topLeft, float squareSizePx);

    // Draw to board with any valid randerTarget
    void draw(sf::RenderTarget &target, const chess::core::Position &position,
              const chess::ui::DragState *drag) const;

    int pixelToSquare(sf::Vector2f mouse) const;

  private:
    /* =============== HELPERS =============== */
    // -- UI helpers
    void drawRankNotation(sf::RenderTarget &target, sf::Text text, int rank,
                          int file) const;
    void drawFileNotation(sf::RenderTarget &target, sf::Text text, int rank,
                          int file) const;

    // Convert square index to {x_pixel, y_pixel}
    sf::Vector2f squareToPixel(int square) const;

    // Convert piece to textured rect to draw
    sf::IntRect pieceToTextureRect(chess::core::Color color,
                                   chess::core::PieceType piece) const;

    sf::Vector2f topLeft;
    float squareSizePx;

    sf::Texture pieceTexture;
    sf::Font font;
};

} // namespace chess::ui
