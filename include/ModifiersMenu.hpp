#pragma once

#include "main.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "HMUI/ModalView.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Button.hpp"


DECLARE_CLASS_CODEGEN(Nya, ModifiersMenu, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, NYA);
    // DECLARE_INSTANCE_FIELD(bool, );
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, nyaButton);


    

    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);

    DECLARE_INSTANCE_METHOD(void, OnDisable);
    DECLARE_INSTANCE_METHOD(void, OnEnable);

    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);

    // Settings buttons and modal
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, settingsModal);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, api_switch);
    DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, sfw_endpoint);
    #ifdef NSFW
        DECLARE_INSTANCE_FIELD(HMUI::SimpleTextDropdown*, nsfw_endpoint);
        DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, nsfw_toggle);
    #endif

    DECLARE_INSTANCE_FIELD(List<StringW>*, sfw_endpoints);
    DECLARE_INSTANCE_FIELD(List<StringW>*, nsfw_endpoints);
    DECLARE_INSTANCE_FIELD(List<StringW>*, api_list);
)
