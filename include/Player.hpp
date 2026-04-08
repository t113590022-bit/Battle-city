//
// Created by link9 on 2026/3/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>      // shared_ptr
#include <string>      // std::string
#include "Map.hpp"

#include <glm/glm.hpp> // glm::vec2

#include "Util/Renderer.hpp"
#include "Character.hpp"

class Player {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    explicit Player(Util::Renderer& root);

    void Init(float x, float y);
    void Update(const Map& map);
    void Clear();

    glm::vec2 GetPosition() const;
    Direction GetDirection() const;

private:
    std::string GetTankImagePath(Direction dir, int frame) const;
    void UpdateAnimation(bool isMoving);
    void ClampToMap(const Map& map);

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Player;

    int m_PlayerX = 0;
    int m_PlayerY = 0;
    int m_MoveSpeed = 3;

    Direction m_Direction = Direction::UP;

    int m_AnimFrame = 0;
    int m_AnimCounter = 0;
    int m_AnimInterval = 8;
};
#endif //PLAYER_HPP