#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "../config/DebugConfig.hpp"
#include "../core/Move.hpp"
#include "../core/Position.hpp"
#include "../ui/board_view.hpp"

namespace chess::ui {

struct DragState {
    bool active = false;
    chess::core::PieceOnSquare piece{};
    sf::Vector2f mousePos{};
};

class InputController {
  public:
    /**
     *
     * Hanldes dragging event and updates game logic
     *
     * @params event - raw input signal (mouse)
     *         window - access to pixel coordinates
     *         board - access to board squares
     *         position - update position on mouse release
     */
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window,
                     const chess::ui::BoardView &board,
                     chess::core::Position &Position,
                     const chess::config::DebugConfig &debugger);

    const DragState *dragState() const { return &drag; }

  private:
    DragState drag;
};

} // namespace chess::ui
