//
// Created by link9 on 2026/4/8.
//
#include "Enemy.hpp"

#include <cstdlib>
#include <glm/glm.hpp>

Enemy::Enemy(Util::Renderer& root)
    : m_Root(root) {}

void Enemy::Init(float x, float y, EnemyType type, bool isPowerUpCarrier) {
    m_X = x;
    m_Y = y;
    m_Type = type;
    m_IsPowerUpCarrier = isPowerUpCarrier;

    m_BlinkCounter = 0;
    m_BlinkAlt = false;

    m_Alive = true;
    m_Direction = Direction::DOWN;
    m_DeathReason = DeathReason::None;

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
    m_Enemy->SetZIndex(40.0f);
    m_Root.AddChild(m_Enemy);
}

void Enemy::UpdateVisualBlink() {
    if (!m_Enemy || !m_Alive) return;

    // 1. 受傷黃閃優先
    if (m_DamageFlashFrames > 0) {
        m_Enemy->SetImage(GetTankImagePath(m_Direction));

        --m_DamageFlashFrames;

        if (m_DamageFlashFrames == 0) {
            m_BlinkCounter = 0;
            m_BlinkAlt = false;
            m_Enemy->SetImage(GetTankImagePath(m_Direction));
        }

        return;
    }

    bool isHeavyBlink = (m_Type == EnemyType::HEAVY && m_HP > 1);
    bool isPowerUpBlink = m_IsPowerUpCarrier;

    if (!isHeavyBlink && !isPowerUpBlink) {
        return;
    }

    ++m_BlinkCounter;

    int interval = isHeavyBlink ? m_HeavyBlinkInterval : m_PowerUpBlinkInterval;

    // 如果這台同時是道具敵人 + 重坦，這裡用道具敵人的速度也可以
    if (isPowerUpBlink) {
        interval = m_PowerUpBlinkInterval;
    }

    if (m_BlinkCounter < interval) {
        return;
    }

    m_BlinkCounter = 0;
    m_BlinkAlt = !m_BlinkAlt;

    m_Enemy->SetImage(GetTankImagePath(m_Direction));
}

std::string Enemy::GetTankImagePath(Direction dir) const {
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
    }

    std::string typeName;

    switch (m_Type) {
        case EnemyType::NORMAL:
            typeName = "normal";
            break;
        case EnemyType::FAST:
            typeName = "fast";
            break;
        case EnemyType::POWER:
            typeName = "power";
            break;
        case EnemyType::HEAVY:
            typeName = "heavy";
            break;
    }

    std::string suffix = "";

    // 受傷黃閃優先
    if (m_Type == EnemyType::HEAVY && m_DamageFlashFrames > 0) {
        suffix = "_yellow";
    }
    // 道具敵人：紅色 <-> 原本圖片 閃爍
    else if (m_IsPowerUpCarrier) {
        suffix = m_BlinkAlt ? "_red" : "";
    }
    // 重坦：平常綠色 <-> 原本圖片；最後一滴血維持原本鐵灰
    else if (m_Type == EnemyType::HEAVY) {
        if (m_HP > 1) {
            suffix = m_BlinkAlt ? "_green" : "";
        } else if (m_HP == 1) {
            suffix = "";
        }
    }

    return std::string(RESOURCE_DIR) +
           "/image/enemy/" +
           typeName +
           "/" +
           dirName +
           suffix +
           ".png";
}

Rect Enemy::GetCollisionRect() const {
    return GetCollisionRectAt(m_X, m_Y);
}

Rect Enemy::GetCollisionRectAt(float x, float y) const {
    const float halfW = 12.0f;
    const float halfH = 12.0f;

    return MakeRect(x, y, halfW, halfH);
}

bool Enemy::CanMoveTo(float newX, float newY, const Map& map, const std::vector<Rect>& blockingRects) const {
    const float halfW = 12.0f;
    const float halfH = 12.0f;

    // 1. 邊界
    if (newX - halfW < map.GetPlayLeft()) return false;
    if (newX + halfW > map.GetPlayRight()) return false;
    if (newY + halfH > map.GetPlayTop()) return false;
    if (newY - halfH < map.GetPlayBottom()) return false;

    // 2. 地圖與半磚
    const glm::vec2 checkPoints[] = {
        // 四個角
        {newX - halfW, newY + halfH},
        {newX + halfW, newY + halfH},
        {newX - halfW, newY - halfH},
        {newX + halfW, newY - halfH},

        // 四邊中心
        {newX,         newY + halfH},
        {newX,         newY - halfH},
        {newX - halfW, newY},
        {newX + halfW, newY},

        // 上下邊 1/4、3/4
        {newX - halfW * 0.5f, newY + halfH},
        {newX + halfW * 0.5f, newY + halfH},
        {newX - halfW * 0.5f, newY - halfH},
        {newX + halfW * 0.5f, newY - halfH},

        // 左右邊 1/4、3/4
        {newX - halfW, newY + halfH * 0.5f},
        {newX - halfW, newY - halfH * 0.5f},
        {newX + halfW, newY + halfH * 0.5f},
        {newX + halfW, newY - halfH * 0.5f},
    };

    for (const auto& point : checkPoints) {
        if (map.IsBlockedAtWorld(point.x, point.y)) {
            return false;
        }
    }

    // 3. 其他坦克
    Rect nextRect = GetCollisionRectAt(newX, newY);

    for (const Rect& blocker : blockingRects) {
        if (IsColliding(nextRect, blocker)) {
            return false;
        }
    }

    return true;
}

Enemy::EnemyIntent Enemy::ChooseIntent(bool hasPlayer) const {
    int r = std::rand() % 100;

    if (!hasPlayer) {
        if (r < 85) return EnemyIntent::AttackBase;
        return EnemyIntent::Wander;
    }

    switch (m_Type) {
        case EnemyType::NORMAL:
            if (r < 70) return EnemyIntent::AttackBase;
            if (r < 85) return EnemyIntent::AttackPlayer;
            return EnemyIntent::Wander;

        case EnemyType::FAST:
            if (r < 80) return EnemyIntent::AttackBase;
            if (r < 90) return EnemyIntent::AttackPlayer;
            return EnemyIntent::Wander;

        case EnemyType::POWER:
            if (r < 50) return EnemyIntent::AttackBase;
            if (r < 85) return EnemyIntent::AttackPlayer;
            return EnemyIntent::Wander;

        case EnemyType::HEAVY:
            if (r < 40) return EnemyIntent::AttackBase;
            if (r < 80) return EnemyIntent::AttackPlayer;
            return EnemyIntent::Wander;
    }

    return EnemyIntent::AttackBase;
}

glm::vec2 Enemy::GetBaseTargetPosition() const {
    return {0.0f, -192.0f};
}

float Enemy::GetDistanceSq(float x, float y, glm::vec2 target) const {
    float dx = x - target.x;
    float dy = y - target.y;

    return dx * dx + dy * dy;
}

void Enemy::ChangeDirectionSmart(const Map& map, const std::vector<Rect>& blockingRects, bool hasPlayer, glm::vec2 playerPos) {
    EnemyIntent intent = ChooseIntent(hasPlayer);

    Direction dirs[4] = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };

    // 先打亂，避免分數相同時永遠選同一方向
    for (int i = 0; i < 4; ++i) {
        int r = std::rand() % 4;
        std::swap(dirs[i], dirs[r]);
    }

    // Wander 就維持原本隨機可走方向
    if (intent == EnemyIntent::Wander) {
        for (Direction dir : dirs) {
            float testX = m_X;
            float testY = m_Y;

            switch (dir) {
                case Direction::UP:
                    testY += m_MoveSpeed;
                    break;
                case Direction::DOWN:
                    testY -= m_MoveSpeed;
                    break;
                case Direction::LEFT:
                    testX -= m_MoveSpeed;
                    break;
                case Direction::RIGHT:
                    testX += m_MoveSpeed;
                    break;
            }

            if (CanMoveTo(testX, testY, map, blockingRects)) {
                m_Direction = dir;

                if (m_Enemy) {
                    m_Enemy->SetImage(GetTankImagePath(m_Direction));
                }

                return;
            }
        }
    }

    glm::vec2 target = GetBaseTargetPosition();

    if (intent == EnemyIntent::AttackPlayer && hasPlayer) {
        target = playerPos;
    }

    bool found = false;
    Direction bestDir = m_Direction;
    float bestScore = 999999999.0f;

    for (Direction dir : dirs) {
        float testX = m_X;
        float testY = m_Y;

        switch (dir) {
            case Direction::UP:
                testY += m_MoveSpeed;
                break;

            case Direction::DOWN:
                testY -= m_MoveSpeed;
                break;

            case Direction::LEFT:
                testX -= m_MoveSpeed;
                break;

            case Direction::RIGHT:
                testX += m_MoveSpeed;
                break;
        }

        if (!CanMoveTo(testX, testY, map, blockingRects)) {
            continue;
        }

        float score = GetDistanceSq(testX, testY, target);

        if (score < bestScore) {
            bestScore = score;
            bestDir = dir;
            found = true;
        }
    }

    if (found) {
        m_Direction = bestDir;

        if (m_Enemy) {
            m_Enemy->SetImage(GetTankImagePath(m_Direction));
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

    // 炮口射出
    const float muzzleOffset = 9.0f;

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
            m_BulletSpeed,
            1
        );

        m_ShootCooldownFrames = m_ShootIntervalFrames;
    }
}

HitDirection Enemy::ConvertBulletDirection(Bullet::Direction dir) const {
    switch (dir) {
        case Bullet::Direction::UP:
            return HitDirection::UP;

        case Bullet::Direction::DOWN:
            return HitDirection::DOWN;

        case Bullet::Direction::LEFT:
            return HitDirection::LEFT;

        case Bullet::Direction::RIGHT:
            return HitDirection::RIGHT;
    }

    return HitDirection::UP;
}

void Enemy::UpdateBullet(Map& map) {
    // ===== 1. 更新子彈 =====
    if (m_Bullet && m_Bullet->IsActive()) {

        m_Bullet->Update();

        float x = m_Bullet->GetX();
        float y = m_Bullet->GetY();

        bool hit = false;
        bool shouldSpawnExplosion = false;

        // ===== 1-1. 檢查是否打到地圖物件：Brick / Steel =====
        TileObject* tile = map.GetTileObjectAtWorld(x, y);

        if (tile && tile->BlocksBullet()) {
            TileHitInfo info;
            info.hitPoint = {x, y};
            info.direction = ConvertBulletDirection(m_Bullet->GetDirection());
            info.power = 1;

            TileHitResult result = tile->OnBulletHit(info);

            if (result.tileDestroyed) {
                map.RemoveTileAtWorld(x, y);
            }

            if (result.bulletStopped) {
                hit = true;
                shouldSpawnExplosion = result.spawnExplosion;
            }
        }

        // ===== 1-2. 檢查是否超出邊界 =====
        if (!hit && m_Bullet->IsOutOfBounds(map)) {
            hit = true;
            shouldSpawnExplosion = true;
        }

        // ===== 1-3. 統一處理命中結果 =====
        if (hit) {
            m_Bullet->Deactivate();

            if (shouldSpawnExplosion) {
                if (m_Explosion) {
                    m_Explosion->Clear();
                    m_Explosion.reset();
                }

                m_Explosion = std::make_unique<Explosion>(m_Root);
                m_Explosion->Init(x, y);
            }
        }
    }

    // ===== 2. Explosion 永遠要更新 =====
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

void Enemy::Update(Map& map, const std::vector<Rect>& blockingRects, bool hasPlayer, glm::vec2 playerPos, bool frozen) {
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

    UpdateVisualBlink();

    if (frozen) {
        UpdateBullet(map);  // 已射出的子彈照常飛
        return;
    }

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

    if (!CanMoveTo(nextX, nextY, map, blockingRects)) {
        if (m_TurnCooldownFrames == 0) {
            ChangeDirectionSmart(map, blockingRects, hasPlayer, playerPos);
            m_TurnCooldownFrames = m_TurnIntervalFrames;
        }
    }
    else {
        m_X = nextX;
        m_Y = nextY;
        m_Enemy->SetPosition({m_X, m_Y});
    }

    if (std::rand() % 100 < 2) {
        ChangeDirectionSmart(map, blockingRects, hasPlayer, playerPos);
    }

    ClampToMap(map);
    m_Enemy->SetPosition({m_X, m_Y});

    if (m_ShootCooldownFrames > 0) {
        --m_ShootCooldownFrames;
    }

    TryShoot();
    UpdateBullet(map);

    UpdateSpawnCollisionGrace();
}

void Enemy::TakeDamage() {
    if (!m_Alive) return;

    --m_HP;

    if (m_HP <= 0) {
        Destroy(DeathReason::Bullet);
        return;
    }

    // 重坦受到傷害時，短暫變黃色
    if (m_Type == EnemyType::HEAVY) {
        m_DamageFlashFrames = m_DamageFlashDurationFrames;
        m_BlinkCounter = 0;
        m_BlinkAlt = false;
    }


    if (m_Enemy) {
        m_Enemy->SetImage(GetTankImagePath(m_Direction));
    }
}

bool Enemy::IsRemovable() const {
    return !m_Alive && !m_Explosion;
}

void Enemy::Destroy(DeathReason reason) {
    if (!m_Alive) return;

    m_DeathReason = reason;
    m_Alive = false;

    if (m_Enemy) {
        m_Enemy->SetVisible(false);
    }

    if (m_Explosion) {
        m_Explosion->Clear();
        m_Explosion.reset();
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

    if (m_Explosion) {
        m_Explosion->Clear();
        m_Explosion.reset();
    }

    if (m_Enemy) {
        m_Root.RemoveChild(m_Enemy);
        m_Enemy.reset();
    }
}

void Enemy::DisableCollisionUntilLeaveSpawnArea(const Rect& spawnArea) {
    m_CollisionEnabled = false;
    m_WaitingLeaveSpawnArea = true;
    m_SpawnAreaRect = spawnArea;
}

void Enemy::UpdateSpawnCollisionGrace() {
    if (!m_WaitingLeaveSpawnArea) {
        return;
    }

    if (!IsAlive()) {
        return;
    }

    Rect enemyRect = GetCollisionRect();

    if (!IsColliding(enemyRect, m_SpawnAreaRect)) {
        m_CollisionEnabled = true;
        m_WaitingLeaveSpawnArea = false;
    }
}

bool Enemy::IsCollisionEnabled() const {
    return m_CollisionEnabled;
}