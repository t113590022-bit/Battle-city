//
// Created by link9 on 2026/5/6.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP

struct Rect {
    float left;
    float right;
    float top;
    float bottom;
};

inline Rect MakeRect(float x, float y, float halfW, float halfH) {
    return {
        x - halfW,
        x + halfW,
        y + halfH,
        y - halfH
    };
}

inline bool IsColliding(const Rect& a, const Rect& b) {
    return !(a.right < b.left ||
             a.left > b.right ||
             a.top < b.bottom ||
             a.bottom > b.top);
}

#endif //COLLISION_HPP