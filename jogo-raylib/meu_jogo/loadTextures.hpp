#ifndef LOADTEXTURES_HPP
#define LOADTEXTURES_HPP

#include "raylib.h"

struct Textures {
    // Player
    Texture2D hamsterWalk;
    Texture2D hamster_hud;

    // Inimigos
    Texture2D enemy1;
    Texture2D enemy2;
    Texture2D enemy3;
    Texture2D enemy4;
    Texture2D boss;

    // Objetos
    Texture2D bullet;
    Texture2D cristal;
    Texture2D cristal_hud;
    Texture2D hearth;
    Texture2D hearth_plus;
    Texture2D speed;
    Texture2D damage;
    Texture2D dash;
    Texture2D machine_gun;
    Texture2D pistol;
    Texture2D shotgun;

    // Fundos
    Texture2D bgMainMenu;
    //Texture2D bgGameSettings;
    //Texture2D bgStore;
    //Texture2D bgVictory;
    //Texture2D bgDefeat;
    //Texture2D bgMatch;
};

// Declaração da função
Textures loadTextures();

#endif