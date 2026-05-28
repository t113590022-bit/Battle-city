//
// Created by link9 on 2026/5/28.
//

#ifndef SHIELDEFFECT_HPP
#define SHIELDEFFECT_HPP

#include <memory>
#include <string>

#include "Util/Renderer.hpp"
#include "Character.hpp"

#include "Player.hpp"


class ShieldEffect {
public:
    explicit ShieldEffect(Util::Renderer& root);

    void Init(Player* player);
    void Update();
    void Clear();

    void SetVisible(bool visible);
    bool IsVisible() const;

private:
    std::string GetShieldImagePath(int frame) const;

private:
    Util::Renderer& m_Root;
    Player* m_Player = nullptr;

    std::shared_ptr<Character> m_Shield;

    bool m_Visible = false;

    int m_AnimFrame = 0;
    int m_AnimCounter = 0;

    // 越小閃越快，1 是每幀切換
    int m_AnimInterval = 1;
};

#endif //SHIELDEFFECT_HPP