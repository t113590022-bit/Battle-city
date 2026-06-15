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
    // 32x32 磚塊切成 4x4，每格 8x8
    static constexpr int PieceRows = 4;
    static constexpr int PieceCols = 4;

private:
    void InitMask(State initialState);
    void CreatePieces();
    void UpdatePiecesVisibility();

    bool IsDestroyed() const;
    bool ContainsSolidPart(const glm::vec2& point) const;

    bool GetPieceIndexFromPoint(
        const glm::vec2& point,
        int& outRow,
        int& outCol
    ) const;

    bool FindSolidHitPoint(
        const TileHitInfo& hit,
        glm::vec2& outHitPoint
    ) const;

    void DestroyPiecesByHit(const TileHitInfo& hit);

    void ClearCell(int row, int col);
    void ClearRect(int rowStart, int rowEnd, int colStart, int colEnd);

    float GetLocalX(const glm::vec2& point) const;
    float GetLocalYFromTop(const glm::vec2& point) const;

    float ColToWorldX(int col) const;
    float RowToWorldY(int row) const;

    int ClampInt(int value, int minValue, int maxValue) const;

    std::string GetPieceImagePath(int row, int col) const;

private:
    Util::Renderer& m_Root;

    float m_X = 0.0f;
    float m_Y = 0.0f;
    int m_TileSize = 32;

    bool m_Solid[PieceRows][PieceCols] = {};
    std::shared_ptr<Character> m_Pieces[PieceRows][PieceCols] = {};
};


#endif // BRICK_HPP