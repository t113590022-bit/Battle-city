//
// Created by link9 on 2026/5/5.
//

#ifndef TILEOBJECT_HPP
#define TILEOBJECT_HPP

#include "TileHitInfo.hpp"

class TileObject {
public:
    virtual ~TileObject() = default;

    virtual bool BlocksTank() const = 0;
    virtual bool BlocksBullet() const = 0;

    virtual bool BlocksTankAtPoint(const glm::vec2& point) const = 0;

    virtual TileHitResult OnBulletHit(const TileHitInfo& hit) = 0;

    virtual void Clear() = 0;

    virtual float GetX() const = 0;
    virtual float GetY() const = 0;
};


#endif //TILEOBJECT_HPP