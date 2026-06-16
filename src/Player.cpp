//
// Created by link9 on 2026/3/26.
//

#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#include <cmath>

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
    const float halfWidth = 9.0f;
    const float halfHeight = 9.0f;

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

bool Player::IsTankRectBlockedByMap(
    float newX,
    float newY,
    const Map& map,
    float halfW,
    float halfH
) const {
    const float left = newX - halfW;
    const float right = newX + halfW;
    const float bottom = newY - halfH;
    const float top = newY + halfH;

    const float step = 2.0f;

    for (float x = left; x <= right; x += step) {
        for (float y = bottom; y <= top; y += step) {
            if (map.IsBlockedAtWorld(x, y)) {
                return true;
            }
        }
    }

    for (float y = bottom; y <= top; y += step) {
        if (map.IsBlockedAtWorld(right, y)) return true;
        if (map.IsBlockedAtWorld(left, y)) return true;
    }

    for (float x = left; x <= right; x += step) {
        if (map.IsBlockedAtWorld(x, top)) return true;
        if (map.IsBlockedAtWorld(x, bottom)) return true;
    }

    if (map.IsBlockedAtWorld(left, top)) return true;
    if (map.IsBlockedAtWorld(right, top)) return true;
    if (map.IsBlockedAtWorld(left, bottom)) return true;
    if (map.IsBlockedAtWorld(right, bottom)) return true;

    return false;
}

float Player::SnapToGrid(float value) const {
    const float gridSize = 32.0f;

    return std::round(value / gridSize) * gridSize;
}

bool Player::TryAlignToGrid(float& value) const {
    const float target = SnapToGrid(value);
    const float diff = target - value;

    // 差距太大就不要硬吸，避免手感太怪
    const float alignTolerance = 7.0f;

    if (std::abs(diff) > alignTolerance) {
        return false;
    }

    if (std::abs(diff) <= m_MoveSpeed) {
        value = target;
    } else {
        value += diff > 0.0f ? m_MoveSpeed : -m_MoveSpeed;
    }

    value = std::round(value);

    return true;
}

void Player::ApplyGridAssist(float& nextX, float& nextY, Direction dir, const Map& map, const std::vector<Rect>& blockingRects) const {
    auto canAssistTo = [&](float testX, float testY) -> bool {
        // 吸附是輔助行為，所以這裡一定用嚴格判斷
        // 如果吸附後會撞牆，就不要吸
        if (IsTankRectBlockedByMap(testX, testY, map, 13.0f, 13.0f)) {
            return false;
        }

        Rect testRect = GetCollisionRectAt(testX, testY);

        for (const Rect& blocker : blockingRects) {
            if (IsColliding(testRect, blocker)) {
                return false;
            }
        }

        return true;
    };

    switch (dir) {
        case Direction::UP:
        case Direction::DOWN: {
            // 上下走時，只嘗試修正 X
            float testX = nextX;
            float testY = nextY;

            if (TryAlignToGrid(testX) && canAssistTo(testX, testY)) {
                nextX = testX;
            }

            break;
        }

        case Direction::LEFT:
        case Direction::RIGHT: {
            // 左右走時，只嘗試修正 Y
            float testX = nextX;
            float testY = nextY;

            if (TryAlignToGrid(testY) && canAssistTo(testX, testY)) {
                nextY = testY;
            }

            break;
        }
    }
}

bool Player::CanMoveTo(float newX, float newY, const Map& map, const std::vector<Rect>& blockingRects) const {
    // 正常碰撞：比較嚴格，避免玩家鑽進窄縫
    const float strictHalfW = 13.0f;
    const float strictHalfH = 13.0f;

    // 脫困碰撞：如果玩家已經卡在窄縫裡，讓他有機會退回來
    const float looseHalfW = 11.5f;
    const float looseHalfH = 11.5f;

    bool currentBlockedStrict =
        IsTankRectBlockedByMap(m_PlayerX, m_PlayerY, map, strictHalfW, strictHalfH);

    bool nextBlockedStrict =
        IsTankRectBlockedByMap(newX, newY, map, strictHalfW, strictHalfH);

    if (nextBlockedStrict) {
        // 如果目前位置本來沒有卡住，就不准進去
        if (!currentBlockedStrict) {
            return false;
        }

        // 如果目前已經卡住，改用較小碰撞箱判斷，讓玩家可以退出來
        if (IsTankRectBlockedByMap(newX, newY, map, looseHalfW, looseHalfH)) {
            return false;
        }
    }

    // 檢查其他坦克 / 基地
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

    Direction desiredDirection = m_Direction;

    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        desiredDirection = Direction::UP;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        desiredDirection = Direction::DOWN;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        desiredDirection = Direction::LEFT;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        desiredDirection = Direction::RIGHT;
        isMoving = true;
    }

    if (isMoving) {
        m_Direction = desiredDirection;

        // 移動前先把垂直軸吸到格線中心
        ApplyGridAssist(nextX, nextY, m_Direction, map, blockingRects);

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

        nextX = std::round(nextX);
        nextY = std::round(nextY);
    }


    // 先做邊界限制到候選座標
    const float halfW = 16.0f;
    const float halfH = 16.0f;

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
