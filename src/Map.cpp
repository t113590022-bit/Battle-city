#include "Map.hpp"
#include <iostream>
#include <random>


static bool IsBrickCode(char code) {
    return code == 'B' ||
           code == 'T' ||
           code == 'D' ||
           code == 'L' ||
           code == 'R' ||
           code == '1' ||
           code == '2' ||
           code == '3' ||
           code == '4';
}

static bool IsSteelCode(char code) {
    return code == 'S' ||
           code == 't' ||
           code == 'd' ||
           code == 'l' ||
           code == 'r' ||
           code == '5' ||
           code == '6' ||
           code == '7' ||
           code == '8';
}

static Brick::State GetBrickStateFromCode(char code) {
    switch (code) {
        case 'T':
            return Brick::State::TopHalf;

        case 'D':
            return Brick::State::BottomHalf;

        case 'L':
            return Brick::State::LeftHalf;

        case 'R':
            return Brick::State::RightHalf;

        case '1':
            return Brick::State::TopLeftQuarter;

        case '2':
            return Brick::State::TopRightQuarter;

        case '3':
            return Brick::State::BottomLeftQuarter;

        case '4':
            return Brick::State::BottomRightQuarter;

        case 'B':
        default:
            return Brick::State::Full;
    }
}

static Steel::State GetSteelStateFromCode(char code) {
    switch (code) {
        case 't':
            return Steel::State::TopHalf;

        case 'd':
            return Steel::State::BottomHalf;

        case 'l':
            return Steel::State::LeftHalf;

        case 'r':
            return Steel::State::RightHalf;

        case '5':
            return Steel::State::TopLeftQuarter;

        case '6':
            return Steel::State::TopRightQuarter;

        case '7':
            return Steel::State::BottomLeftQuarter;

        case '8':
            return Steel::State::BottomRightQuarter;

        case 'S':
        default:
            return Steel::State::Full;
    }
}

Map::Map(Util::Renderer& root)
    : m_Root(root) {}

void Map::Init(const std::vector<std::string>& stageData) {
    m_PlayingBg = std::make_shared<Character>(GetPlayingBgPath());
    m_PlayingBg->SetPosition({0.0f, 0.0f});
    m_PlayingBg->SetZIndex(0.0f);   // 背景最底
    m_Root.AddChild(m_PlayingBg);

    m_StageData = stageData;
    m_OriginalStageData = stageData;

    DrawMap();
}

void Map::DrawMap() {
    m_Tiles.clear();

    m_Tiles.resize(
        GetRowCount(),
        std::vector<std::shared_ptr<TileObject>>(GetColCount(), nullptr)
    );

    int brickCount = 0;
    int steelCount = 0;
    int grassCount = 0;
    int waterCount = 0;
    int emptyCount = 0;
    int unknownCount = 0;

    for (int row = 0; row < GetRowCount(); ++row) {
        for (int col = 0; col < GetColCount(); ++col) {
            char code = m_StageData[row][col];

            float x = ColToWorldX(col);
            float y = RowToWorldY(row);

            if (IsBrickCode(code)) {
                ++brickCount;

                m_Tiles[row][col] = std::make_shared<Brick>(
                    m_Root,
                    x,
                    y,
                    m_TileSize,
                    GetBrickStateFromCode(code)
                );
            }
            else if (IsSteelCode(code)) {
                ++steelCount;

                m_Tiles[row][col] = std::make_shared<Steel>(
                    m_Root,
                    x,
                    y,
                    m_TileSize,
                    GetSteelStateFromCode(code)
                );
            }
            else {
                switch (code) {
                    case 'G': {
                        ++grassCount;

                        m_Tiles[row][col] = std::make_shared<Grass>(
                            m_Root,
                            x,
                            y
                        );
                        break;
                    }

                    case 'W': {
                        ++waterCount;

                        auto water = std::make_shared<Water>(
                            m_Root,
                            x,
                            y
                        );

                        m_Tiles[row][col] = water;
                        m_WaterTiles.push_back(water);
                        break;
                    }

                    case '.': {
                        ++emptyCount;

                        m_Tiles[row][col] = nullptr;
                        break;
                    }

                    default: {
                        ++unknownCount;

                        m_Tiles[row][col] = nullptr;
                        break;
                    }
                }
            }
        }
    }

    std::cout << "Map rows = " << GetRowCount() << std::endl;
    std::cout << "Map cols = " << GetColCount() << std::endl;
    std::cout << "Brick count = " << brickCount << std::endl;
    std::cout << "Steel count = " << steelCount << std::endl;
    std::cout << "Grass count = " << grassCount << std::endl;
    std::cout << "Water count = " << waterCount << std::endl;
    std::cout << "Empty count = " << emptyCount << std::endl;
    std::cout << "Unknown count = " << unknownCount << std::endl;
}

void Map::Update() {
    for (auto& water : m_WaterTiles) {
        if (water) {
            water->Update();
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

    char code = m_StageData[row][col];

    switch (code) {
        case '.':
            return 0;

        case 'B':
        case 'T':
        case 'D':
        case 'L':
        case 'R':
        case '1':
        case '2':
        case '3':
        case '4':
            return 1;

        case 'S':
        case 't':
        case 'd':
        case 'l':
        case 'r':
        case '5':
        case '6':
        case '7':
        case '8':
            return 2;

        case 'G':
            return 3;

        case 'W':
            return 4;

        default:
            return -1;
    }
}

bool Map::IsInsideMapData(int row, int col) const {
    if (row < 0 || row >= static_cast<int>(m_StageData.size())) return false;
    if (col < 0 || col >= static_cast<int>(m_StageData[row].size())) return false;
    return true;
}

bool Map::IsBlocked(int row, int col) const {
    if (!IsInsideMapData(row, col)) {
        return true;
    }

    TileObject* tile = GetTileObject(row, col);

    if (!tile) {
        return false;
    }

    return tile->BlocksTank();
}

bool Map::IsBlockedAtWorld(float x, float y) const {
    int col = WorldToCol(x);
    int row = WorldToRow(y);

    if (!IsInsideMapData(row, col)) {
        return true;
    }

    TileObject* tile = GetTileObject(row, col);

    if (!tile) {
        return false;
    }

    return tile->BlocksTankAtPoint({x, y});
}

TileObject* Map::GetTileObject(int row, int col) const {
    if (!IsInsideMapData(row, col)) {
        return nullptr;
    }

    if (row >= static_cast<int>(m_Tiles.size())) {
        return nullptr;
    }

    if (col >= static_cast<int>(m_Tiles[row].size())) {
        return nullptr;
    }

    return m_Tiles[row][col].get();
}

TileObject* Map::GetTileObjectAtWorld(float x, float y) const {
    int col = WorldToCol(x);
    int row = WorldToRow(y);

    return GetTileObject(row, col);
}

int Map::GetTileSize() const {
    return m_TileSize;
}

void Map::RemoveTileAtWorld(float x, float y) {
    int col = WorldToCol(x);
    int row = WorldToRow(y);

    if (!IsInsideMapData(row, col)) {
        return;
    }

    if (m_Tiles[row][col]) {
        m_Tiles[row][col]->Clear();
        m_Tiles[row][col].reset();
    }

    m_StageData[row][col] = '.';
}

int Map::GetRowCount() const {
    return static_cast<int>(m_StageData.size());
}

int Map::GetColCount() const {
    if (m_StageData.empty()) return 0;
    return static_cast<int>(m_StageData[0].size());
}

std::string Map::GetPlayingBgPath() const {
    return std::string(RESOURCE_DIR) + "/image/map/stage_bg.png";
}

void Map::Clear() {
    if (m_PlayingBg) {
        m_Root.RemoveChild(m_PlayingBg);
        m_PlayingBg.reset();
    }

    for (auto& row : m_Tiles) {
        for (auto& tile : row) {
            if (tile) {
                tile->Clear();
                tile.reset();
            }
        }
    }

    m_Tiles.clear();
    m_WaterTiles.clear();
    m_StageData.clear();
    m_OriginalStageData.clear();
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

void Map::SetTileCode(int row, int col, char code) {
    if (!IsInsideMapData(row, col)) {
        return;
    }

    if (m_Tiles[row][col]) {
        m_Tiles[row][col]->Clear();
        m_Tiles[row][col].reset();
    }

    m_StageData[row][col] = code;

    float x = ColToWorldX(col);
    float y = RowToWorldY(row);

    if (IsBrickCode(code)) {
        m_Tiles[row][col] = std::make_shared<Brick>(
            m_Root,
            x,
            y,
            m_TileSize,
            GetBrickStateFromCode(code)
        );
        return;
    }

    if (IsSteelCode(code)) {
        m_Tiles[row][col] = std::make_shared<Steel>(
            m_Root,
            x,
            y,
            m_TileSize,
            GetSteelStateFromCode(code)
        );
        return;
    }

    m_Tiles[row][col] = nullptr;
}

void Map::SetBaseProtectionSteel() {
    // 上排：4 D 3 的鐵磚
    SetTileCode(11, 5, '8'); // 4 = 右下 1/4 磚 → 8 = 右下 1/4 鐵
    SetTileCode(11, 6, 'd'); // D = 下半磚 → d = 下半鐵
    SetTileCode(11, 7, '7'); // 3 = 左下 1/4 磚 → 7 = 左下 1/4 鐵

    // 下排：R . L 的鐵磚
    SetTileCode(12, 5, 'r'); // R = 右半磚 → r = 右半鐵
    SetTileCode(12, 7, 'l'); // L = 左半磚 → l = 左半鐵
}

void Map::SetBaseProtectionBrick() {
    // 上排：4 D 3
    SetTileCode(11, 5, '4');
    SetTileCode(11, 6, 'D');
    SetTileCode(11, 7, '3');

    // 下排：R . L
    SetTileCode(12, 5, 'R');
    SetTileCode(12, 7, 'L');
}

void Map::RestoreBaseProtection() {
    SetBaseProtectionBrick();
}

bool Map::TryGetRandomOriginalEmptyPosition(glm::vec2& outPos) const {
    std::vector<glm::vec2> candidates;

    for (int row = 0; row < GetRowCount(); ++row) {
        for (int col = 0; col < GetColCount(); ++col) {
            if (row >= static_cast<int>(m_OriginalStageData.size())) {
                continue;
            }

            if (col >= static_cast<int>(m_OriginalStageData[row].size())) {
                continue;
            }

            // 只挑「原本地圖就是空白」的地方
            if (m_OriginalStageData[row][col] != '.') {
                continue;
            }

            // 避免生成在基地本體位置
            if (row == 12 && col == 6) {
                continue;
            }

            // 避免太靠近基地保護磚區域
            if ((row == 11 || row == 12) && col >= 5 && col <= 7) {
                continue;
            }

            float x = ColToWorldX(col);
            float y = RowToWorldY(row);

            candidates.push_back({x, y});
        }
    }

    if (candidates.empty()) {
        return false;
    }

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(
        0,
        static_cast<int>(candidates.size()) - 1
    );

    outPos = candidates[dist(rng)];
    return true;
}