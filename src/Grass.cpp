//
// Created by link9 on 2026/5/14.
//
#include "Grass.hpp"

Grass::Grass(Util::Renderer& root, float x, float y)
    : m_Root(root), m_X(x), m_Y(y) {

    m_Image = std::make_shared<Character>(GetImagePath());
    m_Image->SetPosition({m_X, m_Y});

    // 草叢要蓋住坦克，所以 ZIndex 要比 Player / Enemy 高
    // 如果你的 Player 是 40，Grass 可先設 50
    m_Image->SetZIndex(50.0f);

    m_Image->SetVisible(true);
    m_Root.AddChild(m_Image);
}

bool Grass::BlocksTank() const {
    return false;
}

bool Grass::BlocksBullet() const {
    return false;
}

bool Grass::BlocksTankAtPoint(const glm::vec2& point) const {
    return false;
}

TileHitResult Grass::OnBulletHit(const TileHitInfo& hit) {
    TileHitResult result;

    result.bulletStopped = false;
    result.spawnExplosion = false;
    result.tileDestroyed = false;

    return result;
}

float Grass::GetX() const {
    return m_X;
}

float Grass::GetY() const {
    return m_Y;
}

void Grass::Clear() {
    if (m_Image) {
        m_Root.RemoveChild(m_Image);
        m_Image.reset();
    }
}

std::string Grass::GetImagePath() const {
    return std::string(RESOURCE_DIR) + "/image/map/grass.png";
}