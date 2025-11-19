#include "raylib.h"
#include "loadTextures.hpp"

Textures loadTextures()
{
    Textures tex;
    
    // Player
    tex.hamsterWalk = LoadTexture("assets/sprites/entities/player/hamster.png");
    tex.hamster_hud = LoadTexture("assets/sprites/entities/player/hamster_hud.png");

    // Inimigos
    tex.enemy1 = LoadTexture("assets/sprites/entities/enemy/enemy1.png");
    tex.enemy2 = LoadTexture("assets/sprites/entities/enemy/enemy2.png");
    tex.enemy3 = LoadTexture("assets/sprites/entities/enemy/enemy3.png");
    tex.enemy4 = LoadTexture("assets/sprites/entities/enemy/enemy4.png");
    tex.boss   = LoadTexture("assets/sprites/entities/enemy/boss.png");

    // Objetos
    tex.bullet      = LoadTexture("assets/sprites/objects/bullet.png");
    tex.cristal     = LoadTexture("assets/sprites/objects/cristal.png");
    tex.cristal_hud = LoadTexture("assets/sprites/objects/cristal_hud.png");
    tex.hearth      = LoadTexture("assets/sprites/objects/hearth.png");
    tex.hearth_plus = LoadTexture("assets/sprites/objects/hearth_plus.png");
    tex.speed       = LoadTexture("assets/sprites/objects/speed.png");
    tex.damage      = LoadTexture("assets/sprites/objects/damage.png");
    tex.dash        = LoadTexture("assets/sprites/objects/dash.png");
    tex.machine_gun = LoadTexture("assets/sprites/objects/machine_gun.png");
    tex.pistol      = LoadTexture("assets/sprites/objects/pistol.png");
    tex.shotgun     = LoadTexture("assets/sprites/objects/shotgun.png");

    // Fundos
    tex.bgMainMenu = LoadTexture("assets/sprites/backgrounds/main_menu.png");
    //tex.bgGameSettings = LoadTexture("assets/sprites/backgrounds/game_settings.png");
    //tex.bgStore = LoadTexture("assets/sprites/backgrounds/store.png");
    //tex.bgVictory = LoadTexture("assets/sprites/backgrounds/victory.png");
    //tex.bgDefeat = LoadTexture("assets/sprites/backgrounds/defeat.png");
    //tex.bgMatch = LoadTexture("assets/sprites/backgrounds/match.png");

    return tex;
}