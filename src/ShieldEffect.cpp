//
// Created by link9 on 2026/5/28.
//

#include "ShieldEffect.hpp"

#include "Player.hpp"

ShieldEffect::ShieldEffect(Util::Renderer& root)
    : m_Root(root) {
}

void ShieldEffect::Init(Player* player) {
    m_Player = player;

    m_AnimFrame = 0;
    m_AnimCounter = 0;
    m_Visible = false;

    m_Shield = std::make_shared<Character>(GetShieldImagePath(0));
    m_Shield->SetVisible(false);

    // 玩家 ZIndex 是 40，所以保護罩放 45
    m_Shield->SetZIndex(45.0f);

    if (m_Player) {
        m_Shield->SetPosition(m_Player->GetPosition());
    }

    m_Root.AddChild(m_Shield);
}

void ShieldEffect::Update() {
    if (!m_Shield || !m_Player) {
        return;
    }

    m_Shield->SetPosition(m_Player->GetPosition());

    if (!m_Visible || !m_Player->IsAlive()) {
        m_Shield->SetVisible(false);
        return;
    }

    m_Shield->SetVisible(true);

    ++m_AnimCounter;

    if (m_AnimCounter >= m_AnimInterval) {
        m_AnimCounter = 0;
        m_AnimFrame = (m_AnimFrame + 1) % 2;
        m_Shield->SetImage(GetShieldImagePath(m_AnimFrame));
        m_Shield->SetPosition(m_Player->GetPosition());
    }
}

void ShieldEffect::SetVisible(bool visible) {
    m_Visible = visible;

    if (!m_Shield) {
        return;
    }

    // 如果要顯示，先移到玩家身上，再打開 visible
    if (visible && m_Player) {
        m_Shield->SetPosition(m_Player->GetPosition());
    }

    m_Shield->SetVisible(visible);
}

bool ShieldEffect::IsVisible() const {
    return m_Visible;
}

void ShieldEffect::Clear() {
    if (m_Shield) {
        m_Root.RemoveChild(m_Shield);
        m_Shield.reset();
    }

    m_Player = nullptr;
    m_Visible = false;
}

std::string ShieldEffect::GetShieldImagePath(int frame) const {
    std::string base = std::string(RESOURCE_DIR) + "/image/effect/";

    if (frame == 0) {
        return base + "shield_1.png";
    }

    return base + "shield_2.png";
}