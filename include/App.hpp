#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Image.hpp"
#include "Character.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class Phase {
        TITLE,
        STAGE_INTRO,
        PLAYING,
        STAGE_CLEAR,
        GAME_OVER,
        WIN
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

    void UpdateTitle();
    void UpdatePlaying();
    std::string GetTankImagePath(Direction dir, int frame) const;
    std::string GetTitleImagePath() const;
    void DrawTitle();
    void ClearTitle();
    std::string GetMenuCursorImagePath(int frame) const;
    void CreateTitleMenuCursor();
    void UpdateTitleMenuCursor();

    void UpdatePlayerAnimation(bool isMoving);

private:
    Phase m_Phase = Phase::TITLE;
    State m_CurrentState = State::START;

    Util::Renderer m_Root;
    std::shared_ptr<Character> m_TitleImage;
    std::shared_ptr<Character> m_Player;

    // 封面的上移動畫
    float m_TitleX = 0.0f;
    float m_TitleY = -600.0f;      // 一開始放在畫面下方，數值可再調
    float m_TitleTargetX = 0.0f;
    float m_TitleTargetY = 10.0f;
    float m_TitleSlideSpeed = 6.0f; // 每幀移動速度
    bool m_TitleArrived = false;

    //封面的坦克小游標
    std::shared_ptr<Character> m_MenuCursor;

    int m_MenuIndex = 0;              // 0=1 PLAYER, 1=2 PLAYERS, 2=CONSTRUCTION
    int m_MenuAnimFrame = 0;
    int m_MenuAnimCounter = 0;
    int m_MenuAnimInterval = 12;

    std::vector<glm::vec2> m_MenuPositions = {
        {-180.0f, -45.0f},  // 1 PLAYER 左邊
        {-180.0f, -90.0f},  // 2 PLAYERS 左邊
        {-180.0f, -135.0f}   // CONSTRUCTION 左邊
    };

    int m_PlayerX = 0;
    int m_PlayerY = 0;
    int m_MoveSpeed = 4;

    Direction m_Direction = Direction::UP;

    // 兩張動畫圖：0 / 1
    int m_AnimFrame = 0;

    // 控制多久切一次圖，避免每幀都狂切
    int m_AnimCounter = 0;
    int m_AnimInterval = 8;
};

#endif
