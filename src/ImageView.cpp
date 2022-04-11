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
      } else {
        // Skip the frame with 0 ms
        currentFrame = (currentFrame + 1) % length;
        isFrameNeeded = true;
        frameLength = animationTimings[currentFrame] / 100;
      }

      if (isFrameNeeded)
      {
        auto old_sprite = imageView->get_sprite();
        if (old_sprite != nullptr) {
          
          Destroy(old_sprite);
        }
        if (animationFrames.Length() > currentFrame) {
          auto frame = animationFrames.get(currentFrame);
          if (frame !=nullptr) {
            auto sprite = Sprite::Create(frame,
                                     Rect(0.0f, 0.0f, (float)width, (float)height),
                                     Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
            imageView->set_sprite(sprite);
          }
        }
      }
      frameTime += deltaTime;
    }
  }
}

// Update
void Nya::ImageView::UpdateImage(ArrayW<UnityEngine::Texture2D *> frames, ArrayW<float> timings, float ImageWidth, float ImageHeight)
{
  // Check for nulls
  if (!frames) {
    il2cpp_utils::getLogger().warning("Frames are null, skipping");
    return;
  }
  if (!timings) {
    il2cpp_utils::getLogger().warning("Timings are null, skipping");
    return;
  }

  // CHECK if the gif has zero timings to prevent infinite loop
  float total_length = 0.0f;
  for (int i = 0; i < timings.Length(); i++)
  {
      total_length += timings[i];
  }
  if (total_length == 0) {
    il2cpp_utils::getLogger().warning("Gif has zero timings for some reason, skipping...");
    return;
  }

  // Validate width and height
  if (!(ImageWidth > 0 && ImageHeight > 0)) {
    il2cpp_utils::getLogger().warning("Timings are null, skipping");
    return;
  }
  
  // Clean things
  cleanupTextures();
  
  // stop gif playback
  il2cpp_utils::getLogger().debug("Stopping playing");
  play = false;
  currentFrame = 0;
  
 
  // Update variables
  animationFrames = frames;
  animationTimings = timings;
  currentFrame = 0;

  width = ImageWidth;
  height = ImageHeight;

  // Start playback
  play = true;
}

// Set normal image
void Nya::ImageView::UpdateStaticImage(UnityEngine::Sprite *image)
{
  play = false;
  cleanupTextures();
  currentFrame = 0;
  imageView->set_sprite(image);
}


void Nya::ImageView::cleanupTextures(){
  auto oldFrames = animationFrames;
  UnityEngine::Sprite *oldSprite = imageView->get_sprite();
  UnityEngine::Texture2D *oldTexture = nullptr;
  if (oldSprite != nullptr)
  {
    oldTexture = oldSprite->get_texture();
  }

  // Cleanup
  if (oldTexture != nullptr)
  {
    oldTexture = nullptr;
  }
  if (oldSprite != nullptr)
  {
    UnityEngine::Object::Destroy(oldSprite);
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
        UnityEngine::Object::Destroy(frame);
      }
    }
  }
}
