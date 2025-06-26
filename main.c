#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
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
Flag_t Flag;
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
    InitFlag(&Flag);
    SetWindowIcon(icons.icon_window);

    scene = CreateGameScene("assets/textures/background.png");

    bumpSound = LoadSound("assets/audio/bump.wav");
    jumpSound = LoadSound("assets/audio/jump.wav");
    coinAtlas = LoadTexture("assets/textures/items/coinfix.png");

    InitMario(&Mario);
    InitInimigos();
    InitCoins(coins);
    InitPlatforms(physPlatforms);
    InitCamera(&gameCamera, SCREEN_WIDTH, SCREEN_HEIGHT);

    Rectangle chao[4] = {{0.0f, 445.0f, 1005.0f, 120.0f},{1130.0f, 445.0f, 400.0f, 120.0f}, {1695, 445, 720, 120}, {2540, 445, 1210, 120}};
    Rectangle paredes[2] = {{1912.0f, 332.0f, 75.0f, 75.0f},{2320, 332, 75, 115}};
    Rectangle plataforma[4] = {{580, 265, 300, 57}, {1252, 215, 180, 57}, {2006, 215, 300, 57}, {2447, 215, 300, 57}};


    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(audio.mario_menu);

        BeginDrawing();
        ClearBackground(BLACK);
        MoveCamera(&gameCamera, &Mario, &scene, SCREEN_WIDTH, SCREEN_HEIGHT);
        BeginMode2D(gameCamera);

        //Controle de Telas
        switch (menu_state.currentScreen) {
            case LOADING_GAME:
                LoadingGameScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break;
            case MENU:
                MenuScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break;
            case OPTIONS:
                OptionsScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break;
            case START:
                StartScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break;
            case LOADING_LEVEL:
                LoadingLevelScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break;
            case LEVEL1:
                
                UpdateMario(&Mario, physPlatforms, physPlatCount, bumpSound);
                UpdateInimigos(chao, plataforma, paredes, 2, &Mario); // <-- Atualiza inimigos
                UpdatePlatforms(physPlatforms);
                UpdateCoins(&Mario);
                UpdateFloatingScores();
                Level1Screen(&menu_state, &icons, &audio, &iconsinvisible);
                UpdateFlag(&Flag, &Mario, physPlatforms);
                
                break;
            case CAVERNA:
                break;
            case OPTIONS_LEVEL:
                OptionsLevelScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break; 
            case EXITLEVEL:
                ExitLevelScreen(&menu_state, &icons, &audio, &iconsinvisible);
                break;
            case GAMEOVER:
                //menu_state.framecounter4++;
                 DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                // DrawText("GAME OVER", 340, 200, 50, WHITE);
                // if (menu_state.framecounter4>100){
                //    menu_state.currentScreen = MENU;
               //  }
                 break;
            case MENU2:
                DrawRectangle (0,0,800,600,BLACK);
            //   DrawText("YOU WIN", 400, 280, 50, WHITE);
            break;
        }

        EndMode2D();
        EndDrawing();
    }

    //Liberação dos recursos
    UnloadAll(&icons, &audio);
    UnloadMario(&Mario);
    UnloadFlag(&Flag);

    FILE *p;
    p = fopen("assets/texts_files/score.bin","wb");
    if(p == NULL){
        printf("Erro na criacao do arquivo.");
        return 1;
    }else{
        fwrite(&Mario.stats.score, sizeof(int), 1, p);
    }
    fclose(p);

    p = fopen("assets/texts_files/credits.bin","wb");
    if(p == NULL){
        printf("Erro na criacao do arquivo.");
        return 1;
    }else{
        fwrite(&Mario.stats.coins, sizeof(int), 1, p);
    }
    fclose(p);
  
    CloseWindow();
    return 0;
}
