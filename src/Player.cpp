//
// Created by link9 on 2026/3/26.
//

#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

// struct Rect {
//     float left;
//     float right;
//     float top;
//     float bottom;
// };

// static Rect MakeRect(float x, float y, float halfW, float halfH) {
//     return {
//         x - halfW,
//         x + halfW,
//         y + halfH,
//         y - halfH
//     };
// }
//
// static bool IsColliding(const Rect& a, const Rect& b) {
//     return !(a.right < b.left ||
//              a.left > b.right ||
//              a.top < b.bottom ||
//              a.bottom > b.top);
// }
//
// static bool WouldHitCenterBrick(float nextX, float nextY) {
//     // 玩家碰撞箱，先假設 32x32
//     const float playerHalfW = 16.0f;
//     const float playerHalfH = 16.0f;
//
//     // 中央磚塊碰撞箱，先假設 32x32，位置在 (0,0)
//     const float brickX = 0.0f;
//     const float brickY = 0.0f;
//     const float brickHalfW = 16.0f;
//     const float brickHalfH = 16.0f;
//
//     Rect playerRect = MakeRect(nextX, nextY, playerHalfW, playerHalfH);
//     Rect brickRect = MakeRect(brickX, brickY, brickHalfW, brickHalfH);
//
//     return IsColliding(playerRect, brickRect);
// }

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
    m_Root.AddChild(m_Player);
}

std::string Player::GetTankImagePath(Direction dir, int frame) const {
    switch (dir) {
        case Direction::UP:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-front_lv1.png"
                : std::string(RESOURCE_DIR) + "/image/player/1p-front2_lv1.png";
        case Direction::DOWN:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-back_lv1.png"
                : std::string(RESOURCE_DIR) + "/image/player/1p-back2_lv1.png";
        case Direction::LEFT:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-left_lv1.png"
                : std::string(RESOURCE_DIR) + "/image/player/1p-left2_lv1.png";
        case Direction::RIGHT:
            return frame == 0
                ? std::string(RESOURCE_DIR) + "/image/player/1p-right_lv1.png"
                : std::string(RESOURCE_DIR) + "/image/player/1p-right2_lv1.png";
        default:
            return std::string(RESOURCE_DIR) + "/image/player/1p-front_lv1.png";
    }
}

glm::vec2 Player::GetPosition() const {
    return {m_PlayerX, m_PlayerY};
}

Player::Direction Player::GetDirection() const {
    return m_Direction;
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

bool Player::CanMoveTo(float newX, float newY, const Map &map) const {
    const float halfW = 10.0f;
    const float halfH = 10.0f;

    if (map.IsBlockedAtWorld(newX - halfW, newY + halfH)) return false; // 左上
    if (map.IsBlockedAtWorld(newX + halfW, newY + halfH)) return false; // 右上
    if (map.IsBlockedAtWorld(newX - halfW, newY - halfH)) return false; // 左下
    if (map.IsBlockedAtWorld(newX + halfW, newY - halfH)) return false; // 右下

    return true;
}


void Player::Update(const Map& map) {
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
    const float halfW = 10.0f;
    const float halfH = 10.0f;

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
    if (CanMoveTo(nextX, nextY, map)) {
        m_PlayerX = nextX;
        m_PlayerY = nextY;
    }

    // 再限制在地圖範圍內
    // ClampToMap(map);
    UpdateAnimation(isMoving);
    m_Player->SetPosition({m_PlayerX, m_PlayerY});
}

bool Player::IsAlive() const {
    return m_IsAlive;
}


void Player::Kill() {
    if (!m_IsAlive) return;

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
