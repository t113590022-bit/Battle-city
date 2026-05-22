//
// Created by link9 on 2026/5/5.
//

#ifndef TILEHITINFO_HPP
#define TILEHITINFO_HPP

#include <glm/glm.hpp>

enum class HitDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct TileHitInfo {
    glm::vec2 hitPoint;
    HitDirection direction = HitDirection::UP;
    int power = 1;
};

struct TileHitResult {
    bool bulletStopped = false;
    bool spawnExplosion = false;
    bool tileDestroyed = false;
};

#endif //TILEHITINFO_HPP