//
// Created by link9 on 2026/3/26.
//
#include "Bullet.hpp"

Bullet::Bullet(Util::Renderer& root)
    : m_Root(root) {
    m_Bullet = std::make_shared<Character>(
    std::string(RESOURCE_DIR) + "/image/bullet/bullet_up.png"
    );
    m_Bullet->SetVisible(false);
    m_Bullet->SetZIndex(99.0f);
    m_Root.AddChild(m_Bullet);
}

void Bullet::Init(float x, float y, Direction dir, float speed) {
    m_X = x;
    m_Y = y;
    m_Direction = dir;
    m_Speed = speed;
    m_Active = true;

    m_Bullet->SetImage(GetBulletImagePath(dir));
    m_Bullet->SetPosition({m_X, m_Y});
    m_Bullet->SetVisible(true);
}

std::string Bullet::GetBulletImagePath(Direction dir) const {
    switch (dir) {
        case Direction::UP:
            return std::string(RESOURCE_DIR) + "/image/bullet/bullet_up.png";
        case Direction::DOWN:
            return std::string(RESOURCE_DIR) + "/image/bullet/bullet_down.png";
        case Direction::LEFT:
            return std::string(RESOURCE_DIR) + "/image/bullet/bullet_left.png";
        case Direction::RIGHT:
            return std::string(RESOURCE_DIR) + "/image/bullet/bullet_right.png";
    }

    return std::string(RESOURCE_DIR) + "/image/bullet/bullet_up.png";
}

void Bullet::Update() {
    if (!m_Active) return;

    switch (m_Direction) {
        case Direction::UP:
            m_Y += m_Speed;
            break;
        case Direction::DOWN:
            m_Y -= m_Speed;
            break;
        case Direction::LEFT:
            m_X -= m_Speed;
            break;
        case Direction::RIGHT:
            m_X += m_Speed;
            break;
    }

    m_Bullet->SetPosition({m_X, m_Y});
}

bool Bullet::IsOutOfBounds(const Map& map) const {
    return m_X < map.GetPlayLeft() ||
           m_X > map.GetPlayRight() ||
           m_Y > map.GetPlayTop() ||
           m_Y < map.GetPlayBottom();
}

float Bullet::GetX() const { return m_X; }
float Bullet::GetY() const { return m_Y; }
//
// void Bullet::Clear() {
//     if (m_Bullet) {
//         m_Root.RemoveChild(m_Bullet);
//         m_Bullet.reset();
//     }
// }

bool Bullet::IsActive() const {
    return m_Active;
}

void Bullet::Deactivate() {
    m_Active = false;
    if (m_Bullet) {
        m_Bullet->SetVisible(false);
    }
}