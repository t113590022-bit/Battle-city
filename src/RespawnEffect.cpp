//
// Created by link9 on 2026/4/12.
//

#include "RespawnEffect.hpp"

RespawnEffect::RespawnEffect(Util::Renderer& root)
    : m_Root(root) {}

void RespawnEffect::Init(float x, float y) {
    Clear();

    m_X = x;
    m_Y = y;
    m_Frame = 0;
    m_FrameCounter = 0;
    m_CurrentLoop = 0;
    m_Finished = false;

    m_Effect = std::make_shared<Character>(GetImagePath(0));
    m_Effect->SetPosition({m_X, m_Y});
    m_Effect->SetZIndex(100.0f);
    m_Root.AddChild(m_Effect);
}

std::string RespawnEffect::GetImagePath(int frame) const {
    switch (frame) {
        case 0: return std::string(RESOURCE_DIR) + "/image/effect/respawn_1.png";
        case 1: return std::string(RESOURCE_DIR) + "/image/effect/respawn_2.png";
        case 2: return std::string(RESOURCE_DIR) + "/image/effect/respawn_3.png";
        case 3: return std::string(RESOURCE_DIR) + "/image/effect/respawn_4.png";
        default: return std::string(RESOURCE_DIR) + "/image/effect/respawn_4.png";
    }
}

void RespawnEffect::Update() {
    if (!m_Effect || m_Finished) return;

    m_FrameCounter++;

    if (m_FrameCounter >= m_FrameInterval) {
        m_FrameCounter = 0;
        m_Frame++;

        if (m_Frame >= m_TotalFrames) {
            m_CurrentLoop++;

            if (m_CurrentLoop >= m_TotalLoops) {
                m_Finished = true;
                return;
            }

            m_Frame = 0;
        }

        m_Effect->SetImage(GetImagePath(m_Frame));
    }

    m_Effect->SetPosition({m_X, m_Y});
}

bool RespawnEffect::IsFinished() const {
    return m_Finished;
}

void RespawnEffect::Clear() {
    if (m_Effect) {
        m_Root.RemoveChild(m_Effect);
        m_Effect.reset();
    }
}