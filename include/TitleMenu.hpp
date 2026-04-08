//
// Created by link9 on 2026/3/25.
//

//
// Created by link9 on 2026/3/25.
//

#ifndef TITLEMENU_HPP
#define TITLEMENU_HPP

#include <memory>      // shared_ptr
#include <string>      // std::string
#include <vector>      // std::vector

#include <glm/glm.hpp> // glm::vec2

#include "Util/Renderer.hpp"
#include "Character.hpp"

class TitleMenu {
public:
    explicit TitleMenu(Util::Renderer& root);

    void Init();
    void Update();
    void Clear();

    // bool IsArrived() const;
    // int GetMenuIndex() const;
    // bool IsStartPressed() const;
    bool ShouldStartGame() const { return m_StartGame; }

private:
    std::string GetTitleImagePath() const;
    std::string GetMenuCursorImagePath(int frame) const;
    void DrawTitle();
    void CreateMenuCursor();

private:
    Util::Renderer& m_Root;

    std::shared_ptr<Character> m_TitleImage;
    std::shared_ptr<Character> m_MenuCursor;

    float m_TitleX = 0.0f;
    float m_TitleY = -600.0f;
    float m_TitleTargetX = 0.0f;
    float m_TitleTargetY = 10.0f;
    float m_TitleSlideSpeed = 6.0f;
    bool m_TitleArrived = false;


    int m_MenuIndex = 0;
    int m_MenuAnimFrame = 0;
    int m_MenuAnimCounter = 0;
    int m_MenuAnimInterval = 12;

    bool m_StartGame = false;

    std::vector<glm::vec2> m_MenuPositions = {
        {-188.0f, -69.0f},
        {-188.0f, -134.0f},
        {-188.0f, -198.0f}
    };
};

#endif //TITLEMENU_HPP