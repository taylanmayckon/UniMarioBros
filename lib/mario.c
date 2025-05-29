#include <stdio.h>
#include "raylib.h"
#include "mario.h"
#include "mario_animdb.h"


// (NORMAL MARIO) definições para as sprites dele andando (190x18 total -> 19,0x18/frame)
#define NORMAL_WALK_FRAME_WIDHT_CUT 19.0f
#define NORMAL_WALK_FRAME_HEIGHT_CUT 18.0f

// (SUPER MARIO) definições para as sprites dele andando (228x34 total -> 19,0x34/frame)
#define SUPER_WALK_FRAME_WIDHT_CUT 19.0f
#define SUPER_WALK_FRAME_HEIGHT_CUT 34.0f

// Função para inicializar as sprites
void InitSprite(MarioSprite_t *sprite, Texture2D *texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int frameCount, int currentFrame){
    sprite->spriteSheet = *texture; // Carregando a sprite já renderizada
    sprite->sourceRec = original_frame_pos_scale; // Como o sheet será cortado
    sprite->frameSpeed = frameSpeed; // Tempo com que os frames serão alterados
    sprite->frameTimer = frameTimer; // Contador de tempo atual do frame
    sprite->frameCount = frameCount; // Total de frames
    sprite->currentFrame = currentFrame; // Frame atual
    sprite->revertAnim = false; // Inicia com a animação no sentido normal
}

// Função para inicializar a struct Mario
void InitMario(Mario_t *Mario){
    printf("[InitMario] Running\n");

    Mario->position = (Vector2){300.0f, 300.0f}; // Posição inicial
    Mario->speed = (Vector2){0.0f, 0.0f}; // Inicia em repouso (vx, vy = 0)
    Mario->invincible = false; // Inicia "vencível"
    Mario->facingRight = true; // Virado para direita
    Mario->canJump = true; // Consegue pular
    Mario->powerUpState = STATE_BIG; // Estado do mario (normal, super,)
    Mario->actualState = ACTION_SLIDE; // Parado
    Mario->lives=3; // Contador de vidas
    Mario->score=0; // Pontuação
    Mario->coins=0; // Quant. de moedas

    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Renderizando os arquivos das sprites
    // PS: na notação [x:y], y é inclusivo 
    // -> Super Mario
    Texture2D superMarioWalking = LoadTexture("assets/textures/mario/supermario_walk.png");
    // No que foi renderizado contém:
    // [0] Agachado para esquerda
    // [1] Pulando para esquerda
    // [2] Parando de correr, direção esquerda
    // [3:5] Andando para esquerda
    // [5] Parado virado para esquerda
    // [6] Parado virado para direita
    // [6:8] Andando para direita
    // [9] Parando de correr, direção direita
    // [10] Pulando para direita
    // [11] Agachado para direita
    
    

    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Iniciando cada sprite da struct de animações
    // (MARIO NORMAL) =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    // (SUPER MARIO) =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // PS: é o gigante
     InitSprite(&Mario->animations.superWalkAnim, // Ponteiro para a sprite 
        &superMarioWalking, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f,// Posição da origem dos frames
                    SUPER_WALK_FRAME_WIDHT_CUT, SUPER_WALK_FRAME_HEIGHT_CUT}, // (ESCALA) Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        12, // Quantidade total de frames do sheet
        6); // Frame atual


    // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Pegando os dados do "Banco de dados" de animações
    //Mario->animations.smallMarioAnimDB = InitSmallMarioDB();
    //Mario->animations.superMarioAnimDB = InitSuperMarioDB();
    //Mario->animations.fireMarioAnimDB = InitFireMarioDB();
    
    printf("[InitMario] Finished\n");
}

// Função para mover a sprite de lugar e desenhá-la
void ChangeMarioSpritePosition(Mario_t *Mario, MarioSprite_t *sprite, float width, float height, float width_cut){
    
    // Posição
    sprite->destRec.x = Mario->position.x;
    sprite->destRec.y = Mario->position.y;
    // Proporção, tem que ser ajustada corretamente depois
    sprite->destRec.width = width;
    sprite->destRec.height = height;

    // Alterna o frame da sprite com base no contador
    sprite->sourceRec.x = (float)(sprite->currentFrame*width_cut); // Tem que passar como parâmetro pra função, pode variar de uma sprite para outra
    //printf("[ChangeMarioSpritePosition] SourceRec.x: %.2f\n", sprite->sourceRec.x); // Depuração
    Vector2 origin = {0, 0}; // Ponto de origem para rotação/escala (mantendo só porque, vai que né)
    
    // Finalmente desenhando
    DrawTexturePro(
        sprite->spriteSheet, // A sprite inteira (todos frames)
        sprite->sourceRec, // Como vai cortar a sprite
        sprite->destRec, // Local e tamanho na tela
        origin, // Ponto em que vai rotacionar
        0.0f, // Rotação em graus
        WHITE // Filtro (WHITE = sem alteração)
    );
}

// Função para CONTROLAR a temporização dos frames das sprites
void ChangeSpriteTimer(Mario_t *Mario, MarioSprite_t *sprite, int first_frame, int end_frame){ // first_frame e end_frame são o intervalo da animação
    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    sprite->frameTimer += GetFrameTime();
    // Depuração
    //printf("[ChangeSpriteTimer] Timer: %.2f, Frame Atual:%d\n", sprite->frameTimer, sprite->currentFrame);
    //printf("[ChangeSpriteTimer] GetFrameTime(): %f\n", GetFrameTime());

    if(sprite->frameTimer >= sprite->frameSpeed){ // Verificando se tá na hora de alterar o frame da animação
        sprite->frameTimer = 0; // Reinicia o cronometro

        // Movendo para DIREITA
        if(Mario->facingRight){ 
            // Incrementa (ciclo normal da animação)
            if(!sprite->revertAnim){
                sprite->currentFrame++; // Avança para o próximo frmae
                // Restringindo as animações para valores permitidos
                if(sprite->currentFrame == end_frame){ 
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            // Decrementa (ciclo inverso para suavizar animação)
            else{
                sprite->currentFrame--;
                // Restrigindo as animações para vlaores permitidos
                if(sprite->currentFrame == first_frame){
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
        }


        // Movendo para ESQUERDA
        else{
            // Decrementa (ciclo normal da animação)
            if(!sprite->revertAnim){
                sprite->currentFrame--; 
                // Restrigindo as animações para valores permitidos
                if(sprite->currentFrame == first_frame){
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            // Incrementa (ciclo inverso para suavizar animação)
            else{
                sprite->currentFrame++;
                // Restrigindo as animações para valores permitidos
                if(sprite->currentFrame == end_frame){
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            
        }
        
    }
}

// Animações do Mario parado
void IdleMario(Mario_t *Mario){
    printf("[IdleMario] Running\n");

    switch(Mario->powerUpState){
        // Normal
        case STATE_SMALL:

            break;
        
        // Super mario
        case STATE_BIG:
            if(Mario->facingRight){ // Virado para direita
                Mario->animations.superWalkAnim.currentFrame = 6; // Index dele virado para direita no sheet
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            } // Virado para esquerda
            else{
                Mario->animations.superWalkAnim.currentFrame = 5; // Index dele virado para direita no sheet
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            }
            break;
        
        case STATE_FIRE:

            break;
        default:
            printf("[IdleMario] Erro: Nenhum estado válido");
            break;
    }
}

// Animações de andar
void WalkingMario(Mario_t *Mario){
    printf("[WalkingMario] Running\n");

    switch(Mario->powerUpState){
        // Normal
        case STATE_SMALL:

            break;
        
        // Super mario
        case STATE_BIG:
            if(Mario->facingRight){ // Virado para direita
                ChangeSpriteTimer(Mario, &Mario->animations.superWalkAnim, 6, 8);
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            } // Virado para esquerda
            else{
                ChangeSpriteTimer(Mario, &Mario->animations.superWalkAnim, 3, 5);
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            }
            break;
        
        case STATE_FIRE:

            break;
        default:
            printf("[IdleMario] Erro: Nenhum estado válido");
            break;
    }
}

// Animações de pular
void JumpingMario(Mario_t *Mario){
    printf("[JumpingMario] Running\n");

    switch(Mario->powerUpState){
        // Normal
        case STATE_SMALL:

            break;
        
        // Super mario
        case STATE_BIG:
            if(Mario->facingRight){ // Virado para direita
                Mario->animations.superWalkAnim.currentFrame = 10; // Index dele virado para direita no sheet
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            } // Virado para esquerda
            else{
                Mario->animations.superWalkAnim.currentFrame = 1; // Index dele virado para direita no sheet
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            }
            break;
        
        case STATE_FIRE:

            break;
        default:
            printf("[IdleMario] Erro: Nenhum estado válido");
            break;
    }
}

// Animações de deslizar
void SlidingMario(Mario_t *Mario){
    printf("[SlidingMario] Running\n");

    switch(Mario->powerUpState){
        // Normal
        case STATE_SMALL:

            break;
        
        // Super mario
        case STATE_BIG:
            if(Mario->facingRight){ // Virado para direita
                Mario->animations.superWalkAnim.currentFrame = 9; // Index dele virado para direita no sheet
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            } // Virado para esquerda
            else{
                Mario->animations.superWalkAnim.currentFrame = 2; // Index dele virado para direita no sheet
                //                               Ponteiro para a sprite            Proporção que será desenhado                                 Largura do corte no eixo x
                ChangeMarioSpritePosition(Mario, &Mario->animations.superWalkAnim, SUPER_WALK_FRAME_WIDHT_CUT*4, SUPER_WALK_FRAME_HEIGHT_CUT*4, SUPER_WALK_FRAME_WIDHT_CUT);
            }
            break;
        
        case STATE_FIRE:

            break;
        default:
            printf("[IdleMario] Erro: Nenhum estado válido");
            break;
    }
}

// Função para selecionar qual sprite será manipulada e desenhada
void DrawMario(Mario_t *Mario){
    switch(Mario->actualState){
        // Parado
        case ACTION_IDLE:
            IdleMario(Mario);
            break;
        // Andando/correndo
        case ACTION_WALKING:
            WalkingMario(Mario);
            break;
        // Pulando
        case ACTION_JUMPING:
            JumpingMario(Mario);
            break;
        case ACTION_SLIDE:
            SlidingMario(Mario);
            break;
        default:
            printf("[UpdateMarioSprite] ERRO!");
            break;
    }
}

