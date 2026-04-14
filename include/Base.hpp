//
// Created by link9 on 2026/4/12.
//

#ifndef BASE_HPP
#define BASE_HPP

#include <memory>
#include "Util/Renderer.hpp"
#include "Character.hpp"

class Base {
public:
    explicit Base(Util::Renderer& root);

    void Init(float x, float y);
    void Clear();

    void Destroy();              // 被打爆
    bool IsAlive() const;

    float GetX() const;
    float GetY() const;

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_Base;

    float m_X = 0.0f;
    float m_Y = 0.0f;

    bool m_Alive = true;

    std::string GetBaseImagePath(bool destroyed) const;
};

#endif //BASE_HPP