//
// Created by link9 on 2026/5/5.
//

#include "Steel.hpp"

Steel::Steel(Util::Renderer& root, float x, float y, int tileSize, State initialState)
    : m_Root(root),
      m_X(x),
      m_Y(y),
      m_TileSize(tileSize),
      m_State(initialState)  {

    m_Image = std::make_shared<Character>(GetImagePathByState());
    m_Image->SetPosition({m_X, m_Y});
    m_Image->SetZIndex(1.0f);
    m_Image->SetVisible(true);

    m_Root.AddChild(m_Image);
}

bool Steel::BlocksTank() const {
    return m_State != State::Destroyed;
}

bool Steel::BlocksBullet() const {
    return m_State != State::Destroyed;
}

bool Steel::BlocksTankAtPoint(const glm::vec2& point) const {
    return ContainsSolidPart(point);
}

TileHitResult Steel::OnBulletHit(const TileHitInfo& hit) {
    TileHitResult result;

    if (m_State == State::Destroyed) {
        return result;
    }

    if (!ContainsSolidPart(hit.hitPoint)) {
        result.bulletStopped = false;
        result.spawnExplosion = false;
        result.tileDestroyed = false;
        return result;
    }

    // 只要打到鋼鐵實體，子彈都會停
    result.bulletStopped = true;
    result.spawnExplosion = true;

    // 普通子彈不能破壞鋼鐵
    if (hit.power < 2) {
        result.tileDestroyed = false;
        return result;
    }

    // 穿甲彈可以破壞鋼鐵
    if (m_State == State::Full) {
        m_State = GetHalfStateAfterHit(hit);
        UpdateImage();
        return result;
    }

    // 半鐵 / 1/4 鐵再被穿甲彈打到實體部分，消失
    m_State = State::Destroyed;
    UpdateImage();

    result.tileDestroyed = true;
    return result;
}

bool Steel::ContainsSolidPart(const glm::vec2& point) const {
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

Steel::State Steel::GetHalfStateAfterHit(const TileHitInfo& hit) const {
    float left = m_X - m_TileSize / 2.0f;
    float top = m_Y + m_TileSize / 2.0f;

    float localX = hit.hitPoint.x - left;
    float localYFromTop = top - hit.hitPoint.y;

    float half = m_TileSize / 2.0f;

    switch (hit.direction) {
        case HitDirection::UP:
        case HitDirection::DOWN:
            if (localYFromTop < half) {
                return State::BottomHalf;
            } else {
                return State::TopHalf;
            }

        case HitDirection::LEFT:
        case HitDirection::RIGHT:
            if (localX < half) {
                return State::RightHalf;
            } else {
                return State::LeftHalf;
            }
    }

    return State::Full;
}

void Steel::UpdateImage() {
    if (!m_Image) return;

    if (m_State == State::Destroyed) {
        m_Image->SetVisible(false);
        return;
    }

    m_Image->SetImage(GetImagePathByState());
    m_Image->SetVisible(true);
}

std::string Steel::GetImagePathByState() const {
    std::string base = std::string(RESOURCE_DIR) + "/image/map/";

    switch (m_State) {
        case State::Full:
            return base + "steel_full.png";

        case State::TopHalf:
            return base + "steel_top.png";

        case State::BottomHalf:
            return base + "steel_bottom.png";

        case State::LeftHalf:
            return base + "steel_left.png";

        case State::RightHalf:
            return base + "steel_right.png";

        case State::TopLeftQuarter:
            return base + "steel_q1.png";

        case State::TopRightQuarter:
            return base + "steel_q2.png";

        case State::BottomLeftQuarter:
            return base + "steel_q3.png";

        case State::BottomRightQuarter:
            return base + "steel_q4.png";

        case State::Destroyed:
            return base + "steel_full.png";
    }

    return base + "steel_full.png";
}

void Steel::Clear() {
    if (m_Image) {
        m_Root.RemoveChild(m_Image);
        m_Image.reset();
    }
}

float Steel::GetX() const {
    return m_X;
}

float Steel::GetY() const {
    return m_Y;
}