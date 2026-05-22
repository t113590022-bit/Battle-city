#include "App.hpp"
#include "iostream"
#include "TitleMenu.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Explosion.hpp"
#include "Enemy.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    SDL_StopTextInput();

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

        case Phase::STAGE_INTRO:
            UpdateStageIntro();
            break;

        case Phase::PLAYING:
            UpdatePlaying();
            break;

        case Phase::STAGE_CLEAR:
            UpdateStageClear();
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

    ClearStageHud();
    ClearBullets();
    ClearExplosions();
    ClearEnemies();
    ClearStageClearUi();
    ClearStageIntroUi();

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
        EnterStageIntro(true);
    }
}

void App::EnterStageIntro(bool allowSelect) {
    if (m_TitleMenu) {
        m_TitleMenu->Clear();
        m_TitleMenu.reset();
    }

    ClearStageIntroUi();
    ClearStageClearUi();
    ClearGameOverUi();

    if (!allowSelect) {
        ClearPlayingObjects();
    }

    m_StageIntroScreen = std::make_unique<StageIntroScreen>(m_Root);
    m_StageIntroScreen->Init(
        m_CurrentStage,
        allowSelect,
        StageData::GetStageCount()
    );

    m_Phase = Phase::STAGE_INTRO;
}

void App::UpdateStageIntro() {
    if (!m_StageIntroScreen) return;

    m_StageIntroScreen->Update();

    if (m_StageIntroScreen->IsFinished()) {
        m_CurrentStage = m_StageIntroScreen->GetSelectedStage();

        ClearStageIntroUi();

        EnterPlaying();
    }
}

void App::ClearStageIntroUi() {
    if (m_StageIntroScreen) {
        m_StageIntroScreen->Clear();
        m_StageIntroScreen.reset();
    }
}

void App::UpdateStageClear() {
    if (m_StageClearScreen) {
        m_StageClearScreen->Update();

        if (m_StageClearScreen->IsFinished()) {
            std::cout << "StageClear finished. Current stage before ++ = "
                     << m_CurrentStage << std::endl;

            ClearStageClearUi();

            ++m_CurrentStage;

            std::cout << "Next stage = " << m_CurrentStage << std::endl;
            std::cout << "Stage count = " << StageData::GetStageCount() << std::endl;

            // if (m_CurrentStage > StageData::GetStageCount()) {
            //     std::cout << "Go to WIN phase" << std::endl;
            //     m_Phase = Phase::WIN;
            //     return;
            // }

            EnterStageIntro(false);
        }
    }
}

void App::EnterPlaying() {
    std::cout << "EnterPlaying stage = " << m_CurrentStage << std::endl;

    if (m_TitleMenu) {
        m_TitleMenu->Clear();
        m_TitleMenu.reset();
    }

    ClearPlayingObjects();
    ClearStageClearUi();

    StageConfig stage = StageData::GetStage(m_CurrentStage);

    InitMap(stage.mapData);

    InitBullets(10);   // 先用 10 顆就很夠了
    ClearExplosions();
    InitEnemies();
    ClearGameOverUi();
    ClearStageClearUi();

    PrepareStageEnemies(stage.enemies);

    m_NextSpawnPointIndex = 0;
    m_EnemySpawnCooldownFrames = 0;

    m_PlayerLives = 2;
    m_ShootCooldownFrames = 0;

    StartPlayerRespawn(-64.0f, -192.0f);
    TrySpawnNextEnemy();

    m_Base = std::make_unique<Base>(m_Root);
    m_Base->Init(0.0f, -192.0f);

    InitStageHud();

    m_Phase = Phase::PLAYING;
}

void App::InitStageHud() {
    ClearStageHud();

    m_StageHud = std::make_unique<StageHud>(m_Root);

    m_StageHud->Init(
        static_cast<int>(m_EnemyQueue.size()),
        m_SpawnedEnemies,
        m_PlayerLives,
        m_CurrentStage
    );
}

void App::UpdateStageHud() {
    if (!m_StageHud) return;

    m_StageHud->Update(
        static_cast<int>(m_EnemyQueue.size()),
        m_SpawnedEnemies,
        m_PlayerLives,
        m_CurrentStage
    );
}

void App::ClearStageHud() {
    if (m_StageHud) {
        m_StageHud->Clear();
        m_StageHud.reset();
    }
}

// 預備敵人生成
void App::PrepareStageEnemies(const std::vector<Enemy::EnemyType>& enemies) {
    m_NormalKillCount = 0;
    m_FastKillCount = 0;
    m_PowerKillCount = 0;
    m_HeavyKillCount = 0;

    m_EnemyQueue.clear();

    m_EnemyQueue = enemies;

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

    const float spawnXs[3] = {0.0f, 160.0f, -160.0f};
    const float spawnY = 192.0f;

    float spawnX = spawnXs[m_NextSpawnPointIndex];
    m_NextSpawnPointIndex = (m_NextSpawnPointIndex + 1) % 3;

    StartEnemySpawn(spawnX, spawnY, m_EnemyQueue[m_SpawnedEnemies]);
    ++m_SpawnedEnemies;
}

void App::OnEnemyDestroyed(Enemy::EnemyType type) {
    switch (type) {
        case Enemy::EnemyType::NORMAL:
            ++m_NormalKillCount;
            break;
        case Enemy::EnemyType::FAST:
            ++m_FastKillCount;
            break;
        case Enemy::EnemyType::POWER:
            ++m_PowerKillCount;
            break;
        case Enemy::EnemyType::HEAVY:
            ++m_HeavyKillCount;
            break;
    }
}

void App::UpdatePlaying() {
    if (Util::Input::IsKeyPressed(Util::Keycode::F1)) {
        m_NormalKillCount = 5;
        m_FastKillCount = 4;
        m_PowerKillCount = 6;
        m_HeavyKillCount = 5;
        EnterStageClear();
        return;
    }

    if (m_Map) {
        m_Map->Update();
    }
    
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

    UpdateStageHud();

    HandlePlayerDeath();
    UpdatePlayerRespawn();
    UpdateEnemyRespawn();

    if (m_SpawnedEnemies >= static_cast<int>(m_EnemyQueue.size()) &&
        GetAliveEnemyCount() == 0 &&
        !m_EnemyRespawning) {
            EnterStageClear();
            return;
        }
    }

void App::HandleSystemInput() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::InitMap(const std::vector<std::string>& mapData) {
    if (m_Map) {
        m_Map->Clear();
        m_Map.reset();
    }

    m_Map = std::make_unique<Map>(m_Root);
    m_Map->Init(mapData);
}

void App::InitBullets(int poolSize) {
    ClearBullets();

    for (int i = 0; i < poolSize; ++i) {
        m_Bullets.push_back(std::make_unique<Bullet>(m_Root));
    }
}

void App::InitEnemies() {
    ClearEnemies();
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
    if (!m_Player || !m_Map) return;

    std::vector<Rect> blockingRects;

    for (const auto& enemy : m_Enemies) {
        if (!enemy || !enemy->IsAlive()) continue;

        blockingRects.push_back(enemy->GetCollisionRect());
    }

    if (m_Base && m_Base->IsAlive()) {
        blockingRects.push_back(m_Base->GetCollisionRect());
    }

    m_Player->Update(*m_Map, blockingRects);
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

HitDirection App::ConvertBulletDirection(Bullet::Direction dir) const {
    switch (dir) {
        case Bullet::Direction::UP:
            return HitDirection::UP;

        case Bullet::Direction::DOWN:
            return HitDirection::DOWN;

        case Bullet::Direction::LEFT:
            return HitDirection::LEFT;

        case Bullet::Direction::RIGHT:
            return HitDirection::RIGHT;
    }

    return HitDirection::UP;
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
        if (!bullet || !bullet->IsActive()) continue;

        bullet->Update();

        Rect bulletRect = MakeRect(
            bullet->GetX(),
            bullet->GetY(),
            4.0f,
            4.0f
        );

        bool hit = false;
        bool shouldSpawnExplosion = false;

        float hitX = bullet->GetX();
        float hitY = bullet->GetY();

        // 1. 檢查是否打到敵人
        for (auto& enemy : m_Enemies) {
            if (!enemy || !enemy->IsAlive()) continue;

            Rect enemyRect = MakeRect(
                enemy->GetX(),
                enemy->GetY(),
                10.0f,
                10.0f
            );

            if (IsColliding(bulletRect, enemyRect)) {
                enemy->TakeDamage();

                hit = true;
                shouldSpawnExplosion = true;
                break;
            }
        }

        // 2. 檢查是否打到地圖物件：Brick / Steel
        if (!hit) {
            TileObject* tile = m_Map->GetTileObjectAtWorld(hitX, hitY);

            if (tile && tile->BlocksBullet()) {
                TileHitInfo info;
                info.hitPoint = {hitX, hitY};
                info.direction = ConvertBulletDirection(bullet->GetDirection());
                info.power = 1;

                TileHitResult result = tile->OnBulletHit(info);

                if (result.tileDestroyed) {
                    m_Map->RemoveTileAtWorld(hitX, hitY);
                }

                if (result.bulletStopped) {
                    hit = true;
                    shouldSpawnExplosion = result.spawnExplosion;
                }
            }
        }

        // 3. 檢查是否打到基地
        if (!hit && m_Base && m_Base->IsAlive()) {
            Rect baseRect = MakeRect(
                m_Base->GetX(),
                m_Base->GetY(),
                16.0f,
                16.0f
            );

            if (IsColliding(bulletRect, baseRect)) {
                m_Base->Destroy();
                bullet->Deactivate();

                SpawnExplosion(m_Base->GetX(), m_Base->GetY());

                EnterGameOver();
                continue;
            }
        }

        // 4. 檢查是否超出邊界
        if (!hit && bullet->IsOutOfBounds(*m_Map)) {
            hit = true;
            shouldSpawnExplosion = true;
        }

        // 5. 統一處理命中結果
        if (hit) {
            bullet->Deactivate();

            if (shouldSpawnExplosion) {
                SpawnExplosion(hitX, hitY);
            }

            continue;
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
        if (!enemy) continue;

        std::vector<Rect> blockingRects;

        // 1. 玩家阻擋敵人
        if (m_Player && m_Player->IsAlive()) {
            blockingRects.push_back(m_Player->GetCollisionRect());
        }

        // 2. 其他敵人阻擋這隻敵人
        for (const auto& otherEnemy : m_Enemies) {
            if (!otherEnemy || otherEnemy.get() == enemy.get()) continue;
            if (!otherEnemy->IsAlive()) continue;

            blockingRects.push_back(otherEnemy->GetCollisionRect());
        }

        if (m_Base && m_Base->IsAlive()) {
            blockingRects.push_back(m_Base->GetCollisionRect());
        }

        bool hasPlayer = m_Player && m_Player->IsAlive();
        glm::vec2 playerPos = hasPlayer
            ? m_Player->GetPosition()
            : glm::vec2{0.0f, 0.0f};

        enemy->Update(*m_Map, blockingRects, hasPlayer, playerPos);

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
                    16.0f,
                    16.0f
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
        StartPlayerRespawn(-64.0f, -192.0f);
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
            OnEnemyDestroyed((*it)->GetType());

            (*it)->Clear();
            it = m_Enemies.erase(it);
            ++m_KilledEnemies;
        } else {
            ++it;
        }
    }
}

void App::EnterGameOver() {
    std::cout << "EnterGameOver called. Current stage = "
              << m_CurrentStage << std::endl;

    if (m_Phase == Phase::GAME_OVER) return;

    m_Phase = Phase::GAME_OVER;

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

void App::EnterStageClear() {
    if (m_Phase == Phase::STAGE_CLEAR) return;

    m_Phase = Phase::STAGE_CLEAR;

    m_StageClearScreen = std::make_unique<StageClearScreen>(m_Root);
    m_StageClearScreen->Init(
        m_CurrentStage,
        m_NormalKillCount,
        m_FastKillCount,
        m_PowerKillCount,
        m_HeavyKillCount
    );
}

void App::ClearStageClearUi() {
    if (m_StageClearScreen) {
        m_StageClearScreen->Clear();
        m_StageClearScreen.reset();
    }
}

void App::ClearPlayingObjects() {
    ClearStageHud();

    ClearBullets();
    ClearExplosions();
    ClearEnemies();

    if (m_PlayerRespawnEffect) {
        m_PlayerRespawnEffect->Clear();
        m_PlayerRespawnEffect.reset();
    }

    if (m_EnemyRespawnEffect) {
        m_EnemyRespawnEffect->Clear();
        m_EnemyRespawnEffect.reset();
    }

    m_PlayerRespawning = false;
    m_EnemyRespawning = false;

    if (m_Player) {
        m_Player->Clear();
        m_Player.reset();
    }

    if (m_Base) {
        m_Base->Clear();
        m_Base.reset();
    }

    if (m_Map) {
        m_Map->Clear();
        m_Map.reset();
    }

    ClearGameOverUi();
    ClearStageClearUi();
}