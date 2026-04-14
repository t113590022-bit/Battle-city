//
// Created by link9 on 2026/3/26.
//

#ifndef BULLET_HPP
#define BULLET_HPP

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Map.hpp"

class Bullet {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    explicit Bullet(Util::Renderer& root);

    void Init(float x, float y, Direction dir, float speed);
    void Update();
    // void Clear();
    //
    bool IsOutOfBounds(const Map& map) const;

    float GetX() const;
    float GetY() const;

    bool IsActive() const;
    void Deactivate();
private:
    std::string GetBulletImagePath(Direction dir) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Bullet;

    float m_X = 0.0f;
    float m_Y = 0.0f;
    float m_Speed = 5.0f;


    Direction m_Direction = Direction::UP;

    bool m_Active = false; // 不可視
};


#endif //BULLET_HPP