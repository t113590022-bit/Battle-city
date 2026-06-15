//
// Created by link9 on 2026/3/26.
//

#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Player::Player(Util::Renderer& root)
    : m_Root(root) {}

void Player::Init(float x, float y) {
    m_PlayerX = x;
    m_PlayerY = y;
    m_Direction = Direction::UP;
    m_AnimFrame = 0;
    m_AnimCounter = 0;

    m_Player = std::make_shared<Character>(GetTankImagePath(m_Direction, m_AnimFrame));
    m_Player->SetPosition({m_PlayerX, m_PlayerY});
    m_Player->SetZIndex(40.0f);
    m_Root.AddChild(m_Player);
}

std::string Player::GetTankImagePath(Direction dir, int frame) const {
    int visualLevel = m_UpgradeLevel + 1;

    if (visualLevel < 1) {
        visualLevel = 1;
    }

    if (visualLevel > 4) {
        visualLevel = 4;
    }

    std::string suffix = "_lv" + std::to_string(visualLevel) + ".png";

    switch (dir) {
        case Direction::UP:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-front" + suffix
                : std::string(RESOURCE_DIR) + "/image/player/1p-front2" + suffix;

        case Direction::DOWN:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-back" + suffix
                : std::string(RESOURCE_DIR) + "/image/player/1p-back2" + suffix;

        case Direction::LEFT:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-left" + suffix
                : std::string(RESOURCE_DIR) + "/image/player/1p-left2" + suffix;

        case Direction::RIGHT:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-right" + suffix
                : std::string(RESOURCE_DIR) + "/image/player/1p-right2" + suffix;
    }

    return std::string(RESOURCE_DIR) + "/image/player/1p-front" + suffix;
}

glm::vec2 Player::GetPosition() const {
    return {m_PlayerX, m_PlayerY};
}

Player::Direction Player::GetDirection() const {
    return m_Direction;
}

Rect Player::GetCollisionRect() const {
    return GetCollisionRectAt(m_PlayerX, m_PlayerY);
}

Rect Player::GetCollisionRectAt(float x, float y) const {
    const float halfW = 12.0f;
    const float halfH = 12.0f;

    return MakeRect(x, y, halfW, halfH);
}

void Player::UpdateAnimation(bool isMoving) {
    if (!m_Player) return;

    if (isMoving) {
        m_AnimCounter++;
        if (m_AnimCounter >= m_AnimInterval) {
            m_AnimCounter = 0;
            m_AnimFrame = (m_AnimFrame + 1) % 2;
            m_Player->SetImage(GetTankImagePath(m_Direction, m_AnimFrame));
        }
    } else {
        m_AnimFrame = 0;
        m_AnimCounter = 0;
        m_Player->SetImage(GetTankImagePath(m_Direction, m_AnimFrame));
    }
}

void Player::ClampToMap(const Map& map) {
    // 先假設坦克寬高大約 32x32
    // 如果你的圖實際不是 32，再改這裡
    const float halfWidth = 10.0f;
    const float halfHeight = 10.0f;

    if (m_PlayerX < map.GetPlayLeft() + halfWidth) {
        m_PlayerX = map.GetPlayLeft() + halfWidth;
    }
    if (m_PlayerX > map.GetPlayRight() - halfWidth) {
        m_PlayerX = map.GetPlayRight() - halfWidth;
    }
    if (m_PlayerY > map.GetPlayTop() - halfHeight) {
        m_PlayerY = map.GetPlayTop() - halfHeight;
    }
    if (m_PlayerY < map.GetPlayBottom() + halfHeight) {
        m_PlayerY = map.GetPlayBottom() + halfHeight;
    }
}

bool Player::CanMoveTo(float newX, float newY, const Map &map, const std::vector<Rect>& blockingRects) const {
    const float halfW = 12.0f;
    const float halfH = 12.0f;

    // 檢查碰撞箱邊界上的多個點，避免半磚從邊線中間漏判
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

    // 2. 檢查其他坦克
    Rect nextRect = GetCollisionRectAt(newX, newY);

    for (const Rect& blocker : blockingRects) {
        if (IsColliding(nextRect, blocker)) {
            return false;
        }
    }

    return true;
}


void Player::Update(const Map& map, const std::vector<Rect>& blockingRects) {
    // ===== 1. 如果死了，只更新爆炸 =====
    if (!m_IsAlive) {
        if (m_Explosion) {
            m_Explosion->Update();

            if (m_Explosion->IsFinished()) {
                m_Explosion->Clear();
                m_Explosion.reset();
            }
        }
        return;
    }

    if (!m_Player) return;

    bool isMoving = false;

    float nextX = m_PlayerX;
    float nextY = m_PlayerY;


    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        nextY += m_MoveSpeed;
        m_Direction = Direction::UP;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        nextY -= m_MoveSpeed;
        m_Direction = Direction::DOWN;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        nextX -= m_MoveSpeed;
        m_Direction = Direction::LEFT;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        nextX += m_MoveSpeed;
        m_Direction = Direction::RIGHT;
        isMoving = true;
    }


    // 先做邊界限制到候選座標
    const float halfW = 12.0f;
    const float halfH = 12.0f;

    if (nextX < map.GetPlayLeft() + halfW) {
        nextX = map.GetPlayLeft() + halfW;
    }
    if (nextX > map.GetPlayRight() - halfW) {
        nextX = map.GetPlayRight() - halfW;
    }
    if (nextY > map.GetPlayTop() - halfH) {
        nextY = map.GetPlayTop() - halfH;
    }
    if (nextY < map.GetPlayBottom() + halfH) {
        nextY = map.GetPlayBottom() + halfH;
    }

    // 再檢查 tilemap
    if (CanMoveTo(nextX, nextY, map, blockingRects)) {
        m_PlayerX = nextX;
        m_PlayerY = nextY;
    }

    // 再限制在地圖範圍內
    // ClampToMap(map);
    UpdateAnimation(isMoving);
    m_Player->SetPosition({m_PlayerX, m_PlayerY});

    UpdateSpawnCollisionGrace();
}

bool Player::IsAlive() const {
    return m_IsAlive;
}

void Player::Upgrade() {
    if (m_UpgradeLevel < 3) {
        ++m_UpgradeLevel;
    }

    if (m_Player) {
        m_Player->SetImage(GetTankImagePath(m_Direction, m_AnimFrame));
    }
}

int Player::GetUpgradeLevel() const {
    return m_UpgradeLevel;
}

void Player::SetUpgradeLevel(int level) {
    if (level < 0) {
        level = 0;
    }

    if (level > 3) {
        level = 3;
    }

    m_UpgradeLevel = level;

    if (m_Player) {
        m_Player->SetImage(GetTankImagePath(m_Direction, m_AnimFrame));
    }
}

float Player::GetBulletSpeed() const {
    if (m_UpgradeLevel >= 1) {
        return 9.0f;
    }

    return 6.0f;
}

int Player::GetBulletPower() const {
    if (m_UpgradeLevel >= 3) {
        return 2;
    }

    return 1;
}

bool Player::CanDoubleShot() const {
    return m_UpgradeLevel >= 2;
}

void Player::SetInvincible(bool invincible) {
    m_IsInvincible = invincible;
}

bool Player::IsInvincible() const {
    return m_IsInvincible;
}

void Player::Kill() {
    if (!m_IsAlive) return;
    if (m_IsInvincible) return;

    m_IsAlive = false;

    m_Explosion = std::make_unique<Explosion>(m_Root);
    m_Explosion->Init(m_PlayerX, m_PlayerY);

    if (m_Player) {
        m_Player->SetVisible(false); // 或 Clear()
    }
}

bool Player::IsExplosionFinished() const {
    return !m_IsAlive && !m_Explosion;
}

void Player::Clear() {
    if (m_Player) {
        m_Root.RemoveChild(m_Player);
        m_Player.reset();
    }
}

void Player::DisableCollisionUntilLeaveSpawnArea(const Rect& spawnArea) {
    m_CollisionEnabled = false;
    m_WaitingLeaveSpawnArea = true;
    m_SpawnAreaRect = spawnArea;
}

void Player::UpdateSpawnCollisionGrace() {
    if (!m_WaitingLeaveSpawnArea) {
        return;
    }

    if (!IsAlive()) {
        return;
    }

    Rect playerRect = GetCollisionRect();

    // 完全離開出生區後，才恢復碰撞
    if (!IsColliding(playerRect, m_SpawnAreaRect)) {
        m_CollisionEnabled = true;
        m_WaitingLeaveSpawnArea = false;
    }
}

bool Player::IsCollisionEnabled() const {
    return m_CollisionEnabled;
}
