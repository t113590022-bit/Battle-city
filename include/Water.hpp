//
// Created by link9 on 2026/5/21.
//

#ifndef WATER_HPP
#define WATER_HPP


#include <memory>
#include <string>

#include "TileObject.hpp"
#include "Util/Renderer.hpp"
#include "Character.hpp"

class Water : public TileObject {
public:
    Water(Util::Renderer& root, float x, float y);

    void Update();

    bool BlocksTank() const override;
    bool BlocksBullet() const override;
    bool BlocksTankAtPoint(const glm::vec2& point) const override;

    TileHitResult OnBulletHit(const TileHitInfo& hit) override;

    void Clear() override;

    float GetX() const override;
    float GetY() const override;

private:
    std::string GetImagePath(int frame) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Image;

    float m_X = 0.0f;
    float m_Y = 0.0f;

    int m_Frame = 0;
    int m_FrameCounter = 0;
    int m_FrameInterval = 30;
};

#endif //WATER_HPP