//
// Created by link9 on 2026/5/10.
//

#ifndef STAGEDATA_HPP
#define STAGEDATA_HPP

#include <vector>
#include <string>

#include "Enemy.hpp"

struct StageConfig {
    std::vector<std::string> mapData;
    std::vector<Enemy::EnemyType> enemies;
};

class StageData {
public:
    static StageConfig GetStage(int stageNo);
    static int GetStageCount();
};

#endif //STAGEDATA_HPP