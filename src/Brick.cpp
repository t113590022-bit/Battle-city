//
// Created by link9 on 2026/5/5.
//

#include "Brick.hpp"

#include <algorithm>

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
      m_TileSize(tileSize) {

    InitMask(initialState);
    CreatePieces();
    UpdatePiecesVisibility();
}

void Brick::InitMask(State initialState) {
    for (int row = 0; row < PieceRows; ++row) {
        for (int col = 0; col < PieceCols; ++col) {
            m_Solid[row][col] = false;
        }
    }

    switch (initialState) {
        case State::Full:
            for (int row = 0; row < PieceRows; ++row) {
                for (int col = 0; col < PieceCols; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::TopHalf:
            for (int row = 0; row <= 1; ++row) {
                for (int col = 0; col < PieceCols; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::BottomHalf:
            for (int row = 2; row <= 3; ++row) {
                for (int col = 0; col < PieceCols; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::LeftHalf:
            for (int row = 0; row < PieceRows; ++row) {
                for (int col = 0; col <= 1; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::RightHalf:
            for (int row = 0; row < PieceRows; ++row) {
                for (int col = 2; col <= 3; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::TopLeftQuarter:
            for (int row = 0; row <= 1; ++row) {
                for (int col = 0; col <= 1; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::TopRightQuarter:
            for (int row = 0; row <= 1; ++row) {
                for (int col = 2; col <= 3; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::BottomLeftQuarter:
            for (int row = 2; row <= 3; ++row) {
                for (int col = 0; col <= 1; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::BottomRightQuarter:
            for (int row = 2; row <= 3; ++row) {
                for (int col = 2; col <= 3; ++col) {
                    m_Solid[row][col] = true;
                }
            }
            break;

        case State::Destroyed:
            break;
    }
}

void Brick::CreatePieces() {
    for (int row = 0; row < PieceRows; ++row) {
        for (int col = 0; col < PieceCols; ++col) {
            // 初始不存在的格子不用建立 Character，減少物件數量
            if (!m_Solid[row][col]) {
                m_Pieces[row][col] = nullptr;
                continue;
            }

            auto piece = std::make_shared<Character>(
                GetPieceImagePath(row, col)
            );

            // 你的 16 張圖片是 32x32 透明定位圖，所以全部放在 tile 中心
            piece->SetPosition({m_X, m_Y});
            piece->SetZIndex(1.0f);
            piece->SetVisible(true);

            m_Root.AddChild(piece);
            m_Pieces[row][col] = piece;
        }
    }
}

std::string Brick::GetPieceImagePath(int row, int col) const {
    std::string base = std::string(RESOURCE_DIR) + "/image/map/brick/";

    return base + "brick_" +
           std::to_string(row) +
           std::to_string(col) +
           ".png";
}

void Brick::UpdatePiecesVisibility() {
    for (int row = 0; row < PieceRows; ++row) {
        for (int col = 0; col < PieceCols; ++col) {
            if (m_Pieces[row][col]) {
                m_Pieces[row][col]->SetVisible(m_Solid[row][col]);
            }
        }
    }
}

bool Brick::BlocksTank() const {
    return !IsDestroyed();
}

bool Brick::BlocksBullet() const {
    return !IsDestroyed();
}

bool Brick::BlocksTankAtPoint(const glm::vec2& point) const {
    return ContainsSolidPart(point);
}

bool Brick::IsDestroyed() const {
    for (int row = 0; row < PieceRows; ++row) {
        for (int col = 0; col < PieceCols; ++col) {
            if (m_Solid[row][col]) {
                return false;
            }
        }
    }

    return true;
}

bool Brick::ContainsSolidPart(const glm::vec2& point) const {
    int row = 0;
    int col = 0;

    if (!GetPieceIndexFromPoint(point, row, col)) {
        return false;
    }

    return m_Solid[row][col];
}

float Brick::GetLocalX(const glm::vec2& point) const {
    const float left = m_X - m_TileSize / 2.0f;
    return point.x - left;
}

float Brick::GetLocalYFromTop(const glm::vec2& point) const {
    const float top = m_Y + m_TileSize / 2.0f;
    return top - point.y;
}

bool Brick::GetPieceIndexFromPoint(
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

    const float pieceSize = m_TileSize / 4.0f; // 8

    outCol = static_cast<int>(localX / pieceSize);
    outRow = static_cast<int>(localYFromTop / pieceSize);

    if (outRow < 0 || outRow >= PieceRows) {
        return false;
    }

    if (outCol < 0 || outCol >= PieceCols) {
        return false;
    }

    return true;
}

int Brick::ClampInt(int value, int minValue, int maxValue) const {
    if (value < minValue) {
        return minValue;
    }

    if (value > maxValue) {
        return maxValue;
    }

    return value;
}

float Brick::ColToWorldX(int col) const {
    const float pieceSize = m_TileSize / 4.0f;
    const float left = m_X - m_TileSize / 2.0f;

    return left + pieceSize / 2.0f + col * pieceSize;
}

float Brick::RowToWorldY(int row) const {
    const float pieceSize = m_TileSize / 4.0f;
    const float top = m_Y + m_TileSize / 2.0f;

    return top - pieceSize / 2.0f - row * pieceSize;
}

void Brick::ClearCell(int row, int col) {
    if (row < 0 || row >= PieceRows) {
        return;
    }

    if (col < 0 || col >= PieceCols) {
        return;
    }

    m_Solid[row][col] = false;
}

void Brick::ClearRect(int rowStart, int rowEnd, int colStart, int colEnd) {
    rowStart = std::max(0, rowStart);
    rowEnd = std::min(PieceRows - 1, rowEnd);

    colStart = std::max(0, colStart);
    colEnd = std::min(PieceCols - 1, colEnd);

    for (int row = rowStart; row <= rowEnd; ++row) {
        for (int col = colStart; col <= colEnd; ++col) {
            ClearCell(row, col);
        }
    }
}

bool Brick::FindSolidHitPoint(
    const TileHitInfo& hit,
    glm::vec2& outHitPoint
) const {
    // 原本命中點就是實體，直接用
    if (ContainsSolidPart(hit.hitPoint)) {
        outHitPoint = hit.hitPoint;
        return true;
    }

    int hitRow = 0;
    int hitCol = 0;

    if (!GetPieceIndexFromPoint(hit.hitPoint, hitRow, hitCol)) {
        return false;
    }

    const float localX = GetLocalX(hit.hitPoint);
    const float localY = GetLocalYFromTop(hit.hitPoint);

    const bool verticalCenterHit = (localX >= 8.0f && localX < 24.0f);
    const bool horizontalCenterHit = (localY >= 8.0f && localY < 24.0f);

    const bool hitLeftSide = (localX < 16.0f);
    const bool hitTopSide = (localY < 16.0f);

    int rowStart = 0;
    int rowEnd = PieceRows - 1;
    int colStart = 0;
    int colEnd = PieceCols - 1;

    switch (hit.direction) {
        case HitDirection::UP: {
            if (!verticalCenterHit) {
                colStart = hitLeftSide ? 0 : 2;
                colEnd = hitLeftSide ? 1 : 3;
            }

            for (int row = hitRow; row >= 0; --row) {
                for (int col = colStart; col <= colEnd; ++col) {
                    if (m_Solid[row][col]) {
                        outHitPoint = {
                            hit.hitPoint.x,
                            RowToWorldY(row)
                        };
                        return true;
                    }
                }
            }

            return false;
        }

        case HitDirection::DOWN: {
            if (!verticalCenterHit) {
                colStart = hitLeftSide ? 0 : 2;
                colEnd = hitLeftSide ? 1 : 3;
            }

            for (int row = hitRow; row < PieceRows; ++row) {
                for (int col = colStart; col <= colEnd; ++col) {
                    if (m_Solid[row][col]) {
                        outHitPoint = {
                            hit.hitPoint.x,
                            RowToWorldY(row)
                        };
                        return true;
                    }
                }
            }

            return false;
        }

        case HitDirection::LEFT: {
            if (!horizontalCenterHit) {
                rowStart = hitTopSide ? 0 : 2;
                rowEnd = hitTopSide ? 1 : 3;
            }

            for (int col = hitCol; col >= 0; --col) {
                for (int row = rowStart; row <= rowEnd; ++row) {
                    if (m_Solid[row][col]) {
                        outHitPoint = {
                            ColToWorldX(col),
                            hit.hitPoint.y
                        };
                        return true;
                    }
                }
            }

            return false;
        }

        case HitDirection::RIGHT: {
            if (!horizontalCenterHit) {
                rowStart = hitTopSide ? 0 : 2;
                rowEnd = hitTopSide ? 1 : 3;
            }

            for (int col = hitCol; col < PieceCols; ++col) {
                for (int row = rowStart; row <= rowEnd; ++row) {
                    if (m_Solid[row][col]) {
                        outHitPoint = {
                            ColToWorldX(col),
                            hit.hitPoint.y
                        };
                        return true;
                    }
                }
            }

            return false;
        }
    }

    return false;
}

void Brick::DestroyPiecesByHit(const TileHitInfo& hit) {
    int hitRow = 0;
    int hitCol = 0;

    if (!GetPieceIndexFromPoint(hit.hitPoint, hitRow, hitCol)) {
        return;
    }

    const bool isMaxPower = (hit.power >= 2);

    const float localX = GetLocalX(hit.hitPoint);
    const float localY = GetLocalYFromTop(hit.hitPoint);

    const bool verticalCenterHit = (localX >= 8.0f && localX < 24.0f);
    const bool horizontalCenterHit = (localY >= 8.0f && localY < 24.0f);

    const bool hitLeftSide = (localX < 16.0f);
    const bool hitTopSide = (localY < 16.0f);

    switch (hit.direction) {
        case HitDirection::UP: {
            // 子彈往上，從下方打
            if (isMaxPower) {
                if (verticalCenterHit) {
                    int rowStart = ClampInt(hitRow - 1, 0, PieceRows - 2);
                    ClearRect(rowStart, rowStart + 1, 0, 3);
                } else {
                    int rowStart = ClampInt(hitRow - 1, 0, PieceRows - 2);
                    ClearRect(
                        rowStart,
                        rowStart + 1,
                        hitLeftSide ? 0 : 2,
                        hitLeftSide ? 1 : 3
                    );
                }
            } else {
                if (verticalCenterHit) {
                    // 普通彈打正中：少一整排
                    ClearRect(hitRow, hitRow, 0, 3);
                } else {
                    // 普通彈打側邊：少 1x2
                    ClearRect(
                        hitRow,
                        hitRow,
                        hitLeftSide ? 0 : 2,
                        hitLeftSide ? 1 : 3
                    );
                }
            }

            break;
        }

        case HitDirection::DOWN: {
            // 子彈往下，從上方打
            if (isMaxPower) {
                if (verticalCenterHit) {
                    int rowStart = ClampInt(hitRow, 0, PieceRows - 2);
                    ClearRect(rowStart, rowStart + 1, 0, 3);
                } else {
                    int rowStart = ClampInt(hitRow, 0, PieceRows - 2);
                    ClearRect(
                        rowStart,
                        rowStart + 1,
                        hitLeftSide ? 0 : 2,
                        hitLeftSide ? 1 : 3
                    );
                }
            } else {
                if (verticalCenterHit) {
                    ClearRect(hitRow, hitRow, 0, 3);
                } else {
                    ClearRect(
                        hitRow,
                        hitRow,
                        hitLeftSide ? 0 : 2,
                        hitLeftSide ? 1 : 3
                    );
                }
            }

            break;
        }

        case HitDirection::LEFT: {
            // 子彈往左，從右側打
            if (isMaxPower) {
                if (horizontalCenterHit) {
                    int colStart = ClampInt(hitCol - 1, 0, PieceCols - 2);
                    ClearRect(0, 3, colStart, colStart + 1);
                } else {
                    int colStart = ClampInt(hitCol - 1, 0, PieceCols - 2);
                    ClearRect(
                        hitTopSide ? 0 : 2,
                        hitTopSide ? 1 : 3,
                        colStart,
                        colStart + 1
                    );
                }
            } else {
                if (horizontalCenterHit) {
                    // 普通彈打正中：少一整列
                    ClearRect(0, 3, hitCol, hitCol);
                } else {
                    // 普通彈打側邊：少 2x1
                    ClearRect(
                        hitTopSide ? 0 : 2,
                        hitTopSide ? 1 : 3,
                        hitCol,
                        hitCol
                    );
                }
            }

            break;
        }

        case HitDirection::RIGHT: {
            // 子彈往右，從左側打
            if (isMaxPower) {
                if (horizontalCenterHit) {
                    int colStart = ClampInt(hitCol, 0, PieceCols - 2);
                    ClearRect(0, 3, colStart, colStart + 1);
                } else {
                    int colStart = ClampInt(hitCol, 0, PieceCols - 2);
                    ClearRect(
                        hitTopSide ? 0 : 2,
                        hitTopSide ? 1 : 3,
                        colStart,
                        colStart + 1
                    );
                }
            } else {
                if (horizontalCenterHit) {
                    ClearRect(0, 3, hitCol, hitCol);
                } else {
                    ClearRect(
                        hitTopSide ? 0 : 2,
                        hitTopSide ? 1 : 3,
                        hitCol,
                        hitCol
                    );
                }
            }

            break;
        }
    }
}

TileHitResult Brick::OnBulletHit(const TileHitInfo& hit) {
    TileHitResult result;

    if (IsDestroyed()) {
        return result;
    }

    glm::vec2 solidHitPoint = hit.hitPoint;

    if (!FindSolidHitPoint(hit, solidHitPoint)) {
        result.bulletStopped = false;
        result.spawnExplosion = false;
        result.tileDestroyed = false;
        return result;
    }

    TileHitInfo actualHit = hit;
    actualHit.hitPoint = solidHitPoint;

    result.bulletStopped = true;
    result.spawnExplosion = true;

    DestroyPiecesByHit(actualHit);
    UpdatePiecesVisibility();

    result.tileDestroyed = IsDestroyed();

    return result;
}

void Brick::Clear() {
    for (int row = 0; row < PieceRows; ++row) {
        for (int col = 0; col < PieceCols; ++col) {
            if (m_Pieces[row][col]) {
                m_Root.RemoveChild(m_Pieces[row][col]);
                m_Pieces[row][col].reset();
            }
        }
    }
}

float Brick::GetX() const {
    return m_X;
}

float Brick::GetY() const {
    return m_Y;
}