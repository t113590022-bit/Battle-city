//
// Created by link9 on 2026/4/12.
//

#include "GameOverBanner.hpp"

GameOverBanner::GameOverBanner(Util::Renderer& root)
    : m_Root(root) {}

void GameOverBanner::Init() {
    m_LeftX = -40.0f;
    m_RightX = 40.0f;
    m_Y = -500.0f;
    m_TargetY = 0.0f;
    m_Finished = false;

    m_Left = std::make_shared<Character>(
        std::string(RESOURCE_DIR) + "/image/ui/gameover_left.png"
    );
    m_Right = std::make_shared<Character>(
        std::string(RESOURCE_DIR) + "/image/ui/gameover_right.png"
    );

    m_Left->SetPosition({m_LeftX, m_Y});
    m_Right->SetPosition({m_RightX, m_Y});

    m_Left->SetZIndex(1.0f);
    m_Right->SetZIndex(1.0f);

    m_Root.AddChild(m_Left);
    m_Root.AddChild(m_Right);
}

void GameOverBanner::Update() {
    if (m_Finished) return;
    if (!m_Left || !m_Right) return;

    if (m_Y < m_TargetY) {
        m_Y += m_Speed;
        if (m_Y > m_TargetY) {
            m_Y = m_TargetY;
        }
    } else {
        m_Finished = true;
    }

    m_Left->SetPosition({m_LeftX, m_Y});
    m_Right->SetPosition({m_RightX, m_Y});
}

bool GameOverBanner::IsFinished() const {
    return m_Finished;
}

void GameOverBanner::Clear() {
    if (m_Left) {
        m_Root.RemoveChild(m_Left);
        m_Left.reset();
    }
    if (m_Right) {
        m_Root.RemoveChild(m_Right);
        m_Right.reset();
    }
}