//
// Created by link9 on 2026/3/25.
//

#include "TitleMenu.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

TitleMenu::TitleMenu(Util::Renderer& root)
    : m_Root(root) {}

std::string TitleMenu::GetTitleImagePath() const {
    return std::string(RESOURCE_DIR) + "/image/title/title.png";
}

std::string TitleMenu::GetMenuCursorImagePath(int frame) const {
    if (frame == 0)
        return std::string(RESOURCE_DIR) + "/image/title/menu_tank_1.png";
    return std::string(RESOURCE_DIR) + "/image/title/menu_tank_2.png";
}

void TitleMenu::Init() {
    m_TitleX = 0.0f;
    m_TitleY = -600.0f;
    m_TitleArrived = false;

    m_MenuIndex = 0;
    m_MenuAnimFrame = 0;
    m_MenuAnimCounter = 0;
    m_StartGame = false;

    DrawTitle();
    CreateMenuCursor();
}

void TitleMenu::DrawTitle() {
    m_TitleImage = std::make_shared<Character>(GetTitleImagePath());
    m_TitleImage->SetPosition({0.0f, m_TitleY});
    m_Root.AddChild(m_TitleImage);
}

void TitleMenu::CreateMenuCursor() {
    m_MenuCursor = std::make_shared<Character>(GetMenuCursorImagePath(m_MenuAnimFrame));
    m_MenuCursor->SetPosition(m_MenuPositions[m_MenuIndex]);
    m_MenuCursor->SetVisible(false);
    m_Root.AddChild(m_MenuCursor);
}

void TitleMenu::Update() {
    if (!m_TitleArrived) {
        m_TitleY += m_TitleSlideSpeed;

        if (m_TitleY >= m_TitleTargetY) {
            m_TitleY = m_TitleTargetY;
            m_TitleArrived = true;
            m_MenuCursor->SetVisible(true);
        }

        m_TitleImage->SetPosition({m_TitleX, m_TitleY});
        return;
    }

    // 上下選單
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

    // Enter
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        if (m_MenuIndex == 0) {
            m_StartGame = true;
        }
    }
}

void TitleMenu::Clear() {
    if (m_TitleImage) {
        m_Root.RemoveChild(m_TitleImage);
        m_TitleImage.reset();
    }
    if (m_MenuCursor) {
        m_Root.RemoveChild(m_MenuCursor);
        m_MenuCursor.reset();
    }
}