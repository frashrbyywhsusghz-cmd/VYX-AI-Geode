#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <deque>
#include <string>

using namespace geode::prelude;

struct VYXClickRecord {
    int frame = 0;
    int gap = -1;
    bool isPlayer1 = true;
};

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

        log::info("VYX AI {} loaded", "0.3.0");
        return true;
    }

    void onVYXAI(CCObject*) {
        FLAlertLayer::create(
            "VYX AI",
            "Live gameplay tools are active:\n"
            "• Physics-frame counter\n"
            "• Player 1 / Player 2 jump-input tracker\n"
            "• Input frame numbers and gaps between clicks\n"
            "• Recent input history\n\n"
            "Accurate click-tolerance analysis and macro replay "
            "are not included in this build.",
            "OK"
        )->show();
    }
};

class $modify(VYXPlayLayer, PlayLayer) {
    struct Fields {
        int frameCount = 0;
        int p1Clicks = 0;
        int p2Clicks = 0;
        int lastP1Frame = -1;
        int lastP2Frame = -1;
        int lastInputFrame = -1;
        int lastInputGap = -1;
        bool lastInputPlayer1 = true;
        std::deque<VYXClickRecord> clickHistory;

        CCLayerColor* framePanel = nullptr;
        CCLabelBMFont* titleLabel = nullptr;
        CCLabelBMFont* frameLabel = nullptr;
        CCLabelBMFont* clickLabel = nullptr;
        CCLabelBMFont* lastLabel = nullptr;
        CCLabelBMFont* historyLabel = nullptr;
    };

    bool init(GJGameLevel* level, bool dontCreateObjects) {
        if (!PlayLayer::init(level, dontCreateObjects)) {
            return false;
        }

        auto fields = m_fields.self();
        fields->frameCount = 0;
        fields->p1Clicks = 0;
        fields->p2Clicks = 0;
        fields->lastP1Frame = -1;
        fields->lastP2Frame = -1;
        fields->lastInputFrame = -1;
        fields->lastInputGap = -1;
        fields->lastInputPlayer1 = true;
        fields->clickHistory.clear();
        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        auto fields = m_fields.self();
        fields->frameCount = 0;
        fields->p1Clicks = 0;
        fields->p2Clicks = 0;
        fields->lastP1Frame = -1;
        fields->lastP2Frame = -1;
        fields->lastInputFrame = -1;
        fields->lastInputGap = -1;
        fields->lastInputPlayer1 = true;
        fields->clickHistory.clear();
        refreshOverlay();
    }

    void onTrackedPhysicsFrame() {
        auto fields = m_fields.self();
        ++fields->frameCount;

        if (fields->frameCount % 2 == 0) {
            refreshOverlay();
        }
    }

    void recordJumpInput(bool isPlayer1) {
        if (!Mod::get()->getSettingValue<bool>("click-tracker")) {
            return;
        }

        auto fields = m_fields.self();

        // handleButton is called before the next PlayerObject physics update.
        // Assign the input to the physics frame it is about to enter.
        const int inputFrame = fields->frameCount + 1;
        int& previousFrame = isPlayer1 ? fields->lastP1Frame : fields->lastP2Frame;
        const int gap = previousFrame < 0 ? -1 : inputFrame - previousFrame;
        previousFrame = inputFrame;

        if (isPlayer1) {
            ++fields->p1Clicks;
        }
        else {
            ++fields->p2Clicks;
        }

        fields->lastInputFrame = inputFrame;
        fields->lastInputGap = gap;
        fields->lastInputPlayer1 = isPlayer1;
        fields->clickHistory.push_front({ inputFrame, gap, isPlayer1 });

        while (fields->clickHistory.size() > 4) {
            fields->clickHistory.pop_back();
        }

        refreshOverlay();
    }

    void ensureOverlay() {
        auto fields = m_fields.self();
        if (fields->framePanel) {
            return;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        fields->framePanel = CCLayerColor::create(
            ccc4(12, 9, 22, 205),
            246.f,
            124.f
        );
        if (!fields->framePanel) {
            return;
        }

        fields->framePanel->setAnchorPoint({ 0.f, 1.f });
        fields->framePanel->setPosition({ 10.f, winSize.height - 10.f });
        this->addChild(fields->framePanel, 9999);

        auto makeLabel = [fields](const char* value, float y, float scale, ccColor3B color) {
            auto label = CCLabelBMFont::create(value, "bigFont.fnt");
            if (label) {
                label->setAnchorPoint({ 0.f, 1.f });
                label->setPosition({ 9.f, y });
                label->setScale(scale);
                label->setColor(color);
                fields->framePanel->addChild(label);
            }
            return label;
        };

        fields->titleLabel = makeLabel("VYX INPUT TRACKER", 112.f, 0.28f, ccc3(207, 163, 255));
        fields->frameLabel = makeLabel("FRAME 000000", 94.f, 0.30f, ccc3(255, 255, 255));
        fields->clickLabel = makeLabel("P1 0000   P2 0000", 76.f, 0.27f, ccc3(190, 220, 255));
        fields->lastLabel = makeLabel("LAST --", 58.f, 0.25f, ccc3(255, 220, 150));
        fields->historyLabel = makeLabel("RECENT INPUTS", 39.f, 0.23f, ccc3(215, 215, 225));
    }

    void refreshOverlay() {
        const bool showFrame = Mod::get()->getSettingValue<bool>("frame-counter");
        const bool showClicks = Mod::get()->getSettingValue<bool>("click-tracker");

        ensureOverlay();
        auto fields = m_fields.self();
        if (!fields->framePanel) {
            return;
        }

        fields->framePanel->setVisible(showFrame || showClicks);

        if (fields->frameLabel) {
            fields->frameLabel->setVisible(showFrame);
            fields->frameLabel->setString(
                fmt::format("FRAME {:06d}", fields->frameCount).c_str()
            );
        }

        if (fields->clickLabel) {
            fields->clickLabel->setVisible(showClicks);
            fields->clickLabel->setString(
                fmt::format("P1 {:04d}   P2 {:04d}", fields->p1Clicks, fields->p2Clicks).c_str()
            );
        }

        if (fields->lastLabel) {
            fields->lastLabel->setVisible(showClicks);
            if (fields->lastInputFrame < 0) {
                fields->lastLabel->setString("LAST --");
            }
            else {
                const std::string gapText = fields->lastInputGap < 0
                    ? "--"
                    : fmt::format("{}F", fields->lastInputGap);
                fields->lastLabel->setString(
                    fmt::format(
                        "LAST {} F{:06d} GAP {}",
                        fields->lastInputPlayer1 ? "P1" : "P2",
                        fields->lastInputFrame,
                        gapText
                    ).c_str()
                );
            }
        }

        if (fields->historyLabel) {
            fields->historyLabel->setVisible(showClicks);
            std::string history = "RECENT INPUTS";
            for (const auto& click : fields->clickHistory) {
                const std::string gapText = click.gap < 0
                    ? "--"
                    : fmt::format("+{}F", click.gap);
                history += fmt::format(
                    "\n{} F{:06d} {}",
                    click.isPlayer1 ? "P1" : "P2",
                    click.frame,
                    gapText
                );
            }
            fields->historyLabel->setString(history.c_str());
        }
    }
};

class $modify(VYXPlayerObject, PlayerObject) {
    void update(float dt) {
        PlayerObject::update(dt);

        auto playLayer = PlayLayer::get();
        if (
            playLayer &&
            !playLayer->m_isPaused &&
            this == playLayer->m_player1
        ) {
            static_cast<VYXPlayLayer*>(playLayer)->onTrackedPhysicsFrame();
        }
    }
};

class $modify(VYXBaseGameLayer, GJBaseGameLayer) {
    void handleButton(bool down, int button, bool isPlayer1) {
        if (down && button == 1) {
            auto playLayer = PlayLayer::get();
            if (
                playLayer &&
                this == static_cast<GJBaseGameLayer*>(playLayer) &&
                !playLayer->m_isPaused
            ) {
                static_cast<VYXPlayLayer*>(playLayer)->recordJumpInput(isPlayer1);
            }
        }

        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    }
};
