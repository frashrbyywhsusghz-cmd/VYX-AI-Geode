#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

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

        log::info("VYX AI {} loaded", "0.2.0");
        return true;
    }

    void onVYXAI(CCObject*) {
        FLAlertLayer::create(
            "VYX AI",
            "VYX AI project foundation loaded.\n\n"
            "New: Frame Window Counter for gameplay. "
            "Enable or disable it in the VYX AI mod settings.\n\n"
            "Planned: AI Reader, local level analysis, replay, "
            "Level Creator, and Decoration Creator.",
            "OK"
        )->show();
    }
};

class $modify(VYXPlayLayer, PlayLayer) {
    struct Fields {
        int frameCount = 0;
        CCLabelBMFont* frameLabel = nullptr;
        CCLayerColor* framePanel = nullptr;
        bool counterEnabled = true;
    };

    void update(float dt) {
        PlayLayer::update(dt);

        auto fields = m_fields.self();
        fields->counterEnabled = Mod::get()->getSettingValue<bool>("frame-counter");

        if (!fields->counterEnabled) {
            if (fields->framePanel) {
                fields->framePanel->setVisible(false);
            }
            return;
        }

        if (!fields->framePanel) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            fields->framePanel = CCLayerColor::create(
                ccc4(13, 8, 24, 205),
                176.f,
                27.f
            );
            fields->framePanel->setAnchorPoint({1.f, 1.f});
            fields->framePanel->setPosition({winSize.width - 12.f, winSize.height - 12.f});
            fields->framePanel->setZOrder(9999);
            this->addChild(fields->framePanel);

            fields->frameLabel = CCLabelBMFont::create("FRAME 000000  |  WINDOW +/-2F", "bigFont.fnt");
            if (fields->frameLabel) {
                fields->frameLabel->setAnchorPoint({0.5f, 0.5f});
                fields->frameLabel->setPosition({88.f, 13.5f});
                fields->frameLabel->setScale(0.34f);
                fields->frameLabel->setColor(ccc3(207, 163, 255));
                fields->framePanel->addChild(fields->frameLabel);
            }
        }

        if (fields->framePanel) {
            fields->framePanel->setVisible(true);
        }

        ++fields->frameCount;

        // Refresh the text periodically to avoid unnecessary string allocations.
        if (fields->frameLabel && fields->frameCount % 2 == 0) {
            fields->frameLabel->setString(
                fmt::format("FRAME {:06d}  |  WINDOW +/-2F", fields->frameCount).c_str()
            );
        }
    }
};
