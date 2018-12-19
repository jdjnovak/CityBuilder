#include <SFML/Graphics.hpp>
#include <vector>

#include "animationhandler.hpp"

void AnimationHandler::update(const float dt) {
  if (currentAnim >= this->animations.size() || currentAnim < 0) return;

  float duration = this->animations[currentAnim].duration;

  /* Check if the animation has progressed to a new frame. If so,
   * change to the next frame */
  if (int((timeSinceLoopStart + dt) / duration) > int(timeSinceLoopStart / duration)) {
    int frame = int((timeSinceLoopStart + dt) / duration);
    frame %= this->animations[currentAnim].getLength();
    sf::IntRect rect = this->frameSize;
    rect.left = rect.width * frame;
    rect.top = rect.height * this->currentAnim;
    this->bounds = rect;
  }

  this->timeSinceLoopStart += dt;

  if (this->timeSinceLoopStart > duration * this->animations[currentAnim].getLength()) {
    this->timeSinceLoopStart = 0.0f;
  }

  return;
}

void AnimationHandler::addAnim(Animation& anim) {
  this->animations.push_back(anim);
  return;
}

void AnimationHandler::changeAnim(unsigned int animID) {
  if (this->currentAnim == animID || animID >= this->animations.size() || animID < 0) return;

  this->currentAnim = animID;
  sf::IntRect rect = this->frameSize;
  rect.top = rect.height * animID;
  this->bounds = rect;
  this->timeSinceLoopStart = 0.0;

  return;
}
