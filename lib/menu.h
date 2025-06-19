#ifndef MENU_H
#define MENU_H

// Tamanho da tela
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Enum de telas
typedef enum {
    LOADING_GAME,
    MENU,
    START,
    OPTIONS,
    LOADING_LEVEL,
    LEVEL1,
    OPTIONS_LEVEL,
    EXITLEVEL
} GameScreen;

// Struct de Áudio
typedef struct {
    Sound clickSound;
    Sound entering_level;
    Music mario_menu;
    Music level_1;
} Audio;

// Struct de Texturas
typedef struct {
    Texture2D trivial_games;
    Texture2D logo;
    Texture2D background_menu;
    Texture2D start_button;
    Texture2D start_button_pressed;
    Texture2D options_button;
    Texture2D options_button_pressed;
    Texture2D options_square;
    Texture2D exit_button;
    Texture2D exit_button_pressed;
    Texture2D mapa_do_jogo;
    Texture2D mario_de_lado;
    Texture2D credits_buttom;
    Texture2D score_buttom;
    Texture2D sound_on_buttom;
    Texture2D sound_on_buttom_pressed;
    Texture2D sound_off_buttom;
    Texture2D sound_off_buttom_pressed;
    Texture2D quit_buttom;
    Texture2D quit_buttom_pressed;
    Texture2D block1;
} Icons;

// Struct dos Botões (retângulos invisíveis)
typedef struct {
    Rectangle start_buttonRect;
    Rectangle options_buttonRect;
    Rectangle exit_buttonRect;
    Rectangle exit_optionsRect;
    Rectangle exit_optionsRect2;
    Rectangle exit_mapRect;
    Rectangle sound_buttomRect;
    Rectangle sound_buttomRect2;
    Rectangle quit_buttomRect;
    Rectangle level;
} IconsInvisible;

// Struct do estado do menu
typedef struct {
    char score[1024];
    char credits[1024];
    int framecounter1;
    int framecounter2;
    int framecounter3;
    int sound_counter;
    bool scoreloaded;
    bool scoreloaded2;
    bool startPressed;
    bool optionsPressed;
    bool exitPressed;
    bool exitOptionsPressed;
    bool soundPressedOptions;
    bool exitMapPressed;
    bool enterSoundPlayed;
    bool levelMusicStarted;
    bool quitOptionsPressed;
    double startPressedTime;
    double optionsPressedTime;
    double exitPressedTime;
    double exitOptionsPressedTime;
    double soundPressedTimeOptions;
    double exitMapPressedTime;
    double quitOptionsPressedTime;
    double PRESS_DELAY;
    float speed_mario_automatico;
    Vector2 pos_mario_automatico;
    GameScreen currentScreen;
} MenuState;

// Inicialização
void InitMenuState(MenuState *g);
void InitIcons(Icons *a);
void InitIconsInvisible(IconsInvisible *b, Icons *a);
void InitAudio(Audio *au);

// Telas
void LoadingGameScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void MenuScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void OptionsScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void StartScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void LoadingLevelScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void Level1Screen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void OptionsLevelScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);
void ExitLevelScreen(MenuState *game, Icons *a, Audio *au, IconsInvisible *b);

// Descarregar recursos
void UnloadIcons(Icons *a);
void UnloadAudio(Audio *au);
void UnloadAll(Icons *a, Audio *au);

#endif
