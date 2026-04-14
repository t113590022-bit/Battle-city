//
// Created by link9 on 2026/4/12.
//

#include "Base.hpp"

Base::Base(Util::Renderer& root)
    : m_Root(root) {}

void Base::Init(float x, float y) {
    m_X = x;
    m_Y = y;
    m_Alive = true;

    m_Base = std::make_shared<Character>(GetBaseImagePath(false));
    m_Base->SetPosition({m_X, m_Y});
    m_Base->SetZIndex(1.5f); // 在牆上面、但在坦克下面
    m_Root.AddChild(m_Base);
}

std::string Base::GetBaseImagePath(bool destroyed) const {
    if (!destroyed) {
        return std::string(RESOURCE_DIR) + "/image/base/base_alive.png";
    } else {
        return std::string(RESOURCE_DIR) + "/image/base/base_dead.png";
    }
}

void Base::Destroy() {
    if (!m_Alive) return;

    m_Alive = false;

    if (m_Base) {
        m_Base->SetImage(GetBaseImagePath(true));
    }
}

bool Base::IsAlive() const {
    return m_Alive;
}

float Base::GetX() const { return m_X; }
float Base::GetY() const { return m_Y; }

void Base::Clear() {
    if (m_Base) {
        m_Root.RemoveChild(m_Base);
        m_Base.reset();
    }
}