#include "Map.hpp"

Map::Map(Util::Renderer& root)
    : m_Root(root) {}

void Map::Init() {
    m_PlayingBg = std::make_shared<Character>(GetPlayingBgPath());
    m_PlayingBg->SetPosition({0.0f, 0.0f});
    m_PlayingBg->SetZIndex(0.0f);   // 背景最底
    m_Root.AddChild(m_PlayingBg);

    BuildTestMapData();
    DrawMap();
}

void Map::BuildTestMapData() {
    // 0 = empty, 1 = brick, 2 = steel
    m_MapData = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
        {2,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2,2},

        {2,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2,2},
        {0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0},

        // ===== 基地防護牆 =====
        {0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0},

        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},

        // ===== 玩家活動區 =====
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}
    };
}


void Map::DrawMap() {
    m_MapTiles.clear();
    m_MapTiles.resize(
        GetRowCount(),
        std::vector<std::shared_ptr<Character>>(GetColCount(), nullptr)
    );

    for (int row = 0; row < GetRowCount(); ++row) {
        for (int col = 0; col < GetColCount(); ++col) {
            int tileType = m_MapData[row][col];
            if (tileType == 0) continue;

            std::shared_ptr<Character> tile = nullptr;

            if (tileType == 1) {
                tile = std::make_shared<Character>(GetBrickWallPath());
            }
            else if (tileType == 2) {
                tile = std::make_shared<Character>(GetSteelWallPath());
            }

            if (!tile) continue;

            tile->SetPosition({ColToWorldX(col), RowToWorldY(row)});
            tile->SetZIndex(1.0f),
            m_Root.AddChild(tile);
            m_MapTiles[row][col] = tile;
        }
    }
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
    if (!IsInsideMapData(row, col)) return -1;
    return m_MapData[row][col];
}

bool Map::IsInsideMapData(int row, int col) const {
    if (row < 0 || row >= static_cast<int>(m_MapData.size())) return false;
    if (col < 0 || col >= static_cast<int>(m_MapData[row].size())) return false;
    return true;
}

bool Map::IsBlocked(int row, int col) const {
    int tile = GetTile(row, col);
    return tile == 1 || tile == 2;
}

bool Map::HitTile(float x, float y) {
    int col = WorldToCol(x);
    int row = WorldToRow(y);

    if (!IsInsideMapData(row, col)) {
        return false;   // 超出目前 tile map，不算打到地圖
    }

    int tileType = GetTile(row, col);

    if (tileType == 0) {
        return false;
    }

    if (tileType == 1) {
        // brick：打掉
        m_MapData[row][col] = 0;

        // if (row >= 0 && row < static_cast<int>(m_MapTiles.size()) &&
        //     col >= 0 && col < static_cast<int>(m_MapTiles[row].size()) &&
        //     m_MapTiles[row][col]) {
        //     m_Root.RemoveChild(m_MapTiles[row][col]);
        //     m_MapTiles[row][col].reset();
        //     }

        if (m_MapTiles[row][col]) {
            m_Root.RemoveChild(m_MapTiles[row][col]);
            m_MapTiles[row][col].reset();
        }

        return true;
    }

    if (tileType == 2) {
        // steel：擋住，但不消失
        return true;
    }

    return false;
}

bool Map::IsBlockedAtWorld(float x, float y) const {
    int col = WorldToCol(x);
    int row = WorldToRow(y);
    return IsBlocked(row, col);
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

std::string Map::GetSteelWallPath() const {
    return std::string(RESOURCE_DIR) + "/image/map/steel.png";
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

    for (auto& row : m_MapTiles) {
        for (auto& tile : row) {
            if (tile) {
                m_Root.RemoveChild(tile);
            }
        }
    }

    m_MapTiles.clear();
    m_MapData.clear();
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