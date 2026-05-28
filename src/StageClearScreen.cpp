//
// Created by link9 on 2026/4/20.
//

#include "StageClearScreen.hpp"

StageClearScreen::StageClearScreen(Util::Renderer& root)
    : m_Root(root) {}

void StageClearScreen::Init(
    int stageNo,
    int playerScore,
    int stagePowerUpScore,
    int normalKills,
    int fastKills,
    int powerKills,
    int heavyKills
) {
    Clear();

    m_StageNo = stageNo;
    m_DisplayTotal = 0;
    m_CurrentRow = 0;
    m_FrameCounter = 0;
    m_Finished = false;

    m_PlayerScore = playerScore;
    m_StagePowerUpScore = stagePowerUpScore;

    BuildRows(normalKills, fastKills, powerKills, heavyKills);

    m_FinalScore = 0;
    for (const auto& row : m_Rows) {
        m_FinalScore += row.totalKills * row.scorePerKill;
    }

    CreateUi();
    RefreshTexts();
}

void StageClearScreen::BuildRows(
    int normalKills,
    int fastKills,
    int powerKills,
    int heavyKills
) {
    m_Rows.clear();

    ScoreRow normal;
    normal.type = Enemy::EnemyType::NORMAL;
    normal.totalKills = normalKills;
    normal.countedKills = 0;
    normal.countedScore = 0;
    normal.scorePerKill = 100;
    m_Rows.push_back(normal);

    ScoreRow fast;
    fast.type = Enemy::EnemyType::FAST;
    fast.totalKills = fastKills;
    fast.countedKills = 0;
    fast.countedScore = 0;
    fast.scorePerKill = 200;
    m_Rows.push_back(fast);

    ScoreRow power;
    power.type = Enemy::EnemyType::POWER;
    power.totalKills = powerKills;
    power.countedKills = 0;
    power.countedScore = 0;
    power.scorePerKill = 300;
    m_Rows.push_back(power);

    ScoreRow heavy;
    heavy.type = Enemy::EnemyType::HEAVY;
    heavy.totalKills = heavyKills;
    heavy.countedKills = 0;
    heavy.countedScore = 0;
    heavy.scorePerKill = 400;
    m_Rows.push_back(heavy);
}

void StageClearScreen::CreateUi() {
    const std::string font = GetFontPath();

    m_BlackBg = std::make_shared<Character>(
        std::string(RESOURCE_DIR) + "/image/ui/black_bg.png"
    );

    m_BlackBg->SetPosition({0.0f, 0.0f});
    m_BlackBg->SetZIndex(95.0f);

    m_Root.AddChild(m_BlackBg);

    m_TitleText = std::make_shared<TextObject>(
        font, 28, "STAGE CLEAR", Util::Color(255, 200, 50)
    );
    m_TitleText->SetPosition({0.0f, 165.0f});
    m_TitleText->SetZIndex(100.0f);
    m_Root.AddChild(m_TitleText);

    m_StageText = std::make_shared<TextObject>(
        font, 22, "STAGE " + std::to_string(m_StageNo), Util::Color(255, 255, 255)
    );
    m_StageText->SetPosition({0.0f, 135.0f});
    m_StageText->SetZIndex(100.0f);
    m_Root.AddChild(m_StageText);

    m_PlayerText = std::make_shared<TextObject>(
        font, 22, "I-PLAYER", Util::Color(255, 120, 80)
    );
    m_PlayerText->SetPosition({-85.0f, 95.0f});
    m_PlayerText->SetZIndex(100.0f);
    m_Root.AddChild(m_PlayerText);

    // 最終分數
    m_PlayerScoreText = std::make_shared<TextObject>(
        font, 22, std::to_string(m_PlayerScore), Util::Color(255, 255, 255)
    );
    m_PlayerScoreText->SetPosition({-20.0f, 65.0f});
    m_PlayerScoreText->SetZIndex(100.0f);
    m_Root.AddChild(m_PlayerScoreText);

    m_TotalLabelText = std::make_shared<TextObject>(
        font, 22, "TOTAL", Util::Color(255, 255, 255)
    );
    m_TotalLabelText->SetPosition({-85.0f, -170.0f});
    m_TotalLabelText->SetZIndex(100.0f);
    m_Root.AddChild(m_TotalLabelText);

    m_TotalValueText = std::make_shared<TextObject>(
        font, 22, "0", Util::Color(255, 255, 255)
    );
    m_TotalValueText->SetPosition({55.0f, -170.0f});
    m_TotalValueText->SetZIndex(100.0f);
    m_Root.AddChild(m_TotalValueText);

    m_PtsTexts.clear();
    m_KillTexts.clear();
    m_RowScoreTexts.clear();
    m_TankIcons.clear();

    const float startY = 15.0f;
    const float gapY = 42.0f;

    for (int i = 0; i < 4; ++i) {
        float y = startY - i * gapY;

        auto ptsText = std::make_shared<TextObject>(
            font, 20,
            std::to_string(m_Rows[i].scorePerKill) + " PTS",
            Util::Color(255, 255, 255)
        );
        ptsText->SetPosition({-95.0f, y});
        ptsText->SetZIndex(100.0f);
        m_Root.AddChild(ptsText);
        m_PtsTexts.push_back(ptsText);

        auto killText = std::make_shared<TextObject>(
            font, 20, "0", Util::Color(255, 255, 255)
        );
        killText->SetPosition({45.0f, y});
        killText->SetZIndex(100.0f);
        m_Root.AddChild(killText);
        m_KillTexts.push_back(killText);

        auto rowScoreText = std::make_shared<TextObject>(
            font, 20, "0", Util::Color(255, 255, 255)
        );
        rowScoreText->SetPosition({-230.0f, y});
        rowScoreText->SetZIndex(100.0f);
        // 如果你覺得跟 killText 太近，可以之後微調
        // 這邊先保留，但暫時不 AddChild，避免重疊太亂
        m_Root.AddChild(rowScoreText);
        m_RowScoreTexts.push_back(rowScoreText);

        auto arrow = std::make_shared<Character>(
        std::string(RESOURCE_DIR) + "/image/ui/arrow.png"
        );
        arrow->SetPosition({75.0f, y});
        arrow->SetZIndex(100.0f);
        m_Root.AddChild(arrow);
        m_ArrowIcons.push_back(arrow);

        auto icon = std::make_shared<Character>(GetTankImagePath(m_Rows[i].type));
        icon->SetPosition({115.0f, y});
        icon->SetZIndex(100.0f);
        m_Root.AddChild(icon);
        m_TankIcons.push_back(icon);
    }
}

void StageClearScreen::RefreshTexts() {
    for (size_t i = 0; i < m_Rows.size(); ++i) {
        m_KillTexts[i]->SetText(std::to_string(m_Rows[i].countedKills));
        m_RowScoreTexts[i]->SetText(
            std::to_string(m_Rows[i].countedScore)
        );
    }

    m_TotalValueText->SetText(std::to_string(m_DisplayTotal));
}

void StageClearScreen::Update() {
    if (m_Finished) return;

    if (m_CurrentRow >= static_cast<int>(m_Rows.size())) {
        m_Finished = true;
        return;
    }

    ++m_FrameCounter;

    if (m_FrameCounter < m_CountIntervalFrames) return;

    m_FrameCounter = 0;

    auto& row = m_Rows[m_CurrentRow];

    if (row.countedKills < row.totalKills) {
        row.countedKills++;
        row.countedScore += row.scorePerKill;
        m_DisplayTotal += 1;
        RefreshTexts();
    } else {
        m_CurrentRow++;
    }
}

void StageClearScreen::Clear() {
    if (m_TitleText) {
        m_Root.RemoveChild(m_TitleText);
        m_TitleText.reset();
    }

    if (m_StageText) {
        m_Root.RemoveChild(m_StageText);
        m_StageText.reset();
    }

    if (m_PlayerText) {
        m_Root.RemoveChild(m_PlayerText);
        m_PlayerText.reset();
    }

    if (m_PlayerScoreText) {
        m_Root.RemoveChild(m_PlayerScoreText);
        m_PlayerScoreText.reset();
    }

    if (m_TotalLabelText) {
        m_Root.RemoveChild(m_TotalLabelText);
        m_TotalLabelText.reset();
    }

    if (m_TotalValueText) {
        m_Root.RemoveChild(m_TotalValueText);
        m_TotalValueText.reset();
    }

    if (m_BlackBg) {
        m_Root.RemoveChild(m_BlackBg);
        m_BlackBg.reset();
    }

    for (auto& text : m_PtsTexts) {
        if (text) m_Root.RemoveChild(text);
    }
    m_PtsTexts.clear();

    for (auto& text : m_KillTexts) {
        if (text) m_Root.RemoveChild(text);
    }
    m_KillTexts.clear();

    for (auto& text : m_RowScoreTexts) {
        if (text) m_Root.RemoveChild(text);
    }
    m_RowScoreTexts.clear();

    for (auto& arrow : m_ArrowIcons) {
        if (arrow) m_Root.RemoveChild(arrow);
    }
    m_ArrowIcons.clear();

    for (auto& icon : m_TankIcons) {
        if (icon) m_Root.RemoveChild(icon);
    }
    m_TankIcons.clear();

    m_Rows.clear();
}

std::string StageClearScreen::GetFontPath() const {
    return std::string(RESOURCE_DIR) + "/font/PressStart2P.ttf";
}

std::string StageClearScreen::GetTankImagePath(Enemy::EnemyType type) const {
    switch (type) {
        case Enemy::EnemyType::NORMAL:
            return std::string(RESOURCE_DIR) + "/image/enemy/normal/down.png";
        case Enemy::EnemyType::FAST:
            return std::string(RESOURCE_DIR) + "/image/enemy/fast/down.png";
        case Enemy::EnemyType::POWER:
            return std::string(RESOURCE_DIR) + "/image/enemy/power/down.png";
        case Enemy::EnemyType::HEAVY:
            return std::string(RESOURCE_DIR) + "/image/enemy/heavy/down.png";
    }

    return std::string(RESOURCE_DIR) + "/image/enemy/normal/down.png";
}