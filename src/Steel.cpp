//
// Created by link9 on 2026/5/5.
//

#include "Steel.hpp"

Steel::Steel(
    Util::Renderer& root,
    float x,
    float y,
    int tileSize,
    State initialState
)
    : m_Root(root),
      m_X(x),
      m_Y(y),
      m_TileSize(tileSize) {

    InitMask(initialState);
    CreatePieces();
    UpdatePiecesVisibility();
}

void Steel::InitMask(State initialState) {
    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Cols; ++col) {
            m_Solid[row][col] = false;
        }
    }

    switch (initialState) {
        case State::Full:
            m_Solid[0][0] = true;
            m_Solid[0][1] = true;
            m_Solid[1][0] = true;
            m_Solid[1][1] = true;
            break;

        case State::TopHalf:
            m_Solid[0][0] = true;
            m_Solid[0][1] = true;
            break;

        case State::BottomHalf:
            m_Solid[1][0] = true;
            m_Solid[1][1] = true;
            break;

        case State::LeftHalf:
            m_Solid[0][0] = true;
            m_Solid[1][0] = true;
            break;

        case State::RightHalf:
            m_Solid[0][1] = true;
            m_Solid[1][1] = true;
            break;

        case State::TopLeftQuarter:
            m_Solid[0][0] = true;
            break;

        case State::TopRightQuarter:
            m_Solid[0][1] = true;
            break;

        case State::BottomLeftQuarter:
            m_Solid[1][0] = true;
            break;

        case State::BottomRightQuarter:
            m_Solid[1][1] = true;
            break;

        case State::Destroyed:
            break;
    }
}

void Steel::CreatePieces() {
    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Cols; ++col) {
            auto piece = std::make_shared<Character>(
                GetPieceImagePath(row, col)
            );

            // 如果 steel_q1 ~ steel_q4 是 32x32 透明定位圖，全部放在中心
            piece->SetPosition({m_X, m_Y});

            piece->SetZIndex(1.0f);
            piece->SetVisible(m_Solid[row][col]);

            m_Root.AddChild(piece);
            m_Pieces[row][col] = piece;
        }
    }
}

std::string Steel::GetPieceImagePath(int row, int col) const {
    std::string base = std::string(RESOURCE_DIR) + "/image/map/";

    if (row == 0 && col == 0) {
        return base + "steel_q1.png"; // 左上
    }

    if (row == 0 && col == 1) {
        return base + "steel_q2.png"; // 右上
    }

    if (row == 1 && col == 0) {
        return base + "steel_q3.png"; // 左下
    }

    return base + "steel_q4.png";     // 右下
}

void Steel::UpdatePiecesVisibility() {
    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Cols; ++col) {
            if (m_Pieces[row][col]) {
                m_Pieces[row][col]->SetVisible(m_Solid[row][col]);
            }
        }
    }
}

bool Steel::BlocksTank() const {
    return !IsDestroyed();
}

bool Steel::BlocksBullet() const {
    return !IsDestroyed();
}

bool Steel::BlocksTankAtPoint(const glm::vec2& point) const {
    return ContainsSolidPart(point);
}

TileHitResult Steel::OnBulletHit(const TileHitInfo& hit) {
    TileHitResult result;

    if (IsDestroyed()) {
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

    // 穿甲彈才會破壞鋼鐵
    DestroyByPiercingHit(hit);
    UpdatePiecesVisibility();

    result.tileDestroyed = IsDestroyed();

    return result;
}

bool Steel::IsDestroyed() const {
    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Cols; ++col) {
            if (m_Solid[row][col]) {
                return false;
            }
        }
    }

    return true;
}

bool Steel::ContainsSolidPart(const glm::vec2& point) const {
    int row = 0;
    int col = 0;

    if (!GetPieceIndexFromPoint(point, row, col)) {
        return false;
    }

    return m_Solid[row][col];
}

float Steel::GetLocalX(const glm::vec2& point) const {
    const float left = m_X - m_TileSize / 2.0f;
    return point.x - left;
}

float Steel::GetLocalYFromTop(const glm::vec2& point) const {
    const float top = m_Y + m_TileSize / 2.0f;
    return top - point.y;
}

bool Steel::GetPieceIndexFromPoint(
    const glm::vec2& point,
    int& outRow,
    int& outCol
) const {
    float localX = GetLocalX(point);
    float localYFromTop = GetLocalYFromTop(point);

    if (localX < 0.0f || localX >= m_TileSize) {
        return false;
    }

    if (localYFromTop < 0.0f || localYFromTop >= m_TileSize) {
        return false;
    }

    const float pieceSize = m_TileSize / 2.0f; // 16

    outCol = static_cast<int>(localX / pieceSize);
    outRow = static_cast<int>(localYFromTop / pieceSize);

    if (outRow < 0 || outRow >= Rows) {
        return false;
    }

    if (outCol < 0 || outCol >= Cols) {
        return false;
    }

    return true;
}

void Steel::DestroyByPiercingHit(const TileHitInfo& hit) {
    int row = 0;
    int col = 0;

    if (!GetPieceIndexFromPoint(hit.hitPoint, row, col)) {
        return;
    }

    float localX = GetLocalX(hit.hitPoint);
    float localYFromTop = GetLocalYFromTop(hit.hitPoint);

    // 32x32 鋼磚中間區域：8 ~ 24
    // 打中這個範圍，視為打正中間，消半磚
    const bool verticalCenterHit =
        localX >= 8.0f && localX < 24.0f;

    const bool horizontalCenterHit =
        localYFromTop >= 8.0f && localYFromTop < 24.0f;

    switch (hit.direction) {
        case HitDirection::UP:
        case HitDirection::DOWN:
            if (verticalCenterHit) {
                // 上下方向打正中間：消掉目前命中的整排
                ClearRect(row, row, 0, 1);
            } else {
                // 上下方向打偏：只消掉目前命中的 1/4
                ClearCell(row, col);
            }
            break;

        case HitDirection::LEFT:
        case HitDirection::RIGHT:
            if (horizontalCenterHit) {
                // 左右方向打正中間：消掉目前命中的整列
                ClearRect(0, 1, col, col);
            } else {
                // 左右方向打偏：只消掉目前命中的 1/4
                ClearCell(row, col);
            }
            break;
    }
}

void Steel::ClearCell(int row, int col) {
    if (row < 0 || row >= Rows) {
        return;
    }

    if (col < 0 || col >= Cols) {
        return;
    }

    m_Solid[row][col] = false;
}

void Steel::ClearRect(int rowStart, int rowEnd, int colStart, int colEnd) {
    if (rowStart < 0) rowStart = 0;
    if (rowEnd >= Rows) rowEnd = Rows - 1;

    if (colStart < 0) colStart = 0;
    if (colEnd >= Cols) colEnd = Cols - 1;

    for (int row = rowStart; row <= rowEnd; ++row) {
        for (int col = colStart; col <= colEnd; ++col) {
            ClearCell(row, col);
        }
    }
}

void Steel::Clear() {
    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Cols; ++col) {
            if (m_Pieces[row][col]) {
                m_Root.RemoveChild(m_Pieces[row][col]);
                m_Pieces[row][col].reset();
            }
        }
    }
}

float Steel::GetX() const {
    return m_X;
}

float Steel::GetY() const {
    return m_Y;
}