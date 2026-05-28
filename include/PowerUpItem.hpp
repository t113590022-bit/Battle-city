//
// Created by link9 on 2026/5/24.
//

#ifndef POWERUPITEM_HPP
#define POWERUPITEM_HPP

#include <memory>
#include <string>

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Collision.hpp"

enum class PowerUpType {
    Clock,
    Star,
    Helmet,
    Grenade,
    Shovel,
    Tank
};

class PowerUpItem {
public:
    explicit PowerUpItem(Util::Renderer& root);

    void Init(float x, float y, PowerUpType type);
    void Clear();

    bool IsActive() const;
    void Deactivate();

    PowerUpType GetType() const;
    Rect GetCollisionRect() const;

private:
    std::string GetImagePath(PowerUpType type) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Image;

    float m_X = 0.0f;
    float m_Y = 0.0f;

    PowerUpType m_Type = PowerUpType::Tank;
    bool m_Active = false;
};

#endif //POWERUPITEM_HPP