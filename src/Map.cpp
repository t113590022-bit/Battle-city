#include "Map.hpp"

Map::Map(Util::Renderer& root)
    : m_Root(root) {}

void Map::Init() {
    m_PlayingBg = std::make_shared<Character>(GetPlayingBgPath());
    m_PlayingBg->SetPosition({0.0f, 0.0f});
    m_Root.AddChild(m_PlayingBg);

    DrawMap();
}

void Map::DrawMap() {
    auto tile = std::make_shared<Character>(GetBrickWallPath());
    tile->SetPosition({0.0f, 0.0f});
    m_Root.AddChild(tile);
    m_MapTiles.push_back(tile);
}

int Map::WorldToCol(float x) const {
    return static_cast<int>((x - m_MapLeft) / m_TileSize);
}

int Map::WorldToRow(float y) const {
    return static_cast<int>((m_MapTop - y) / m_TileSize);
}

float Map::ColToWorldX(int col) const {
    return m_MapLeft + col * m_TileSize + m_TileSize / 2.0f;
}

float Map::RowToWorldY(int row) const {
    return m_MapTop - row * m_TileSize - m_TileSize / 2.0f;
}

int Map::GetTile(int row, int col) const {
    if (row < 0 || row >= static_cast<int>(m_MapData.size())) return 1;
    if (col < 0 || col >= static_cast<int>(m_MapData[row].size())) return 1;
    return m_MapData[row][col];
}

bool Map::IsBlocked(int row, int col) const {
    return GetTile(row, col) != 0;
}

int Map::GetTileSize() const {
    return m_TileSize;
}

int Map::GetRowCount() const {
    return static_cast<int>(m_MapData.size());
}

int Map::GetColCount() const {
    if (m_MapData.empty()) return 0;
    return static_cast<int>(m_MapData[0].size());
}

std::string Map::GetPlayingBgPath() const {
    return std::string(RESOURCE_DIR) + "/image/map/stage_bg.png";
}

std::string Map::GetBrickWallPath() const {
    return std::string(RESOURCE_DIR) + "/image/map/brick.png";
}

// void Map::DrawFill() {
//     const int TILE = 16;
//
//     for (float y = m_MapTop; y >= m_MapBottom; y -= TILE) {
//         for (float x = m_MapLeft; x <= m_MapRight; x += TILE) {
//             auto tile = std::make_shared<Character>(GetBrickWallPath());
//             tile->SetPosition({x, y});
//             m_Root.AddChild(tile);
//             m_MapTiles.push_back(tile);
//         }
//     }
// }
//
// void Map::DrawBorderTest() {
//     const int TILE = 16;
//
//     for (float x = m_MapLeft; x <= m_MapRight; x += TILE) {
//         auto tile = std::make_shared<Character>(GetBrickWallPath());
//         tile->SetPosition({x, m_MapTop});
//         m_Root.AddChild(tile);
//         m_MapTiles.push_back(tile);
//     }
//
//     for (float x = m_MapLeft; x <= m_MapRight; x += TILE) {
//         auto tile = std::make_shared<Character>(GetBrickWallPath());
//         tile->SetPosition({x, m_MapBottom});
//         m_Root.AddChild(tile);
//         m_MapTiles.push_back(tile);
//     }
//
//     for (float y = m_MapTop; y >= m_MapBottom; y -= TILE) {
//         auto tile = std::make_shared<Character>(GetBrickWallPath());
//         tile->SetPosition({m_MapLeft, y});
//         m_Root.AddChild(tile);
//         m_MapTiles.push_back(tile);
//     }
//
//     for (float y = m_MapTop; y >= m_MapBottom; y -= TILE) {
//         auto tile = std::make_shared<Character>(GetBrickWallPath());
//         tile->SetPosition({m_MapRight, y});
//         m_Root.AddChild(tile);
//         m_MapTiles.push_back(tile);
//     }
// }

void Map::Clear() {
    if (m_PlayingBg) {
        m_Root.RemoveChild(m_PlayingBg);
        m_PlayingBg.reset();
    }

    for (auto& tile : m_MapTiles) {
        m_Root.RemoveChild(tile);
    }
    m_MapTiles.clear();
}

float Map::GetLeft() const {
    return m_MapLeft;
}

float Map::GetRight() const {
    return m_MapRight;
}

float Map::GetTop() const {
    return m_MapTop;
}

float Map::GetBottom() const {
    return m_MapBottom;
}

float Map::GetPlayLeft() const {
    return m_PlayAreaLeft;
}

float Map::GetPlayRight() const {
    return m_PlayAreaRight;
}

float Map::GetPlayTop() const {
    return m_PlayAreaTop;
}

float Map::GetPlayBottom() const {
    return m_PlayAreaBottom;
}