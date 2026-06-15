//
// Created by link9 on 2026/6/9.
//
#include "ScorePopup.hpp"

ScorePopup::ScorePopup(Util::Renderer& root)
    : m_Root(root) {
}

void ScorePopup::Init(float x, float y, int score) {
    m_Timer = 0;
    m_Finished = false;

    m_Image = std::make_shared<Character>(GetScoreImagePath(score));
    m_Image->SetPosition({x, y});
    m_Image->SetZIndex(80.0f);
    m_Image->SetVisible(true);

    m_Root.AddChild(m_Image);
}

void ScorePopup::Update() {
    if (m_Finished) {
        return;
    }

    ++m_Timer;

    if (m_Timer >= m_LifeFrames) {
        m_Finished = true;

        if (m_Image) {
            m_Image->SetVisible(false);
        }
    }
}

void ScorePopup::Clear() {
    if (m_Image) {
        m_Root.RemoveChild(m_Image);
        m_Image.reset();
    }

    m_Finished = true;
}

bool ScorePopup::IsFinished() const {
    return m_Finished;
}

std::string ScorePopup::GetScoreImagePath(int score) const {
    std::string base = std::string(RESOURCE_DIR) + "/image/score/";

    switch (score) {
        case 100:
            return base + "pic-_771.png";

        case 200:
            return base + "pic-_773.png";

        case 300:
            return base + "pic-_775.png";

        case 400:
            return base + "pic-_777.png";

        case 500:
            return base + "pic-_779.png";
    }

    return base + "pic-_771.png";
}