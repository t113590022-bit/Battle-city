//
// Created by link9 on 2026/5/14.
//

#ifndef GRASS_HPP
#define GRASS_HPP

#include <memory>
#include <string>

#include "TileObject.hpp"
#include "Util/Renderer.hpp"
#include "Character.hpp"

class Grass : public TileObject {
public:
    Grass(Util::Renderer& root, float x, float y);

    bool BlocksTank() const override;
    bool BlocksBullet() const override;
    bool BlocksTankAtPoint(const glm::vec2& point) const override;

    TileHitResult OnBulletHit(const TileHitInfo& hit) override;

    float GetX() const override;
    float GetY() const override;

    void Clear() override;

private:
    std::string GetImagePath() const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Image;

    float m_X = 0.0f;
    float m_Y = 0.0f;
};

#endif //GRASS_HPP