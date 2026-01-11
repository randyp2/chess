#pragma once // Define header once

// namespace is good practice here to show ownership
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
namespace chess::ui {

class BoardView {
  public:
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
    sf::Vector2f topLeft;
    float squareSizePx;
};
} // namespace chess::ui
