//
// Created by link9 on 2026/4/4.
//

#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <memory>
#include <string>

#include "Util/Renderer.hpp"
#include "Character.hpp"

class Explosion {
public:
    explicit Explosion(Util::Renderer& root);

    void Init(float x, float y);
    void Update();
    void Clear();

    bool IsFinished() const;

private:
    std::string GetExplosionImagePath(int frame) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Explosion;

    float m_X = 0.0f;
    float m_Y = 0.0f;

    int m_Frame = 0;
    int m_FrameCounter = 0;
    int m_FrameInterval = 5;   // 調速度用
    int m_TotalFrames = 3;

    bool m_Finished = false;
};

#endif //EXPLOSION_HPP