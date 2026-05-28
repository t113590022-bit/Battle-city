//
// Created by link9 on 2026/5/24.
//

#include "PowerUpItem.hpp"

PowerUpItem::PowerUpItem(Util::Renderer& root)
    : m_Root(root) {}

void PowerUpItem::Init(float x, float y, PowerUpType type) {
    m_X = x;
    m_Y = y;
    m_Type = type;
    m_Active = true;

    m_Image = std::make_shared<Character>(GetImagePath(type));
    m_Image->SetPosition({m_X, m_Y});
    m_Image->SetZIndex(70.0f);
    m_Image->SetVisible(true);

    m_Root.AddChild(m_Image);
}

void PowerUpItem::Clear() {
    if (m_Image) {
        m_Root.RemoveChild(m_Image);
        m_Image.reset();
    }

    m_Active = false;
}

bool PowerUpItem::IsActive() const {
    return m_Active;
}

void PowerUpItem::Deactivate() {
    m_Active = false;

    if (m_Image) {
        m_Image->SetVisible(false);
    }
}

PowerUpType PowerUpItem::GetType() const {
    return m_Type;
}

Rect PowerUpItem::GetCollisionRect() const {
    return MakeRect(m_X, m_Y, 14.0f, 14.0f);
}

std::string PowerUpItem::GetImagePath(PowerUpType type) const {
    std::string base = std::string(RESOURCE_DIR) + "/image/powerup/";

    switch (type) {
        case PowerUpType::Clock:
            return base + "clock.png";

        case PowerUpType::Star:
            return base + "star.png";

        case PowerUpType::Helmet:
            return base + "helmet.png";

        case PowerUpType::Grenade:
            return base + "grenade.png";

        case PowerUpType::Shovel:
            return base + "shovel.png";

        case PowerUpType::Tank:
            return base + "tank.png";
    }

    return base + "tank.png";
}