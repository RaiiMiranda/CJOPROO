// -------------------------------------------------------------------------------------------------------------------------------------------
// Código do Hamstorm
// top-down arena shooter roguelike (jogo de tiro com área limitada; fases cronometradas; se morrer, recomeça)
// -------------------------------------------------------------------------------------------------------------------------------------------

#include "raylib.h"           // para incluir a biblioteca raylib
#include "raymath.h"          // para MatrixRotateXYZ(), Vector2Normalize
#include "init.hpp"           // para inicializar as variaveis
#include "loadTextures.hpp"   // para carregar as texturas
#include "unloadTextures.hpp" // para descarregar as texturas
#include <string>             // para incluir strings

// -------------------------------------------------------------------------------------------------------------------------------------------
// Função para Spawnar Inimigos
// -------------------------------------------------------------------------------------------------------------------------------------------
void spawnEnemy(Enemy enemy[], int type, int i)
{
    enemy[i].active = true;
    enemy[i].position.x = GetRandomValue(MIN_AREA, MAX_AREA - ENEMY_SIZE);
    enemy[i].position.y = GetRandomValue(MIN_AREA, MAX_AREA - ENEMY_SIZE);

    enemy[i].type = type;

    if (type == 1) 
    {
        enemy[i].vel = 3.0f;
        enemy[i].lives = 5;
    } 
    else if (type == 2) 
    {
        enemy[i].vel = 4.0f;
        enemy[i].lives = 10;
    }
    else if (type == 3) 
    {
        enemy[i].vel = 10.0f;
        enemy[i].lives = 15;
    }
    else if (type == 4) 
    {
        enemy[i].vel = 8.0f;
        enemy[i].lives = 15;
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// Função Principal
// -------------------------------------------------------------------------------------------------------------------------------------------
int main(void) 
{
    // ---------------------------------------------------------------------------------------------------------------------------------------
    // Inicialização
    // ---------------------------------------------------------------------------------------------------------------------------------------
    // Constantes para o tamanho da janela
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    // Inicializando o tamanho da janela e título dela
    InitWindow(screenWidth, screenHeight, "Hamstorm"); 
    HideCursor();

    // Inicializando o sistema de áudio
    InitAudioDevice();

    // Definindo as sprites
    Textures tex = loadTextures();

    // Variáveis para todas as animações
    int totalFrames = 2;     // para o player e o cristal
    int currentFrame = 0;    // para o player e o cristal
    float frameTime = 0.15f; // para o player e o cristal
    float timer = 0.0f;      // para todos
    float bossTimer = 0.0f;  // para o boss

    // Variáveis para animação do Player
    int frameWidthPlayer = tex.hamsterWalk.width / totalFrames;
    int frameHeightPlayer = tex.hamsterWalk.height;

    Rectangle frameRecPlayer = { 0, 0, (float)frameWidthPlayer, (float)frameHeightPlayer };

    // Variáveis para animação dos Inimigos
    int totalFramesEnemy = 5;
    float frameTimeEnemy = 0.15f;
    int currentFrameEnemy = 0;

    // Tamanho dos frames dos inimigos simples
    int frameWidthEnemy = tex.enemy1.width / totalFramesEnemy;
    int frameHeightEnemy = tex.enemy1.height;

    Rectangle frameRecEnemy = { 0, 0, (float)frameWidthEnemy, (float)frameHeightEnemy };

    // Tamanho dos frames do boss
    int frameWidthBoss = tex.boss.width / totalFramesEnemy;
    int frameHeightBoss = tex.boss.height;

    Rectangle frameRecBoss = { 0, 0, (float)frameWidthBoss, (float)frameHeightBoss };

    // Variáveis para animação dos Cristais
    int frameWidthCristal = tex.cristal.width / totalFrames;
    int frameHeightCristal = tex.cristal.height;

    Rectangle frameRecCristal = { 0, 0, (float)frameWidthCristal, (float)frameHeightCristal };

    // Declarando o player
    Player player = { 0 };
    player.position = { (float)screenWidth/2.0f, (float)screenHeight/2.0f };
    player.direction = { 0, 1 }; // começa olhando para baixo
    player.damage = 1.0f;
    player.vel = 10.0f;
    player.lives = 15;
    player.crystals = 0;
    //player.powerUp = false;

    // Hitbox do player
    Rectangle playerRect = {
        player.position.x - tex.hamsterWalk.width / 2,
        player.position.y - tex.hamsterWalk.height / 2,
        (float)tex.hamsterWalk.width,
        (float)tex.hamsterWalk.height
    };

    // Declarando a camera do player
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f }; // ponto de visão na tela (em pixels) - no caso o centro da tela
    camera.rotation = 0.0f;
    camera.zoom = 0.4f;
    Vector2 originalOffset = { screenWidth/2.0f, screenHeight/2.0f };

    // Declarando os inimigos
    Enemy enemy[NUM_MAX_ENEMIES] = { 0 };

    // Declarando o boss
    Boss boss = { 0 };
    boss.active = false;
    boss.vel = 8.0f;
    boss.lives = 100;

    // Declarando os tiros
    Shoot shoots[NUM_MAX_SHOOTS] = { 0 };

    // Declarando os cristais
    Crystal crystals[NUM_MAX_CRYSTALS] = { 0 };

    // Declarando os itens compráveis
    Store item [] = {
        { "Extra Life", 50, 50, 2, false },
        { "Damage", 50, 50, 1, false },
        { "Speed", 40, 40, 1, false },
        { "Health Recovery", 60, 60, 1, false },
        { "Dash", 300, 300, 1, false }
    };

    // Declarando variáveis globais
    GameScreen currentScreen = MENU;       // para a tela inicial começar no Menu
    Characters currentCharacter = HAMSTER; // para começar um persona selecionado
    Vector2 mousePosition = { 0 };         // para a posição inicial do mouse
    int currentLife = player.lives;        // para guardar a vida atual do player
    int totalEnemiesKilled = 0;            // para estatísticas finais
    float recoveryCooldown = 0.0f;         // para cooldown de recuperação de vida
    float shakeTime = 0.0f;                // para o tempo de tremor da tela

    // Declarando variáveis globais para controlar as waves
    Waves currentWave = FIRST; // para a wave inicial
    float waveTime = 30.0f;    // para definir o tempo de cada wave
    int enemiesKill = 0;       // para saber quantos inimigos morreram
    int spawnedEnemies = 0;    // para saber quantos ja spawnaram

    // Declarando variáveis globais para controlar cooldowns
    float lastSpawn1 = 0.0f;         // para pegar o tempo do último spawn
    float lastSpawn2 = 0.0f;         // para pegar o tempo do último spawn
    float lastSpawn3 = 0.0f;         // para pegar o tempo do último spawn
    float lastSpawn4 = 0.0f;         // para pegar o tempo do último spawn
    float lastRecovery = 0.0f;       // para pegar o tempo do último recovery
    float lastShootTime = 0.0f;      // para pegar o último tiro disparado
    float lastDamageTime = 0.0f;     // para pegar o último dano levado
    float damageAnimTime = 0.0f;     // para pegar o tempo de animacao
    float damageAnimDuration = 0.2f; // para pegar a duracao da animacao de dano

    // Declarando booleanos
    bool gameRunning = true;       // para controlar o botao de Quit
    bool gamePaused = false;       // para controlar se o jogo está pausado ou não
    bool isShaking = false;        // para controlar o tremor da tela
    bool waveEnded = false;        // para controlar se a wave acabou
    bool isBossDead = false;       // para controlar se o boss morreu
    bool playerTookDamage = false; // para controlar se o player tomou dano

    // Declarando a fonte dos textos
    Font font = LoadFont("assets/fonts/romulus.png");

    // Declarando os sons
    Music music = LoadMusicStream("assets/sounds/music01.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5f);

    Sound sdMachineGun = LoadSound("assets/sounds/machine_gun.mp3");
    Sound sdPistol     = LoadSound("assets/sounds/pistol.mp3");
    Sound sdShotgun    = LoadSound("assets/sounds/shotgun.mp3");

    SetSoundVolume(sdPistol, 0.4f);

    // Definindo que o jogo vai rodar a 60fps
    SetTargetFPS(60); 
    // ---------------------------------------------------------------------------------------------------------------------------------------


    // ---------------------------------------------------------------------------------------------------------------------------------------
    // Looping Principal do Jogo
    // ---------------------------------------------------------------------------------------------------------------------------------------
    // Enquanto não fechou a janela (ESQ ou botao de fechar) - esse while repete tudo frame a frame
    while (gameRunning && !WindowShouldClose())  
    {
        // Começando a tocar a música
        UpdateMusicStream(music);

        Vector2 mousePos = GetMousePosition();
        
        // Controlando o que cada Tela vai fazer
        switch (currentScreen)
        {
            // ------------------------------------------------------------------------------------------------------------------------------------
            // Tela de Menu Inicial
            // ------------------------------------------------------------------------------------------------------------------------------------
            case MENU:
            {
                // Definição dos Botões
                // Pegando a posição do mouse
                mousePosition = GetMousePosition();

                // Definindo o tamanho dos botões com pos x, pos y, width, height
                Rectangle btnPlay    = { 100, 380, 280, 50 }; 
                Rectangle btnQuit    = { 390, 380, 280, 50 };

                // Condições para verificar qual botão está sendo apertado com o mouse
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnPlay))
                    currentScreen = GAME_SETTINGS; // aqui seria GAME_SETTINGS ne

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnQuit))
                    gameRunning = false; // não usar CloseWindow() aqui, porque precisaria fazer os 'Unload' junto, mas é melhor fazer tudo no final

                // Resetando o personagem quando volta para o Menu
                currentCharacter = HAMSTER;

                // Desenhando a Tela do Menu
                BeginDrawing();

                    // Limpa o fundo com a cor de fundo
                    ClearBackground(RAYWHITE);

                    DrawTextureEx(tex.bgMainMenu, { 0,0 }, 0.0f, 1, WHITE);

                    // Desenhando o título do jogo
                    DrawTextEx(font, "HAMSTORM", { 180, 25 }, 80, 2, WHITE);

                    // Desenhando o botão de Play
                    if (CheckCollisionPointRec(mousePosition, btnPlay)) 
                    {
                        DrawRectangleRec(btnPlay, WHITE);
                        DrawTextEx(font, "Play", {btnPlay.x + 110, btnPlay.y + 5}, 40, -2, DARKGRAY);
                    }
                    else 
                    {
                        DrawRectangleRec(btnPlay, DARKGRAY);
                        DrawTextEx(font, "Play", {btnPlay.x + 110, btnPlay.y + 5}, 40, -2, WHITE);
                    }
                    
                    // Desenhando o botão de Sair
                    if (CheckCollisionPointRec(mousePosition, btnQuit)) 
                    {
                        DrawRectangleRec(btnQuit, WHITE);
                        DrawTextEx(font, "Quit", {btnQuit.x + 115, btnQuit.y + 5}, 40, -2, DARKGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(btnQuit, DARKGRAY);
                        DrawTextEx(font, "Quit", {btnQuit.x + 115, btnQuit.y + 5}, 40, -2, WHITE);
                    }

                    // Desenhando o cursor depois de tudo
                    DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE);

                EndDrawing();
                break;
            }

            // ------------------------------------------------------------------------------------------------------------------------------------
            // Tela de Definição de Personagem/Habilidades
            // ------------------------------------------------------------------------------------------------------------------------------------
            case GAME_SETTINGS:
            {
                // Escolha dos personagens
                // 1. Hamster
                // 2. Raposa
                // 3. Furao

                mousePosition = GetMousePosition();

                // Definindo o tamanho dos botões com pos x, pos y, width, height
                Rectangle btnFox     = { 100, 75, 180, 250 };
                Rectangle btnHamster = { 300, 60, 200, 280 }; 
                Rectangle btnFurrest = { 520, 75, 180, 250 };
                Rectangle btnPlay    = { 175, 360, 200, 50 };
                Rectangle btnBack    = { 390, 360, 200, 50 }; 
            
                // Condições para verificar qual botão está sendo apertado com o mouse
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnFox)) currentCharacter = FOX;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnHamster)) currentCharacter = HAMSTER;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnFurrest)) currentCharacter = FERRET;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnBack)) currentScreen = MENU;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnPlay)) currentScreen = GAMEPLAY;
                
                // Desenhando a Tela de Opções
                BeginDrawing();

                    // Colocando a cor de fundo
                    Color bgColor = { 75, 61, 68, 255 };
                    ClearBackground(bgColor);

                    // Destacar qual botão está escolhido
                    // Desenhando o botão Fox 
                    DrawRectangleRec(btnFox, WHITE);
                    DrawTextEx(font, "Fox", { btnFox.x + 55, btnFox.y + 10 }, 40, 2, DARKGRAY);

                    // Desenhando o botão Hamster
                    Color bgColor2 = { 87, 72, 82, 255 };
                    DrawRectangleRec(btnHamster, bgColor2);

                    Color textColor = { 210, 201, 165, 255 };
                    DrawTextEx(font, "Hamster", { btnHamster.x + 22, btnHamster.y + 10 }, 40, 2, textColor);
                    DrawTextureEx(tex.hamster_hud, { 330, 120 }, 0.0f, 4, WHITE);

                    // Desenhando o botão Furrest
                    DrawRectangleRec(btnFurrest, WHITE);
                    DrawTextEx(font, "Furrest", { btnFurrest.x + 15, btnFurrest.y + 10 }, 40, 2, DARKGRAY);

                    // Desenhando o botão de Play
                    if (CheckCollisionPointRec(mousePosition, btnPlay)) 
                    {
                        DrawRectangleRec(btnPlay, DARKGRAY);
                        DrawTextEx(font, "Play", { btnPlay.x + 60, btnPlay.y + 5 }, 40, -2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnPlay, WHITE);
                        DrawTextEx(font, "Play", { btnPlay.x + 60, btnPlay.y + 5 }, 40, -2, DARKGRAY);
                    }

                    // Desenhando o botão de Back
                    if (CheckCollisionPointRec(mousePosition, btnBack)) 
                    {
                        DrawRectangleRec(btnBack, DARKGRAY);
                        DrawTextEx(font, "Back", { btnBack.x + 60, btnBack.y + 5 }, 40, -2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnBack, WHITE);
                        DrawTextEx(font, "Back", { btnBack.x + 60, btnBack.y + 5 }, 40, -2, DARKGRAY);
                    }

                    // Desenhando o cursor depois de tudo
                    DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE);

                EndDrawing();

                break;
            }
            
            // ------------------------------------------------------------------------------------------------------------------------------------
            // Tela da Partida em si
            // ------------------------------------------------------------------------------------------------------------------------------------
            case GAMEPLAY:
            {
                // Se apertar P, então pausa ou despausa o jogo
                if (IsKeyPressed(KEY_P))
                    gamePaused = !gamePaused;
                
                // Se o jogo não estiver pausado, então roda esse código
                if (!gamePaused)
                {
                    float currentTime = GetTime();    // para pegar o tempo total desde o inicio do programa e usar nos cooldowns
                    float deltaTime = GetFrameTime(); // para pegar o tempo que passou desde o último frame e usar no cronometro
                    
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Animação dos Cristais
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    timer += GetFrameTime(); 
                    if (timer >= frameTime) 
                    {
                        timer = 0.0f; 
                        currentFrame = (currentFrame + 1) % totalFrames; 
                        frameRecCristal.x = (float)currentFrame * frameWidthCristal; 
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Animação do Player
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    timer += GetFrameTime(); 
                    if (timer >= frameTime) 
                    {
                        timer = 0.0f; 
                        currentFrame = (currentFrame + 1) % totalFrames; 
                        frameRecPlayer.x = (float)currentFrame * frameWidthPlayer; 
                    }
                    
                    // Se o player tomou dano
                    if (playerTookDamage)
                    {
                        damageAnimTime += deltaTime;
                        if (damageAnimTime >= damageAnimDuration)
                            playerTookDamage = false;
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Animação dos Inimigos
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    timer += GetFrameTime(); 
                    if (timer >= frameTimeEnemy) 
                    {
                        timer = 0.0f; 
                        currentFrameEnemy = (currentFrameEnemy + 1) % totalFramesEnemy; 
                        frameRecEnemy.x = (float)currentFrameEnemy * frameWidthEnemy; 
                    }
                    
                    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                    {
                        // Se o inimigo tomou dano
                        if (enemy[i].tookDamage)
                        {
                            damageAnimTime += deltaTime;
                            if (damageAnimTime >= damageAnimDuration)
                                enemy[i].tookDamage = false; 
                        }
                    }

                    bossTimer += GetFrameTime();
                    if (bossTimer >= frameTime)
                    {
                        bossTimer = 0.0f;
                        currentFrameEnemy = (currentFrameEnemy + 1) % totalFramesEnemy;
                        frameRecBoss.x = currentFrameEnemy * frameWidthBoss;
                    }

                    // Se o boss tomou dano
                    if (boss.tookDamage)
                    {
                        damageAnimTime += deltaTime;
                        if (damageAnimTime >= damageAnimDuration)
                            boss.tookDamage = false;
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Controlando as Waves
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    switch (currentWave)
                    {
                        // Caso seja a Primeira Onda
                        case FIRST:
                        {  
                            // Spawnando gradualmente os inimigos da wave
                            float spawnCooldown1 = 1.5f; // cooldown de spawn do inimigo 1
                            waveEnded = false;
                            boss.active = false;
                            isBossDead = false;

                            // Se deu o tempo do cooldown 1
                            if (currentTime - lastSpawn1 >= spawnCooldown1)
                            {
                                // Então, spawna os inimigos 1 da wave
                                spawnEnemy(enemy, 1, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn1 = currentTime; 
                            }

                            // Diminui o tempo da wave
                            waveTime -= deltaTime;

                            // Se acabou o tempo
                            if (waveTime <= 0)
                            {
                                // Pega o total de inimigos mortos para estatítica final
                                totalEnemiesKilled += enemiesKill;

                                // Zera o enemiesKill para começar a contagem da próxima wave
                                enemiesKill = 0;

                                // Desativa todos os inimigos antes
                                for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                                    enemy[i].active = false;

                                // Prepara as variáveis para a próxima onda
                                currentWave = SECOND;
                                spawnedEnemies = 0;       // reinicia spawn da nova wave
                                lastSpawn1 = currentTime; // reinicia cooldown 1
                                waveTime = 45.0f;
                                waveEnded = true;
                                player.lives = currentLife; // para reiniciar as vidas

                                // Troca de tela
                                currentScreen = STORE;
                            }

                            break;  
                        }
                        
                        // Caso seja a Segunda Onda
                        case SECOND:
                        {
                            // Spawnando gradualmente os inimigos da segunda wave
                            float spawnCooldown1 = 1.5f; // para definir o tempo de cooldown entre os spawns
                            float spawnCooldown2 = 2.5f; // cooldown de spawn do inimigo 2
                            waveEnded = false;

                            // Se o deu o tempo do cooldown 1
                            if (currentTime - lastSpawn1 >= spawnCooldown1)
                            {
                                // Então, spawna os inimigos 1 da wave
                                spawnEnemy(enemy, 1, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn1 = currentTime; 
                            }

                            // Se deu o tempo do cooldown 2
                            if (currentTime - lastSpawn2 >= spawnCooldown2)
                            {
                                // Então, spawna os inimigos 2 da wave
                                spawnEnemy(enemy, 2, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn2 = currentTime; 
                            }

                            // Diminui o tempo da wave
                            waveTime -= deltaTime;

                            // Se acabou o tempo
                            if (waveTime <= 0)
                            {
                                // Pega o total de inimigos mortos para estatítica final
                                totalEnemiesKilled += enemiesKill;
                                
                                // Zera o enemiesKill para começar a contagem da próxima wave
                                enemiesKill = 0;

                                // Desativa todos os inimigos antes
                                for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                                    enemy[i].active = false;

                                // Prepara as variáveis para a próxima onda
                                currentWave = THIRD;
                                spawnedEnemies = 0;       // reinicia spawn da nova wave
                                lastSpawn1 = currentTime; // reinicia cooldown
                                lastSpawn2 = currentTime; // reinicia cooldown 2
                                waveTime = 50.0f;
                                waveEnded = true; 
                                player.lives = currentLife; // para reiniciar as vidas

                                // Troca de tela
                                currentScreen = STORE;
                            }

                            break;
                        }

                        // Caso seja a Terceira Onda
                        case THIRD:
                        {
                            // Spawnando gradualmente os inimigos da terceira wave
                            float spawnCooldown1 = 1.5f; // para definir o tempo de cooldown entre os spawns
                            float spawnCooldown2 = 2.5f; // cooldown de spawn do inimigo 2
                            float spawnCooldown3 = 3.5f; // cooldown de spawn do inimigo 3
                            waveEnded = false;

                            // Se o deu o tempo do cooldown 1
                            if (currentTime - lastSpawn1 >= spawnCooldown1)
                            {
                                // Então, spawna os inimigos 1 da wave
                                spawnEnemy(enemy, 1, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn1 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 2
                            if (currentTime - lastSpawn2 >= spawnCooldown2)
                            {
                                // Então, spawna os inimigos 2 da wave
                                spawnEnemy(enemy, 2, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn2 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 3
                            if (currentTime - lastSpawn3 >= spawnCooldown3)
                            {
                                // Então, spawna os inimigos 3 da wave
                                spawnEnemy(enemy, 3, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn3 = currentTime; 
                            }

                            // Diminui o tempo da wave
                            waveTime -= deltaTime;

                            // Se acabou o tempo
                            if (waveTime <= 0)
                            {
                                // Pega o total de inimigos mortos para estatítica final
                                totalEnemiesKilled += enemiesKill;

                                // Zera o enemiesKill para começar a contagem da próxima wave
                                enemiesKill = 0;

                                // Desativa todos os inimigos antes
                                for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                                    enemy[i].active = false;

                                // Prepara as variáveis para a próxima onda
                                currentWave = FOURTH;
                                spawnedEnemies = 0;       // reinicia spawn da nova wave
                                lastSpawn1 = currentTime; // reinicia cooldown
                                lastSpawn2 = currentTime; // reinicia cooldown
                                lastSpawn3 = currentTime; // reinicia cooldown
                                waveTime = 60.0f;
                                waveEnded = true;
                                player.lives = currentLife; // para reiniciar as vidas

                                // Troca de tela
                                currentScreen = STORE;
                            }

                            break;
                        }

                        // Caso seja a Quarta Onda
                        case FOURTH:
                        {
                            // Spawnando gradualmente os inimigos da quarta wave
                            float spawnCooldown1 = 1.5f; // para definir o tempo de cooldown entre os spawns
                            float spawnCooldown2 = 2.5f; // cooldown de spawn do inimigo 2
                            float spawnCooldown3 = 2.5f; // cooldown de spawn do inimigo 3
                            float spawnCooldown4 = 3.5f; // cooldown de spawn do inimigo 4
                            waveEnded = false;

                            // Se o deu o tempo do cooldown 1
                            if (currentTime - lastSpawn1 >= spawnCooldown1)
                            {
                                // Então, spawna os inimigos 1 da wave
                                spawnEnemy(enemy, 1, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn1 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 2
                            if (currentTime - lastSpawn2 >= spawnCooldown2)
                            {
                                // Então, spawna os inimigos 2 da wave
                                spawnEnemy(enemy, 2, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn2 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 3
                            if (currentTime - lastSpawn3 >= spawnCooldown3)
                            {
                                // Então, spawna os inimigos 3 da wave
                                spawnEnemy(enemy, 3, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn3 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 4
                            if (currentTime - lastSpawn4 >= spawnCooldown4)
                            {
                                // Então, spawna os inimigos 4 da wave
                                spawnEnemy(enemy, 4, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn4 = currentTime; 
                            }

                            // Diminui o tempo da wave
                            waveTime -= deltaTime;

                            // Se acabou o tempo
                            if (waveTime <= 0)
                            {
                                // Pega o total de inimigos mortos para estatítica final
                                totalEnemiesKilled += enemiesKill;

                                // Zera o enemiesKill para começar a contagem da próxima wave
                                enemiesKill = 0;

                                // Desativa todos os inimigos antes
                                for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                                    enemy[i].active = false;

                                // Prepara as variáveis para a próxima onda
                                currentWave = FIFTH;
                                spawnedEnemies = 0;       // reinicia spawn da nova wave
                                lastSpawn1 = currentTime; // reinicia cooldown
                                lastSpawn2 = currentTime; // reinicia cooldown
                                lastSpawn3 = currentTime; // reinicia cooldown
                                lastSpawn4 = currentTime; // reinicia cooldown
                                waveTime = 90.0f;
                                waveEnded = true;
                                player.lives = currentLife; // para reiniciar as vidas

                                // Troca de tela
                                currentScreen = STORE;
                            }

                            break;
                        }

                        // Caso seja a Quinta Onda
                        case FIFTH:
                        {
                            // Spawnando gradualmente os inimigos da quinta wave
                            float spawnCooldown1 = 2.5f;      // para definir o tempo de cooldown entre os spawns
                            float spawnCooldown2 = 2.5f;      // cooldown de spawn do inimigo 2
                            float spawnCooldown3 = 4.0f;      // cooldown de spawn do inimigo 3
                            float spawnCooldown4 = 5.0f;      // cooldown de spawn do inimigo 4
                            float spawnCooldownBoss = 200.0f; // para o boss nao spawnar novamente, pois a wave 5 acaba em 3 minutos
                            waveEnded = false;
                            boss.active = true;

                            if (currentTime - lastSpawn1 >= spawnCooldownBoss)
                            {
                                if (boss.active)
                                {
                                    boss.position.x = GetRandomValue(MIN_AREA, MAX_AREA - 200);
                                    boss.position.y = GetRandomValue(MIN_AREA, MAX_AREA - 200);
                                }
                            }

                            // Se o deu o tempo do cooldown 1
                            if (currentTime - lastSpawn1 >= spawnCooldown1)
                            {
                                // Então, spawna os inimigos 1 da wave
                                spawnEnemy(enemy, 1, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn1 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 2
                            if (currentTime - lastSpawn2 >= spawnCooldown2)
                            {
                                // Então, spawna os inimigos 2 da wave
                                spawnEnemy(enemy, 2, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn2 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 3
                            if (currentTime - lastSpawn3 >= spawnCooldown3)
                            {
                                // Então, spawna os inimigos 3 da wave
                                spawnEnemy(enemy, 3, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn3 = currentTime; 
                            }

                            // Se o deu o tempo do cooldown 4
                            if (currentTime - lastSpawn4 >= spawnCooldown4)
                            {
                                // Então, spawna os inimigos 4 da wave
                                spawnEnemy(enemy, 4, spawnedEnemies);

                                // Incrementa já que o inimigou spawnou e reinicia o tempo do cooldown
                                spawnedEnemies++;
                                lastSpawn4 = currentTime; 
                            }

                            // Diminui o tempo da wave
                            waveTime -= deltaTime;

                            // Se o player matou o boss
                            if (isBossDead)
                            {
                                // Então player ganhou
                                // Pega o total de inimigos mortos para estatítica final
                                totalEnemiesKilled += enemiesKill;
                                waveEnded = true;

                                // acho q nao precisa, pq troca de tela, e na tela de vitoria ja faz isso, testar dps
                                // Desativa todos os inimigos antes
                                //for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                                //    enemy[i].active = false;

                                currentScreen = VICTORY;
                            }
                            // Se o tempo acabou player perdeu
                            else if (waveTime <= 0)
                            {
                                waveEnded = true;

                                // Então player perdeu
                                // currentScreen = DEFEAT;
                            }

                            break;
                        }

                        default:
                            break;
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Movimentação, Dash e Recuperação de Vida do Player
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Fazendo o player recuperar vida por segundo, se ter comprado o item de recuperação de vida
                    if (recoveryCooldown > 0)
                    {
                        if (currentTime - lastRecovery >= recoveryCooldown)
                        {
                            if (player.lives < currentLife)
                            {
                                player.lives += 1;
                                lastRecovery = currentTime;
                            }
                        }
                    }   
                    
                    // Declara o vetor 'move' com os campos x e y zerados
                    Vector2 playerMove = { 0, 0 };

                    // Se apertar 1/2/3, então muda o tipo de arma do player
                    if (IsKeyDown(KEY_ONE)) currentCharacter = FOX;      // metralhadora
                    if (IsKeyDown(KEY_TWO)) currentCharacter = HAMSTER;  // pistola
                    if (IsKeyDown(KEY_THREE)) currentCharacter = FERRET; // doze
                    
                    // Se pressionar WASD o player se mexe
                    if (IsKeyDown(KEY_D)) playerMove.x += 1;
                    if (IsKeyDown(KEY_A)) playerMove.x -= 1; 
                    if (IsKeyDown(KEY_S)) playerMove.y += 1;
                    if (IsKeyDown(KEY_W)) playerMove.y -= 1;

                    // Se o player estiver se movendo
                    if (playerMove.x != 0 || playerMove.y != 0)
                    {
                        // Então normaliza para manter velocidade constante e não dobrar a velocidade na diagonal
                        playerMove = Vector2Normalize(playerMove);
                        player.position.x += playerMove.x * player.vel;
                        player.position.y += playerMove.y * player.vel;

                        // E atualiza a direção que o player está olhando
                        player.direction = playerMove;

                        /* 
                        Vector2Normalize(move)
                        -> Calcula a magnitude do vetor 'move' usando o Teorema de Pitágoras e retorna um vetor novo com magnitude 1
                        -> sem essa função a magnitude seria maior que 1 e a velocidade na diagonal iria dobrar

                        move = (0.707, 0.707);
                        player.position.x += 0.707 * 4.0; // ~2.828 px no X
                        player.position.y += 0.707 * 4.0; // ~2.828 px no Y
                        */

                        // Enquanto ele estiver se movendo, o impede de sair da área definida
                        if (player.position.x < MIN_AREA + tex.hamsterWalk.width/2) player.position.x = MIN_AREA + tex.hamsterWalk.width/2;
                        if (player.position.x > MAX_AREA - tex.hamsterWalk.width/2) player.position.x = MAX_AREA - tex.hamsterWalk.width/2;
                        if (player.position.y < MIN_AREA + tex.hamsterWalk.height/2) player.position.y = MIN_AREA + tex.hamsterWalk.height/2;
                        if (player.position.y > MAX_AREA - tex.hamsterWalk.height/2 - 90) player.position.y = MAX_AREA - tex.hamsterWalk.height/2 - 90;
                    }

                    // Se o player ter comprado a habilidade de dash e estiver apertando espaço
                    if (item[4].active && IsKeyPressed(KEY_SPACE))
                    {
                        player.position.x += player.direction.x * 50;
                        player.position.y += player.direction.y * 50;
                    }

                    // Definindo o alvo da camera sendo o player
                    camera.target = (Vector2){ player.position.x, player.position.y };

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Movimentação e Colisão dos Inimigos
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                    {
                        if (enemy[i].active)
                        {   
                            // Pega a direção do inimigo
                            Vector2 enemyMove = player.position - enemy[i].position;
                            enemyMove = Vector2Normalize(enemyMove);

                            // Movimenta os inimigos
                            enemy[i].position.x += enemyMove.x * enemy[i].vel;
                            enemy[i].position.y += enemyMove.y * enemy[i].vel;

                            // Enquanto ele estiver se movendo, o impede de sair da área definida
                            if (enemy[i].position.x < MIN_AREA + ENEMY_SIZE) enemy[i].position.x = MIN_AREA + ENEMY_SIZE;
                            if (enemy[i].position.x > MAX_AREA - ENEMY_SIZE) enemy[i].position.x = MAX_AREA - ENEMY_SIZE;
                            if (enemy[i].position.y < MIN_AREA + ENEMY_SIZE) enemy[i].position.y = MIN_AREA + ENEMY_SIZE;
                            if (enemy[i].position.y > MAX_AREA - ENEMY_SIZE) enemy[i].position.y = MAX_AREA - ENEMY_SIZE;

                            // Verificando o próximo inimigo para afastá-los e não deixar eles se sobreporem
                            for (int j = i + 1; j < NUM_MAX_ENEMIES; j++)
                            {
                                // Se ambos estiverem ativo
                                if (enemy[i].active && enemy[j].active)
                                {
                                    // Define a HitBox deles
                                    Rectangle enemy1Rect = { enemy[i].position.x, enemy[i].position.y, ENEMY_SIZE, ENEMY_SIZE };
                                    Rectangle enemy2Rect = { enemy[j].position.x, enemy[j].position.y, ENEMY_SIZE, ENEMY_SIZE };

                                    // Se eles se colidirem
                                    if (CheckCollisionRecs(enemy1Rect, enemy2Rect))
                                    {
                                        // Se a posição inimigo j estiver mais para a direita que inimigo i
                                        if (enemy[j].position.x > enemy[i].position.x)
                                            enemy[j].position.x += 1.0f; // Então empurra ele para direita
                                        // Se a posição inimigo j estiver mais para a esquerda que inimigo i
                                        else
                                            enemy[j].position.x -= 1.0f; // Então empura ele para esquerda 

                                        // Se a posição inimigo j estiver mais para baixo que inimigo i
                                        if (enemy[j].position.y > enemy[i].position.y)
                                            enemy[j].position.y += 1.0f; // Então empurra ele para baixo
                                        // Se a posição inimigo j estiver mais para cima que inimigo i
                                        else
                                            enemy[j].position.y -= 1.0f; // Então empurra ele para cima
                                    }
                                }
                            }
                        }
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Movimentação do Boss
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    if (boss.active)
                    {   
                        // Pega a direção do inimigo
                        Vector2 bossMove = player.position - boss.position;
                        bossMove = Vector2Normalize(bossMove);

                        // Movimenta os inimigos
                        boss.position.x += bossMove.x * boss.vel;
                        boss.position.y += bossMove.y * boss.vel;

                        // Enquanto ele estiver se movendo, o impede de sair da área definida
                        if (boss.position.x < MIN_AREA + 200) boss.position.x = MIN_AREA + 200;
                        if (boss.position.x > MAX_AREA - 200) boss.position.x = MAX_AREA - 200;
                        if (boss.position.y < MIN_AREA + 200) boss.position.y = MIN_AREA + 200;
                        if (boss.position.y > MAX_AREA - 200) boss.position.y = MAX_AREA - 200;
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Sistema de Tiros e Colisão com Inimigo
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Variáveis para controlar os tiros
                    float shootCooldown = 0.0f; // intervalo entre os tiros

                    if (currentCharacter == HAMSTER) shootCooldown = 0.4f; // pistola
                    if (currentCharacter == FOX) shootCooldown = 0.15f;    // metralha
                    if (currentCharacter == FERRET) shootCooldown = 1.0f;  // shotgun

                    // Se for escolhido o personagem HAMSTER ou FOX e passou o tempo suficiente para o próximo tiro e estiver apertando o botão esquerdo do mouse
                    // currentTime - lastShootTime (retorna o tempo que passou desde o último tiro)
                    // >= shootCooldown            (se esse tempo for maior ou igual ao tempo de cooldown, então cria o novo tiro)
                    if ((currentCharacter == HAMSTER || currentCharacter == FOX) && currentTime - lastShootTime >= shootCooldown && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    {
                        for (int i = 0; i < NUM_MAX_SHOOTS; i++) 
                        {
                            // Se o tiro não estiver ativo, então cria ele
                            if (!shoots[i].active)
                            {
                                // Pegando a posição do mouse para os tiros sairem na direção do mouse
                                Vector2 mousePos = GetMousePosition();
                                Vector2 worldMouse = GetScreenToWorld2D(mousePos, camera);

                                // Definindo os tiros
                                shoots[i].position = player.position;
                                shoots[i].direction = Vector2Normalize(Vector2Subtract(worldMouse, player.position));
                                shoots[i].active = true;
                                shoots[i].vel = 10.0f;

                                if (currentCharacter == FOX) PlaySound(sdMachineGun);
                                if (currentCharacter == HAMSTER) PlaySound(sdPistol);

                                // Reiniciando o cooldown
                                lastShootTime = currentTime;

                                break; // para criar um tiro por vez
                            }
                        }
                    }

                    // Se o personagem escolhido for FERRET e passou o tempo de cooldown e esta apertando o botao esquerdo do mouse
                    if (currentCharacter == FERRET && currentTime - lastShootTime >= shootCooldown && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (!shoots[j].active)
                            {
                                Vector2 offset;
                                if (j == 0) offset = (Vector2){20, 0};
                                if (j == 1) offset = (Vector2){80, 0};
                                if (j == 2) offset = (Vector2){150, 0};

                                shoots[j].position = Vector2Add(player.position, offset);

                                Vector2 mousePos = GetMousePosition();
                                Vector2 worldMouse = GetScreenToWorld2D(mousePos, camera);
                                shoots[j].direction = Vector2Normalize(Vector2Subtract(worldMouse, player.position));
                                shoots[j].active = true;
                                shoots[j].vel = 10.0f;
                            }
                        }

                        PlaySound(sdShotgun);
                        lastShootTime = currentTime;
                    }

                    // Atualizando os tiros
                    for (int i = 0; i < NUM_MAX_SHOOTS; i++)
                    {
                        // Se o tiro estiver ativo
                        if (shoots[i].active)
                        {
                            // Movimentando o tiro (os tiros vao começar a partir da posição dele mais a direção vezes a velocidade)
                            shoots[i].position.x += shoots[i].direction.x * shoots[i].vel;
                            shoots[i].position.y += shoots[i].direction.y * shoots[i].vel;

                            // Limitando até onde os tiros vão
                            if (shoots[i].position.x < MIN_AREA || shoots[i].position.x > MAX_AREA || shoots[i].position.y < MIN_AREA || shoots[i].position.y > MAX_AREA)
                                shoots[i].active = false;

                            // Se o boss estiver ativo
                            if (boss.active)
                            {
                                // Define as Hitboxes dos tiros e do boss
                                Rectangle shootRect = { shoots[i].position.x, shoots[i].position.y, 20, 20 };
                                Rectangle bossRect = { boss.position.x, boss.position.y, 200, 200 };

                                // Se colidir o tiro no boss
                                if (CheckCollisionRecs(bossRect, shootRect))
                                {
                                    // Então o boss tomou dano
                                    boss.tookDamage = true;
                                    damageAnimTime = 0.0f;

                                    // Então o tiro some e o inimigo perde vida
                                    shoots[i].active = false;

                                    // Boss perde vida
                                    boss.lives -= player.damage;

                                    // Ativando a variável para o tremor da camera
                                    isShaking = true;
                                    shakeTime = 0.2f; // tempo de tremor

                                    // Se a vida do inimigo acabar
                                    if (boss.lives <= 0)
                                    {
                                        // Então ele fica inativo e incrementa o enemiesKill
                                        boss.active = false;
                                        isBossDead = true;
                                        enemiesKill++;
                                    }
                                }
                            }

                            // Verificando se o tiro colidiu com o inimigo 1
                            for (int j = 0; j < NUM_MAX_ENEMIES; j++)
                            {
                                // Se o inimigo estiver ativo
                                if (enemy[j].active)
                                {
                                    // Define as Hitboxes dos tiros e dos inimigos
                                    Rectangle shootRect = { shoots[i].position.x, shoots[i].position.y, 20, 20 };
                                    Rectangle enemyRect = { enemy[j].position.x, enemy[j].position.y, ENEMY_SIZE, ENEMY_SIZE };

                                    // Se colidir o tiro no inimigo
                                    if (CheckCollisionRecs(enemyRect, shootRect))
                                    {
                                        // Então o inimigo tomou dano
                                        enemy[j].tookDamage = true;
                                        damageAnimTime = 0.0f;

                                        // Então o tiro some e o inimigo perde vida
                                        shoots[i].active = false;

                                        // Inimigo perde vida
                                        enemy[j].lives -= player.damage;

                                        // Ativando a variável para o tremor da camera
                                        isShaking = true;
                                        shakeTime = 0.2f; // tempo de tremor
                                        
                                        // Se a vida do inimigo acabar
                                        if (enemy[j].lives <= 0)
                                        {
                                            // Então ele fica inativo e incrementa o enemiesKill
                                            enemy[j].active = false;
                                            enemiesKill++;

                                            for (int k = 0; k < NUM_MAX_CRYSTALS; k++)
                                            {
                                                // E dropa cristais na posição dele
                                                if (!crystals[k].active)
                                                {
                                                    crystals[k].active = true;                // ativa o cristal
                                                    crystals[k].enemy = enemy[j].type;        // para mudar a quantidade de cristal que dropa por inimigo
                                                    crystals[k].position = enemy[j].position; // para dropar na posição do inimigo
                                                    break; // para criar só um cristal por vez
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } 
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Destruindo Tiros e Cristais quando a Wave acaba
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    if (waveEnded)
                    {
                        // Apagango os tiros
                        for (int i = 0; i < NUM_MAX_SHOOTS; i++)
                            if (shoots[i].active)
                                shoots[i].active = false;

                        // Apagando os cristais
                        for (int k = 0; k < NUM_MAX_CRYSTALS; k++)
                            if (crystals[k].active)
                                crystals[k].active = false;
                    } 

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Colisão Player e Cristal
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    for (int k = 0; k < NUM_MAX_CRYSTALS; k++)
                    {
                        // Se o cristal estiver ativo
                        if (crystals[k].active)
                        {
                            // Define a Hitbox do cristal
                            Rectangle crystalRect = { crystals[k].position.x, crystals[k].position.y, 60, 60 };

                            // Se o player colidir entao some o cristal e incrementa os cristais do player
                            if (CheckCollisionRecs(crystalRect, playerRect))
                            {
                                // Se foi o inimigo 1 que morreu
                                if (crystals[k].enemy == 1)
                                {
                                    crystals[k].active = false;
                                    player.crystals += 10;
                                }

                                // Se foi o inimigo 2 que morreu
                                if (crystals[k].enemy == 2)
                                {
                                    crystals[k].active = false;
                                    player.crystals += 15;
                                }

                                // Se foi o inimigo 3 que morreu
                                if (crystals[k].enemy == 3)
                                {
                                    crystals[k].active = false;
                                    player.crystals += 20;
                                }

                                // Se foi o inimigo 4 que morreu
                                if (crystals[k].enemy == 4)
                                {
                                    crystals[k].active = false;
                                    player.crystals += 25;
                                }
                            }
                        }
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Colisão Inimigo e Player
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Atualizar o playerRect toda vez que o player se mexer antes de verificar as colisões
                    playerRect.x = player.position.x - tex.hamsterWalk.width / 2;
                    playerRect.y = player.position.y - tex.hamsterWalk.height / 2;  

                    // Variáveis para o cooldown de dano sofrido pelo player
                    static float damageCooldown = 0.5f; // tempo do cooldown
                    int collisions = 0;                 // contar quantos inimigos estão colidindo

                    // Conta quantos inimigos estão colidindo com o player
                    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                    {
                        Rectangle enemyRect = { enemy[i].position.x, enemy[i].position.y, ENEMY_SIZE, ENEMY_SIZE };

                        // Se for o inimigo ativo, então verifica se ele colidiu com o player, se colidiu incrementa collisions
                        if (enemy[i].active)
                            if (CheckCollisionRecs(playerRect, enemyRect))
                                collisions++;
                    }

                    // Diminui o tempo do cooldown proporcional a quantidade de inimigos
                    float currentDamageCooldown = damageCooldown - 0.1f * (collisions - 1); // ex: 0.5 - 0.1 * (1 - 1) -> cooldown continua em 0.5 | ex: 0.5 - 0.1 * (2 - 1) -> cooldown = 0.4
                    if (currentDamageCooldown < 0.1f) currentDamageCooldown = 0.1f;         // pra manter em 0.1 e não negativar o cooldown

                    // Se colidiu e passou o tempo do cooldown
                    if (collisions > 0  && currentTime - lastDamageTime >= currentDamageCooldown)
                    {
                        // Fazendo a colisão do player com o inimigo
                        for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                        {
                            // Se for o inimigo ativo
                            if (enemy[i].active)
                            {
                                // Então define a Hitbox do inimigo
                                Rectangle enemyRect = { enemy[i].position.x, enemy[i].position.y, ENEMY_SIZE, ENEMY_SIZE };

                                // Verifica se o player colidiu com o inimigo
                                if (CheckCollisionRecs(playerRect, enemyRect))
                                {
                                    // Se colidir o player perde vida
                                    // Troca para a sprite de dano
                                    playerTookDamage = true;
                                    damageAnimTime = 0.0f;

                                    // Treme a tela
                                    isShaking = true; // ativa a variável para o tremor da camera
                                    shakeTime = 0.2f; // tempo de tremor
                                    
                                    // Tira vida do player conforme o dano de cada inimigo
                                    if (enemy[i].type == 1) player.lives--;
                                    if (enemy[i].type == 2) player.lives -= 2;
                                    if (enemy[i].type == 3) player.lives -= 3;
                                    if (enemy[i].type == 4) player.lives -= 3;

                                    // Se o player morreu
                                    if (player.lives <= 0)
                                    {
                                        // Atualiza o total antes de trocar de tela
                                        totalEnemiesKilled += enemiesKill;
                                        currentScreen = DEFEAT;
                                    }

                                    break; // para só ocorrer um por frame
                                }
                            }
                        }

                        // Reinicia o cooldown
                        lastDamageTime = currentTime;
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Colisão Boss e Player
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Se passou o tempo do cooldown
                    if (currentTime - lastDamageTime >= currentDamageCooldown)
                    {
                        // Define a hitbox do boss
                        Rectangle bossRect = { boss.position.x, boss.position.y, 200, 200 };

                        // Verifica se o player colidiu com o boss e passou o tempo para levar dano novamente
                        if (CheckCollisionRecs(playerRect, bossRect))
                        {
                            // Entaõ player tomou dano
                            playerTookDamage = true;
                            damageAnimTime = 0.0f;

                            // Se colidir o player perde vida
                            isShaking = true; // ativa a variável para o tremor da camera
                            shakeTime = 0.2f; // tempo de tremor

                            // Diminui 5 de vida do player
                            player.lives -= 5;

                            // Se o player morreu
                            if (player.lives <= 0)
                            {
                                // Atualiza o total antes de trocar de tela
                                totalEnemiesKilled += enemiesKill;
                                currentScreen = DEFEAT;
                            }

                            //break;
                        }

                        // Reinicia o cooldown
                        lastDamageTime = currentTime;
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Tremor na Tela
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Variável para pegar o tempo que passou desde o último frame (pro tremor durar sempre o mesmo tempo, mesmo que o FPS mude)
                    float timeBetweenFrames = GetFrameTime(); 

                    // Se o tiro colidiu com o inimigo, então treme a tela
                    if (isShaking)
                    {
                        // Atualiza o offset da camera
                        camera.offset.x = originalOffset.x + GetRandomValue(-1,1);
                        camera.offset.y = originalOffset.y + GetRandomValue(-1,1);

                        // Reduz o tempo restante do tremor
                        shakeTime -= timeBetweenFrames;

                        // Se o tempo de tremor passou
                        if (shakeTime <= 0)
                        {
                            // Então volta ao normal
                            isShaking = false;
                            camera.offset = originalOffset;
                        }
                    }

                    // ------------------------------------------------------------------------------------------------------------------------------------
                    // Desenhando a Partida
                    // ------------------------------------------------------------------------------------------------------------------------------------
                    BeginDrawing();

                        // Limpa o fundo com a cor de fundo
                        ClearBackground(RAYWHITE);

                        // Usar a camera para desenhar o player e o cenario
                        BeginMode2D(camera);

                            // ------------------------------------------------------------------------------------------------------------------------------------
                            // Desenhando o chão do jogo
                            // ------------------------------------------------------------------------------------------------------------------------------------
                            Color bgColor = { 75, 61, 68, 255 };
                            Color bgColor2 = { 87, 72, 82, 255 };
                            
                            DrawRectangle(MIN_AREA - 1000, MIN_AREA - 1000, (MAX_AREA - MIN_AREA) + 2000, (MAX_AREA - MIN_AREA) + 2000, bgColor);
                            DrawRectangle(MIN_AREA, MIN_AREA, MAX_AREA - MIN_AREA, MAX_AREA - MIN_AREA, bgColor2);
                            

                            // ------------------------------------------------------------------------------------------------------------------------------------
                            // Desenhando a sprite do player
                            // ------------------------------------------------------------------------------------------------------------------------------------
                            Rectangle destRec = { player.position.x, player.position.y, frameRecPlayer.width * 4.0f, frameRecPlayer.height * 4.0f };

                            // Desenhando a sprite de dano do player, se ele ter levado dano 
                            if (playerTookDamage)
                                DrawTexturePro(tex.hamsterWalk, frameRecPlayer, destRec, (Vector2){ destRec.width/2, destRec.height/2 }, 0.0f, RED);
                            else
                                DrawTexturePro(tex.hamsterWalk, frameRecPlayer, destRec, (Vector2){ destRec.width/2, destRec.height/2 }, 0.0f, WHITE);


                            // ------------------------------------------------------------------------------------------------------------------------------------
                            // Desenhando as armas
                            // ------------------------------------------------------------------------------------------------------------------------------------
                            if (currentCharacter == FOX) DrawTextureEx(tex.machine_gun, { player.position.x - 20, player.position.y }, 0.0f, 4.0f, WHITE);
                            if (currentCharacter == HAMSTER) DrawTextureEx(tex.pistol, { player.position.x - 50, player.position.y - 20 }, 0.0f, 0.3f, WHITE);
                            if (currentCharacter == FERRET) DrawTextureEx(tex.shotgun, { player.position.x - 60, player.position.y - 100 }, 0.0f, 0.5f, WHITE);


                            // ------------------------------------------------------------------------------------------------------------------------------------
                            // Desenhando os cristais ativos
                            // ------------------------------------------------------------------------------------------------------------------------------------
                            for (int i = 0; i < NUM_MAX_CRYSTALS; i++)
                            {
                                Rectangle destRecCristal = { crystals[i].position.x, crystals[i].position.y, frameRecCristal.width * 2.0f, frameRecCristal.height * 2.0f };

                                if (crystals[i].active)
                                    DrawTexturePro(tex.cristal, frameRecCristal, destRecCristal, (Vector2){ destRecCristal.width/2, destRecCristal.height/2 }, 0.0f, WHITE); 
                            }    
                            
                            // Desenhando os inimigos ativos
                            for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                            {
                                Rectangle destRecEnemy = { enemy[i].position.x, enemy[i].position.y, frameRecEnemy.width * 4.0f, frameRecEnemy.height * 4.0f };

                                if (enemy[i].active)
                                {
                                    if (enemy[i].tookDamage)
                                    { 
                                        if (enemy[i].type == 1) DrawTexturePro(tex.enemy1, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, RED); 
                                        if (enemy[i].type == 2) DrawTexturePro(tex.enemy2, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, RED); 
                                        if (enemy[i].type == 3) DrawTexturePro(tex.enemy3, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, RED); 
                                        if (enemy[i].type == 4) DrawTexturePro(tex.enemy4, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, RED);  
                                    }
                                    else
                                    {
                                        if (enemy[i].type == 1) DrawTexturePro(tex.enemy1, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, WHITE); 
                                        if (enemy[i].type == 2) DrawTexturePro(tex.enemy2, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, WHITE); 
                                        if (enemy[i].type == 3) DrawTexturePro(tex.enemy3, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, WHITE); 
                                        if (enemy[i].type == 4) DrawTexturePro(tex.enemy4, frameRecEnemy, destRecEnemy, (Vector2){ destRecEnemy.width/2, destRecEnemy.height/2 }, 0.0f, WHITE);  
                                    }
                                    
                                    DrawText(TextFormat("%d", enemy[i].lives), enemy[i].position.x - 8, enemy[i].position.y + 5, 50, WHITE);
                                }       
                            }

                            Rectangle destRecBoss = { boss.position.x, boss.position.y, frameRecEnemy.width * 6.0f, frameRecEnemy.height * 6.0f };

                            // Desenhando o boss
                            if (boss.active)
                            {
                                if (boss.tookDamage) DrawTexturePro(tex.boss, frameRecEnemy, destRecBoss, (Vector2){ destRecBoss.width/2, destRecBoss.height/2 }, 0.0f, RED); 
                                else DrawTexturePro(tex.boss, frameRecEnemy, destRecBoss, (Vector2){ destRecBoss.width/2, destRecBoss.height/2 }, 0.0f, WHITE);  
                                
                                DrawText(TextFormat("%d", boss.lives), boss.position.x, boss.position.y, 50, WHITE);
                            }

                            // Desenhando os tiros ativos
                            for (int i = 0; i < NUM_MAX_SHOOTS; i++)
                                if (shoots[i].active)
                                    DrawTextureEx(tex.bullet, shoots[i].position, 0.0f, 2.0f, WHITE);   

                        EndMode2D();

                        // Desenhando o HUD
                        DrawTextEx(font, TextFormat("%.1f", waveTime), { screenWidth / 2 - 10, 20 }, 30, 2, WHITE);
                        
                        // Desenhando retângulo atrás do ícones
                        Color hudColor = { 132, 120, 117, 255 };
                        DrawRectangle(10, 30, 100, 180, hudColor);

                        DrawTextureEx(tex.cristal_hud, { 22, 40 }, 0.0f, 1.0f, WHITE);
                        DrawTextEx(font, TextFormat("%d", player.crystals), { 60, 45 }, 20, 2, WHITE);

                        DrawTextureEx(tex.hearth, { 20, 75 }, 0.0f, 1.0f, WHITE);
                        DrawTextEx(font,TextFormat("%d", player.lives), { 60, 77 }, 20, 2, WHITE);

                        DrawTextureEx(tex.damage, { 20, 100 }, 0.0f, 1.0f, WHITE);
                        DrawTextEx(font, TextFormat("%.1f", player.damage), { 60, 102 }, 20, 2, WHITE);

                        DrawTextureEx(tex.speed, { 20, 125 }, 0.0f, 1.0f, WHITE);
                        DrawTextEx(font, TextFormat("%.1f", player.vel), { 60, 127 }, 20, 2, WHITE);

                        DrawTextureEx(tex.hearth_plus, { 20, 150 }, 0.0f, 1.0f, WHITE);
                        DrawTextEx(font, TextFormat("%.1f", recoveryCooldown), { 60, 152 }, 20, 2, WHITE);

                        if (item[4].active)
                            DrawTextureEx(tex.dash, { 20, 175 }, 0.0f, 1.0f, WHITE);

                        DrawTextEx(font, "[1]", { 20, 220 }, 20, 2, WHITE);
                        DrawTextureEx(tex.machine_gun, { 50, 220 }, 0.0f, 1.5f, WHITE);

                        DrawTextEx(font, "[2]", { 20, 260 }, 20, 2, WHITE);
                        DrawTextureEx(tex.pistol, { 50, 245 }, 0.0f, 0.1f, WHITE);

                        DrawTextEx(font, "[3]", { 20, 300 }, 20, 2, WHITE);
                        DrawTextureEx(tex.shotgun, { 50, 270 }, 0.0f, 0.15f, WHITE);
                        
                        DrawTextEx(font, TextFormat("Current Wave: %d", currentWave), { 20, 420 }, 20, 2, WHITE);

                        //DrawText(TextFormat("EnemiesKills: %d", enemiesKill), 10, 390, 20, WHITE);
                        //DrawText(TextFormat("Total de inimigos mortos: %d", totalEnemiesKilled), 10, 420, 20, WHITE);

                        // Desenhando o cursor depois de tudo
                        DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE);

                    EndDrawing();
                }
                // Se o jogo estiver pausado, então roda esse código
                else
                {
                    // Pegando a posição do mouse
                    mousePosition = GetMousePosition();

                    // Hitbox do botão de desistir
                    Rectangle btnGiveUp = { 250, 230, 300, 50 };
                    Rectangle btnBack = { 250, 290, 300, 50 };

                    // Se apertar no botao de desistir, então vai para a cena de Game Settings
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnGiveUp)) currentScreen = DEFEAT;

                    // Se apertar no botao de voltar, então despausa o jogo
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnBack)) gamePaused = false;

                    BeginDrawing();

                        // Limpa o fundo com a cor de fundo
                        ClearBackground(RAYWHITE);

                        // Desenhando a caixa do menu de pause
                        DrawRectangle(150, 100, 500, 300, Fade(BLACK, 0.2f));
                        DrawTextEx(font, "Jogo Pausado", { screenWidth / 2 - 170, 150 }, 50, 2, DARKGRAY);

                        // Desenhando botão de desistir
                        DrawRectangleRec(btnGiveUp, WHITE);
                        DrawTextEx(font, "Give up", { 340, 238 }, 30, 2, DARKGRAY);

                        // Desenhando botão de voltar
                        DrawRectangleRec(btnBack, WHITE);
                        DrawTextEx(font, "Back", { 360, 298 }, 30, 2, DARKGRAY);

                        // Desenhando o cursor depois de tudo
                        DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE); 

                    EndDrawing();
                }

                break;
            }

            // ------------------------------------------------------------------------------------------------------------------------------------
            // Tela da Loja
            // ------------------------------------------------------------------------------------------------------------------------------------
            case STORE:
            {
                mousePosition = GetMousePosition();

                // Definindo o tamanho dos botões com pos x, pos y, width, height
                Rectangle btnPlay           = { 250, 380, 300, 50 };
                Rectangle btnExtraLife      = { 367, 50, 150, 200 };
                Rectangle btnDamage         = { 523, 50, 150, 200 };
                Rectangle btnSpeed          = { 290, 180, 150, 170 };
                Rectangle btnHealthRecovery = { 445, 180, 150, 170 };
                Rectangle btnDash           = { 600, 180, 150, 170 };

                // Comprável: Vida Extra
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnExtraLife)) 
                {
                    // Se o player tiver dinheiro
                    if (player.crystals >= item[0].currentPrice)
                    {
                        player.crystals -= item[0].currentPrice;   // desconta os cristais
                        player.lives += item[0].effect;            // realiza a melhoria
                        item[0].currentPrice += item[0].basePrice; // aumenta o preço
                        currentLife = player.lives;                // para as waves começarem com a vida recarregada
                    }
                    //else
                    //{
                        // pra fazer um draw de dinhero insuficiente
                        //noCash = true;
                    //}
                }

                // Comprável: Dano
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnDamage)) 
                {
                    // Se o player tiver dinheiro
                    if (player.crystals >= item[1].currentPrice)
                    {
                        player.crystals -= item[1].currentPrice;   // desconta os cristais
                        player.damage += item[1].effect;           // realiza a melhoria
                        item[1].currentPrice += item[1].basePrice; // aumenta o preço
                    }
                    //else
                    //{
                        // pra fazer um draw de dinhero insuficiente
                        //noCash = true;
                    //}
                }

                // Comprável: Velocidade
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnSpeed)) 
                {
                    // Se o player tiver dinheiro
                    if (player.crystals >= item[2].currentPrice)
                    {
                        player.crystals -= item[2].currentPrice;   // desconta os cristais
                        player.vel += item[2].effect;              // realiza a melhoria
                        item[2].currentPrice += item[2].basePrice; // aumenta o preço
                    }
                    //else
                    //{
                        // pra fazer um draw de dinhero insuficiente
                        //noCash = true;
                    //}
                }

                // Comprável: Recuperar Vida
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnHealthRecovery)) 
                {
                    // Se o player tiver dinheiro
                    if (player.crystals >= item[3].currentPrice)
                    {
                        player.crystals -= item[3].currentPrice;   // desconta os cristais
                        recoveryCooldown += item[3].effect;        // realiza a melhoria
                        item[3].currentPrice += item[3].basePrice; // aumenta o preço
                    }
                    //else
                    //{
                        // pra fazer um draw de dinhero insuficiente
                        //noCash = true;
                    //}
                }

                // Comprável: Recuperar Vida
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnDash)) 
                {
                    // Se o player tiver dinheiro
                    if (player.crystals >= item[4].currentPrice)
                    {
                        player.crystals -= item[4].currentPrice; // desconta os cristais
                        item[4].active = true;                   // ativa o dash
                    }
                    //else
                    //{
                        // pra fazer um draw de dinhero insuficiente
                        //noCash = true;
                    //}
                }

                // Apertou, então vai para a partida
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnPlay)) currentScreen = GAMEPLAY;

                // Desenhando a Tela de Opções
                BeginDrawing();

                    // Limpa o fundo com a cor de fundo
                    ClearBackground(RAYWHITE);

                    //DrawText("Atributos Player",);
                    // colocar os atributos do player aqui

                    // Desenhando o total de cristais do player
                    DrawTextureEx(tex.cristal_hud, { 10, 10 }, 0.0f, 1, WHITE);
                    DrawText(TextFormat("%d", player.crystals), 35, 15, 20, DARKGRAY);

                    // Desenhando o Player
                    DrawTextureEx(tex.hamster_hud, { 50, 50 }, 0.0f, 6, WHITE);

                    // Desenhando o botão de Play
                    if (CheckCollisionPointRec(mousePosition, btnPlay)) 
                    {
                        DrawRectangleRec(btnPlay, WHITE);
                        DrawTextEx(font, "Play", {btnPlay.x + 110, btnPlay.y + 5}, 40, -2, DARKGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(btnPlay, DARKGRAY);
                        DrawTextEx(font, "Play", {btnPlay.x + 110, btnPlay.y + 5}, 40, -2, WHITE);
                    }

                    // Desenhando botão de comprar vida extra
                    if (CheckCollisionPointRec(mousePosition, btnExtraLife))
                    {
                        DrawRectangleRec(btnExtraLife, ORANGE);
                        DrawTextEx(font, "Extra Life", { btnExtraLife.x + 25, btnExtraLife.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[0].effect), { btnExtraLife.x + 25, btnExtraLife.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[0].currentPrice), { btnExtraLife.x + 25, btnExtraLife.y + 80 }, 20, 2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnExtraLife, DARKGRAY);
                        DrawTextEx(font, "Extra Life", { btnExtraLife.x + 25, btnExtraLife.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[0].effect), { btnExtraLife.x + 25, btnExtraLife.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[0].currentPrice), { btnExtraLife.x + 25, btnExtraLife.y + 80 }, 20, 2, WHITE);
                    }
                    
                    // Desenhando botão de comprar dano extra
                    if (CheckCollisionPointRec(mousePosition, btnDamage))
                    {
                        DrawRectangleRec(btnDamage, ORANGE);
                        DrawTextEx(font, "Damage", { btnDamage.x + 40, btnDamage.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[1].effect), { btnDamage.x + 25, btnDamage.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[1].currentPrice), { btnDamage.x + 25, btnDamage.y + 80 }, 20, 2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnDamage, DARKGRAY);
                        DrawTextEx(font, "Damage", { btnDamage.x + 40, btnDamage.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[1].effect), { btnDamage.x + 25, btnDamage.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[1].currentPrice), { btnDamage.x + 25, btnDamage.y + 80 }, 20, 2, WHITE);
                    }

                    // Desenhando botão de comprar velocidade extra
                    if (CheckCollisionPointRec(mousePosition, btnSpeed))
                    {
                        DrawRectangleRec(btnSpeed, ORANGE);
                        DrawTextEx(font, "Speed", { btnSpeed.x + 45, btnSpeed.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[2].effect), { btnSpeed.x + 25, btnSpeed.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[2].currentPrice), { btnSpeed.x + 25, btnSpeed.y + 80 }, 20, 2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnSpeed, GRAY);
                        DrawTextEx(font, "Speed", { btnSpeed.x + 45, btnSpeed.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[2].effect), { btnSpeed.x + 25, btnSpeed.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[2].currentPrice), { btnSpeed.x + 25, btnSpeed.y + 80 }, 20, 2, WHITE);
                    }
                    
                    // Desenhando botão de comprar regeneração de vida
                    if (CheckCollisionPointRec(mousePosition, btnHealthRecovery))
                    {
                        DrawRectangleRec(btnHealthRecovery, ORANGE);
                        DrawTextEx(font, "Life Regen", { btnHealthRecovery.x + 18, btnHealthRecovery.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[3].effect), { btnHealthRecovery.x + 25, btnHealthRecovery.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[3].currentPrice), { btnHealthRecovery.x + 25, btnHealthRecovery.y + 80 }, 20, 2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnHealthRecovery, GRAY);
                        DrawTextEx(font, "Life Regen", { btnHealthRecovery.x + 18, btnHealthRecovery.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Efeito: %.1f", item[3].effect), { btnHealthRecovery.x + 25, btnHealthRecovery.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[3].currentPrice), { btnHealthRecovery.x + 25, btnHealthRecovery.y + 80 }, 20, 2, WHITE);
                    }

                    // Desenhando botão de comprar dash
                    if (CheckCollisionPointRec(mousePosition, btnDash))
                    {
                        DrawRectangleRec(btnDash, ORANGE);
                        DrawTextEx(font, "Dash", { btnDash.x + 50, btnDash.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, "Habilidade", { btnDash.x + 18, btnDash.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[4].currentPrice), { btnDash.x + 25, btnDash.y + 80 }, 20, 2, WHITE);
                    }
                    else
                    {
                        DrawRectangleRec(btnDash, GRAY);
                        DrawTextEx(font, "Dash", { btnDash.x + 50, btnDash.y + 10 }, 20, 2, WHITE);
                        DrawTextEx(font, "Habilidade", { btnDash.x + 18, btnDash.y + 50 }, 20, 2, WHITE);
                        DrawTextEx(font, TextFormat("Preco: %d", item[4].currentPrice), { btnDash.x + 25, btnDash.y + 80 }, 20, 2, WHITE);
                    }
                    
                    // Desenhando o cursor depois de tudo
                    DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE);

                EndDrawing();

                break;
            }

            // ------------------------------------------------------------------------------------------------------------------------------------
            // Tela de Vitória
            // ------------------------------------------------------------------------------------------------------------------------------------
            case VICTORY:
            {
                mousePosition = GetMousePosition();

                // Definindo o tamanho dos botões com - { pos x, pos y, width, height }
                Rectangle btnNewGame = { 230, 380, 300, 50 };

                // Apertou, então vai para o menu de opções
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnNewGame)) 
                {
                    currentScreen = GAME_SETTINGS;

                    // Resetando todas as variáveis para começar um novo jogo
                    // Resetando o player
                    currentCharacter = HAMSTER;
                    player.position = { (float)screenWidth/2.0f, (float)screenHeight/2.0f };
                    player.direction = { 0, 1 }; // começa olhando para baixo
                    player.damage = 1.0f;
                    player.vel = 10.0f;
                    player.lives = 15;
                    player.crystals = 0;

                    // Resetando a wave
                    currentWave = FIRST;
                    enemiesKill = 0;
                    lastSpawn1 = 0.0f;
                    lastSpawn2 = 0.0f;
                    spawnedEnemies = 0;
                    waveTime = 30.0f;

                    // Resetando cooldowns
                    recoveryCooldown = 0.0f;
                    lastRecovery = 0.0f;
                    lastShootTime = 0.0f;
                    lastDamageTime = 0.0f;

                    // Resetando as estatísticas
                    totalEnemiesKilled = 0;

                    // Resetando a loja
                    for (int i = 0; i < 5; i++)
                    {
                        item[i].currentPrice = item[i].basePrice;
                        if (i == 4) item[i].active = false;
                    }

                    // Desativando os inimigos
                    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                        enemy[i].active = false;

                    // Apagango os tiros
                    for (int i = 0; i < NUM_MAX_SHOOTS; i++)
                        if (shoots[i].active)
                            shoots[i].active = false;

                    // Desativando os cristais
                    for (int k = 0; k < NUM_MAX_CRYSTALS; k++)
                        if (crystals[k].active)
                            crystals[k].active = false;
                }

                BeginDrawing();

                    // Limpa o fundo com a cor de fundo
                    ClearBackground(RAYWHITE);
                   
                    DrawTextEx(font, "Victory", { screenWidth / 2 - 200, 100 }, 100, 2, DARKGRAY);
                    DrawTextEx(font, TextFormat("Enemies Defeated: %d", totalEnemiesKilled), { screenWidth / 2 - 130, 200 }, 20, 2, DARKGRAY);

                    // Desenhando o botão de Novo jogo
                    if (CheckCollisionPointRec(mousePosition, btnNewGame))
                    {
                        DrawRectangleRec(btnNewGame, WHITE);
                        DrawTextEx(font, "New Game", { btnNewGame.x + 70, btnNewGame.y + 10 }, 30, 2, DARKGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(btnNewGame, DARKGRAY);
                        DrawTextEx(font, "New Game", { btnNewGame.x + 70, btnNewGame.y + 10 }, 30, 2, WHITE);
                    }

                    // Desenhando o cursor depois de tudo
                    DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE);
                    
                EndDrawing();

                break;
            }

            // ------------------------------------------------------------------------------------------------------------------------------------
            // Tela de Derrota
            // ------------------------------------------------------------------------------------------------------------------------------------
            case DEFEAT:
            {
                // Pegando a posição do mouse
                mousePosition = GetMousePosition();

                // Despausando o jogo
                gamePaused = false;

                // Definindo o tamanho do botão com - { pos x, pos y, width, height }
                Rectangle btnNewGame = { 230, 380, 300, 50 };

                // Apertou, então vai para o menu de opções
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, btnNewGame)) 
                {
                    currentScreen = GAME_SETTINGS;

                    // Resetando todas as variáveis para começar um novo jogo
                    // Resetando o player
                    currentCharacter = HAMSTER;
                    player.position = { (float)screenWidth/2.0f, (float)screenHeight/2.0f };
                    player.direction = { 0, 1 }; // começa olhando para baixo
                    player.damage = 1.0f;
                    player.vel = 10.0f;
                    player.lives = 15;
                    player.crystals = 0;

                    // Resetando a wave
                    currentWave = FIRST;
                    enemiesKill = 0;
                    lastSpawn1 = 0.0f;
                    lastSpawn2 = 0.0f;
                    spawnedEnemies = 0;
                    waveTime = 30.0f;

                    // Resetando cooldowns
                    recoveryCooldown = 0.0f;
                    lastRecovery = 0.0f;
                    lastShootTime = 0.0f;
                    lastDamageTime = 0.0f;

                    // Resetando as estatísticas
                    totalEnemiesKilled = 0;

                    // Resetando a loja
                    for (int i = 0; i < 5; i++)
                    {
                        item[i].currentPrice = item[i].basePrice;
                        if (i == 4) item[i].active = false;
                    }

                    // Desativando os inimigos
                    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
                        enemy[i].active = false;

                    // Apagango os tiros
                    for (int i = 0; i < NUM_MAX_SHOOTS; i++)
                        if (shoots[i].active)
                            shoots[i].active = false;

                    // Desativando os cristais
                    for (int k = 0; k < NUM_MAX_CRYSTALS; k++)
                        if (crystals[k].active)
                            crystals[k].active = false;
                }

                // Desenhando a Tela de Opções
                BeginDrawing();

                    // Limpa o fundo com a cor de fundo
                    ClearBackground(RAYWHITE);

                    // Desenhando os textos finais
                    DrawTextEx(font, "Defeat", { screenWidth / 2 - 170, 100 }, 100, 2, DARKGRAY);
                    DrawTextEx(font, TextFormat("Enemies Defeated: %d", totalEnemiesKilled), { screenWidth / 2 - 130, 200} , 20, 2,DARKGRAY);

                    // Desenhando o botão de Novo jogo
                    if (CheckCollisionPointRec(mousePosition, btnNewGame))
                    {
                        DrawRectangleRec(btnNewGame, WHITE);
                        DrawTextEx(font, "New Game", { btnNewGame.x + 70, btnNewGame.y + 10 }, 30, 2, DARKGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(btnNewGame, DARKGRAY);
                        DrawTextEx(font, "New Game", { btnNewGame.x + 70, btnNewGame.y + 10 }, 30, 2, WHITE);
                    }

                    // Desenhando o cursor depois de tudo
                    DrawTexture(tex.bullet, mousePos.x, mousePos.y, WHITE);

                EndDrawing();

                break;
            }

            default:
                break;
        }
    }
    // ---------------------------------------------------------------------------------------------------------------------------------------


    // ---------------------------------------------------------------------------------------------------------------------------------------
    // Finalização
    // ---------------------------------------------------------------------------------------------------------------------------------------
    // Descarrega os recursos usados (garante que nenhum código gráfico rode depois de CloseWindow())
    unloadTextures(tex);
    UnloadMusicStream(music);
    UnloadSound(sdMachineGun);
    UnloadSound(sdPistol);
    UnloadSound(sdShotgun);

    // Fecha o sistema de áudio
    CloseAudioDevice();

    // Fecha a janela do jogo
    CloseWindow();
    // ---------------------------------------------------------------------------------------------------------------------------------------

    return 0;
}
