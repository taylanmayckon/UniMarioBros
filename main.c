#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "mario.h"
#include "scene.h"
#include "platform.h"
#include "camera.h"
#include "coin.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Inicializando Mario_t
Mario_t Mario;
// Inicializando a câmera
Camera2D gameCamera;
// Inicializando o vetor plataformas dinâmicas
PhysPlatform_t physPlatforms[MAX_PHYS_PLATFORMS];

// Variavel de tempo para testes
float timer = 0.0f;

// Placeholders para não bugar compilação
Sound bumpSound;
Sound jumpSound;

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

int main (void){
	GameScreen currentScreen = LOADING_GAME;
	FILE *p;
	char score [1024] = {0};
	char credits [1024] = {0};
	int framecounter1=0;
	int framecounter2=0;
	int framecounter3=0;
	int sound_counter = 0;
	bool scoreloaded = false;
	bool scoreloaded2 = false;
	bool startPressed = false;
	bool optionsPressed = false;
	bool exitPressed = false;
	bool exitOptionsPressed = false;
	bool soundPressedOptions = false;
	bool exitMapPressed = false;
	bool enterSoundPlayed = false;
	bool levelMusicStarted = false;
	bool quitOptionsPressed = false;
	bool quitOptionsPressedTime = false;
	double startPressedTime = 0.0;
	double optionsPressedTime = 0.0;
	double exitPressedTime = 0.0;
	double exitOptionsPressedTime = 0.0;
	double soundPressedTimeOptions = 0.0;
	double exitMapPressedTime = 0.0;
	const double PRESS_DELAY = 0.20;
	float speed_mario_automatico = 100.0f;
	Vector2 pos_mario_automatico = {170,195};
	
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Mario Bros");
	SetExitKey (KEY_NULL);
	InitAudioDevice ();
	
	Sound clickSound = LoadSound ("assets/audio/click.wav");
	Sound entering_level = LoadSound ("assets/audio/entering_level.wav");
	SetSoundVolume(clickSound,0.7f);
	SetSoundVolume(clickSound,0.5f);
	
	Music mario_menu = LoadMusicStream ("assets/audio/jump_up.mp3");
	SetMusicVolume (mario_menu,0.1f);
	
	Music level_1 = LoadMusicStream ("assets/audio/Ground_Theme.mp3");
	SetMusicVolume(level_1, 0.5f);
	
	Image icon = LoadImage ("assets/textures/icons/icon.png");
	SetWindowIcon (icon);
	UnloadImage (icon);
	
	Texture2D trivial_games = LoadTexture("assets/textures/icons/trivial_games.png");
	Texture2D logo = LoadTexture("assets/textures/icons/logo.png");
	Texture2D background = LoadTexture ("assets/textures/icons/background_menu.png");
	Texture2D start_button = LoadTexture ("assets/textures/icons/play.png");
	Texture2D start_button_pressed = LoadTexture ("assets/textures/icons/play_pressed.png");
	Texture2D options_button = LoadTexture ("assets/textures/icons/options.png");
	Texture2D options_button_pressed = LoadTexture ("assets/textures/icons/options_pressed.png");
	Texture2D options_square = LoadTexture ("assets/textures/icons/options_square.png");
	Texture2D exit_button = LoadTexture ("assets/textures/icons/exit.png");
	Texture2D exit_button_pressed = LoadTexture ("assets/textures/icons/exit_pressed.png");
	Texture2D mapa_do_jogo = LoadTexture("assets/textures/icons/mapa_do_jogo.png");
	Texture2D mario_de_lado = LoadTexture("assets/textures/icons/mario_menu.png");
	Texture2D credits_buttom = LoadTexture("assets/textures/icons/credits.png");
	Texture2D score_buttom = LoadTexture("assets/textures/icons/score.png");
	Texture2D sound_on_buttom = LoadTexture("assets/textures/icons/sound_on.png");
	Texture2D sound_on_buttom_pressed = LoadTexture("assets/textures/icons/sound_on_pressed.png");
	Texture2D sound_off_buttom = LoadTexture("assets/textures/icons/sound_off.png");
	Texture2D sound_off_buttom_pressed = LoadTexture("assets/textures/icons/sound_off_pressed.png");
	Texture2D quit_buttom = LoadTexture("assets/textures/icons/quit_bottom.png");
	Texture2D quit_buttom_pressed = LoadTexture("assets/textures/icons/quit_bottom_pressed.png");
	
	int posx_logo = GetScreenWidth()/2 - logo.width/2;
	int posy_logo = 130;
	int posx_start = GetScreenWidth()/2 - start_button.width/2;
	int posy_start = posy_logo + logo.height + 40;
	int posx_options = 730;
	int posy_options = 10;
	int posx_exit = 18;
	int posy_exit = 10;
	
	Rectangle start_buttonRect = {posx_start,posy_start,start_button.width,start_button.height};
	Rectangle options_buttonRect = {posx_options,posy_options,options_button.width,options_button.height};
	Rectangle exit_buttonRect = {posx_exit,posy_exit,exit_button.width,exit_button.height};
	Rectangle exit_optionsRect = {560,103,exit_button.width,exit_button.height};
	Rectangle exit_optionsRect2 = {560,103,exit_button.width,exit_button.height};
	Rectangle exit_mapRect = {650,80,exit_button.width,exit_button.height};
	Rectangle sound_buttomRect = {370,160,sound_on_buttom.width,sound_on_buttom.height};
	Rectangle sound_buttomRect2 = {370,220,sound_on_buttom.width,sound_on_buttom.height};
	Rectangle quit_buttomRect = {320,300,quit_buttom.width,quit_buttom.height};
	Rectangle level = {244,114,40,40};
	
	SetTargetFPS (60);
	
	GameScene scene = CreateGameScene("assets/textures/background.png"); // Função da biblioteca Scene para configurar o cenário
    // (PLACHOLDER) bloco para teste de DrawBlocks
    Texture2D block1 = LoadTexture("assets/textures/blocks/block1.png");

    bumpSound = LoadSound("assets/audio/bump.wav"); // Carrega som bump
    jumpSound = LoadSound("assets/audio/jump.wav"); // Carrega som pulo

    coinAtlas = LoadTexture("assets/textures/items/coin.png"); // Carrega textura moeda

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    InitPlatforms(physPlatforms); // Inicializando as plataformas
    InitCamera(&gameCamera, SCREEN_WIDTH, SCREEN_HEIGHT); // Inicializando a câmera
	
	while (!WindowShouldClose()){
		
		UpdateMusicStream (mario_menu);
		
		// Processando o back
        UpdateMario(&Mario, physPlatforms, physPlatCount, bumpSound); // Movimentação, física e preparação de output para outras libs
        // HandleMarioPlatformCollisions(&Mario, physPlatforms, bumpSound); // Atualiza a colisão do Mario
        UpdatePlatforms(physPlatforms);
        UpdateCoins(&Mario); // Atualiza as moedas
        MoveCamera(&gameCamera, Mario, SCREEN_WIDTH, SCREEN_HEIGHT); // Atualiza os parâmetros da câmera

		float dt = GetFrameTime();
		
		//Processando o front
		BeginDrawing ();
		ClearBackground(BLACK);
		BeginMode2D(gameCamera); 
	
		switch (currentScreen){
			case LOADING_GAME:
				framecounter1++;
				DrawTexture (trivial_games,GetScreenWidth()/2 - trivial_games.width/2,-40,WHITE);
				DrawRectangleLines((GetScreenWidth()/2 - 500/2)-4, 396, 509, 40, WHITE);
				if (framecounter1<100){
					DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 0, 0, WHITE);
				}
				else if (framecounter1<200){
					DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 50, 30, WHITE);
				}
				else if (framecounter1<300){
					DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 150, 30, WHITE);
				}
				else if (framecounter1<400){
					DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 450, 30, WHITE);
					PlayMusicStream (mario_menu);
				}
				else if (framecounter1<500){
					DrawRectangle(GetScreenWidth()/2 - 500/2, 400, 500, 30, WHITE);
				}
				else{
					currentScreen = MENU;
					PlayMusicStream (mario_menu);
				}
				break;
			case MENU:
				if (!IsMusicStreamPlaying (mario_menu) && sound_counter == 0){
					PlayMusicStream (mario_menu);
				}
				DrawTexture (background,0,0,WHITE);
				DrawTexture (logo,posx_logo,posy_logo,WHITE);
				DrawTexture (start_button,posx_start,posy_start,WHITE);
				DrawTexture (options_button,posx_options,posy_options,WHITE);
				DrawTexture (exit_button,posx_exit,posy_exit,WHITE);
				
				Vector2 mousePoint = GetMousePosition();
				if (!startPressed) {
		      	  DrawTexture(start_button, posx_start, posy_start, WHITE);
        			if (CheckCollisionPointRec(mousePoint, start_buttonRect)) {
            			DrawRectangle(posx_start+3, posy_start+1,start_button.width-6, start_button.height-6,Fade(BLACK, 0.2f));
           					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                				if (sound_counter == 0) {
                					PlaySound(clickSound);
								}
                				startPressed = true;
                				startPressedTime = GetTime();
           					}
        			}
   				}
    			else {
        			DrawTexture(start_button_pressed, posx_start + 3, posy_start + 1, WHITE);
        			if ((GetTime() - startPressedTime) >= PRESS_DELAY) {
            			currentScreen = START;
            			startPressed = false;
        			}
    			}
				if (!optionsPressed) {
   		 			DrawTexture(options_button, posx_options, posy_options, WHITE);
    				if (CheckCollisionPointRec(mousePoint, options_buttonRect)) {
        				DrawRectangle(posx_options + 1, posy_options + 1,options_button.width, options_button.height,Fade(BLACK, 0.2f));
        				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
            					PlaySound(clickSound);
							}
            				optionsPressed = true;
            				optionsPressedTime = GetTime();
        				}
    				}
				}
				else {
   			 		DrawTexture(options_button_pressed, posx_options + 3, posy_options + 3, LIGHTGRAY);
    				if ((GetTime() - optionsPressedTime) >= PRESS_DELAY) {
        				currentScreen = OPTIONS;
        				optionsPressed = false;
    				}
				}
				if (!exitPressed) {
    				DrawTexture(exit_button, posx_exit, posy_exit, WHITE);
    				if (CheckCollisionPointRec(mousePoint, exit_buttonRect)) {
        				DrawRectangle(posx_exit + 1, posy_exit + 1,exit_button.width, exit_button.height,Fade(BLACK, 0.2f));
        				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
            					PlaySound(clickSound);	
							}
            				exitPressed = true;
            				exitPressedTime = GetTime();
        					}
   	 					}
				}
				else {
    				DrawTexture(exit_button_pressed, posx_exit + 3, posy_exit + 2, LIGHTGRAY);
    				if ((GetTime() - exitPressedTime) >= PRESS_DELAY) {
        				CloseWindow();
    				}
				}
				break;
			case OPTIONS:
				DrawTexture (background,0,0,WHITE);	
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
				DrawTexture (options_square, GetScreenWidth()/2 - options_square.width/2, GetScreenHeight()/2 - options_square.height/2,WHITE);
				DrawTexture (score_buttom,320,230,WHITE);
				DrawTexture (credits_buttom,320,340,WHITE);
				DrawRectangle (295, 290, 200, 40, (Color){240,230,180,255});
				DrawRectangle (295, 400, 200, 40, (Color){240,230,180,255});
				if (!scoreloaded){
					p = fopen ("C:\\ProjetosRaylib\\assets\\texts_files\\score.txt","r");
					if (p == NULL){
						printf ("Erro na criação na criação do arquivo \"score.txt\"!\n");
					}
					else {
						fread (score,sizeof (char),sizeof (score)-1,p);
						fclose (p);
					}
					scoreloaded = true;
				}
				if (!scoreloaded2){
					p = fopen ("C:\\ProjetosRaylib\\assets\\texts_files\\credits.txt","r");
					if (p == NULL){
						printf ("Erro na criação do arquivo \"credits.txt\"!\n");
					}
					else {
						fread (credits,sizeof (char),sizeof (credits)-1,p);
						fclose (p);
					}
					scoreloaded2 = true;
				}
				DrawText (score,305,290,45,BLACK);
				DrawText (credits,305,400,45,BLACK);
				Vector2 mousePoint2 = GetMousePosition();
				if (!exitOptionsPressed) {
	    			DrawTexture(exit_button, 560, 103, WHITE);
    				if (CheckCollisionPointRec(mousePoint2, exit_optionsRect)) {
        				DrawRectangle(560, 103, exit_button.width, exit_button.height, Fade(BLACK, 0.2f));
        				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
            					PlaySound(clickSound);	
							}
            				exitOptionsPressed     = true;
            				exitOptionsPressedTime = GetTime();
        				}
    				}
				}
				else {
    				DrawTexture(exit_button_pressed, 563, 105, LIGHTGRAY);
    				if ((GetTime() - exitOptionsPressedTime) >= PRESS_DELAY) {
        				currentScreen = MENU;
        				exitOptionsPressed = false;
    				}
				}
				if (!soundPressedOptions) {
    				if (sound_counter == 0) {
        				DrawTexture(sound_on_buttom, 370, 160, WHITE);
    				} 				
					else {
        				DrawTexture(sound_off_buttom, 370, 160, WHITE);
   					 }
    				if (CheckCollisionPointRec(mousePoint2, sound_buttomRect)) {
        				DrawRectangle(370, 160, sound_on_buttom.width, sound_on_buttom.height, Fade(BLACK, 0.2f));
						if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
								PlaySound(clickSound);
							}
            				soundPressedOptions      = true;
            				soundPressedTimeOptions  = GetTime();
       					}
   		 			}
				}
				else {
    				if (sound_counter == 0) {
        				DrawTexture(sound_on_buttom_pressed, 373, 162, LIGHTGRAY);
    				} 
					else {
     	   				DrawTexture(sound_off_buttom_pressed, 373, 162, LIGHTGRAY);
    				}
    				if ((GetTime() - soundPressedTimeOptions) >= PRESS_DELAY) {
        				if (sound_counter == 0) {
            				PauseMusicStream(mario_menu);
            				sound_counter = 1;
        				} 
						else {
            				ResumeMusicStream(mario_menu);
            				sound_counter = 0;
        				}
        				soundPressedOptions = false;
    				}
				}
				break;
			case START:
				if (pos_mario_automatico.x < 265) {
        			pos_mario_automatico.x += speed_mario_automatico * dt;
        			if (pos_mario_automatico.x > 265){
        				pos_mario_automatico.x = 265;	
					}
    			}
    			else {
    				if (pos_mario_automatico.y > 120) {
        				pos_mario_automatico.y -= speed_mario_automatico * dt;
        				if (pos_mario_automatico.y < 120){
        					pos_mario_automatico.y = 120;
							currentScreen = LOADING_LEVEL;	
						}
    				}
				}
				DrawTexture (background,0,0,WHITE);
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
				DrawTexture (mapa_do_jogo, GetScreenWidth()/2 - mapa_do_jogo.width/2, (GetScreenHeight()/2 - mapa_do_jogo.height/2),WHITE);
				DrawTexture (mario_de_lado,(int)pos_mario_automatico.x,(int)pos_mario_automatico.y,WHITE);
				DrawTexture (exit_button,650,80,WHITE);
				
				Vector2 mousePoint3 = GetMousePosition();
				if (!exitMapPressed) {
    				DrawTexture(exit_button, 650, 80, WHITE);
    				if (CheckCollisionPointRec(mousePoint3, exit_mapRect)) {
        				DrawRectangle(650, 80, exit_button.width, exit_button.height, Fade(BLACK, 0.2f));
        				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
           					if (sound_counter == 0) {
           						PlaySound(clickSound);	
							}            						
							exitMapPressed     = true;
            				exitMapPressedTime = GetTime();
        				}
    				}
				}
				else {
    				DrawTexture(exit_button_pressed, 653, 82, LIGHTGRAY);
   					if ((GetTime() - exitMapPressedTime) >= PRESS_DELAY) {
       					currentScreen      = MENU;
       					exitMapPressed     = false;
   					}					
				}
    			break;
    		case LOADING_LEVEL:
    			PauseMusicStream(mario_menu);
    			framecounter2++;
    			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    			DrawText("LEVEL 1", 300, 200, 50, WHITE);
    			DrawRectangleLines(GetScreenWidth()/2 - 500/2 - 4, 296, 509, 40, WHITE);
    			DrawRectangle(GetScreenWidth()/2 - 500/2, 300, 98, 30, Fade(WHITE, 0.3f));
    			DrawRectangle(GetScreenWidth()/2 - 500/2 + 100, 300, 98, 30, Fade(WHITE, 0.3f));
    			DrawRectangle(GetScreenWidth()/2 - 500/2 + 200, 300, 98, 30, Fade(WHITE, 0.3f));
    			DrawRectangle(GetScreenWidth()/2 - 500/2 + 300, 300, 98, 30, Fade(WHITE, 0.3f));
    			DrawRectangle(GetScreenWidth()/2 - 500/2 + 400, 300, 98, 30, Fade(WHITE, 0.3f));
    
    			int blocks = framecounter2 / 50;
    			for (int i = 1; i <= blocks && i <= 5; i++) {
    			    DrawRectangle(GetScreenWidth()/2 - 500/2 + (i-1)*100,300, 98, 30, WHITE);
    			}
    
    			if (framecounter2 >= 350 && !enterSoundPlayed) {
    				if (sound_counter == 0){
    					PlaySound(entering_level);
					}
    	    		enterSoundPlayed = true;
   				}
    
    			if (enterSoundPlayed && !levelMusicStarted) {
    	  			if (!IsSoundPlaying(entering_level)) {
    	        		if (sound_counter == 0){
    	        			PlayMusicStream(level_1);	
						}
    	        		levelMusicStarted = true;
    	        		currentScreen     = LEVEL1;
    	   			}
    			}
				break;
   			case LEVEL1:
   				UpdateMusicStream (level_1);
   				if (!IsMusicStreamPlaying (level_1) && sound_counter == 0){
   					PlayMusicStream (level_1);	
				}
				
   				if (IsKeyPressed (KEY_P) || IsKeyPressed (KEY_ESCAPE)){
   					currentScreen = OPTIONS_LEVEL;	
				}       
				
        		DrawGameScene(scene); // Função da biblioteca Scene para desenhar o cenário
        		DrawCoins(); // Desenha moedas
        		DrawBlocks(physPlatforms, block1);
        		DrawMario(&Mario); // Atualiza a posição do desenho do Mario	
   				break;
   			case OPTIONS_LEVEL:
   				UpdateMusicStream (level_1);
   				DrawTexture (background,0,0,WHITE);	
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
				DrawTexture (options_square, GetScreenWidth()/2 - options_square.width/2, GetScreenHeight()/2 - options_square.height/2,WHITE);
				Vector2 mousePoint4 = GetMousePosition();
				if (!exitOptionsPressed) {
	    			DrawTexture(exit_button, 560, 103, WHITE);
    				if (CheckCollisionPointRec(mousePoint4, exit_optionsRect2)) {
        				DrawRectangle(560, 103, exit_button.width, exit_button.height, Fade(BLACK, 0.2f));
        				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
            					PlaySound(clickSound);	
							}
            				exitOptionsPressed     = true;
            				exitOptionsPressedTime = GetTime();
        				}
    				}
				}
				else {
    				DrawTexture(exit_button_pressed, 563, 105, LIGHTGRAY);
    				if ((GetTime() - exitOptionsPressedTime) >= PRESS_DELAY) {
        				currentScreen = LEVEL1;
        				exitOptionsPressed = false;
    				}
				}
				if (!soundPressedOptions) {
    				if (sound_counter == 0) {
        				DrawTexture(sound_on_buttom, 370, 220, WHITE);
    				} 				
					else {
        				DrawTexture(sound_off_buttom, 370, 220, WHITE);
   					 }
    				if (CheckCollisionPointRec(mousePoint4, sound_buttomRect2)) {
        				DrawRectangle(370, 220, sound_on_buttom.width, sound_on_buttom.height, Fade(BLACK, 0.2f));
						if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
								PlaySound(clickSound);
							}
            				soundPressedOptions      = true;
            				soundPressedTimeOptions  = GetTime();
       					}
   		 			}
				}
				else {
    				if (sound_counter == 0) {
        				DrawTexture(sound_on_buttom_pressed, 373, 222, LIGHTGRAY);
    				} 
					else {
     	   				DrawTexture(sound_off_buttom_pressed, 373, 222, LIGHTGRAY);
    				}
    				if ((GetTime() - soundPressedTimeOptions) >= PRESS_DELAY) {
        				if (sound_counter == 0) {
            				PauseMusicStream(level_1);
            				sound_counter = 1;
        				} 
						else {
            				ResumeMusicStream(level_1);
            				sound_counter = 0;
        				}
        				soundPressedOptions = false;
    				}
				}
				if (!quitOptionsPressed) {
	    			DrawTexture (quit_buttom,320,300,WHITE);
    				if (CheckCollisionPointRec(mousePoint4, quit_buttomRect)) {
        				DrawRectangle(320, 300, quit_buttom.width, quit_buttom.height, Fade(BLACK, 0.2f));
        				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            				if (sound_counter == 0) {
            					PlaySound(clickSound);	
							}
            				quitOptionsPressed     = true;
            				quitOptionsPressedTime = GetTime();
        				}
    				}
				}
				else {
    				DrawTexture (quit_buttom_pressed,320,300,LIGHTGRAY);
    				if ((GetTime() - quitOptionsPressedTime) >= PRESS_DELAY) {
        				currentScreen = EXITLEVEL;
        				quitOptionsPressed = false;
    				}
				}
				break;
				case EXITLEVEL:
					PauseMusicStream(level_1);
    				framecounter3++;
    				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    				DrawText("EXIT LEVEL", 250, 200, 50, WHITE);
    				DrawRectangleLines(GetScreenWidth()/2 - 500/2 - 4, 296, 509, 40, WHITE);
    				DrawRectangle(GetScreenWidth()/2 - 500/2, 300, 98, 30, Fade(WHITE, 0.3f));
    				DrawRectangle(GetScreenWidth()/2 - 500/2 + 100, 300, 98, 30, Fade(WHITE, 0.3f));
    				DrawRectangle(GetScreenWidth()/2 - 500/2 + 200, 300, 98, 30, Fade(WHITE, 0.3f));
    				DrawRectangle(GetScreenWidth()/2 - 500/2 + 300, 300, 98, 30, Fade(WHITE, 0.3f));
    				DrawRectangle(GetScreenWidth()/2 - 500/2 + 400, 300, 98, 30, Fade(WHITE, 0.3f));
    
    				int blocks2 = framecounter3 / 50;
    				for (int i = 1; i <= blocks2 && i <= 5; i++) {
    				    DrawRectangle(GetScreenWidth()/2 - 500/2 + (i-1)*100,300, 98, 30, WHITE);
    				}
    	
	    			if (framecounter3 >= 350) {
    		        	currentScreen     = MENU;
    		   		}
					break;
		}
		EndMode2D();
		EndDrawing();
	}
	
	UnloadTexture (quit_buttom_pressed);
	UnloadTexture (quit_buttom);
	UnloadTexture (sound_off_buttom_pressed);
	UnloadTexture (sound_off_buttom);
	UnloadTexture (sound_on_buttom_pressed);
	UnloadTexture (sound_on_buttom);	
	UnloadTexture (score_buttom);	
	UnloadTexture (credits_buttom);
	UnloadTexture (mario_de_lado);
	UnloadTexture (mapa_do_jogo);
	UnloadTexture (exit_button_pressed);
	UnloadTexture (exit_button);
	UnloadTexture (options_square);
	UnloadTexture (options_button_pressed);
	UnloadTexture (options_button);
	UnloadTexture (start_button_pressed);
	UnloadTexture (start_button);
	UnloadTexture (logo);
	UnloadTexture (trivial_games);
	
	UnloadGameScene(scene); // Função da biblioteca Scene para liberar os recursos alocados dinamicamente
	
	UnloadSound (entering_level);
	UnloadSound (clickSound);
	StopMusicStream (mario_menu);
	UnloadMusicStream (level_1);
	UnloadMusicStream (mario_menu);
	CloseAudioDevice ();
	
	CloseWindow ();
	
	return 0;	
}
