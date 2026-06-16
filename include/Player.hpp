//
// Created by link9 on 2026/3/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>      // shared_ptr
#include <string>      // std::string
#include <vector>
#include "Map.hpp"
#include "Explosion.hpp"
#include "Collision.hpp"

#include <glm/glm.hpp> // glm::vec2

#include "Util/Renderer.hpp"
#include "Character.hpp"

class Player {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    explicit Player(Util::Renderer& root);

    void Init(float x, float y);
    void Update(const Map& map, const std::vector<Rect>& blockingRects);
    void Clear();

    Rect GetCollisionRect() const;
    Rect GetCollisionRectAt(float x, float y) const;

    bool IsAlive() const;
    bool IsExplosionFinished() const;
    void Kill();

    glm::vec2 GetPosition() const;
    Direction GetDirection() const;

    // 玩家強度
    void Upgrade();
    int GetUpgradeLevel() const;
    void SetUpgradeLevel(int level);

    float GetBulletSpeed() const;
    int GetBulletPower() const;
    bool CanDoubleShot() const;

    // 無敵
    void SetInvincible(bool invincible);
    bool IsInvincible() const;

    // 出生碰撞
    void DisableCollisionUntilLeaveSpawnArea(const Rect& spawnArea);
    void UpdateSpawnCollisionGrace();

    bool IsCollisionEnabled() const;

private:
    std::string GetTankImagePath(Direction dir, int frame) const;
    void UpdateAnimation(bool isMoving);
    void ClampToMap(const Map& map);
    bool CanMoveTo(float newX, float newY, const Map& map, const std::vector<Rect>& blockingRects) const;

    float SnapToGrid(float value) const;
    bool TryAlignToGrid(float& value) const;
    void ApplyGridAssist(float& nextX, float& nextY, Direction dir, const Map& map, const std::vector<Rect>& blockingRects) const;

    bool IsTankRectBlockedByMap(float newX, float newY, const Map& map, float halfW = 13.0f, float halfH = 13.0f) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Player;

    float m_PlayerX = 0.0f;
    float m_PlayerY = 0.0f;
    int m_MoveSpeed = 1;

    Direction m_Direction = Direction::UP;

    int m_AnimFrame = 0;
    int m_AnimCounter = 0;
    int m_AnimInterval = 8;

    bool m_IsAlive = true;
    std::unique_ptr<Explosion> m_Explosion;

    int m_UpgradeLevel = 0;

    bool m_IsInvincible = false;

    bool m_CollisionEnabled = true;
    bool m_WaitingLeaveSpawnArea = false;
    Rect m_SpawnAreaRect{};
};
#endif //PLAYER_HPP