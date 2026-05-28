//
// Created by link9 on 2026/3/25.
//

#ifndef MAP_HPP
#define MAP_HPP

#include <memory>      // shared_ptr
#include <string>      // std::string
#include <vector>      // std::vector
#include <glm/glm.hpp>

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "TileObject.hpp"
#include "Brick.hpp"
#include "Steel.hpp"
#include "Grass.hpp"
#include "Water.hpp"

class Map {
public:
    explicit Map(Util::Renderer& root);

    void Init(const std::vector<std::string>& stageData);
    void Clear();
    void Update();

    int GetTile(int row, int col) const;
    // bool HitTile(float x, float y);

    bool IsBlocked(int row, int col) const;
    bool IsBlockedAtWorld(float x, float y) const;

    TileObject* GetTileObject(int row, int col) const;
    TileObject* GetTileObjectAtWorld(float x, float y) const;

    void RemoveTileAtWorld(float x, float y);

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

    // 工兵鏟道具
    void SetBaseProtectionSteel();
    void SetBaseProtectionBrick();
    void RestoreBaseProtection();

    bool TryGetRandomOriginalEmptyPosition(glm::vec2& outPos) const;

private:
    // void BuildTestMapData();
    void DrawMap();

    std::string GetPlayingBgPath() const;

    bool IsInsideMapData(int row, int col) const;

    // 工兵鏟
    void SetTileCode(int row, int col, char code);

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_PlayingBg;

    // 15 關可換這個資料
    std::vector<std::string> m_StageData;
    std::vector<std::string> m_OriginalStageData;

    // 每一格真正的地圖物件
    std::vector<std::vector<std::shared_ptr<TileObject>>> m_Tiles;

    std::vector<std::shared_ptr<Water>> m_WaterTiles;

    int m_TileSize = 32;

    float m_MapLeft = -208.0f;
    float m_MapRight = 208.0f;
    float m_MapTop = 208.0f;
    float m_MapBottom = -208.0f;

    // 黑色可活動範圍（給玩家/子彈用）
    float m_PlayAreaLeft = -206.0f;
    float m_PlayAreaRight = 206.0f;
    float m_PlayAreaTop = 206.0f;
    float m_PlayAreaBottom = -206.0f;
};
#endif //MAP_HPP