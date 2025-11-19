#include "raylib.h"
#include "unloadTextures.hpp"

// Método para descarregar as texturas do jogo
void unloadTextures(Textures tex)
{
    // Player
    UnloadTexture(tex.hamsterWalk);
    UnloadTexture(tex.hamster_hud);

    // Inimigos
    UnloadTexture(tex.enemy1);
    UnloadTexture(tex.enemy2);
    UnloadTexture(tex.enemy3);
    UnloadTexture(tex.enemy4);
    UnloadTexture(tex.boss);

    // Objetos
    UnloadTexture(tex.bullet);
    UnloadTexture(tex.cristal);
    UnloadTexture(tex.cristal_hud);
    UnloadTexture(tex.hearth);
    UnloadTexture(tex.hearth_plus);
    UnloadTexture(tex.speed);
    UnloadTexture(tex.damage);
    UnloadTexture(tex.dash);
    UnloadTexture(tex.machine_gun);
    UnloadTexture(tex.pistol);
    UnloadTexture(tex.shotgun);

    // Fundos
    UnloadTexture(tex.bgMainMenu);
    //UnloadTexture(tex.bgGameSettings);
    //UnloadTexture(tex.bgStore);
    //UnloadTexture(tex.bgVictory);
    //UnloadTexture(tex.bgDefeat);
    //UnloadTexture(tex.bgMatch);
}