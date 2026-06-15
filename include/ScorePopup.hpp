//
// Created by link9 on 2026/6/9.
//

#ifndef SCOREPOPUP_HPP
#define SCOREPOPUP_HPP

#include <memory>
#include <string>

#include "Util/Renderer.hpp"
#include "Character.hpp"

class ScorePopup {
public:
    explicit ScorePopup(Util::Renderer& root);

    void Init(float x, float y, int score);
    void Update();
    void Clear();

    bool IsFinished() const;

private:
    std::string GetScoreImagePath(int score) const;

private:
    Util::Renderer& m_Root;
    std::shared_ptr<Character> m_Image;

    int m_Timer = 0;
    int m_LifeFrames = 30;
    bool m_Finished = false;
};

#endif //SCOREPOPUP_HPP