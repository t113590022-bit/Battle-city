//
// Created by link9 on 2026/4/12.
//

#ifndef GAMEOVERBANNER_HPP
#define GAMEOVERBANNER_HPP

#include <memory>
#include "Util/Renderer.hpp"
#include "Character.hpp"

class GameOverBanner {
public:
    explicit GameOverBanner(Util::Renderer& root);

    void Init();
    void Update();
    void Clear();

    bool IsFinished() const;

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_Left;
    std::shared_ptr<Character> m_Right;

    float m_LeftX = -40.0f;
    float m_RightX = 40.0f;
    float m_Y = -500.0f;

    float m_TargetY = 0.0f;
    float m_Speed = 3.0f;

    bool m_Finished = false;
};

#endif