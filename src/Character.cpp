#include "Character.hpp"
// #include "Util/Image.hpp"
#include "Util/Animation.hpp"


Character::Character(const std::string& ImagePath) {
    SetImage(ImagePath);

    ResetPosition();
}

void Character::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;

    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}


void Character::Open() {
    // m_ImagePath = GA_RESOURCE_DIR"/Image/Character/door_open.png"; // 加這行
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Character::Play() {
    // 這裡放原本動畫播放的邏輯（給蜜蜂或球使用）
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (temp) {
        temp->Play();
    }
}

void Character::SetLooping(bool looping) {
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (temp) {
        temp->SetLooping(looping);
    }
}

bool Character::IsPlaying() const {
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (temp) {
        return temp->GetState() == Util::Animation::State::PLAY;
    }
    // 靜態圖回傳false
    return false;
}