//
// Created by link9 on 2026/4/20.
//

#ifndef STAGECLEARSCREEN_HPP
#define STAGECLEARSCREEN_HPP

#include <memory>
#include <vector>
#include <string>

#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Character.hpp"
#include "Enemy.hpp"
#include "TextObject.hpp"

class StageClearScreen {
public:
    struct ScoreRow {
        Enemy::EnemyType type;
        int totalKills = 0;
        int countedKills = 0;
        int scorePerKill = 0;
        int countedScore = 0;
    };

public:
    explicit StageClearScreen(Util::Renderer& root);

    void Init(
        int stageNo,
        int playerScore,
        int stagePowerUpScore,
        int normalKills,
        int fastKills,
        int powerKills,
        int heavyKills
    );

    void Update();
    void Clear();

    bool IsFinished() const { return m_Finished; }

private:
    void BuildRows(
        int normalKills,
        int fastKills,
        int powerKills,
        int heavyKills
    );

    void CreateUi();
    void RefreshTexts();

    std::string GetFontPath() const;
    std::string GetTankImagePath(Enemy::EnemyType type) const;

private:
    Util::Renderer& m_Root;

    std::vector<ScoreRow> m_Rows;

    int m_StageNo = 1;
    int m_FinalScore = 0;
    int m_DisplayTotal = 0;

    int m_CurrentRow = 0;
    int m_FrameCounter = 0;
    int m_CountIntervalFrames = 8;   // 每幾幀加一台，可調
    bool m_Finished = false;

    int m_PlayerScore = 0;
    int m_StagePowerUpScore = 0;

    std::shared_ptr<Character> m_BlackBg;
    std::vector<std::shared_ptr<Character>> m_ArrowIcons;
    // 文字
    std::shared_ptr<TextObject> m_TitleText;
    std::shared_ptr<TextObject> m_StageText;
    std::shared_ptr<TextObject> m_PlayerText;
    std::shared_ptr<TextObject> m_PlayerScoreText;
    std::shared_ptr<TextObject> m_TotalLabelText;
    std::shared_ptr<TextObject> m_TotalValueText;

    std::vector<std::shared_ptr<TextObject>> m_PtsTexts;
    std::vector<std::shared_ptr<TextObject>> m_KillTexts;
    std::vector<std::shared_ptr<TextObject>> m_RowScoreTexts;

    // 坦克圖
    std::vector<std::shared_ptr<Character>> m_TankIcons;
};

#endif //STAGECLEARSCREEN_HPP