//
// Created by link9 on 2026/5/11.
//

#include "StageIntroScreen.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

StageIntroScreen::StageIntroScreen(Util::Renderer& root)
    : m_Root(root) {}

void StageIntroScreen::Init(int stageNo, bool allowSelect, int maxStage) {
    Clear();

    m_SelectedStage = stageNo;
    m_AllowSelect = allowSelect;
    m_MaxStage = maxStage;

    if (m_MaxStage < 1) {
        m_MaxStage = 1;
    }

    if (m_SelectedStage < 1) {
        m_SelectedStage = 1;
    }

    if (m_AllowSelect && m_SelectedStage > m_MaxStage) {
        m_SelectedStage = m_MaxStage;
    }

    m_State = State::Closing;
    m_Finished = false;
    m_HoldFrames = 0;

    m_TopPanelY = m_OpenTopY;
    m_BottomPanelY = m_OpenBottomY;

    CreatePanels();
    CreateText();

    if (m_StageText) {
        m_StageText->SetVisible(false);
    }
}

void StageIntroScreen::CreatePanels() {
    m_TopPanel = std::make_shared<Character>(GetPanelPath());
    m_TopPanel->SetPosition({0.0f, m_TopPanelY});
    m_TopPanel->SetZIndex(95.0f);
    m_TopPanel->SetVisible(true);
    m_Root.AddChild(m_TopPanel);

    m_BottomPanel = std::make_shared<Character>(GetPanelPath());
    m_BottomPanel->SetPosition({0.0f, m_BottomPanelY});
    m_BottomPanel->SetZIndex(95.0f);
    m_BottomPanel->SetVisible(true);
    m_Root.AddChild(m_BottomPanel);
}

void StageIntroScreen::CreateText() {
    m_StageText = std::make_shared<TextObject>(
        GetFontPath(),
        28,
        "STAGE  " + std::to_string(m_SelectedStage),
        Util::Color(0, 0, 0)
    );

    m_StageText->SetPosition({0.0f, 0.0f});
    m_StageText->SetZIndex(100.0f);
    m_Root.AddChild(m_StageText);
}

void StageIntroScreen::Update() {
    switch (m_State) {
        case State::Closing:
            UpdateClosing();
            break;

        case State::Selecting:
            UpdateSelecting();
            break;

        case State::Holding:
            UpdateHolding();
            break;

        case State::Opening:
            UpdateOpening();
            break;

        case State::Finished:
            break;
    }
}

void StageIntroScreen::UpdateClosing() {
    m_TopPanelY -= m_MoveSpeed;
    m_BottomPanelY += m_MoveSpeed;

    if (m_TopPanelY <= m_ClosedTopY) {
        m_TopPanelY = m_ClosedTopY;
    }

    if (m_BottomPanelY >= m_ClosedBottomY) {
        m_BottomPanelY = m_ClosedBottomY;
    }

    if (m_TopPanel) {
        m_TopPanel->SetPosition({0.0f, m_TopPanelY});
    }

    if (m_BottomPanel) {
        m_BottomPanel->SetPosition({0.0f, m_BottomPanelY});
    }

    if (m_TopPanelY == m_ClosedTopY &&
        m_BottomPanelY == m_ClosedBottomY) {

        if (m_StageText) {
            m_StageText->SetVisible(true);
        }

        if (m_AllowSelect) {
            m_State = State::Selecting;
        } else {
            m_State = State::Holding;
        }
    }
}

void StageIntroScreen::UpdateSelecting() {
    HandleSelectInput();

    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        if (m_StageText) {
            m_StageText->SetVisible(false);
        }

        m_State = State::Opening;
    }
}

void StageIntroScreen::UpdateHolding() {
    ++m_HoldFrames;

    if (m_HoldFrames >= m_HoldMaxFrames) {
        if (m_StageText) {
            m_StageText->SetVisible(false);
        }

        m_State = State::Opening;
    }
}

void StageIntroScreen::UpdateOpening() {
    m_TopPanelY += m_MoveSpeed;
    m_BottomPanelY -= m_MoveSpeed;

    if (m_TopPanelY >= m_OpenTopY) {
        m_TopPanelY = m_OpenTopY;
    }

    if (m_BottomPanelY <= m_OpenBottomY) {
        m_BottomPanelY = m_OpenBottomY;
    }

    if (m_TopPanel) {
        m_TopPanel->SetPosition({0.0f, m_TopPanelY});
    }

    if (m_BottomPanel) {
        m_BottomPanel->SetPosition({0.0f, m_BottomPanelY});
    }

    if (m_TopPanelY == m_OpenTopY &&
        m_BottomPanelY == m_OpenBottomY) {

        m_State = State::Finished;
        m_Finished = true;
    }
}

void StageIntroScreen::HandleSelectInput() {
    bool changed = false;

    if (Util::Input::IsKeyDown(Util::Keycode::W) ||
        Util::Input::IsKeyDown(Util::Keycode::UP)) {

        --m_SelectedStage;
        changed = true;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::S) ||
        Util::Input::IsKeyDown(Util::Keycode::DOWN)) {

        ++m_SelectedStage;
        changed = true;
    }

    if (m_SelectedStage < 1) {
        m_SelectedStage = m_MaxStage;
    }

    if (m_SelectedStage > m_MaxStage) {
        m_SelectedStage = 1;
    }

    if (changed) {
        RefreshStageText();
    }
}

void StageIntroScreen::RefreshStageText() {
    if (!m_StageText) return;

    m_StageText->SetText(
        "STAGE  " + std::to_string(m_SelectedStage)
    );
}

std::string StageIntroScreen::GetPanelPath() const {
    return std::string(RESOURCE_DIR) + "/image/ui/stage_panel_gray.png";
}

std::string StageIntroScreen::GetFontPath() const {
    return std::string(RESOURCE_DIR) + "/font/PressStart2P.ttf";
}

void StageIntroScreen::Clear() {
    if (m_TopPanel) {
        m_Root.RemoveChild(m_TopPanel);
        m_TopPanel.reset();
    }

    if (m_BottomPanel) {
        m_Root.RemoveChild(m_BottomPanel);
        m_BottomPanel.reset();
    }

    if (m_StageText) {
        m_Root.RemoveChild(m_StageText);
        m_StageText.reset();
    }
}