//
// Created by link9 on 2026/5/5.
//

#ifndef BRICK_HPP
#define BRICK_HPP

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "TileObject.hpp"

class Brick : public TileObject {
public:
    enum class State {
        Full,
        TopHalf,
        BottomHalf,
        LeftHalf,
        RightHalf,

        TopLeftQuarter,
        TopRightQuarter,
        BottomLeftQuarter,
        BottomRightQuarter,

        Destroyed
    };

public:
    Brick(
        Util::Renderer& root,
        float x,
        float y,
        int tileSize,
        State initialState = State::Full
    );

    bool BlocksTank() const override;
    bool BlocksBullet() const override;
    bool BlocksTankAtPoint(const glm::vec2& point) const override;

    TileHitResult OnBulletHit(const TileHitInfo& hit) override;

    void Clear() override;

    float GetX() const override;
    float GetY() const override;

private:
    bool ContainsSolidPart(const glm::vec2& point) const;
    State GetHalfStateAfterHit(const TileHitInfo& hit) const;

    void UpdateImage();
    std::string GetImagePathByState() const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Image;

    float m_X = 0.0f;
    float m_Y = 0.0f;
    int m_TileSize = 32;

    State m_State = State::Full;
};

#endif //BRICK_HPP