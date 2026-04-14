//
// Created by link9 on 2026/3/25.
//

#ifndef MAP_HPP
#define MAP_HPP

#include <memory>      // shared_ptr
#include <string>      // std::string
#include <vector>      // std::vector


#include "Util/Renderer.hpp"
#include "Character.hpp"


class Map {
public:
    explicit Map(Util::Renderer& root);

    void Init();
    // void DrawFill();
    // void DrawBorderTest();
    void Clear();

    int GetTile(int row, int col) const;
    bool IsBlocked(int row, int col) const;
    bool HitTile(float x, float y);
    bool IsBlockedAtWorld(float x, float y) const;

    int WorldToCol(float x) const;
    int WorldToRow(float y) const;

    float ColToWorldX(int col) const;
    float RowToWorldY(int row) const;

    float GetLeft() const;
    float GetRight() const;
    float GetTop() const;
    float GetBottom() const;

    //黑色可活動範圍（給玩家/子彈用）
    float GetPlayLeft() const;
    float GetPlayRight() const;
    float GetPlayTop() const;
    float GetPlayBottom() const;

    int GetTileSize() const;
    int GetRowCount() const;
    int GetColCount() const;

private:
    void BuildTestMapData();
    void DrawMap();

    std::string GetPlayingBgPath() const;
    std::string GetBrickWallPath() const;
    std::string GetSteelWallPath() const;

    bool IsInsideMapData(int row, int col) const;

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_PlayingBg;
    // 跟 m_MapData 對應的 sprite 陣列
    std::vector<std::vector<std::shared_ptr<Character>>> m_MapTiles;

    // 0 = empty, 1 = brick, 2 = steel
    std::vector<std::vector<int>> m_MapData;

    int m_TileSize = 32;

    float m_MapLeft = -416.0f;
    float m_MapRight = 416.0f;
    float m_MapTop = 416.0f;
    float m_MapBottom = -416.0f;

    // 黑色可活動範圍（給玩家/子彈用）
    float m_PlayAreaLeft = -412.0f;
    float m_PlayAreaRight = 412.0f;
    float m_PlayAreaTop = 412.0f;
    float m_PlayAreaBottom = -412.0f;
};
#endif //MAP_HPP