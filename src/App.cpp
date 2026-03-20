#include "App.hpp"

// #include "external/freetype/include/freetype/internal/ftobjs.h"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

std::string App::GetTitleImagePath() const {
    return std::string(RESOURCE_DIR) + "/image/title/title.png";
}

std::string App::GetMenuCursorImagePath(int frame) const {
    if (frame == 0) {
        return std::string(RESOURCE_DIR) + "/image/title/menu_tank_1.png";
    }
    return std::string(RESOURCE_DIR) + "/image/title/menu_tank_2.png";
}

void App::DrawTitle() {
    m_TitleImage = std::make_shared<Character>(GetTitleImagePath());
    m_TitleImage->m_Transform.scale = {0.7f, 0.7f};
    m_TitleImage->SetPosition({0.0f, 10.0f});
    m_Root.AddChild(m_TitleImage);
}

void App::ClearTitle() {
    if (m_TitleImage) {
        m_Root.RemoveChild(m_TitleImage);
        m_TitleImage.reset();
    }
    if (m_MenuCursor) {
        m_Root.RemoveChild(m_MenuCursor);
        m_MenuCursor.reset();
    }
}

void App::CreateTitleMenuCursor() {
    m_MenuCursor = std::make_shared<Character>(GetMenuCursorImagePath(m_MenuAnimFrame));
    m_MenuCursor->SetPosition(m_MenuPositions[m_MenuIndex]);
    m_MenuCursor->SetVisible(false);
    m_Root.AddChild(m_MenuCursor);
}

void App::UpdateTitle() {
    //判斷封面是否移動到位
    if (!m_TitleArrived) {
        if (m_TitleY < m_TitleTargetY) {
            m_TitleY += m_TitleSlideSpeed;

            if (m_TitleY >= m_TitleTargetY) {
                m_TitleY = m_TitleTargetY;
                m_TitleArrived = true;

                if (m_MenuCursor) {
                    m_MenuCursor->SetVisible(true);
                }
            }

            m_TitleImage->SetPosition({m_TitleX, m_TitleY});
        }
    } else {
        if (Util::Input::IsKeyDown(Util::Keycode::W)) {
            if (m_MenuIndex > 0) {
                m_MenuIndex--;
                m_MenuCursor->SetPosition(m_MenuPositions[m_MenuIndex]);
            }
        }
        else if (Util::Input::IsKeyDown(Util::Keycode::S)) {
            if (m_MenuIndex < 2) {
                m_MenuIndex++;
                m_MenuCursor->SetPosition(m_MenuPositions[m_MenuIndex]);
            }
        }

        // 游標動畫
        m_MenuAnimCounter++;
        if (m_MenuAnimCounter >= m_MenuAnimInterval) {
            m_MenuAnimCounter = 0;
            m_MenuAnimFrame = (m_MenuAnimFrame + 1) % 2;
            m_MenuCursor->SetImage(GetMenuCursorImagePath(m_MenuAnimFrame));
        }

        if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            if (m_MenuIndex == 0) {
                ClearTitle();

                if (m_MenuCursor) {
                    m_Root.RemoveChild(m_MenuCursor);
                    m_MenuCursor.reset();
                }

                m_PlayerX = 0;
                m_PlayerY = -300;

                m_Player = std::make_shared<Character>(GetTankImagePath(m_Direction, m_AnimFrame));
                m_Player->SetPosition({(float)m_PlayerX, (float)m_PlayerY});
                m_Root.AddChild(m_Player);

                m_Phase = Phase::PLAYING;
            }
            else if (m_MenuIndex == 1) {
                // 先不做任何事
            }
            else if (m_MenuIndex == 2) {
                // 先不做任何事
            }
        }
    }
}


std::string App::GetTankImagePath(Direction dir, int frame) const {
    switch (dir) {
        case Direction::UP:
            return frame == 0
                ?std::string(RESOURCE_DIR) + "/image/player/1p-front_lv1.png"
                :std::string(RESOURCE_DIR) + "/image/player/1p-front2_lv1.png";
        case Direction::DOWN:
            return frame == 0
                ?std::string(RESOURCE_DIR) + "/image/player/1p-back_lv1.png"
                :std::string(RESOURCE_DIR) + "/image/player/1p-back2_lv1.png";
        case Direction::LEFT:
            return frame == 0
                ?std::string(RESOURCE_DIR) + "/image/player/1p-left_lv1.png"
                :std::string(RESOURCE_DIR) + "/image/player/1p-left2_lv1.png";
        case Direction::RIGHT:
            return frame == 0
                ?std::string(RESOURCE_DIR) + "/image/player/1p-right_lv1.png"
                :std::string(RESOURCE_DIR) + "/image/player/1p-right2_lv1.png";
    }

    return "";
}


void App::UpdatePlaying() {
    bool isMoving = false;

    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        m_PlayerY += m_MoveSpeed;
        m_Direction = Direction::UP;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        m_PlayerY -= m_MoveSpeed;
        m_Direction = Direction::DOWN;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        m_PlayerX -= m_MoveSpeed;
        m_Direction = Direction::LEFT;
        isMoving = true;
    } else if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        m_PlayerX += m_MoveSpeed;
        m_Direction = Direction::RIGHT;
        isMoving = true;
    }

    if (isMoving) {
        m_AnimCounter++;
        if (m_AnimCounter >= m_AnimInterval) {
            m_AnimCounter = 0;
            m_AnimFrame = (m_AnimFrame + 1) % 2;

            m_Player -> SetImage(
                GetTankImagePath(m_Direction, m_AnimFrame)
            );
        }
    } else {
        m_AnimFrame = 0;
        m_AnimCounter = 0;
        m_Player->SetImage(GetTankImagePath(m_Direction, m_AnimFrame));
    }

    m_Player->SetPosition({(float)m_PlayerX, (float)m_PlayerY});
}


void App::Start() {
    LOG_TRACE("Start");

    m_Phase = Phase::TITLE;

    m_TitleX = 0.0f;
    m_TitleY = -600.0f;   // 從下方出現
    m_TitleTargetX = 0.0f;
    m_TitleTargetY = 10.0f;
    m_TitleSlideSpeed = 6.0f;
    m_TitleArrived = false;

    DrawTitle();
    CreateTitleMenuCursor();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    //TODO: do your things here and delete this line <3
    switch (m_Phase) {
        case Phase::TITLE:
            UpdateTitle();
            break;
        case Phase::STAGE_INTRO:
            //之後加
            break;
        case Phase::PLAYING:
            UpdatePlaying();
            break;
        case Phase::STAGE_CLEAR:
            //之後加
            break;
        case Phase::GAME_OVER:
            //之後加
            break;
        case Phase::WIN:
            //之後加
            break;
    }

    m_Root.Update();
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
