//
// Created by link9 on 2026/4/24.
//

#ifndef TEXT_OBJECT_HPP
#define TEXT_OBJECT_HPP

#include <memory>
#include <string>

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

class TextObject : public Util::GameObject {
public:
    TextObject(
        const std::string& font,
        int size,
        const std::string& text,
        const Util::Color& color = Util::Color(255, 255, 255)
    )
        : Util::GameObject(
            std::make_shared<Util::Text>(font, size, text, color),
            0.0f
        ) {
        m_Text = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
    }

    void SetText(const std::string& text) {
        if (m_Text) {
            m_Text->SetText(text);
        }
    }

    void SetColor(const Util::Color& color) {
        if (m_Text) {
            m_Text->SetColor(color);
        }
    }

    void SetPosition(const glm::vec2& position) {
        m_Transform.translation = position;
    }

    void SetScale(const glm::vec2& scale) {
        m_Transform.scale = scale;
    }

private:
    std::shared_ptr<Util::Text> m_Text = nullptr;
};

#endif //TEXTOBJECT_HPP