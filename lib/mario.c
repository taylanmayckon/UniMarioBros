#include "raylib.h"
#include "mario.h"
#include <stdio.h>

// Definições para a sprite andando normal (right) (190x18 total -> 19,0x18/frame)
#define NORMAL_WALK_FRAME_WIDHT_CUT 19.0f
#define NORMAL_WALK_FRAME_HEIGHT_CUT 18.0f

void InitSprite(MarioSprite_t *sprite, Texture2D *texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int frameIdleIndex, int firstFrameIndex, int frameCount, int currentFrame){
    sprite->spriteSheet = *texture; // Carregando a sprite já renderizada
    sprite->sourceRec = original_frame_pos_scale;
    sprite->frameSpeed = frameSpeed;
    sprite->frameTimer = frameTimer;
    sprite->frameIdleIndex = frameIdleIndex;
    sprite->firstFrameIndex = firstFrameIndex;
    sprite->frameCount = frameCount;
    sprite->currentFrame = currentFrame;
    sprite->revertAnim = false; // Inicia com a animação no sentido normal
}

void InitMario(Mario_t *Mario){
    // Função genérica para inicializar a struct Mario
    printf("[InitMario] Running\n");

    Mario->position = (Vector2){300.0f, 300.0f}; // Posição inicial
    Mario->speed = (Vector2){0.0f, 0.0f}; // Inicia em repouso (vx, vy = 0)
    Mario->invincible = false; // Inicia "vencível"
    Mario->facingRight = true; // Virado para direita
    Mario->canJump = true; // Consegue pular
    Mario->actualState = ACTION_IDLE; // Parado
    Mario->lives=3; // Contador de vidas
    Mario->score=0; // Pontuação
    Mario->coins=0; // Quant. de moedas

    // Iniciando cada sprite da struct de animações
    // (MARIO NORMAL) Andando para direita
    Texture2D normalMarioWalking = LoadTexture("assets/textures/mario/mario1.png"); // Carregando uma vez só para direita/esquerda
    InitSprite(&Mario->animations.normalRightWalkAnim, // Ponteiro para a sprite 
        &normalMarioWalking, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f,// Posição da origem dos frames
                    NORMAL_WALK_FRAME_WIDHT_CUT, NORMAL_WALK_FRAME_HEIGHT_CUT}, // (ESCALA) Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6, // Offset do framesheet, quando não começa no index 0 do arquivo png
        6, // Index do primeiro frame
        3, // Quantidade total de frames válidos da animação no sheet
        6); // Frame atual
    // (MARIO NORMAL) Andando para esquerda
    InitSprite(&Mario->animations.normalLeftWalkAnim, // Ponteiro para a sprite 
        &normalMarioWalking, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f,// Posição da origem dos frames
                    NORMAL_WALK_FRAME_WIDHT_CUT, NORMAL_WALK_FRAME_HEIGHT_CUT}, // (ESCALA) Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        3, // Offset do framesheet, quando não começa no index 0 do arquivo png
        1, // Index do primeiro frame
        10, // Quantidade total de frames
        3); // Frame atual
    
    printf("[InitMario] Finished\n");
}

void ChangeMarioSpritePosition(Mario_t *Mario, MarioSprite_t *sprite, float width, float height, float width_cut){
    // Função para mover a sprite de lugar e desenhá-la
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

void ChangeSpriteTimer(Mario_t *Mario, MarioSprite_t *sprite){
    // Função para CONTROLAR a temporização dos frames das sprites
    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    sprite->frameTimer += GetFrameTime();
    // Depuração
    //printf("[ChangeSpriteTimer] Timer: %.2f, Frame Atual:%d\n", sprite->frameTimer, sprite->currentFrame);
    //printf("[ChangeSpriteTimer] GetFrameTime(): %f\n", GetFrameTime());

    // Verificando se tá na hora de alterar o frame da animação
    if(sprite->frameTimer >= sprite->frameSpeed){
        sprite->frameTimer = 0; // Reinicia o cronometro

        // Movendo para DIREITA
        if(Mario->facingRight){ 
            // Incrementa (ciclo normal da animação)
            if(!sprite->revertAnim){
                sprite->currentFrame++; // Avança para o próximo frmae
                // Restringindo as animações para valores permitidos
                if(sprite->currentFrame == (sprite->frameIdleIndex + sprite->frameCount)-1){ 
                    // Tradução: verifica se chegou até o ultimo frame da animação  (o Idle)
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            // Decrementa (ciclo inverso para suavizar animação)
            else{
                sprite->currentFrame--;
                // Restrigindo as animações para vlaores permitidos
                if(sprite->currentFrame == sprite->frameIdleIndex){ // Pra direita
                    // Tradução: verifica se chegou até o primeiro frame da animação (o Idle)
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
                if(sprite->currentFrame == sprite->firstFrameIndex){
                    // Tradução: verifica se chegou até o ultimo frame da animação (o Idle)
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            // Incrementa (ciclo inverso para suavizar animação)
            else{
                sprite->currentFrame++;
                // Restrigindo as animações para valores permitidos
                if(sprite->currentFrame == sprite->frameIdleIndex){
                    // Tradução: verifica se chegou até o ultimo frame da animação (o Idle)
                    sprite->revertAnim = !sprite->revertAnim; // Aciona a inversão para tornar animação suave
                }
            }
            
        }
        
    }
}

void IdleMario(Mario_t *Mario){
    printf("[IdleMario] Running\n");
    // Virado para direita:
    if(Mario->facingRight){
        Mario->animations.normalRightWalkAnim.currentFrame = Mario->animations.normalRightWalkAnim.frameIdleIndex; // INDEX do frame dele parado para DIREITA
        ChangeMarioSpritePosition(Mario, 
            &Mario->animations.normalRightWalkAnim, // Ponteiro para a sprite a ser modificada
            NORMAL_WALK_FRAME_WIDHT_CUT*4, NORMAL_WALK_FRAME_HEIGHT_CUT*4, // Proporção do desenho
            NORMAL_WALK_FRAME_WIDHT_CUT);  // Largura do corte no eixo x
    }
    // Virado para esquerda
    else{
        Mario->animations.normalLeftWalkAnim.currentFrame = Mario->animations.normalLeftWalkAnim.frameIdleIndex; // INDEX do frame dele parado para ESQUERDA
        ChangeMarioSpritePosition(Mario, 
            &Mario->animations.normalLeftWalkAnim, // Ponteiro para a sprite a ser modificada
            NORMAL_WALK_FRAME_WIDHT_CUT*4, NORMAL_WALK_FRAME_HEIGHT_CUT*4, // Proporção do desenho
            NORMAL_WALK_FRAME_WIDHT_CUT);  // Largura do corte no eixo x
    }
}

void WalkingMario(Mario_t *Mario){
    // Virado para direita:
    if(Mario->facingRight){
        ChangeSpriteTimer(Mario, &Mario->animations.normalRightWalkAnim);
        ChangeMarioSpritePosition(Mario, 
            &Mario->animations.normalRightWalkAnim, // Ponteiro para a sprite a ser modificada
            NORMAL_WALK_FRAME_WIDHT_CUT*4, NORMAL_WALK_FRAME_HEIGHT_CUT*4, // Proporção do desenho
            NORMAL_WALK_FRAME_WIDHT_CUT); // Largura do corte no eixo x
    }
    // Virado para esquerda:
    else{
        ChangeSpriteTimer(Mario, &Mario->animations.normalLeftWalkAnim);
        ChangeMarioSpritePosition(Mario, &Mario->animations.normalLeftWalkAnim, // Ponteiro para a sprite a ser modificada
        NORMAL_WALK_FRAME_WIDHT_CUT*4, NORMAL_WALK_FRAME_HEIGHT_CUT*4, // Proporção do desenho
        NORMAL_WALK_FRAME_WIDHT_CUT); // Largura do corte no eixo x
    }
}

void DrawMario(Mario_t *Mario){
    // Função para selecionar qual sprite será manipulada e desenhada

    // Por enquanto tá só para o Mario normal
    switch(Mario->actualState){
        // Caso esteja parado
        case ACTION_IDLE:
            IdleMario(Mario);
            break;
        case ACTION_WALKING:
            WalkingMario(Mario);
            break;
        case ACTION_JUMPING:

            break;
        case ACTION_FALLING:

            break;
        default:
            printf("[UpdateMarioSprite] ERRO!");
            break;
    }
}

