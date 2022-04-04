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
    DECLARE_CTOR(ctor);
    DECLARE_SIMPLE_DTOR();

    DECLARE_INSTANCE_FIELD(float, floatVar);
    DECLARE_INSTANCE_FIELD(float, frameTime);

    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, imageView);
    DECLARE_INSTANCE_FIELD(UnityEngine::Texture2D*, texture);
)