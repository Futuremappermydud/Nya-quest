// OurClass.cpp
#include "ImageView.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Time.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/SpriteMeshType.hpp"

// Necessary
DEFINE_TYPE(Nya, ImageView);

using namespace UnityEngine;

// Start
void Nya::ImageView::ctor()
{
  floatVar = 1.0f;
  frameTime = 0.0f;

  currentFrame = 0;
  width = 0.0f;
  height = 0.0f;
  ArrayW<float> animationTimings;
  ArrayW<UnityEngine::Texture2D *> animationFrames;
  play = false;

  imageView = this->get_gameObject()->GetComponent<HMUI::ImageView *>();
  // Constructor!
}

// Update
void Nya::ImageView::Update()
{
  if (play)
  {
    int length = animationFrames.Length();
    if (length > 0)
    {
      // Destroy old stuff

      float deltaTime = Time::get_deltaTime();

      bool isFrameNeeded = false;

      // Count frame length
      float frameLength = animationTimings[currentFrame] / 100;
      if (frameLength > 0.0f)
      {
        // Basic delta time based frame switching
        while (frameTime > frameLength)
        {
          currentFrame = (currentFrame + 1) % length;
          isFrameNeeded = true;
          frameTime = frameTime - frameLength;
          frameLength = animationTimings[currentFrame] / 100;
        }
      }

      if (isFrameNeeded)
      {
        Destroy(imageView->get_sprite());
        auto sprite = Sprite::Create(animationFrames.get(currentFrame),
                                     Rect(0.0f, 0.0f, (float)width, (float)height),
                                     Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        imageView->set_sprite(sprite);
      }
      frameTime += deltaTime;
    }
  }
}

// Update
void Nya::ImageView::UpdateImage(ArrayW<UnityEngine::Texture2D *> frames, ArrayW<float> timings, float ImageWidth, float ImageHeight)
{
  // stop gif playback
  play = false;

  // get refs fot cleanup
  auto oldFrames = animationFrames;
  UnityEngine::Sprite *oldSprite = imageView->get_sprite();
  UnityEngine::Texture2D *oldTexture = nullptr;
  if (oldSprite != nullptr)
  {
    oldTexture = oldSprite->get_texture();
  }

  // Update variables
  animationFrames = frames;
  animationTimings = timings;
  currentFrame = 0;

  width = ImageWidth;
  height = ImageHeight;

  // Cleanup
  if (oldTexture != nullptr)
  {
    Destroy(oldTexture);
  }
  if (oldSprite != nullptr)
  {
    Destroy(oldSprite);
  }
  // Clear all previous textures
  if (oldFrames)
  {
    int length = oldFrames.Length();
    // TODO: memory cleanup
    for (int i = 0; i < length; i++)
    {
      auto frame = oldFrames[i];
      if (frame != nullptr)
      {
        Destroy(frame);
      }
    }
  }

  // Start playback
  play = true;
}

// Set normal image
void Nya::ImageView::UpdateStaticImage(UnityEngine::Sprite *image)
{
  // Stop gifs
  play = false;

  // Prepare stuff for clean
  auto oldFrames = animationFrames;
  UnityEngine::Sprite *oldSprite = imageView->get_sprite();
  UnityEngine::Texture2D *oldTexture = nullptr;
  if (oldSprite != nullptr)
  {
    oldTexture = oldSprite->get_texture();
  }

  
  currentFrame = 0;
  
  imageView->set_sprite(image);
  
  
  // Clean
  if (oldTexture != nullptr)
  {
    Destroy(oldTexture);
  }
  if (oldSprite != nullptr)
  {
    Destroy(oldSprite);
  }
  // Clear all previous textures
  if (oldFrames)
  {
    int length = oldFrames.Length();
    // TODO: memory cleanup
    for (int i = 0; i < length; i++)
    {
      auto frame = oldFrames[i];
      if (frame != nullptr)
      {
        Destroy(frame);
      }
    }
  }
}
