#define RAPIDJSON_HAS_STDSTRING 1
#include "ModifiersMenu.hpp"
#include "WebUtils.hpp"
#include "ModConfig.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "System/IO/File.hpp"

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

void Nya::ModifiersMenu::DidActivate(bool firstActivation)
{
    if(firstActivation)
    {
        // APIS: waifu.pics
        std::vector<StringW>  api_list = { "waifu.pics" };
        std::vector<StringW>  sfw_endpoints = { 
            "waifu", "neko", "shinobu", "megumin", "bully", "cuddle", "cry", "hug", "awoo", "kiss", "lick", "pat", "smug", "bonk", "yeet", "blush", "smile", "wave", "highfive", "handhold", "nom", "bite", "glomp", "slap", "kill", "kick", "happy", "wink", "poke", "dance", "cringe" 
            };
        std::vector<StringW>  nsfw_endpoints = {
            "waifu",
            "neko",
            "trap",
            "blowjob"
        };

        auto vert = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);

        NYA = QuestUI::BeatSaberUI::CreateImage(vert->get_transform(), nullptr, Vector2::get_zero(), Vector2(50, 50));
        NYA->set_preserveAspect(true);
        auto ele = NYA->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
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

                            WebUtils::GetAsync(url, 10.0, [this](long code, std::string result){
                                std::vector<uint8_t> bytes(result.begin(), result.end());

                                getLogger().debug("Downloaded Image!");
                                getLogger().debug("%lu", bytes.size());
                                switch (code)
                                {
                                    case 200:
                                        getLogger().debug("Les go!");
                                        QuestUI::MainThreadScheduler::Schedule([this, bytes]
                                        {
                                            Destroy(this->NYA->get_sprite()->get_texture());
                                            Destroy(this->NYA->get_sprite());
                                            this->NYA->set_sprite(QuestUI::BeatSaberUI::VectorToSprite(bytes));
                                            this->nyaButton->set_interactable(true);
                                        });
                                        getLogger().debug("Les go! x2");
                                        //delete arr;

                                }
                            });
                            break;
                    }
                });
            });

        // Settings button
        UnityEngine::UI::Button* settingsButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Settings"), "PracticeButton",
        [this, sfw_endpoints, nsfw_endpoints, api_list]() {
            getLogger().debug("Settings button clicked");
            // Run UI on the main thread
            QuestUI::MainThreadScheduler::Schedule([this, sfw_endpoints, api_list, nsfw_endpoints]
            {
                HMUI::ModalView* modal =  QuestUI::BeatSaberUI::CreateModal(get_transform(),  { 65, 65 }, nullptr);

                // Create a text that says "Hello World!" and set the parent to the container.
                UnityEngine::UI::VerticalLayoutGroup* vert = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(modal->get_transform());
                vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                vert->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(60.0);


                TMPro::TextMeshProUGUI* title = QuestUI::BeatSaberUI::CreateText(vert->get_transform(), "Settings");
                title->GetComponent<TMPro::TMP_Text*>()->set_alignment(TMPro::TextAlignmentOptions::Center);
                title->GetComponent<TMPro::TMP_Text*>()->set_fontSize(7.0);

                // API Selection (nothing to select for now)
                std::string API = getModConfig().API.GetValue();
                QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("API"), API, api_list , [](StringW value){
                    getModConfig().API.SetValue(std::string(value));
                });

                // SFW endpoint switch
                std::string SFWEndpoint = getModConfig().SFWEndpoint.GetValue();
                QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("SFW endpoint"), SFWEndpoint, sfw_endpoints, [](StringW value){
                    getModConfig().SFWEndpoint.SetValue(std::string(value));
                 
                });

                #ifdef NSFW
                // NSFW endpoint selector
                std::string NSFWEndpoint = getModConfig().NSFWEndpoint.GetValue();
                QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("NSFW endpoint"), NSFWEndpoint, nsfw_endpoints, [](StringW value){
                    getModConfig().NSFWEndpoint.SetValue(std::string(value));
                });

                // NSFW toggle
                bool NSFWEnabled = getModConfig().NSFWEnabled.GetValue();
                QuestUI::BeatSaberUI::CreateToggle(vert->get_transform(),  to_utf16("NSFW toggle"), NSFWEnabled,  [](bool isChecked){ 
                    getModConfig().NSFWEnabled.SetValue(isChecked);
                });
                #endif

                UnityEngine::UI::HorizontalLayoutGroup* horz = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vert->get_transform());
                horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                horz->set_spacing(10);


                UnityEngine::UI::Button* closeButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Close"), "PracticeButton",
                [this, modal]() {
                    modal->Hide(true, nullptr);
                });

                modal->Show(true, true, nullptr);
            });
        });
    }

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
                WebUtils::GetAsyncBytes(url, 10.0, [this](long code, std::shared_ptr<std::vector<uint8_t>> result){
                    getLogger().debug("Downloaded Image!");
                    getLogger().debug("%lu", result.get()->size());
                    switch (code)
                    {
                        case 200:
                            getLogger().debug("Les go!");
                            QuestUI::MainThreadScheduler::Schedule([this, result]
                            {
                                this->NYA->set_sprite(QuestUI::BeatSaberUI::VectorToSprite(*result.get()));
                                this->nyaButton->set_interactable(true);
                            });
                            getLogger().debug("Les go! x2");
                    }
                });
                break;
        }
    });
}
