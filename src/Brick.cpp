//
// Created by link9 on 2026/5/5.
//

#include "Brick.hpp"

Brick::Brick(
    Util::Renderer& root,
    float x,
    float y,
    int tileSize,
    State initialState
)
    : m_Root(root),
      m_X(x),
      m_Y(y),
      m_TileSize(tileSize),
      m_State(initialState) {

    m_Image = std::make_shared<Character>(GetImagePathByState());
    m_Image->SetPosition({m_X, m_Y});
    m_Image->SetZIndex(1.0f);
    m_Image->SetVisible(true);

    m_Root.AddChild(m_Image);
}

bool Brick::BlocksTank() const {
    return m_State != State::Destroyed;
}

bool Brick::BlocksBullet() const {
    return m_State != State::Destroyed;
}

bool Brick::BlocksTankAtPoint(const glm::vec2& point) const {
    return ContainsSolidPart(point);
}

TileHitResult Brick::OnBulletHit(const TileHitInfo& hit) {
    TileHitResult result;

    if (m_State == State::Destroyed) {
        return result;
    }

    // 子彈打到這一格，但可能是半磚 / 1/4 磚不存在的透明區域
    if (!ContainsSolidPart(hit.hitPoint)) {
        result.bulletStopped = false;
        result.spawnExplosion = false;
        result.tileDestroyed = false;
        return result;
    }

    result.bulletStopped = true;
    result.spawnExplosion = true;

    if (m_State == State::Full) {
        m_State = GetHalfStateAfterHit(hit);
        UpdateImage();
        return result;
    }

    // 半磚或 1/4 磚再被打中實體部分，就整格消失
    m_State = State::Destroyed;
    UpdateImage();

    result.tileDestroyed = true;
    return result;
}

bool Brick::ContainsSolidPart(const glm::vec2& point) const {
    if (m_State == State::Destroyed) {
        return false;
    }

    if (m_State == State::Full) {
        return true;
    }

    float left = m_X - m_TileSize / 2.0f;
    float top = m_Y + m_TileSize / 2.0f;

    float localX = point.x - left;
    float localYFromTop = top - point.y;

    float half = m_TileSize / 2.0f;

    switch (m_State) {
        case State::TopHalf:
            return localYFromTop < half;

        case State::BottomHalf:
            return localYFromTop >= half;

        case State::LeftHalf:
            return localX < half;

        case State::RightHalf:
            return localX >= half;

        case State::TopLeftQuarter:
            return localX < half && localYFromTop < half;

        case State::TopRightQuarter:
            return localX >= half && localYFromTop < half;

        case State::BottomLeftQuarter:
            return localX < half && localYFromTop >= half;

        case State::BottomRightQuarter:
            return localX >= half && localYFromTop >= half;

        case State::Full:
            return true;

        case State::Destroyed:
            return false;
    }

    return false;
}

Brick::State Brick::GetHalfStateAfterHit(const TileHitInfo& hit) const {
    float left = m_X - m_TileSize / 2.0f;
    float top = m_Y + m_TileSize / 2.0f;

    float localX = hit.hitPoint.x - left;
    float localYFromTop = top - hit.hitPoint.y;

    float half = m_TileSize / 2.0f;

    switch (hit.direction) {
        case HitDirection::UP:
        case HitDirection::DOWN:
            if (localYFromTop < half) {
                // 打到上半部，所以留下下半部
                return State::BottomHalf;
            } else {
                // 打到下半部，所以留下上半部
                return State::TopHalf;
            }

        case HitDirection::LEFT:
        case HitDirection::RIGHT:
            if (localX < half) {
                // 打到左半部，所以留下右半部
                return State::RightHalf;
            } else {
                // 打到右半部，所以留下左半部
                return State::LeftHalf;
            }
    }

    return State::Full;
}

void Brick::UpdateImage() {
    if (!m_Image) return;

    if (m_State == State::Destroyed) {
        m_Image->SetVisible(false);
        return;
    }

    m_Image->SetImage(GetImagePathByState());
    m_Image->SetVisible(true);
}

std::string Brick::GetImagePathByState() const {
    std::string base = std::string(RESOURCE_DIR) + "/image/map/";

    switch (m_State) {
        case State::Full:
            return base + "brick_full.png";

        case State::TopHalf:
            return base + "brick_top.png";

        case State::BottomHalf:
            return base + "brick_bottom.png";

        case State::LeftHalf:
            return base + "brick_left.png";

        case State::RightHalf:
            return base + "brick_right.png";

        case State::TopLeftQuarter:
            return base + "brick_q1.png";

        case State::TopRightQuarter:
            return base + "brick_q2.png";

        case State::BottomLeftQuarter:
            return base + "brick_q3.png";

        case State::BottomRightQuarter:
            return base + "brick_q4.png";

        case State::Destroyed:
            return base + "brick_full.png";
    }

    return base + "brick_full.png";
}

void Brick::Clear() {
    if (m_Image) {
        m_Root.RemoveChild(m_Image);
        m_Image.reset();
    }
}

float Brick::GetX() const {
    return m_X;
}

float Brick::GetY() const {
    return m_Y;
}