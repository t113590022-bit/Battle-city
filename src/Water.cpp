//
// Created by link9 on 2026/5/21.
//

#include "Water.hpp"

Water::Water(Util::Renderer& root, float x, float y)
    : m_Root(root), m_X(x), m_Y(y) {

    m_Image = std::make_shared<Character>(GetImagePath(m_Frame));
    m_Image->SetPosition({m_X, m_Y});

    // 水在地圖層，不需要蓋住坦克
    m_Image->SetZIndex(1.0f);

    m_Image->SetVisible(true);
    m_Root.AddChild(m_Image);
}

void Water::Update() {
    if (!m_Image) return;

    ++m_FrameCounter;

    if (m_FrameCounter < m_FrameInterval) {
        return;
    }

    m_FrameCounter = 0;
    m_Frame = (m_Frame + 1) % 2;

    m_Image->SetImage(GetImagePath(m_Frame));
}

bool Water::BlocksTank() const {
    return true;
}

bool Water::BlocksBullet() const {
    return false;
}

bool Water::BlocksTankAtPoint(const glm::vec2& /*point*/) const {
    return true;
}

TileHitResult Water::OnBulletHit(const TileHitInfo& /*hit*/) {
    TileHitResult result;

    result.bulletStopped = false;
    result.spawnExplosion = false;
    result.tileDestroyed = false;

    return result;
}

void Water::Clear() {
    if (m_Image) {
        m_Root.RemoveChild(m_Image);
        m_Image.reset();
    }
}

float Water::GetX() const {
    return m_X;
}

float Water::GetY() const {
    return m_Y;
}

std::string Water::GetImagePath(int frame) const {
    std::string base = std::string(RESOURCE_DIR) + "/image/map/";

    if (frame == 0) {
        return base + "sea01.png";
    }

    return base + "sea02.png";
}