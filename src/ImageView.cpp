// OurClass.cpp
#include "ImageView.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Time.hpp"
#include "HMUI/ImageView.hpp"

// Necessary
DEFINE_TYPE(Nya, ImageView);

using namespace UnityEngine;

// Start 
void Nya::ImageView::ctor() {
  floatVar = 1.0f;
  frameTime = 0.0f;
  

//   texture = UnityEngine::Texture2D::get_whiteTexture();


//   Sprite::Create(texture,  )
//   imageView = this->get_gameObject()->GetComponent<HMUI::ImageView*>()->get_sprite()->set_texture(texture);
  // Constructor!
}

// Update
void Nya::ImageView::Update() {
  // Update method! YAY
//    float deltaTime = Time::get_deltaTime();
//    while (/* condition */)
//    {
//        /* code */
//    }
   
//    frameTime += deltaTime;
    
//   il2cpp_utils::getLogger().debug("Update ran in %fs", Time::get_deltaTime());

//   il2cpp_utils::getLogger().debug("Update ran in %fs", this->frameTime );
  
}