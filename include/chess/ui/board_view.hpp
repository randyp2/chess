#pragma once // Define header once

// namespace is good practice here to show ownership
#include <SFML/Graphics/Font.hpp>
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
    void draw(sf::RenderTarget &target) const;

  private:
    // -- Helpers to draw board
    void drawRankNotation(sf::RenderTarget &target, sf::Text text, int rank,
                          int file) const;
    void drawFileNotation(sf::RenderTarget &target, sf::Text text, int rank,
                          int file) const;

    sf::Vector2f topLeft;
    float squareSizePx;

    sf::Texture pieceTexture;
    sf::Font font;
};

} // namespace chess::ui
