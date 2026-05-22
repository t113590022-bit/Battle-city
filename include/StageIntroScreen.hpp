//
// Created by link9 on 2026/5/11.
//

#ifndef STAGEINTROSCREEN_HPP
#define STAGEINTROSCREEN_HPP

#include <memory>
#include <string>

#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "TextObject.hpp"

class StageIntroScreen {
public:
    explicit StageIntroScreen(Util::Renderer& root);

    void Init(int stageNo, bool allowSelect, int maxStage);
    void Update();
    void Clear();

    bool IsFinished() const { return m_Finished; }
    int GetSelectedStage() const { return m_SelectedStage; }

private:
    enum class State {
        Closing,
        Selecting,
        Holding,
        Opening,
        Finished
    };

private:
    void CreatePanels();
    void CreateText();

    void UpdateClosing();
    void UpdateSelecting();
    void UpdateHolding();
    void UpdateOpening();

    void HandleSelectInput();
    void RefreshStageText();

    std::string GetPanelPath() const;
    std::string GetFontPath() const;

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_TopPanel;
    std::shared_ptr<Character> m_BottomPanel;
    std::shared_ptr<TextObject> m_StageText;

    State m_State = State::Closing;

    bool m_AllowSelect = false;
    bool m_Finished = false;

    int m_SelectedStage = 1;
    int m_MaxStage = 1;

    // 416x416 play area，panel 416x208
    float m_OpenTopY = 720.0f;
    float m_OpenBottomY = -720.0f;

    float m_ClosedTopY = 225.0f;
    float m_ClosedBottomY = -225.0f;

    float m_TopPanelY = 720.0f;
    float m_BottomPanelY = -720.0f;

    float m_MoveSpeed = 12.0f;

    int m_HoldFrames = 0;
    int m_HoldMaxFrames = 60;
};

#endif //STAGEINTROSCREEN_HPP