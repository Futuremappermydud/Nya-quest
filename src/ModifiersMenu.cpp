#define RAPIDJSON_HAS_STDSTRING 1
#include "ModifiersMenu.hpp"
#include "Utils/WebUtils.hpp"
#include "ImageView.hpp"
#include "ModConfig.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "System/IO/File.hpp"
#include "Utils/Gif.hpp"
#include "Utils/Utils.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"

using namespace UnityEngine;

DEFINE_TYPE(Nya, ModifiersMenu);


// Function gets url for the current selected category
std::string get_api_path() {
    // Get all config parametes
    std::string API = getModConfig().API.GetValue();
    std::string SFWEndpoint = getModConfig().SFWEndpoint.GetValue();
    
    #ifdef NSFW
        std::string NSFWEndpoint = getModConfig().NSFWEndpoint.GetValue();
        bool NSFWEnabled = getModConfig().NSFWEnabled.GetValue();
    #else
        bool NSFWEnabled = false;
        std::string NSFWEndpoint = "";
    #endif

    if (API == "waifu.pics") {
        std::string url = "https://api.waifu.pics/";
        url += NSFWEnabled? "nsfw/": "sfw/";        

        if (NSFWEnabled) {
            url += NSFWEndpoint;
        } else {
            url += SFWEndpoint;
        };

        return url;
    } else {
        return "";
    }
}



// Disable
void Nya::ModifiersMenu::OnDisable() {
    // getLogger().debug("Disabled");
}

// Enable (runs when the component appears)
void Nya::ModifiersMenu::OnEnable() {
    // getLogger().debug("Enabled");
}

void Nya::ModifiersMenu::ctor() {
    getLogger().debug("Creator runs");

    // APIS: waifu.pics
    this->api_list =  Nya::Utils::vectorToList({"waifu.pics" });

    this->sfw_endpoints = Nya::Utils::vectorToList({ 
        "waifu", "neko", "shinobu", "megumin", "bully", "cuddle", "cry", "hug", "awoo", "kiss", "lick", "pat", "smug", "bonk", "yeet", "blush", "smile", "wave", "highfive", "handhold", "nom", "bite", "glomp", "slap", "kill", "kick", "happy", "wink", "poke", "dance", "cringe" 
    });
    this->nsfw_endpoints = Nya::Utils::vectorToList({
        "waifu",
        "neko",
        "trap",
        "blowjob"
    });

    auto vert = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
    vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);

    NYA = QuestUI::BeatSaberUI::CreateImage(vert->get_transform(), nullptr, Vector2::get_zero(), Vector2(50, 50));
    NYA->set_preserveAspect(true);
    auto ele = NYA->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
    auto view = NYA->get_gameObject()->AddComponent<Nya::ImageView*>();
    ele->set_preferredHeight(50);
    ele->set_preferredWidth(50);

    auto horz = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vert->get_transform());
    horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
    horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
    horz->set_spacing(10);

    this->nyaButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), "Nya", "PlayButton",
        [this]() {
            this->nyaButton->set_interactable(false);

            WebUtils::GetAsync(get_api_path(), 10.0, [&](long code, std::string result){
                switch (code)
                {
                    case 200:
                        rapidjson::Document document;
                        document.Parse(result);
                        if(document.HasParseError() || !document.IsObject())
                            return;
                        std::string url = "";
                        if(document.HasMember("url"))
                        {
                            url = document.FindMember("url")->value.GetString();
                        }
                        getLogger().debug("%s", url.c_str());
                        
                        WebUtils::GetAsync(url, 10.0, [this, url](long code, std::string result){
                            std::vector<uint8_t> bytes(result.begin(), result.end());
                            
                            getLogger().debug("Downloaded Image!");
                            getLogger().debug("%lu", bytes.size());
                            switch (code)
                            {
                                case 200:
                                    if(url.find(".gif") != std::string::npos) {
                                        getLogger().debug("Found a gif");

                                        
                                            QuestUI::MainThreadScheduler::Schedule([this, result]
                                            {
                                                std::string resCopy = result;

                                                // Decode the gif
                                                Gif gif(resCopy);
                                                int parseResult = gif.Parse();
                                                int slurpResult = gif.Slurp();
                                                int width = gif.get_width();
                                                int height = gif.get_height();
                                                int length = gif.get_length();
                                                AllFramesResult result = gif.get_all_frames();
                                                
                                
                                                Nya::ImageView* view = NYA->get_gameObject()->GetComponent<Nya::ImageView*>();
                                                view->UpdateImage(result.frames,result.timings,  (float)width, (float)height);
                                                
                                            });
                                    } else {
                                        getLogger().debug("Static image");
                                        QuestUI::MainThreadScheduler::Schedule([this, bytes]
                                        {
                                            Nya::ImageView* view = NYA->get_gameObject()->GetComponent<Nya::ImageView*>();
                                            UnityEngine::Sprite* sprite = QuestUI::BeatSaberUI::VectorToSprite(bytes);
                                            view->UpdateStaticImage(sprite);
                                        });
                                    }

                                    QuestUI::MainThreadScheduler::Schedule([this]
                                    {
                                        this->nyaButton->set_interactable(true);
                                    });
                            }
                        });
                        break;
                }
            });
        });
   
    // Settings button
    UnityEngine::UI::Button* settingsButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Settings"), "PracticeButton",
    [this]() {
        getLogger().debug("Settings button clicked");
        // Run UI on the main thread
        QuestUI::MainThreadScheduler::Schedule([this]
        {
            std::string API = getModConfig().API.GetValue();
            std::string SFWEndpoint = getModConfig().SFWEndpoint.GetValue();

            // Restore api endpoint state
            this->api_switch->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<StringW>*>(this->api_list));
            this->api_switch->set_selectedIndex(Nya::Utils::findStrIndexInList(this->api_list,API));
            
            // Restore sfw endpoint state
            this->sfw_endpoint->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<StringW>*>(this->sfw_endpoints));
            this->sfw_endpoint->set_selectedIndex(Nya::Utils::findStrIndexInList(this->sfw_endpoints,SFWEndpoint));

            #ifdef NSFW
                bool NSFWEnabled = getModConfig().NSFWEnabled.GetValue();
                StringW NSFWEndpoint = getModConfig().NSFWEndpoint.GetValue();

                // Restore nsfw state
                this->nsfw_endpoint->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<StringW>*>(this->nsfw_endpoints));
                this->nsfw_endpoint->set_selectedIndex(Nya::Utils::findStrIndexInList(this->nsfw_endpoints, NSFWEndpoint));
                this->nsfw_toggle->set_isOn(NSFWEnabled);
            #endif

    
            this->settingsModal->Show(true, true, nullptr);
        });
    });

    {
        this->settingsModal =  QuestUI::BeatSaberUI::CreateModal(get_transform(),  { 65, 65 }, nullptr);

        // Create a text that says "Hello World!" and set the parent to the container.
        UnityEngine::UI::VerticalLayoutGroup* vert = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(this->settingsModal->get_transform());
        vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        vert->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(60.0);


        TMPro::TextMeshProUGUI* title = QuestUI::BeatSaberUI::CreateText(vert->get_transform(), "Settings");
        title->GetComponent<TMPro::TMP_Text*>()->set_alignment(TMPro::TextAlignmentOptions::Center);
        title->GetComponent<TMPro::TMP_Text*>()->set_fontSize(7.0);

        // API Selection (nothing to select for now)
        std::string API = getModConfig().API.GetValue();
        this->api_switch = QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("API"),  "Loading..", {"Loading.."} , [](StringW value){
            getModConfig().API.SetValue(std::string(value));
        });


        // SFW endpoint switch
        std::string SFWEndpoint = getModConfig().SFWEndpoint.GetValue();
        this->sfw_endpoint = QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("SFW endpoint"),  "Loading..", {"Loading.."}, [](StringW value){
            getModConfig().SFWEndpoint.SetValue(std::string(value));
            
        });

        #ifdef NSFW
        // NSFW endpoint selector
        // std::string NSFWEndpoint = getModConfig().NSFWEndpoint.GetValue();
        this->nsfw_endpoint = QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("NSFW endpoint"), "Loading..", {"Loading.."}, [](StringW value){
            getModConfig().NSFWEndpoint.SetValue(std::string(value));
        });

        // NSFW toggle
        bool NSFWEnabled = getModConfig().NSFWEnabled.GetValue();
        this->nsfw_toggle = QuestUI::BeatSaberUI::CreateToggle(vert->get_transform(),  to_utf16("NSFW toggle"), NSFWEnabled,  [](bool isChecked){ 
            getModConfig().NSFWEnabled.SetValue(isChecked);
        });
        #endif

        UnityEngine::UI::HorizontalLayoutGroup* horz = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vert->get_transform());
        horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        horz->set_spacing(10);


        UnityEngine::UI::Button* closeButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Close"), "PracticeButton",
        [this]() {
            this->settingsModal->Hide(true, nullptr);
        });
    }
    
}

void Nya::ModifiersMenu::DidActivate(bool firstActivation)
{
    if(firstActivation)
    {
        WebUtils::GetAsync(get_api_path(), 10.0, [&](long code, std::string result){
            switch (code)
            {
                case 200:
                    rapidjson::Document document;
                    document.Parse(result);
                    if(document.HasParseError() || !document.IsObject())
                        return;
                    std::string url = "";
                    if(document.HasMember("url"))
                    {
                        url = document.FindMember("url")->value.GetString();
                    }
                    getLogger().debug("%s", url.c_str());
                    WebUtils::GetAsync(url, 10.0, [this, url](long code,  std::string result){
                        std::vector<uint8_t> bytes(result.begin(), result.end());     
                        getLogger().debug("Downloaded Image!");
                        getLogger().debug("%lu", bytes.size());

                        switch (code)
                        {
                            case 200:
                                if(url.find(".gif") != std::string::npos) {
                                    getLogger().debug("Found a gif");
                                    QuestUI::MainThreadScheduler::Schedule([this, result]
                                    {
                                        std::string resCopy = result;

                                        // Decode the gif
                                        Gif gif(resCopy);
                                        int parseResult = gif.Parse();
                                        int slurpResult = gif.Slurp();
                                        int width = gif.get_width();
                                        int height = gif.get_height();
                                        int length = gif.get_length();
                                        AllFramesResult result = gif.get_all_frames();
                        
                                        Nya::ImageView* view = NYA->get_gameObject()->GetComponent<Nya::ImageView*>();
                                        if (view == nullptr) {
                                            view = NYA->get_gameObject()->AddComponent<Nya::ImageView*>();
                                        }
                                        view->UpdateImage(result.frames,result.timings,  (float)width, (float)height);
                                    });

                                } else {
                                    QuestUI::MainThreadScheduler::Schedule([this, bytes]
                                    {
                                        Nya::ImageView* view = NYA->get_gameObject()->GetComponent<Nya::ImageView*>();
                                        UnityEngine::Sprite* sprite = QuestUI::BeatSaberUI::VectorToSprite(bytes);
                                        view->UpdateStaticImage(sprite);
                                        this->nyaButton->set_interactable(true);
                                    });
                                }
                        }
                    });
                    break;
            }
        });
    }

    
}

void Nya::ModifiersMenu::dtor(){
  il2cpp_utils::getLogger().debug("Destruct");
}