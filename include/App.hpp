#ifndef APP_HPP
#define APP_HPP

#include "vector"
#include "cstddef"

#include "TitleMenu.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Bullet.hpp"
#include  "Explosion.hpp"

#include "pch.hpp" // IWYU pragma: export
#include "Util/Image.hpp"
#include "Character.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"

class TitleMenu;
class Player;
class Map;
class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class Phase {
        TITLE,
        STAGE_INTRO,
        PLAYING,
        STAGE_CLEAR,
        GAME_OVER,
        WIN
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End();

    // App();
    // ~App();   // 只宣告，不在這裡定義

private:
    Bullet::Direction ConvertDirection(Player::Direction dir) const;
    glm::vec2 GetBulletSpawnPosition(glm::vec2 playerPos, Player::Direction dir) const;

    void SpawnBullet();
    void SpawnExplosion(float x, float y);
    void UpdateBullets();
    void UpdateExplosions();


private:
    State m_CurrentState = State::START;
    Phase m_Phase = Phase::TITLE;

    Util::Renderer m_Root;

    int m_ShootCooldownFrames = 0;
    int m_ShootIntervalFrames = 70;   // 60 FPS 下約 0.16 秒


    std::unique_ptr<TitleMenu> m_TitleMenu;
    std::unique_ptr<Player> m_Player;
    std::unique_ptr<Map> m_Map;

    struct ExplosionRequest {
        float x;
        float y;
    };


    std::vector<std::unique_ptr<Bullet>> m_Bullets;
    std::vector<std::unique_ptr<Explosion>> m_Explosions;

    std::shared_ptr<Character> m_PlayingBg;
};

#endif
