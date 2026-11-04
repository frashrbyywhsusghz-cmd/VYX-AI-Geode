#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(VYXMainMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");
        if (!menu) {
            log::warn("VYX AI: bottom menu was not found");
            return true;
        }

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        if (!sprite) {
            sprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        }
        if (!sprite) {
            log::warn("VYX AI: could not create menu button sprite");
            return true;
        }

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(VYXMainMenuLayer::onVYXAI)
        );
        button->setID("vyx-ai-entry"_spr);
        menu->addChild(button);
        menu->updateLayout();

        log::info("VYX AI {} loaded", "0.1.0");
        return true;
    }

    void onVYXAI(CCObject*) {
        FLAlertLayer::create(
            "VYX AI",
            "Project foundation loaded.\n\n"
            "Planned: AI Reader, local level analysis, replay, "
            "Level Creator, and Decoration Creator.\n\n"
            "These features are not active in this starter build yet.",
            "OK"
        )->show();
    }
};
