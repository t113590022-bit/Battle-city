#include "Character.hpp"
#include "Util/Animation.hpp"
#include <unordered_map>
#include "Util/Image.hpp"

Character::Character(const std::string& ImagePath) {
    SetImage(ImagePath);

    ResetPosition();
}

void Character::SetImage(const std::string& ImagePath) {
    static std::unordered_map<std::string, std::weak_ptr<Util::Image>> imageCache;

    m_ImagePath = ImagePath;

    auto it = imageCache.find(m_ImagePath);

    if (it != imageCache.end()) {
        auto cachedImage = it->second.lock();

        if (cachedImage) {
            m_Drawable = cachedImage;
            return;
        }
    }

    auto newImage = std::make_shared<Util::Image>(m_ImagePath);
    imageCache[m_ImagePath] = newImage;
    m_Drawable = newImage;
}


void Character::Open() {
    SetImage(m_ImagePath);
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