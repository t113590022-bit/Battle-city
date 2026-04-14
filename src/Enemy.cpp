//
// Created by link9 on 2026/4/8.
//
#include "Enemy.hpp"

#include <cstdlib>
#include <glm/glm.hpp>

Enemy::Enemy(Util::Renderer& root)
    : m_Root(root) {}

void Enemy::Init(float x, float y, EnemyType type) {
    m_X = x;
    m_Y = y;
    m_Type = type;
    m_Alive = true;
    m_Direction = Direction::DOWN;

    // ===== 依種類設定屬性 =====
    switch (m_Type) {
        case EnemyType::NORMAL:
            m_HP = 1;
            m_MoveSpeed = 1.0f;
            m_BulletSpeed = 5.0f;
            break;

        case EnemyType::FAST:
            m_HP = 1;
            m_MoveSpeed = 2.5f;
            m_BulletSpeed = 5.0f;
            break;

        case EnemyType::POWER:
            m_HP = 1;
            m_MoveSpeed = 1.0f;
            m_BulletSpeed = 8.0f;
            break;

        case EnemyType::HEAVY:
            m_HP = 4;
            m_MoveSpeed = 0.5f;
            m_BulletSpeed = 5.0f;
            break;
    }

    m_Enemy = std::make_shared<Character>(GetTankImagePath(m_Direction));
    m_Enemy->SetPosition({m_X, m_Y});
    m_Enemy->SetZIndex(98.0f); // 比地圖高、但不用壓過爆炸
    m_Root.AddChild(m_Enemy);
}

std::string Enemy::GetTankImagePath(Direction dir) const {
    std::string typeFolder;

    switch (m_Type) {
        case EnemyType::NORMAL:
            typeFolder = "normal";
            break;
        case EnemyType::FAST:
            typeFolder = "fast";
            break;
        case EnemyType::POWER:
            typeFolder = "power";
            break;
        case EnemyType::HEAVY:
            typeFolder = "heavy";
            break;
        default:
            typeFolder = "normal";
            break;
    }

    std::string dirName;

    switch (dir) {
        case Direction::UP:
            dirName = "up";
            break;
        case Direction::DOWN:
            dirName = "down";
            break;
        case Direction::LEFT:
            dirName = "left";
            break;
        case Direction::RIGHT:
            dirName = "right";
            break;
        default:
            dirName = "down";
            break;
    }

    return std::string(RESOURCE_DIR) + "/image/enemy/" + typeFolder + "/" + dirName + ".png";
}

bool Enemy::CanMoveTo(float newX, float newY, const Map& map) const {
    const float halfW = 10.0f;
    const float halfH = 10.0f;

    if (newX - halfW < map.GetPlayLeft()) return false;
    if (newX + halfW > map.GetPlayRight()) return false;
    if (newY + halfH > map.GetPlayTop()) return false;
    if (newY - halfH < map.GetPlayBottom()) return false;

    if (map.IsBlockedAtWorld(newX - halfW, newY + halfH)) return false;
    if (map.IsBlockedAtWorld(newX + halfW, newY + halfH)) return false;
    if (map.IsBlockedAtWorld(newX - halfW, newY - halfH)) return false;
    if (map.IsBlockedAtWorld(newX + halfW, newY - halfH)) return false;

    return true;
}

// void Enemy::ChangeDirectionRandom() {
//     int r = std::rand() % 4;
//     m_Direction = static_cast<Direction>(r);
//
//     if (m_Enemy) {
//         m_Enemy->SetImage(GetTankImagePath(m_Direction));
//     }
// }

void Enemy::ChangeDirectionSmart(const Map& map) {
    Direction dirs[4] = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    for (int i = 0; i < 4; ++i) {
        int r = std::rand() % 4;
        std::swap(dirs[i], dirs[r]);
    }

    for (Direction dir : dirs) {
        float testX = m_X;
        float testY = m_Y;

        switch (dir) {
            case Direction::UP:    testY += m_MoveSpeed; break;
            case Direction::DOWN:  testY -= m_MoveSpeed; break;
            case Direction::LEFT:  testX -= m_MoveSpeed; break;
            case Direction::RIGHT: testX += m_MoveSpeed; break;
        }

        if (CanMoveTo(testX, testY, map)) {
            m_Direction = dir;
            if (m_Enemy) {
                m_Enemy->SetImage(GetTankImagePath(m_Direction));
            }
            return;
        }
    }
}

bool Enemy::HasActiveBullet() const {
    return m_Bullet && m_Bullet->IsActive();
}

Bullet::Direction Enemy::ConvertToBulletDirection() const {
    switch (m_Direction) {
        case Direction::UP:
            return Bullet::Direction::UP;
        case Direction::DOWN:
            return Bullet::Direction::DOWN;
        case Direction::LEFT:
            return Bullet::Direction::LEFT;
        case Direction::RIGHT:
            return Bullet::Direction::RIGHT;
    }

    return Bullet::Direction::DOWN;
}

glm::vec2 Enemy::GetBulletSpawnPosition() const {
    float bulletX = m_X;
    float bulletY = m_Y;

    const float muzzleOffset = 20.0f;

    switch (m_Direction) {
        case Direction::UP:
            bulletY += muzzleOffset;
            break;
        case Direction::DOWN:
            bulletY -= muzzleOffset;
            break;
        case Direction::LEFT:
            bulletX -= muzzleOffset;
            break;
        case Direction::RIGHT:
            bulletX += muzzleOffset;
            break;
    }

    return {bulletX, bulletY};
}

void Enemy::TryShoot() {
    if (!m_Alive) return;
    if (m_ShootCooldownFrames != 0) return;
    if (HasActiveBullet()) return;

    // 先用簡單機率開火
    if (std::rand() % 100 < 2) {
        glm::vec2 bulletPos = GetBulletSpawnPosition();

        if (!m_Bullet) {
            m_Bullet = std::make_unique<Bullet>(m_Root);
        }

        m_Bullet->Init(
            bulletPos.x,
            bulletPos.y,
            ConvertToBulletDirection(),
            m_BulletSpeed
        );

        m_ShootCooldownFrames = m_ShootIntervalFrames;
    }
}

void Enemy::UpdateBullet(Map& map) {
    // ===== 1. 更新子彈 =====
    if (m_Bullet && m_Bullet->IsActive()) {

        m_Bullet->Update();

        float x = m_Bullet->GetX();
        float y = m_Bullet->GetY();

        if (map.HitTile(x, y) || m_Bullet->IsOutOfBounds(map)) {
            m_Bullet->Deactivate();

            m_Explosion = std::make_unique<Explosion>(m_Root);
            m_Explosion->Init(x, y);
        }
    }

    // ===== 2. Explosion 永遠要更新（重點） =====
    if (m_Explosion) {
        m_Explosion->Update();

        if (m_Explosion->IsFinished()) {
            m_Explosion->Clear();
            m_Explosion.reset();
        }
    }
}

void Enemy::ClampToMap(const Map& map) {
    const float halfW = 10.0f;
    const float halfH = 10.0f;

    if (m_X < map.GetPlayLeft() + halfW) {
        m_X = map.GetPlayLeft() + halfW;
    }
    if (m_X > map.GetPlayRight() - halfW) {
        m_X = map.GetPlayRight() - halfW;
    }
    if (m_Y > map.GetPlayTop() - halfH) {
        m_Y = map.GetPlayTop() - halfH;
    }
    if (m_Y < map.GetPlayBottom() + halfH) {
        m_Y = map.GetPlayBottom() + halfH;
    }
}

void Enemy::Update(Map& map) {
    if (!m_Enemy) return;

    if (!m_Alive) {
        if (m_Explosion) {
            m_Explosion->Update();

            if (m_Explosion->IsFinished()) {
                m_Explosion->Clear();
                m_Explosion.reset();
            }
        }
        return;
    };

    if (m_TurnCooldownFrames > 0) {
        --m_TurnCooldownFrames;
    }

    float nextX = m_X;
    float nextY = m_Y;

    switch (m_Direction) {
        case Direction::UP:
            nextY += m_MoveSpeed;
            break;
        case Direction::DOWN:
            nextY -= m_MoveSpeed;
            break;
        case Direction::LEFT:
            nextX -= m_MoveSpeed;
            break;
        case Direction::RIGHT:
            nextX += m_MoveSpeed;
            break;
    }

    if (!CanMoveTo(nextX, nextY, map)) {
        if (m_TurnCooldownFrames == 0) {
            ChangeDirectionSmart(map);
            m_TurnCooldownFrames = m_TurnIntervalFrames;
        }
    }
    else {
        m_X = nextX;
        m_Y = nextY;
        m_Enemy->SetPosition({m_X, m_Y});
    }

    if (std::rand() % 100 < 2) {
        ChangeDirectionSmart(map);
    }

    ClampToMap(map);
    m_Enemy->SetPosition({m_X, m_Y});

    if (m_ShootCooldownFrames > 0) {
        --m_ShootCooldownFrames;
    }

    TryShoot();
    UpdateBullet(map);
}

void Enemy::TakeDamage() {
    if (!m_Alive) return;

    --m_HP;

    if (m_HP <= 0) {
        Destroy();
    } else {
        // 之後可以做重坦受傷閃爍
    }
}

bool Enemy::IsRemovable() const {
    return !m_Alive && !m_Explosion;
}

void Enemy::Destroy() {
    if (!m_Alive) return;

    m_Alive = false;

    if (m_Enemy) {
        m_Enemy->SetVisible(false);
    }

    m_Explosion = std::make_unique<Explosion>(m_Root);
    m_Explosion->Init(m_X, m_Y);

    if (m_Bullet && m_Bullet->IsActive()) {
        m_Bullet->Deactivate();
    }
}

void Enemy::Clear() {
    if (m_Bullet) {
        m_Bullet->Deactivate();
        m_Bullet.reset();
    }

    if (m_Enemy) {
        m_Root.RemoveChild(m_Enemy);
        m_Enemy.reset();
    }
}