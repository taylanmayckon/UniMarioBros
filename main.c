#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include "mario.h"
#include "scene.h"
#include "platform.h"
#include "camera.h"
#include "coin.h"
#include "inimigos.h"
#include "menu.h"

GameScene scene;
Mario_t Mario;
Camera2D gameCamera;
PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];
Sound bumpSound;
Sound jumpSound;
extern Texture2D coinAtlas;

int main(void) {
    //Inicialização da Janela e Áudio
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Mario Bros");
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    //Structs principais
    MenuState menu_state;
    Icons icons;
    IconsInvisible iconsinvisible;
    Audio audio;

    //Inicializações
    InitMenuState(&menu_state);
    InitIcons(&icons);
    InitIconsInvisible(&iconsinvisible, &icons);
    InitAudio(&audio);

    scene = CreateGameScene("assets/textures/background.png");

    bumpSound = LoadSound("assets/audio/bump.wav");
    jumpSound = LoadSound("assets/audio/jump.wav");
    coinAtlas = LoadTexture("assets/textures/items/coin.png");
    Texture2D texturaInimigos = LoadTexture("assets/textures/inimigos.png");
    Texture2D tuneis = LoadTexture("assets/textures/blocks/tunnels.png");

    InitMario(&Mario);
    InitInimigos();
    InitPlatforms(physPlatforms);
    InitCamera(&gameCamera, SCREEN_WIDTH, SCREEN_HEIGHT);

    Rectangle chao[4] = {{0.0f, 445.0f, 1005.0f, 120.0f},{1130.0f, 445.0f, 400.0f, 120.0f}, {1695, 445, 720, 120}, {2540, 445, 1210, 120}};
    Rectangle paredes[2] = {{1912.0f, 332.0f, 75.0f, 75.0f},{2320, 332, 75, 115}};
    Rectangle plataforma[4] = {{580, 265, 300, 57}, {1252, 215, 180, 57}, {2006, 215, 300, 57}, {2447, 215, 300, 57}}; //3 ultimas plataformas mudou a coordenada


    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(audio.mario_menu);

        MoveCamera(&gameCamera, Mario, SCREEN_WIDTH, SCREEN_HEIGHT);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(gameCamera);

        //Controle de Telas
        // switch (menu_state.currentScreen) {
        //     case LOADING_GAME:
        //         LoadingGameScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        //     case MENU:
        //         MenuScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        //     case OPTIONS:
        //         OptionsScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        //     case START:
        //         StartScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        //     case LOADING_LEVEL:
        //         LoadingLevelScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        //     case LEVEL1:
                UpdateMario(&Mario, physPlatforms, physPlatCount, bumpSound);
                UpdateInimigos(chao, plataforma, paredes, 2, &Mario); // <-- Atualiza inimigos
                UpdatePlatforms(physPlatforms);
                UpdateCoins(&Mario);
                Level1Screen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        //     case OPTIONS_LEVEL:
        //         OptionsLevelScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break; 
        //     case EXITLEVEL:
        //         ExitLevelScreen(&menu_state, &icons, &audio, &iconsinvisible);
        //         break;
        // }

        EndMode2D();
        EndDrawing();
    }

    //Liberação dos recursos
    UnloadAll(&icons, &audio);
    UnloadMario(&Mario);
  
    CloseWindow();

    return 0;
}