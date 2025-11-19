#ifndef INIT_HPP
#define INIT_HPP

#include "raylib.h"
#include "raymath.h" // para MatrixRotateXYZ(), Vector2Normalize
#include <string>

// -------------------------------------------------------------------------------------------------------------------------------------------
// Definição de Constantes
// -------------------------------------------------------------------------------------------------------------------------------------------
#define NUM_MAX_CRYSTALS 50
#define NUM_MAX_SHOOTS 50
#define NUM_MAX_ENEMIES 50

#define MIN_AREA -1000
#define MAX_AREA 1000
#define ENEMY_SIZE 100.0f

// -------------------------------------------------------------------------------------------------------------------------------------------
// Definição de Enums
// -------------------------------------------------------------------------------------------------------------------------------------------
enum GameScreen 
{
    MENU,
    GAME_SETTINGS,
    GAMEPLAY, 
    STORE,
    VICTORY, 
    DEFEAT 
};

enum Waves
{
    FIRST = 1, 
    SECOND,
    THIRD,
    FOURTH,
    FIFTH
};

enum Characters
{
    HAMSTER = 1,
    FOX,
    FERRET
};

// -------------------------------------------------------------------------------------------------------------------------------------------
// Definição de Structs
// -------------------------------------------------------------------------------------------------------------------------------------------
class Player
{
    public:
        Vector2 position;
        Vector2 direction;
        float damage;
        float vel;
        int crystals;
        int lives;
        //bool powerUp;
};

class Boss
{
    public:
        Vector2 position;
        bool active;
        float vel;
        int lives;
        bool tookDamage;
};

class Enemy
{
    public:
        Vector2 position;
        bool active;
        float vel;
        int lives;
        int type;
        bool tookDamage;
};

class Shoot
{
    public:
        Vector2 position;
        Vector2 direction;
        bool active;
        float vel;
};

class Crystal 
{
    public:
        Vector2 position;
        bool active;
        int enemy;
};

class Store
{
    public:
        std::string name;
        int basePrice;
        int currentPrice;
        float effect;
        bool active;
};

#endif