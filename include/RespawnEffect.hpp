//
// Created by link9 on 2026/4/12.
//

#ifndef RESPAWNEFFECT_HPP
#define RESPAWNEFFECT_HPP

#include <memory>
#include <string>
#include "Util/Renderer.hpp"
#include "Character.hpp"

class RespawnEffect {
public:
    explicit RespawnEffect(Util::Renderer& root);

    void Init(float x, float y);
    void Update();
    void Clear();

    bool IsFinished() const;

private:
    std::string GetImagePath(int frame) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Effect;

    float m_X = 0.0f;
    float m_Y = 0.0f;

    int m_Frame = 0;
    int m_FrameCounter = 0;
    int m_FrameInterval = 8;   // 每張圖停幾幀
    int m_TotalFrames = 4;

    bool m_Finished = false;
};


#endif //RESPAWNEFFECT_HPP