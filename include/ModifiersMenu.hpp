#pragma once

#include "main.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/UI/Button.hpp"


DECLARE_CLASS_CODEGEN(Nya, ModifiersMenu, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, NYA);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, nyaButton);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstactivation);
)
