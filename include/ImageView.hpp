// OurClass.hpp
#pragma once

#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/Texture2D.hpp"

DECLARE_CLASS_CODEGEN(Nya, ImageView, UnityEngine::MonoBehaviour,
  public:
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, UpdateImage, ArrayW<UnityEngine::Texture2D*> frames, ArrayW<float> timings , float ImageWidth, float ImageHeight);
    DECLARE_INSTANCE_METHOD(void, UpdateStaticImage, UnityEngine::Sprite* image);
    DECLARE_INSTANCE_METHOD(void, cleanupTextures);
    
    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);

    DECLARE_INSTANCE_FIELD(float, floatVar);
    DECLARE_INSTANCE_FIELD(int, currentFrame);
    DECLARE_INSTANCE_FIELD(float, frameTime);
    DECLARE_INSTANCE_FIELD(bool, play);

    DECLARE_INSTANCE_FIELD(float, width);
    DECLARE_INSTANCE_FIELD(float, height);


    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, imageView);
    DECLARE_INSTANCE_FIELD(UnityEngine::Texture2D*, texture);
    DECLARE_INSTANCE_FIELD(ArrayW<UnityEngine::Texture2D*>, animationFrames);
    DECLARE_INSTANCE_FIELD(ArrayW<float>, animationTimings);
)