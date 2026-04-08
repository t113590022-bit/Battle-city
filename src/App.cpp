#include "App.hpp"
#include "TitleMenu.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Explosion.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

struct Rect {
    float left;
    float right;
    float top;
    float bottom;
};

Rect MakeRect(float x, float y, float halfW, float halfH) {
    return {
        x - halfW,
        x + halfW,
        y + halfH,
        y - halfH
    };
}

bool IsColliding(const Rect& a, const Rect& b) {
    return !(a.right < b.left ||
             a.left > b.right ||
             a.top < b.bottom ||
             a.bottom > b.top);
}

void App::Start() {
    LOG_TRACE("Start");

    m_Phase = Phase::TITLE;

    m_TitleMenu = std::make_unique<TitleMenu>(m_Root);
    m_TitleMenu->Init();

    m_CurrentState = State::UPDATE;
}

Bullet::Direction App::ConvertDirection(Player::Direction dir) const {
    switch (dir) {
        case Player::Direction::UP:
            return Bullet::Direction::UP;
        case Player::Direction::DOWN:
            return Bullet::Direction::DOWN;
        case Player::Direction::LEFT:
            return Bullet::Direction::LEFT;
        case Player::Direction::RIGHT:
            return Bullet::Direction::RIGHT;
    }

    return Bullet::Direction::UP;
}

glm::vec2 App::GetBulletSpawnPosition(glm::vec2 playerPos, Player::Direction dir) const {
    float bulletX = playerPos.x;
    float bulletY = playerPos.y;

    const float muzzleOffset = 20.0f;

    switch (dir) {
        case Player::Direction::UP:
            bulletY += muzzleOffset;
            break;
        case Player::Direction::DOWN:
            bulletY -= muzzleOffset;
            break;
        case Player::Direction::LEFT:
            bulletX -= muzzleOffset;
            break;
        case Player::Direction::RIGHT:
            bulletX += muzzleOffset;
            break;
    }

    return {bulletX, bulletY};
}

void App::SpawnBullet() {
    if (!m_Player) return;

    glm::vec2 playerPos = m_Player->GetPosition();
    Player::Direction playerDir = m_Player->GetDirection();
    glm::vec2 bulletPos = GetBulletSpawnPosition(playerPos, playerDir);

    for (auto& bullet : m_Bullets) {
        if (!bullet->IsActive()) {
            bullet->Init(bulletPos.x, bulletPos.y, ConvertDirection(playerDir));
            return;
        }
    }
}

void App::SpawnExplosion(float x, float y) {
    auto explosion = std::make_unique<Explosion>(m_Root);
    explosion->Init(x, y);
    m_Explosions.push_back(std::move(explosion));
}

void App::UpdateBullets() {
    if (!m_Map) return;

    for (auto& bullet : m_Bullets) {
        if (!bullet->IsActive()) continue;

        bullet->Update();

        Rect bulletRect = MakeRect(bullet->GetX(), bullet->GetY(), 4.0f, 4.0f);
        Rect brickRect  = MakeRect(0.0f, 0.0f, 16.0f, 16.0f);

        bool hit = false;
        float hitX = bullet->GetX();
        float hitY = bullet->GetY();

        if (IsColliding(bulletRect, brickRect)) {
            hit = true;
        }
        else if (bullet->IsOutOfBounds(*m_Map)) {
            hit = true;
        }

        if (hit) {
            bullet->Deactivate();
            SpawnExplosion(hitX, hitY);
        }
    }
}

void App::UpdateExplosions() {
    std::vector<size_t> toRemove;

    for (size_t i = 0; i < m_Explosions.size(); ++i) {
        m_Explosions[i]->Update();

        if (m_Explosions[i]->IsFinished()) {
            toRemove.push_back(i);
        }
    }

    for (int i = static_cast<int>(toRemove.size()) - 1; i >= 0; --i) {
        size_t idx = toRemove[i];
        m_Explosions[idx]->Clear();
        m_Explosions.erase(m_Explosions.begin() + static_cast<long long>(idx));
    }
}

void App::Update() {
    switch (m_Phase) {
        case Phase::TITLE:
            m_TitleMenu->Update();

            if (m_TitleMenu->ShouldStartGame()) {
                m_TitleMenu->Clear();

                m_Map = std::make_unique<Map>(m_Root);
                m_Map->Init();

                m_Player = std::make_unique<Player>(m_Root);
                m_Player->Init(0.0f, -300.0f);

                m_Bullets.clear();
                for (int i = 0; i < 10000; ++i) {
                    m_Bullets.push_back(std::make_unique<Bullet>(m_Root));
                }

                for (auto& explosion : m_Explosions) {
                    explosion->Clear();
                }
                m_Explosions.clear();

                m_Phase = Phase::PLAYING;
            }
            break;

        case Phase::PLAYING:
            if (m_ShootCooldownFrames > 0) {
                --m_ShootCooldownFrames;
            }

            if (m_Player && m_Map) {
                m_Player->Update(*m_Map);

                if (Util::Input::IsKeyPressed(Util::Keycode::SPACE) &&
                    m_ShootCooldownFrames == 0) {
                    SpawnBullet();
                    m_ShootCooldownFrames = m_ShootIntervalFrames;
                }
            }

            UpdateBullets();
            UpdateExplosions();
            break;

        case Phase::STAGE_INTRO:
            break;
        case Phase::STAGE_CLEAR:
            break;
        case Phase::GAME_OVER:
            break;
        case Phase::WIN:
            break;
    }

    m_Root.Update();

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() {
    LOG_TRACE("End");

    for (auto& bullet : m_Bullets) {
        bullet->Deactivate();
    }
    m_Bullets.clear();

    for (auto& explosion : m_Explosions) {
        explosion->Clear();
    }
    m_Explosions.clear();

    if (m_Player) {
        m_Player->Clear();
        m_Player.reset();
    }

    if (m_Map) {
        m_Map->Clear();
        m_Map.reset();
    }

    if (m_TitleMenu) {
        m_TitleMenu->Clear();
        m_TitleMenu.reset();
    }
}