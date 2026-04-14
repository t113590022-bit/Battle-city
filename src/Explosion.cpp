#include "Explosion.hpp"

Explosion::Explosion(Util::Renderer& root)
    : m_Root(root) {
}

void Explosion::Init(float x, float y) {
    Clear();

    m_X = x;
    m_Y = y;
    m_Frame = 0;
    m_FrameCounter = 0;
    m_Finished = false;

    m_Explosion = std::make_shared<Character>(GetExplosionImagePath(0));
    m_Explosion->SetPosition({m_X, m_Y});
    m_Explosion->SetZIndex(98.0f);
    m_Root.AddChild(m_Explosion);
}

std::string Explosion::GetExplosionImagePath(int frame) const {
    switch (frame) {
        case 0: return std::string(RESOURCE_DIR) + "/image/effect/explosion_1.png";
        case 1: return std::string(RESOURCE_DIR) + "/image/effect/explosion_2.png";
        case 2: return std::string(RESOURCE_DIR) + "/image/effect/explosion_3.png";
        default: return std::string(RESOURCE_DIR) + "/image/effect/explosion_3.png";
    }
}

void Explosion::Update() {
    if (!m_Explosion || m_Finished) return;

    m_FrameCounter++;

    if (m_FrameCounter >= m_FrameInterval) {
        m_FrameCounter = 0;
        m_Frame++;

        if (m_Frame >= m_TotalFrames) {
            m_Finished = true;
            return;
        }

        m_Explosion->SetImage(GetExplosionImagePath(m_Frame));
        m_Explosion->SetVisible(true);
    }

    m_Explosion->SetPosition({m_X, m_Y});
}

bool Explosion::IsFinished() const {
    return m_Finished;
}

void Explosion::Clear() {
    if (m_Explosion) {
        m_Root.RemoveChild(m_Explosion);
        m_Explosion.reset();
    }
}