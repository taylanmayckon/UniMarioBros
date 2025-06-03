#include <stdio.h>
#include "raylib.h"
#include "mario.h"

// Inicializando Mario_t
Mario_t Mario;

// Variavel de tempo para testes
float timer = 0.0f;

int main(void){
    InitWindow(800, 450, "UniMarioBros");

    SetTargetFPS(60);

    InitMario(&Mario); // Inicializando as structs do Mario com valores
    Mario.actualState = ACTION_WALKING;
    
    while (!WindowShouldClose()) {
        // Estrutura basica pra testar troca de animação
        if(timer<=5.0f){
           timer += GetFrameTime();
           Mario.powerUpState = STATE_SMALL;
        }
        else if(timer<=10.0f){
            timer += GetFrameTime();
            Mario.powerUpState = STATE_SUPER;
            //Mario.facingRight = !Mario.facingRight;
        }
        else if(timer<=15.0f){
            timer += GetFrameTime();
            Mario.powerUpState = STATE_FIRE;
        }
        else{
            timer = 0.0f;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMario(&Mario);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
