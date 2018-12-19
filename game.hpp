#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "texturemanager.hpp"
#include "tile.hpp"

class GameState;

class Game {
private:
  void loadTextures();
  void loadTiles();
  
public:
  const static int tileSize = 8;
  std::stack<GameState*> states;
  sf::RenderWindow window;
  TextureManager texmgr;
  sf::Sprite background;

  std::map<std::string, Tile> tileAtlas;

  void pushState(GameState* state);
  void popState();
  void changeState(GameState* state);
  GameState* peekState();

  void gameLoop();

  Game();
  ~Game();
};

#endif /* GAME_HPP */
