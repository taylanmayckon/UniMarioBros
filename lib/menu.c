#include "menu.h"

// Variáveis globais compartilhadas
extern GameScene scene;
extern Mario_t Mario;
extern Camera2D gameCamera;
extern PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];
extern Sound bumpSound;
extern Sound jumpSound;
extern Texture2D coinAtlas;

// ==========================
// Inicialização
// ==========================
void InitMenuState(MenuState *g) {
    g->framecounter1 = 0;
    g->framecounter2 = 0;
    g->framecounter3 = 0;
    g->sound_counter = 0;
    g->scoreloaded = false;
    g->scoreloaded2 = false;
    g->startPressed = false;
    g->optionsPressed = false;
    g->exitPressed = false;
    g->exitOptionsPressed = false;
    g->soundPressedOptions = false;
    g->exitMapPressed = false;
    g->enterSoundPlayed = false;
    g->levelMusicStarted = false;
    g->quitOptionsPressed = false;
    g->quitOptionsPressedTime = false;
    g->startPressedTime = 0.0;
    g->optionsPressedTime = 0.0;
    g->exitPressedTime = 0.0;
    g->exitOptionsPressedTime = 0.0;
    g->soundPressedTimeOptions = 0.0;
    g->exitMapPressedTime = 0.0;
    g->quitOptionsPressedTime = 0.0;
    g->PRESS_DELAY = 0.20;
    g->speed_mario_automatico = 100.0f;
    g->pos_mario_automatico = (Vector2){170, 195};
    strcpy(g->score, "");
    strcpy(g->credits, "");
    g->currentScreen = LOADING_GAME;
}

void InitIcons(Icons *a) {
    a->trivial_games = LoadTexture("assets/textures/icons/trivial_games.png");
    a->logo = LoadTexture("assets/textures/icons/logo.png");
    a->background_menu = LoadTexture("assets/textures/icons/background_menu.png");
    a->start_button = LoadTexture("assets/textures/icons/play.png");
    a->start_button_pressed = LoadTexture("assets/textures/icons/play_pressed.png");
    a->options_button = LoadTexture("assets/textures/icons/options.png");
    a->options_button_pressed = LoadTexture("assets/textures/icons/options_pressed.png");
    a->options_square = LoadTexture("assets/textures/icons/options_square.png");
    a->exit_button = LoadTexture("assets/textures/icons/exit.png");
    a->exit_button_pressed = LoadTexture("assets/textures/icons/exit_pressed.png");
    a->mapa_do_jogo = LoadTexture("assets/textures/icons/mapa_do_jogo.png");
    a->mario_de_lado = LoadTexture("assets/textures/icons/mario_menu.png");
    a->credits_buttom = LoadTexture("assets/textures/icons/credits.png");
    a->score_buttom = LoadTexture("assets/textures/icons/score.png");
    a->sound_on_buttom = LoadTexture("assets/textures/icons/sound_on.png");
    a->sound_on_buttom_pressed = LoadTexture("assets/textures/icons/sound_on_pressed.png");
    a->sound_off_buttom = LoadTexture("assets/textures/icons/sound_off.png");
    a->sound_off_buttom_pressed = LoadTexture("assets/textures/icons/sound_off_pressed.png");
    a->quit_buttom = LoadTexture("assets/textures/icons/quit_bottom.png");
    a->quit_buttom_pressed = LoadTexture("assets/textures/icons/quit_bottom_pressed.png");
    a->block1 = LoadTexture("assets/textures/blocks/block1.png");
}

void InitIconsInvisible(IconsInvisible *b, Icons *a) {
    int posx_logo = GetScreenWidth()/2 - a->logo.width/2;
    int posy_logo = 130;
    int posx_start = GetScreenWidth()/2 - a->start_button.width/2;
    int posy_start = posy_logo + a->logo.height + 40;
    int posx_options = 730;
    int posy_options = 10;
    int posx_exit = 18;
    int posy_exit = 10;

    b->start_buttonRect = (Rectangle){posx_start, posy_start, a->start_button.width, a->start_button.height};
    b->options_buttonRect = (Rectangle){posx_options, posy_options, a->options_button.width, a->options_button.height};
    b->exit_buttonRect = (Rectangle){posx_exit, posy_exit, a->exit_button.width, a->exit_button.height};
    b->exit_optionsRect = (Rectangle){560, 103, a->exit_button.width, a->exit_button.height};
    b->exit_optionsRect2 = (Rectangle){560, 103, a->exit_button.width, a->exit_button.height};
    b->exit_mapRect = (Rectangle){650, 80, a->exit_button.width, a->exit_button.height};
    b->sound_buttomRect = (Rectangle){370, 160, a->sound_on_buttom.width, a->sound_on_buttom.height};
    b->sound_buttomRect2 = (Rectangle){370, 220, a->sound_on_buttom.width, a->sound_on_buttom.height};
    b->quit_buttomRect = (Rectangle){320, 300, a->quit_buttom.width, a->quit_buttom.height};
    b->level = (Rectangle){244, 114, 40, 40};
}

void InitAudio(Audio *au) {
    au->clickSound = LoadSound("assets/audio/click.wav");
    au->entering_level = LoadSound("assets/audio/entering_level.wav");
    au->mario_menu = LoadMusicStream("assets/audio/jump_up.mp3");
    au->level_1 = LoadMusicStream("assets/audio/Ground_Theme.mp3");

    SetSoundVolume(au->clickSound, 0.7f);
    SetMusicVolume(au->mario_menu, 0.1f);
    SetMusicVolume(au->level_1, 0.5f);
}
void LoadingGameScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    g->framecounter1++;
    DrawTexture(a->trivial_games, GetScreenWidth()/2 - a->trivial_games.width/2, -40, WHITE);
    DrawRectangleLines((GetScreenWidth()/2 - 500/2)-4, 396, 509, 40, WHITE);
    
    if (g->framecounter1 < 100) {
        DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 0, 0, WHITE);
    } else if (g->framecounter1 < 200) {
        DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 50, 30, WHITE);
    } else if (g->framecounter1 < 300) {
        DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 150, 30, WHITE);
    } else if (g->framecounter1 < 400) {
        DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 450, 30, WHITE);
        PlayMusicStream(au->mario_menu);
    } else if (g->framecounter1 < 500) {
        DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 500, 30, WHITE);
    } else {
        g->currentScreen = MENU;
        PlayMusicStream(au->mario_menu);
    }
}

void MenuScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    if (!IsMusicStreamPlaying(au->mario_menu) && g->sound_counter == 0) {
        PlayMusicStream(au->mario_menu);
    }

    DrawTexture(a->background_menu, 0, -10, WHITE);
    DrawTexture(a->logo, GetScreenWidth()/2 - a->logo.width/2, 130, WHITE);
    DrawTexture(a->start_button, b->start_buttonRect.x, b->start_buttonRect.y, WHITE);
    DrawTexture(a->options_button, b->options_buttonRect.x, b->options_buttonRect.y, WHITE);
    DrawTexture(a->exit_button, b->exit_buttonRect.x, b->exit_buttonRect.y, WHITE);

    Vector2 mouse = GetMousePosition();

    // Botão Start
    if (!g->startPressed) {
        if (CheckCollisionPointRec(mouse, b->start_buttonRect)) {
            DrawRectangle(b->start_buttonRect.x+3, b->start_buttonRect.y+1, b->start_buttonRect.width-6, b->start_buttonRect.height-6, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->startPressed = true;
                g->startPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->start_button_pressed, b->start_buttonRect.x + 3, b->start_buttonRect.y + 1, WHITE);
        if ((GetTime() - g->startPressedTime) >= g->PRESS_DELAY) {
            g->currentScreen = START;
            g->startPressed = false;
        }
    }

    // Botão Options
    if (!g->optionsPressed) {
        if (CheckCollisionPointRec(mouse, b->options_buttonRect)) {
            DrawRectangle(b->options_buttonRect.x + 1, b->options_buttonRect.y + 1, b->options_buttonRect.width, b->options_buttonRect.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->optionsPressed = true;
                g->optionsPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->options_button_pressed, b->options_buttonRect.x + 3, b->options_buttonRect.y + 3, LIGHTGRAY);
        if ((GetTime() - g->optionsPressedTime) >= g->PRESS_DELAY) {
            g->currentScreen = OPTIONS;
            g->optionsPressed = false;
        }
    }

    // Botão Exit
    if (!g->exitPressed) {
        if (CheckCollisionPointRec(mouse, b->exit_buttonRect)) {
            DrawRectangle(b->exit_buttonRect.x + 1, b->exit_buttonRect.y + 1, b->exit_buttonRect.width, b->exit_buttonRect.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->exitPressed = true;
                g->exitPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->exit_button_pressed, b->exit_buttonRect.x + 3, b->exit_buttonRect.y + 2, LIGHTGRAY);
        if ((GetTime() - g->exitPressedTime) >= g->PRESS_DELAY) {
            CloseWindow();
        }
    }
}

void OptionsScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    FILE *p;

    DrawTexture(a->background_menu, 0, -10, WHITE);
    DrawRectangle(0, -10, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawTexture(a->options_square, GetScreenWidth()/2 - a->options_square.width/2, GetScreenHeight()/2 - a->options_square.height/2, WHITE);
    DrawTexture(a->score_buttom, 320, 230, WHITE);
    DrawTexture(a->credits_buttom, 320, 340, WHITE);
    DrawRectangle(295, 290, 200, 40, (Color){240,230,180,255});
    DrawRectangle(295, 400, 200, 40, (Color){240,230,180,255});

    if (!g->scoreloaded) {
        p = fopen("assets/texts_files/score.txt", "r");
        if (p == NULL) {
            printf("Erro na abertura do arquivo \"score.txt\"!\n");
        } else {
            fread(g->score, sizeof(char), sizeof(g->score) - 1, p);
            fclose(p);
        }
        g->scoreloaded = true;
    }

    if (!g->scoreloaded2) {
        p = fopen("assets/texts_files/credits.txt", "r");
        if (p == NULL) {
            printf("Erro na abertura do arquivo \"credits.txt\"!\n");
        } else {
            fread(g->credits, sizeof(char), sizeof(g->credits) - 1, p);
            fclose(p);
        }
        g->scoreloaded2 = true;
    }

    DrawText(g->score, 305, 290, 45, BLACK);
    DrawText(g->credits, 305, 400, 45, BLACK);

    Vector2 mouse = GetMousePosition();

    // Botão Exit Options
    if (!g->exitOptionsPressed) {
        DrawTexture(a->exit_button, 560, 103, WHITE);
        if (CheckCollisionPointRec(mouse, b->exit_optionsRect)) {
            DrawRectangle(560, 103, a->exit_button.width, a->exit_button.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->exitOptionsPressed = true;
                g->exitOptionsPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->exit_button_pressed, 563, 105, LIGHTGRAY);
        if ((GetTime() - g->exitOptionsPressedTime) >= g->PRESS_DELAY) {
            g->currentScreen = MENU;
            g->exitOptionsPressed = false;
        }
    }

    // Botão Sound
    if (!g->soundPressedOptions) {
        if (g->sound_counter == 0) {
            DrawTexture(a->sound_on_buttom, 370, 160, WHITE);
        } else {
            DrawTexture(a->sound_off_buttom, 370, 160, WHITE);
        }
        if (CheckCollisionPointRec(mouse, b->sound_buttomRect)) {
            DrawRectangle(370, 160, a->sound_on_buttom.width, a->sound_on_buttom.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->soundPressedOptions = true;
                g->soundPressedTimeOptions = GetTime();
            }
        }
    } else {
        if (g->sound_counter == 0) {
            DrawTexture(a->sound_on_buttom_pressed, 373, 162, LIGHTGRAY);
        } else {
            DrawTexture(a->sound_off_buttom_pressed, 373, 162, LIGHTGRAY);
        }
        if ((GetTime() - g->soundPressedTimeOptions) >= g->PRESS_DELAY) {
            if (g->sound_counter == 0) {
                PauseMusicStream(au->mario_menu);
                g->sound_counter = 1;
            } else {
                ResumeMusicStream(au->mario_menu);
                g->sound_counter = 0;
            }
            g->soundPressedOptions = false;
        }
    }
}

void StartScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    float dt = GetFrameTime();

    if (g->pos_mario_automatico.x < 265) {
        g->pos_mario_automatico.x += g->speed_mario_automatico * dt;
        if (g->pos_mario_automatico.x > 265) {
            g->pos_mario_automatico.x = 265;
        }
    } else {
        if (g->pos_mario_automatico.y > 120) {
            g->pos_mario_automatico.y -= g->speed_mario_automatico * dt;
            if (g->pos_mario_automatico.y < 120) {
                g->pos_mario_automatico.y = 120;
                g->currentScreen = LOADING_LEVEL;
            }
        }
    }

    DrawTexture(a->background_menu, 0, -10, WHITE);
    DrawRectangle(0, -10, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    DrawTexture(a->mapa_do_jogo, GetScreenWidth()/2 - a->mapa_do_jogo.width/2, GetScreenHeight()/2 - a->mapa_do_jogo.height/2, WHITE);
    DrawTexture(a->mario_de_lado, (int)g->pos_mario_automatico.x, (int)g->pos_mario_automatico.y, WHITE);
    DrawTexture(a->exit_button, 650, 80, WHITE);

    Vector2 mouse = GetMousePosition();

    if (!g->exitMapPressed) {
        if (CheckCollisionPointRec(mouse, b->exit_mapRect)) {
            DrawRectangle(650, 80, a->exit_button.width, a->exit_button.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->exitMapPressed = true;
                g->exitMapPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->exit_button_pressed, 653, 82, LIGHTGRAY);
        if ((GetTime() - g->exitMapPressedTime) >= g->PRESS_DELAY) {
            g->currentScreen = MENU;
            g->exitMapPressed = false;
        }
    }
}

void LoadingLevelScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    PauseMusicStream(au->mario_menu);
    g->framecounter2++;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    DrawText("LEVEL 1", 300, 200, 50, WHITE);
    DrawRectangleLines(GetScreenWidth()/2 - 500/2 - 4, 296, 509, 40, WHITE);
    DrawRectangle(GetScreenWidth()/2 - 500/2, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 100, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 200, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 300, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 400, 300, 98, 30, Fade(WHITE, 0.3f));

    int blocks = g->framecounter2 / 50;
    for (int i = 1; i <= blocks && i <= 5; i++) {
        DrawRectangle(GetScreenWidth()/2 - 500/2 + (i-1)*100, 300, 98, 30, WHITE);
    }

    if (g->framecounter2 >= 350 && !g->enterSoundPlayed) {
        if (g->sound_counter == 0) {
            PlaySound(au->entering_level);
        }
        g->enterSoundPlayed = true;
    }

    if (g->enterSoundPlayed && !g->levelMusicStarted) {
        if (!IsSoundPlaying(au->entering_level)) {
            if (g->sound_counter == 0) {
                PlayMusicStream(au->level_1);
            }
            g->levelMusicStarted = true;
            g->currentScreen = LEVEL1;
        }
    }
}

void Level1Screen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    UpdateMusicStream(au->level_1);

    if (!IsMusicStreamPlaying(au->level_1) && g->sound_counter == 0) {
        PlayMusicStream(au->level_1);
    }

    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        g->currentScreen = OPTIONS_LEVEL;
    }

    DrawGameScene(scene);
    DrawCoins();
    DrawBlocks(physPlatforms, a->block1);
    DrawMario(&Mario);
}

void OptionsLevelScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    UpdateMusicStream(au->level_1);

    DrawGameScene(scene);
    DrawRectangle(0, 0, scene.background.width, scene.background.height, Fade(BLACK, 0.5f));
    DrawTexture(a->options_square, GetScreenWidth()/2 - a->options_square.width/2, GetScreenHeight()/2 - a->options_square.height/2, WHITE);

    Vector2 mouse = GetMousePosition();

    // Botão Exit Options
    if (!g->exitOptionsPressed) {
        DrawTexture(a->exit_button, 560, 103, WHITE);
        if (CheckCollisionPointRec(mouse, b->exit_optionsRect2)) {
            DrawRectangle(560, 103, a->exit_button.width, a->exit_button.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->exitOptionsPressed = true;
                g->exitOptionsPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->exit_button_pressed, 563, 105, LIGHTGRAY);
        if ((GetTime() - g->exitOptionsPressedTime) >= g->PRESS_DELAY) {
            g->currentScreen = LEVEL1;
            g->exitOptionsPressed = false;
        }
    }

    // Botão Sound
    if (!g->soundPressedOptions) {
        if (g->sound_counter == 0) {
            DrawTexture(a->sound_on_buttom, 370, 220, WHITE);
        } else {
            DrawTexture(a->sound_off_buttom, 370, 220, WHITE);
        }
        if (CheckCollisionPointRec(mouse, b->sound_buttomRect2)) {
            DrawRectangle(370, 220, a->sound_on_buttom.width, a->sound_on_buttom.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->soundPressedOptions = true;
                g->soundPressedTimeOptions = GetTime();
            }
        }
    } else {
        if (g->sound_counter == 0) {
            DrawTexture(a->sound_on_buttom_pressed, 373, 222, LIGHTGRAY);
        } else {
            DrawTexture(a->sound_off_buttom_pressed, 373, 222, LIGHTGRAY);
        }
        if ((GetTime() - g->soundPressedTimeOptions) >= g->PRESS_DELAY) {
            if (g->sound_counter == 0) {
                PauseMusicStream(au->level_1);
                g->sound_counter = 1;
            } else {
                ResumeMusicStream(au->level_1);
                g->sound_counter = 0;
            }
            g->soundPressedOptions = false;
        }
    }

    // Botão Quit
    if (!g->quitOptionsPressed) {
        DrawTexture(a->quit_buttom, 320, 300, WHITE);
        if (CheckCollisionPointRec(mouse, b->quit_buttomRect)) {
            DrawRectangle(320, 300, a->quit_buttom.width, a->quit_buttom.height, Fade(BLACK, 0.2f));
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (g->sound_counter == 0) PlaySound(au->clickSound);
                g->quitOptionsPressed = true;
                g->quitOptionsPressedTime = GetTime();
            }
        }
    } else {
        DrawTexture(a->quit_buttom_pressed, 320, 300, LIGHTGRAY);
        if ((GetTime() - g->quitOptionsPressedTime) >= g->PRESS_DELAY) {
            g->currentScreen = EXITLEVEL;
            g->quitOptionsPressed = false;
        }
    }
}

void ExitLevelScreen(MenuState *g, Icons *a, Audio *au, IconsInvisible *b) {
    PauseMusicStream(au->level_1);
    g->framecounter3++;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    DrawText("EXIT LEVEL", 250, 200, 50, WHITE);
    DrawRectangleLines(GetScreenWidth()/2 - 500/2 - 4, 296, 509, 40, WHITE);
    DrawRectangle(GetScreenWidth()/2 - 500/2, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 100, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 200, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 300, 300, 98, 30, Fade(WHITE, 0.3f));
    DrawRectangle(GetScreenWidth()/2 - 500/2 + 400, 300, 98, 30, Fade(WHITE, 0.3f));

    int blocks2 = g->framecounter3 / 50;
    for (int i = 1; i <= blocks2 && i <= 5; i++) {
        DrawRectangle(GetScreenWidth()/2 - 500/2 + (i-1)*100, 300, 98, 30, WHITE);
    }

    if (g->framecounter3 >= 350) {
        g->currentScreen = MENU;
    }
}
// ==========================
// Descarregar recursos
// ==========================
void UnloadIcons(Icons *a) {
    UnloadTexture(a->trivial_games);
    UnloadTexture(a->logo);
    UnloadTexture(a->background_menu);
    UnloadTexture(a->start_button);
    UnloadTexture(a->start_button_pressed);
    UnloadTexture(a->options_button);
    UnloadTexture(a->options_button_pressed);
    UnloadTexture(a->options_square);
    UnloadTexture(a->exit_button);
    UnloadTexture(a->exit_button_pressed);
    UnloadTexture(a->mapa_do_jogo);
    UnloadTexture(a->mario_de_lado);
    UnloadTexture(a->credits_buttom);
    UnloadTexture(a->score_buttom);
    UnloadTexture(a->sound_on_buttom);
    UnloadTexture(a->sound_on_buttom_pressed);
    UnloadTexture(a->sound_off_buttom);
    UnloadTexture(a->sound_off_buttom_pressed);
    UnloadTexture(a->quit_buttom);
    UnloadTexture(a->quit_buttom_pressed);
    UnloadTexture(a->block1);
}

void UnloadAudio(Audio *au) {
    UnloadSound(au->clickSound);
    UnloadSound(au->entering_level);
    StopMusicStream(au->mario_menu);
    UnloadMusicStream(au->mario_menu);
    StopMusicStream(au->level_1);
    UnloadMusicStream(au->level_1);
}

void UnloadAll(Icons *a, Audio *au) {
    UnloadIcons(a);
    UnloadAudio(au);
    UnloadGameScene(scene);
    UnloadTexture(coinAtlas);
    UnloadSound(bumpSound);
    UnloadSound(jumpSound);
    CloseAudioDevice();
    CloseWindow();
}

