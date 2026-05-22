//
// Created by link9 on 2026/5/22.
//

#ifndef STAGEHUD_HPP
#define STAGEHUD_HPP

#include <memory>
#include <string>
#include <vector>

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "TextObject.hpp"

class StageHud {
public:
    explicit StageHud(Util::Renderer& root);

    void Init(
        int totalEnemies,
        int spawnedEnemies,
        int playerLives,
        int stageNo
    );

    void Update(
        int totalEnemies,
        int spawnedEnemies,
        int playerLives,
        int stageNo
    );

    void Clear();

private:
    void CreateEnemyIcons(int totalEnemies);
    void RefreshEnemyIcons(int spawnedEnemies);
    void CreatePlayerLives(int playerLives);
    void RefreshPlayerLives(int playerLives);
    void CreateStageNumber(int stageNo);
    void RefreshStageNumber(int stageNo);

    std::string GetEnemyIconPath() const;
    std::string GetPlayerIconPath() const;
    std::string GetFlagIconPath() const;
    std::string GetIpIconPath() const;
    std::string GetFontPath() const;

private:
    Util::Renderer& m_Root;

    std::vector<std::shared_ptr<Character>> m_EnemyIcons;

    std::shared_ptr<Character> m_IpIcon;
    std::shared_ptr<Character> m_PlayerIcon;
    std::shared_ptr<TextObject> m_PlayerLivesText;

    std::shared_ptr<Character> m_FlagIcon;
    std::shared_ptr<TextObject> m_StageText;

    int m_TotalEnemies = 0;
    int m_LastSpawnedEnemies = -1;
    int m_LastPlayerLives = -1;
    int m_LastStageNo = -1;
};

#endif //STAGEHUD_HPP