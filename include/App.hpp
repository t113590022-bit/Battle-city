#ifndef APP_HPP
#define APP_HPP

#include "vector"
#include "cstddef"

#include "TitleMenu.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Bullet.hpp"
#include  "Explosion.hpp"
#include "Enemy.hpp"
#include "RespawnEffect.hpp"
#include "Base.hpp"
#include "GameOverBanner.hpp"

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

private:
    void UpdateTitle();
    void EnterPlaying();
    void UpdatePlaying();
    void HandleSystemInput();

    void InitMap();
    void InitPlayer();
    void InitBullets(int poolSize);

    void ClearBullets();
    void ClearExplosions();
    void ClearEnemies();

    void UpdateShootCooldown();
    void UpdatePlayer();
    void HandleShootInput();

    Bullet::Direction ConvertDirection(Player::Direction dir) const;
    glm::vec2 GetBulletSpawnPosition(glm::vec2 playerPos, Player::Direction dir) const;

    void SpawnBullet();
    void SpawnExplosion(float x, float y);
    void UpdateBullets();
    void UpdateExplosions();

    void InitEnemies();
    void UpdateEnemies();
    void PrepareStageEnemies();
    void TrySpawnNextEnemy();
    int GetAliveEnemyCount() const;
    void RemoveDeadEnemies();

    void HandlePlayerDeath();
    void StartPlayerRespawn(float x, float y);
    void UpdatePlayerRespawn();

    void StartEnemySpawn(float x, float y, Enemy::EnemyType type);
    void UpdateEnemyRespawn();

    void EnterGameOver();
    void UpdateGameOver();
    void ClearGameOverUi();

private:
    State m_CurrentState = State::START;
    Phase m_Phase = Phase::TITLE;

    Util::Renderer m_Root;

    int m_ShootCooldownFrames = 0;
    int m_ShootIntervalFrames = 70;   // 60 FPS 下約 1.17 秒

    int m_NextSpawnPointIndex = 0;
    int m_EnemySpawnCooldownFrames = 0;
    int m_EnemySpawnIntervalFrames = 200;


    std::unique_ptr<TitleMenu> m_TitleMenu;
    std::unique_ptr<Player> m_Player;
    std::unique_ptr<Map> m_Map;

    std::vector<std::unique_ptr<Bullet>> m_Bullets;
    std::vector<std::unique_ptr<Explosion>> m_Explosions;
    std::vector<std::unique_ptr<Enemy>> m_Enemies;
    std::shared_ptr<Character> m_PlayingBg;

    int m_PlayerLives = 2;

    // 玩家重生
    std::unique_ptr<RespawnEffect> m_PlayerRespawnEffect;
    bool m_PlayerRespawning = false;
    float m_PlayerRespawnX = -200.0f;
    float m_PlayerRespawnY = -384.0f;

    // 敵人出生
    std::unique_ptr<RespawnEffect> m_EnemyRespawnEffect;
    bool m_EnemyRespawning = false;
    float m_EnemySpawnX = 0.0f;
    float m_EnemySpawnY = 392.0f;

    // 敵人細節
    std::vector<Enemy::EnemyType> m_EnemyQueue;

    Enemy::EnemyType m_PendingEnemyType = Enemy::EnemyType::NORMAL;

    int m_SpawnedEnemies = 0;
    int m_KilledEnemies = 0;
    int m_MaxActiveEnemies = 4;

    //老鷹基地
    std::unique_ptr<Base> m_Base;

    // Game Over
    std::shared_ptr<Character> m_GameOverBg;
    std::unique_ptr<GameOverBanner> m_GameOverBanner;
};

#endif
