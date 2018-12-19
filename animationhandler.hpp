#ifndef ANIMATION_HANDLER_HPP
#define ANIMATION_HANDLER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
  unsigned int startFrame;
  unsigned int endFrame;
  float duration;

  Animation(unsigned int startFrame, unsigned int endFrame, float duration) {
    this->startFrame = startFrame;
    this->endFrame = endFrame;
    this->duration = duration;
  }

  unsigned int getLength() { return endFrame - startFrame + 1; }

};

class AnimationHandler {
private:
  std::vector<Animation> animations;
  float timeSinceLoopStart;
  int currentAnim;

public:
  void addAnim(Animation& anim);
  void update(const float dt);
  void changeAnim(unsigned int animNum);

  // Current section of the texture to be displayed
  sf::IntRect bounds;
  // Pixel dimensions of each frame
  sf::IntRect frameSize;

  AnimationHandler() {
    this->timeSinceLoopStart = 0.0f;
    this->currentAnim = -1;
  }

  AnimationHandler(const sf::IntRect& frameSize) {
    this->frameSize = frameSize;
    this->timeSinceLoopStart = 0.0f;
    this->currentAnim = -1;
  }
};

#endif /* ANIMATION_HANDLER_HPP */
