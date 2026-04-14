#include "App.hpp"
#include "TitleMenu.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Explosion.hpp"
#include "Enemy.hpp"

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

void App::Update() {
    switch (m_Phase) {
        case Phase::TITLE:
            UpdateTitle();
            break;

        case Phase::PLAYING:
            UpdatePlaying();
            break;

        case Phase::STAGE_INTRO:
            break;

        case Phase::STAGE_CLEAR:
            break;

        case Phase::GAME_OVER:
            UpdateGameOver();
            break;

        case Phase::WIN:
            break;
    }

    m_Root.Update();
    HandleSystemInput();
}

void App::End() {
    LOG_TRACE("End");

    ClearBullets();
    ClearExplosions();
    ClearEnemies();

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

void App::UpdateTitle() {
    if (!m_TitleMenu) return;

    m_TitleMenu->Update();

    if (m_TitleMenu->ShouldStartGame()) {
        EnterPlaying();
    }
}

void App::EnterPlaying() {
    if (m_TitleMenu) {
        m_TitleMenu->Clear();
        m_TitleMenu.reset();
    }

    InitMap();
    // InitPlayer();
    InitBullets(10);   // 先用 10 顆就很夠了
    ClearExplosions();
    InitEnemies();
    ClearGameOverUi();

    PrepareStageEnemies();

    m_NextSpawnPointIndex = 0;
    m_EnemySpawnCooldownFrames = 0;

    StartPlayerRespawn(-200.0f, -384.0f);
    // 不要直接固定生一隻，改成交給 TrySpawnNextEnemy()
    TrySpawnNextEnemy();

    m_Base = std::make_unique<Base>(m_Root);

    // 地圖底部中央
    m_Base->Init(0.0f, -368.0f);

    m_PlayerLives = 2;
    m_ShootCooldownFrames = 0;
    m_Phase = Phase::PLAYING;
}

// 預備敵人生成
void App::PrepareStageEnemies() {
    m_EnemyQueue.clear();

    m_EnemyQueue = {
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::FAST,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::POWER,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::FAST,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::HEAVY,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::POWER,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::FAST,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::NORMAL,
        Enemy::EnemyType::HEAVY,
        Enemy::EnemyType::POWER,
        Enemy::EnemyType::FAST,
        Enemy::EnemyType::HEAVY
    };

    m_SpawnedEnemies = 0;
    m_KilledEnemies = 0;
}

// 場上已生成敵人
int App::GetAliveEnemyCount() const {
    int count = 0;

    for (const auto& enemy : m_Enemies) {
        if (enemy && enemy->IsAlive()) {
            ++count;
        }
    }

    return count;
}

void App::TrySpawnNextEnemy() {
    if (m_EnemyRespawning) return;
    if (m_EnemySpawnCooldownFrames > 0) return;

    if (GetAliveEnemyCount() >= m_MaxActiveEnemies) return;

    if (m_SpawnedEnemies >= static_cast<int>(m_EnemyQueue.size())) return;

    const float spawnXs[3] = {0.0f, 320.0f, -320.0f};
    const float spawnY = 392.0f;

    float spawnX = spawnXs[m_NextSpawnPointIndex];
    m_NextSpawnPointIndex = (m_NextSpawnPointIndex + 1) % 3;

    StartEnemySpawn(spawnX, spawnY, m_EnemyQueue[m_SpawnedEnemies]);
    ++m_SpawnedEnemies;
}

void App::UpdatePlaying() {
    if (m_EnemySpawnCooldownFrames > 0) {
        --m_EnemySpawnCooldownFrames;
    }
    UpdateShootCooldown();
    UpdatePlayer();
    HandleShootInput();
    UpdateBullets();
    UpdateExplosions();
    UpdateEnemies();

    RemoveDeadEnemies();
    TrySpawnNextEnemy();

    HandlePlayerDeath();
    UpdatePlayerRespawn();
    UpdateEnemyRespawn();
}

void App::HandleSystemInput() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::InitMap() {
    if (m_Map) {
        m_Map->Clear();
        m_Map.reset();
    }

    m_Map = std::make_unique<Map>(m_Root);
    m_Map->Init();
}

// void App::InitPlayer() {
//
//      if (m_Player) {
//          m_Player->Clear();
//          m_Player.reset();
//      }
//
//      m_Player = std::make_unique<Player>(m_Root);
//      m_Player->Init(0.0f, -300.0f);
// }

void App::InitBullets(int poolSize) {
    ClearBullets();

    for (int i = 0; i < poolSize; ++i) {
        m_Bullets.push_back(std::make_unique<Bullet>(m_Root));
    }
}

void App::InitEnemies() {
    ClearEnemies();
    // m_Enemies.clear();
    //
    // auto enemy = std::make_unique<Enemy>(m_Root);
    // enemy->Init(0.0f, 392.0f);
    //
    // m_Enemies.push_back(std::move(enemy));
}

void App::ClearBullets() {
    for (auto& bullet : m_Bullets) {
        if (bullet) {
            bullet->Deactivate();
        }
    }
    m_Bullets.clear();
}

void App::ClearExplosions() {
    for (auto& explosion : m_Explosions) {
        if (explosion) {
            explosion->Clear();
        }
    }
    m_Explosions.clear();
}

void App::ClearEnemies() {
    for (auto& enemy : m_Enemies) {
        if (enemy) {
            enemy->Clear();
        }
    }
    m_Enemies.clear();
}

void App::UpdateShootCooldown() {
    if (m_ShootCooldownFrames > 0) {
        --m_ShootCooldownFrames;
    }
}

void App::UpdatePlayer() {
    if (m_Player && m_Map) {
        m_Player->Update(*m_Map);
    }
}

void App::HandleShootInput() {
    if (!m_Player) return;
    if (m_ShootCooldownFrames != 0) return;

    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        SpawnBullet();
        m_ShootCooldownFrames = m_ShootIntervalFrames;
    }
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
            bullet->Init(bulletPos.x, bulletPos.y, ConvertDirection(playerDir), 6.0f);
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

        bool hit = false;
        float hitX = bullet->GetX();
        float hitY = bullet->GetY();

        for (auto& enemy : m_Enemies) {
            if (!enemy->IsAlive()) continue;

            Rect enemyRect = MakeRect(enemy->GetX(), enemy->GetY(), 10.0f, 10.0f);

            if (IsColliding(bulletRect, enemyRect)) {
                hit = true;
                enemy->TakeDamage();
                bullet->Deactivate();
                break;
            }
        }

        if (!hit && m_Map->HitTile(hitX, hitY)) {
            hit = true;
        }
        else if (!hit && bullet->IsOutOfBounds(*m_Map)) {
            hit = true;
        }

        if (hit) {
            bullet->Deactivate();
            SpawnExplosion(hitX, hitY);
        }

        if (m_Base && m_Base->IsAlive()) {
            float bx = bullet->GetX();
            float by = bullet->GetY();

            const float halfW = 10.0f;
            const float halfH = 10.0f;

            float baseX = m_Base->GetX();
            float baseY = m_Base->GetY();

            if (bx > baseX - halfW && bx < baseX + halfW &&
                by > baseY - halfH && by < baseY + halfH) {

                m_Base->Destroy();
                bullet->Deactivate();

                SpawnExplosion(baseX, baseY);

                EnterGameOver(); // ← 直接結束
            }
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

void App::UpdateEnemies() {
    if (!m_Map) return;

    for (auto& enemy : m_Enemies) {
        enemy->Update(*m_Map);

        // ===== 新增：敵人子彈打玩家 =====
        if (!m_Player || !m_Player->IsAlive()) continue;

        if (enemy->HasActiveBullet()) {
            auto* bullet = enemy->GetBullet();

            Rect bulletRect = MakeRect(
                bullet->GetX(),
                bullet->GetY(),
                4.0f,
                4.0f
            );

            // ===== 先檢查基地 =====
            if (m_Base && m_Base->IsAlive()) {
                Rect baseRect = MakeRect(
                    m_Base->GetX(),
                    m_Base->GetY(),
                    10.0f,
                    10.0f
                );

                if (IsColliding(bulletRect, baseRect)) {
                    bullet->Deactivate();
                    m_Base->Destroy();
                    SpawnExplosion(m_Base->GetX(), m_Base->GetY());
                    EnterGameOver();
                    continue;
                }
            }

            // ===== 再檢查玩家 =====
            if (!m_Player || !m_Player->IsAlive()) continue;

            Rect playerRect = MakeRect(
                m_Player->GetPosition().x,
                m_Player->GetPosition().y,
                10.0f,
                10.0f
            );

            if (IsColliding(bulletRect, playerRect)) {

                float hitX = bullet->GetX();
                float hitY = bullet->GetY();

                // 刪子彈
                bullet->Deactivate();

                // 爆炸（用 App 的爆炸池，比 Enemy 自己那個好）
                SpawnExplosion(hitX, hitY);

                // 玩家死亡
                m_Player->Kill();

                break;
            }
        }
    }
}

void App::StartPlayerRespawn(float x, float y) {
    m_PlayerRespawning = true;
    m_PlayerRespawnX = x;
    m_PlayerRespawnY = y;

    m_PlayerRespawnEffect = std::make_unique<RespawnEffect>(m_Root);
    m_PlayerRespawnEffect->Init(x, y);
}

void App::UpdatePlayerRespawn() {
    if (!m_PlayerRespawning || !m_PlayerRespawnEffect) return;

    m_PlayerRespawnEffect->Update();

    if (m_PlayerRespawnEffect->IsFinished()) {
        m_PlayerRespawnEffect->Clear();
        m_PlayerRespawnEffect.reset();

        if (m_Player) {
            m_Player->Clear();
            m_Player.reset();
        }

        m_Player = std::make_unique<Player>(m_Root);
        m_Player->Init(m_PlayerRespawnX, m_PlayerRespawnY);

        m_PlayerRespawning = false;
    }
}

void App::HandlePlayerDeath() {
    if (!m_Player) return;
    if (m_Player->IsAlive()) return;
    if (!m_Player->IsExplosionFinished()) return;
    if (m_PlayerRespawning) return;

    --m_PlayerLives;

    if (m_PlayerLives > 0) {
        StartPlayerRespawn(-200.0f, -384.0f);
    } else {
        EnterGameOver();
    }
}

void App::StartEnemySpawn(float x, float y, Enemy::EnemyType type) {
    m_EnemyRespawning = true;
    m_EnemySpawnX = x;
    m_EnemySpawnY = y;
    m_PendingEnemyType = type;

    m_EnemyRespawnEffect = std::make_unique<RespawnEffect>(m_Root);
    m_EnemyRespawnEffect->Init(x, y);
}

void App::UpdateEnemyRespawn() {
    if (!m_EnemyRespawning || !m_EnemyRespawnEffect) return;

    m_EnemyRespawnEffect->Update();

    if (m_EnemyRespawnEffect->IsFinished()) {
        m_EnemyRespawnEffect->Clear();
        m_EnemyRespawnEffect.reset();

        auto enemy = std::make_unique<Enemy>(m_Root);
        enemy->Init(m_EnemySpawnX, m_EnemySpawnY, m_PendingEnemyType);
        m_Enemies.push_back(std::move(enemy));

        m_EnemyRespawning = false;
        m_EnemySpawnCooldownFrames = m_EnemySpawnIntervalFrames;
    }
}

void App::RemoveDeadEnemies() {
    for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ) {
        if (*it && (*it)->IsRemovable()) {
            (*it)->Clear();
            it = m_Enemies.erase(it);
            ++m_KilledEnemies;
        } else {
            ++it;
        }
    }
}

void App::EnterGameOver() {
    if (m_Phase == Phase::GAME_OVER) return;

    m_Phase = Phase::GAME_OVER;

    m_GameOverBg = std::make_shared<Character>(
        std::string(RESOURCE_DIR) + "/image/ui/black_bg.png"
    );
    m_GameOverBg->SetPosition({0.0f, 0.0f});
    m_GameOverBg->SetZIndex(150.0f);
    m_Root.AddChild(m_GameOverBg);

    m_GameOverBanner = std::make_unique<GameOverBanner>(m_Root);
    m_GameOverBanner->Init();
}

void App::UpdateGameOver() {
    if (m_GameOverBanner) {
        m_GameOverBanner->Update();
    }
}

void App::ClearGameOverUi() {
    if (m_GameOverBanner) {
        m_GameOverBanner->Clear();
        m_GameOverBanner.reset();
    }

    if (m_GameOverBg) {
        m_Root.RemoveChild(m_GameOverBg);
        m_GameOverBg.reset();
    }
}