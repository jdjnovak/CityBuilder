#include "game.hpp"
#include "gamestatestart.hpp"

int main() {
  Game game;
  game.pushState(new GameStateStart(&game));
  game.gameLoop();
  return 0;
}
