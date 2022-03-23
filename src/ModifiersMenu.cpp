#define RAPIDJSON_HAS_STDSTRING 1
#include "ModifiersMenu.hpp"
#include "WebUtils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "System/IO/File.hpp"

using namespace UnityEngine;

DEFINE_TYPE(Nya, ModifiersMenu);


void Nya::ModifiersMenu::DidActivate(bool firstActivation)
{
    if(firstActivation)
    {
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
                WebUtils::GetAsync("https://api.waifu.pics/sfw/neko", 10.0, [&](long code, std::string result){
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
                                        auto arr = il2cpp_utils::vectorToArray(bytes);
                                        QuestUI::MainThreadScheduler::Schedule([this, arr]
                                        {
                                            Destroy(this->NYA->get_sprite()->get_texture());
                                            Destroy(this->NYA->get_sprite());
                                            this->NYA->set_sprite(QuestUI::BeatSaberUI::ArrayToSprite(arr));
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

            UnityEngine::UI::Button* settingsButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Settings"), "PracticeButton",
            [this]() {

            });
    }
    WebUtils::GetAsync("https://api.waifu.pics/sfw/neko", 10.0, [&](long code, std::string result){
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
                            auto arr = il2cpp_utils::vectorToArray(*result.get());
                            QuestUI::MainThreadScheduler::Schedule([this, arr]
                            {
                                this->NYA->set_sprite(QuestUI::BeatSaberUI::ArrayToSprite(arr));
                                this->nyaButton->set_interactable(true);
                            });
                            getLogger().debug("Les go! x2");
                    }
                });
                break;
        }
    });
}
