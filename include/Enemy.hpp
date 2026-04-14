//
// Created by link9 on 2026/4/8.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <memory>

#include "Map.hpp"
#include "Bullet.hpp"
#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Explosion.hpp"

class Enemy {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class EnemyType {
        NORMAL,
        FAST,
        POWER,
        HEAVY
    };

    explicit Enemy(Util::Renderer& root);

    void Init(float x, float y, EnemyType type );
    void Update(Map& map);
    void Clear();

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

    bool IsAlive() const { return m_Alive; }
    void Destroy();

    bool HasActiveBullet() const;
    Bullet* GetBullet() const { return m_Bullet.get(); }
    void TakeDamage(); // FOR 重坦扛四發
    bool IsRemovable() const;

    EnemyType GetType() const { return m_Type; }
    int GetHP() const { return m_HP; }

private:
    std::string GetTankImagePath(Direction dir) const;
    bool CanMoveTo(float newX, float newY, const Map& map) const;
    void ChangeDirectionSmart(const Map& map);

    void ClampToMap(const Map& map);

    void TryShoot();
    void UpdateBullet(Map& map);

    Bullet::Direction ConvertToBulletDirection() const;
    glm::vec2 GetBulletSpawnPosition() const;

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_Enemy;
    std::unique_ptr<Bullet> m_Bullet;

    float m_X = 300.0f;
    float m_Y = -400.0f;
    float m_MoveSpeed = 2.0f;

    Direction m_Direction = Direction::DOWN;
    bool m_Alive = true;

    int m_ShootCooldownFrames = 0;
    int m_ShootIntervalFrames = 90;

    int m_TurnCooldownFrames = 0;
    int m_TurnIntervalFrames = 12;

    EnemyType m_Type = EnemyType::NORMAL;

    int m_HP = 1;
    float m_BulletSpeed = 5.0f;

    std::unique_ptr<Explosion> m_Explosion;
};
#endif //ENEMY_HPP