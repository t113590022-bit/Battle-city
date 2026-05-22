//
// Created by link9 on 2026/5/22.
//

#include "StageHud.hpp"

StageHud::StageHud(Util::Renderer& root)
    : m_Root(root) {}

void StageHud::Init(
    int totalEnemies,
    int spawnedEnemies,
    int playerLives,
    int stageNo
) {
    Clear();

    m_TotalEnemies = totalEnemies;
    m_LastSpawnedEnemies = -1;
    m_LastPlayerLives = -1;
    m_LastStageNo = -1;

    CreateEnemyIcons(totalEnemies);
    CreatePlayerLives(playerLives);
    CreateStageNumber(stageNo);

    Update(totalEnemies, spawnedEnemies, playerLives, stageNo);
}

void StageHud::Update(
    int totalEnemies,
    int spawnedEnemies,
    int playerLives,
    int stageNo
) {
    if (totalEnemies != m_TotalEnemies) {
        CreateEnemyIcons(totalEnemies);
        m_TotalEnemies = totalEnemies;
        m_LastSpawnedEnemies = -1;
    }

    if (spawnedEnemies != m_LastSpawnedEnemies) {
        RefreshEnemyIcons(spawnedEnemies);
        m_LastSpawnedEnemies = spawnedEnemies;
    }

    if (playerLives != m_LastPlayerLives) {
        RefreshPlayerLives(playerLives);
        m_LastPlayerLives = playerLives;
    }

    if (stageNo != m_LastStageNo) {
        RefreshStageNumber(stageNo);
        m_LastStageNo = stageNo;
    }
}

void StageHud::CreateEnemyIcons(int totalEnemies) {
    for (auto& icon : m_EnemyIcons) {
        if (icon) {
            m_Root.RemoveChild(icon);
        }
    }

    m_EnemyIcons.clear();

    const float leftX = 236.0f;
    const float rightX = 260.0f;
    const float startY = 180.0f;
    const float rowGap = 18.0f;

    for (int i = 0; i < totalEnemies; ++i) {
        int row = i / 2;
        int col = i % 2;

        float x = (col == 0) ? leftX : rightX;
        float y = startY - row * rowGap;

        auto icon = std::make_shared<Character>(GetEnemyIconPath());
        icon->SetPosition({x, y});
        icon->SetZIndex(80.0f);
        icon->SetVisible(true);

        m_Root.AddChild(icon);
        m_EnemyIcons.push_back(icon);
    }
}

void StageHud::RefreshEnemyIcons(int spawnedEnemies) {
    int remaining = m_TotalEnemies - spawnedEnemies;

    if (remaining < 0) {
        remaining = 0;
    }

    if (remaining > m_TotalEnemies) {
        remaining = m_TotalEnemies;
    }

    for (int i = 0; i < static_cast<int>(m_EnemyIcons.size()); ++i) {
        if (!m_EnemyIcons[i]) continue;

        // vector 排序是：
        // 左上、右上、下一排左、下一排右 ...
        // 所以 i >= remaining 的會從右下、左下開始消失
        m_EnemyIcons[i]->SetVisible(i < remaining);
    }
}

void StageHud::CreatePlayerLives(int playerLives) {
    m_IpIcon = std::make_shared<Character>(GetIpIconPath());
    m_IpIcon->SetPosition({248.0f, -70.0f});
    m_IpIcon->SetZIndex(80.0f);
    m_Root.AddChild(m_IpIcon);

    m_PlayerIcon = std::make_shared<Character>(GetPlayerIconPath());
    m_PlayerIcon->SetPosition({235.0f, -105.0f});
    m_PlayerIcon->SetZIndex(80.0f);
    m_Root.AddChild(m_PlayerIcon);

    m_PlayerLivesText = std::make_shared<TextObject>(
        GetFontPath(),
        20,
        std::to_string(playerLives),
        Util::Color(0, 0, 0)
    );

    m_PlayerLivesText->SetPosition({265.0f, -108.0f});
    m_PlayerLivesText->SetZIndex(80.0f);
    m_Root.AddChild(m_PlayerLivesText);
}

void StageHud::RefreshPlayerLives(int playerLives) {
    if (m_PlayerLivesText) {
        m_PlayerLivesText->SetText(std::to_string(playerLives));
    }
}

void StageHud::CreateStageNumber(int stageNo) {
    m_FlagIcon = std::make_shared<Character>(GetFlagIconPath());
    m_FlagIcon->SetPosition({250.0f, -160.0f});
    m_FlagIcon->SetZIndex(80.0f);
    m_Root.AddChild(m_FlagIcon);

    m_StageText = std::make_shared<TextObject>(
        GetFontPath(),
        20,
        std::to_string(stageNo),
        Util::Color(0, 0, 0)
    );

    m_StageText->SetPosition({252.0f, -198.0f});
    m_StageText->SetZIndex(80.0f);
    m_Root.AddChild(m_StageText);
}

void StageHud::RefreshStageNumber(int stageNo) {
    if (m_StageText) {
        m_StageText->SetText(std::to_string(stageNo));
    }
}

std::string StageHud::GetEnemyIconPath() const {
    return std::string(RESOURCE_DIR) + "/image/ui/hud_enemy.png";
}

std::string StageHud::GetPlayerIconPath() const {
    return std::string(RESOURCE_DIR) + "/image/ui/hud_player.png";
}

std::string StageHud::GetFlagIconPath() const {
    return std::string(RESOURCE_DIR) + "/image/ui/hud_flag.png";
}

std::string StageHud::GetIpIconPath() const {
    return std::string(RESOURCE_DIR) + "/image/ui/hud_ip.png";
}

std::string StageHud::GetFontPath() const {
    return std::string(RESOURCE_DIR) + "/font/PressStart2P.ttf";
}

void StageHud::Clear() {
    for (auto& icon : m_EnemyIcons) {
        if (icon) {
            m_Root.RemoveChild(icon);
        }
    }
    m_EnemyIcons.clear();

    if (m_IpIcon) {
        m_Root.RemoveChild(m_IpIcon);
        m_IpIcon.reset();
    }

    if (m_PlayerIcon) {
        m_Root.RemoveChild(m_PlayerIcon);
        m_PlayerIcon.reset();
    }

    if (m_PlayerLivesText) {
        m_Root.RemoveChild(m_PlayerLivesText);
        m_PlayerLivesText.reset();
    }

    if (m_FlagIcon) {
        m_Root.RemoveChild(m_FlagIcon);
        m_FlagIcon.reset();
    }

    if (m_StageText) {
        m_Root.RemoveChild(m_StageText);
        m_StageText.reset();
    }
}