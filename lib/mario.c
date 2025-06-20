#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "mario.h"
#include "mario_animdb.h"
#include "platform.h"
#include "collisions.h"


// (NORMAL MARIO) definições para as sprites dele andando (192x18 total -> 16x18/frame)
#define NORMAL_MARIO_FRAME_WIDHT_CUT 16.0f
#define NORMAL_MARIO_FRAME_HEIGHT_CUT 16.0f

// (SUPER MARIO) definições para as sprites dele andando (228x34 total -> 16,0x34/frame)
#define SUPER_MARIO_FRAME_WIDHT_CUT 16.0f
#define SUPER_MARIO_FRAME_HEIGHT_CUT 30.0f

// (FIRE MARIO) placeholders, falta alterar
#define FIRE_MARIO_FRAME_WIDHT_CUT 16.0f
#define FIRE_MARIO_FRAME_HEIGHT_CUT 31.0f

// Escala que vai ser desenhado na tela a sprite
#define MARIO_SPRITE_SCALE 3.0f

// Posicao inicial do mario
#define MARIO_START_POSITION (Vector2){260.0f, 300.0f}

// Constantes de Física e Movimento 
#define MARIO_WALK_SPEED 200.0f // Velocidade de caminhada base
#define MARIO_RUN_SPEED 320.0f // Velocidade de corrida
#define MARIO_JUMP_STRENGTH 730.0f // Força inicial do pulo
#define GRAVITY 1300.0f // Aceleração da gravidade (pixels/s^2)
#define MAX_FALL_SPEED 650.0f // Velocidade máxima de queda
#define GROUND_FRICTION_COEFF 0.85f // Coeficiente de atrito com o chão (quanto menor, maior o atrito)
#define AIR_FRICTION_COEFF 0.98f // Coeficiente de atrito no ar (2 anos de aero, se tá no ar tem arrasto)
#define SLIDE_DECELERATION 450.0f // Desaceleração ao deslizar (pixels/s^2)
#define STOP_SPEED_THRESHOLD 10.0f  // Abaixo desta Vy, considera-se parado
#define GROUND_Y 450.0f // Placeholder de chão só para testes (tem que pegar isso do código de colisões)


// Retangulo de colisão do Mario
void MarioHitbox(Mario_t *Mario){
    MarioSprite_t *sprite = Mario->assets.activeSprite; // Sprite ativa
    float width  = sprite->frameWidthCut * MARIO_SPRITE_SCALE; // Largura
    float height = sprite->frameHeightCut * MARIO_SPRITE_SCALE; // Altura

    float x = Mario->position.x - (width / 2.0f); // X do retângulo
    float y = Mario->position.y - height; // Y do retângulo

    Mario->hitbox = (Rectangle){
        x + 12.0f,  // Ajuste X
        y, // Ajuste Y
        width - 24.0f, // Ajuste largura
        height + 2.0f  // Ajuste altura 
    };
}

// Funcao para resetar o Mario quando morre 
void ResetMario(Mario_t *Mario){
    Mario->position = MARIO_START_POSITION; // Posição inicial
    Mario->speed = (Vector2){0.0f, 0.0f}; // Inicia em repouso (vx, vy = 0)
    Mario->invincible = false; // Inicia "vencível"
    Mario->facingRight = true; // Virado para direita
    Mario->powerUpState = STATE_SMALL; // Estado do mario (normal, super,)
    Mario->actualState = ACTION_IDLE; // Parado
    Mario->canMove = true; // Booleano para indicar se pode se mover ou não 
    Mario->canJump = false; // Inicia bloquenado pulo
    Mario->deadStarted = false; // Indica que nao comecou a morrer ainda
    Mario->deadSoundTimer = 0.0f; // Timer do som de morte
    Mario->deadSoundPlayed = false; // Reseta flag se tocou som de morte
}


// Animação de morte do Mario
void deathAnim(Mario_t *Mario, int death_frame){ 
    MarioSprite_t *sprite = Mario->assets.activeSprite; // Pegando o endereço da sprite de morte
    float dt = GetFrameTime();
    Mario->assets.activeSprite->currentFrame = death_frame;

    // Gravidade de morte (a mesma do movimento de pulo, mas sem colisao)
    Mario->speed.y += GRAVITY * dt;
    if (Mario->speed.y > MAX_FALL_SPEED) {
        Mario->speed.y = MAX_FALL_SPEED;
    }
    // -> Movimento e colisao no Eixo Y
    Mario->position.y += Mario->speed.y * dt;
}


// Pega inputs do teclado, processa a física e gera outputs para outras libs
void UpdateMario(Mario_t *Mario, PhysPlatform_t *physPlatforms, int physPlatCount, Sound bumpSound) {
    // Considera que está morto sempre que some da tela
    if(Mario->position.y > 700.0f){ // O fim da tela + uns quebrados
        // Se o Mario caiu, ele está no processo de morrer
        Mario->actualState = ACTION_DYING; 
        
        if (!Mario->deadSoundPlayed) {
            PlaySound(Mario->assets.deathSound);
            Mario->deadSoundPlayed = true;
            Mario->deadSoundTimer = 0.0f;
        }
        
        Mario->deadSoundTimer += GetFrameTime();

        if (Mario->deadSoundTimer >= 4.0f) {
            Mario->lives--;
            Mario->actualState = ACTION_DEAD_ALREADY; // AGORA sim, ele está pronto para o reset
        }
        return; // Retorna para não processar mais nada
    }

    // Sinaliza que tem que travar o movimento na morte
    if(Mario->actualState == ACTION_DYING){
        if(!Mario->deadStarted){
            Mario->speed.y = -MARIO_JUMP_STRENGTH; // Aplica o impulso inicial da morte
            Mario->deadStarted = true; // Sinaliza que comecou animacao de morte agora
            // Verifica se pode tocar o som e sinaliza que ja tocou o mesmo
            if(!Mario->deadSoundPlayed){
                PlaySound(Mario->assets.deathSound);
                Mario->deadSoundPlayed = true;
                Mario->deadSoundTimer = 0.0f;
            }
        }
        Mario->canMove = false;
    }

    // Trava o movimento do mario quando necessario (tipo morrer ou pegar cogumelo/flor), UpdateMario retorna já aqui
    if(!Mario->canMove){
        Mario->speed.x = 0; // Para garantir que não deslize
        return;
    }

    float dt = GetFrameTime(); // Para o "Cálculo de integral" com o intervalo de tempo entre os frames sendo o dt
    bool isOnGround = false; // Assume que inicialmente o Mario est[a no ar no inicio de cada frame

    // Leitura de inputs do teclado
    bool wantsToMoveLeft = IsKeyDown(KEY_LEFT);
    bool wantsToMoveRight = IsKeyDown(KEY_RIGHT);
    bool wantsToCrouch = IsKeyDown(KEY_DOWN);
    bool isTryingToRun = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT); // Detecta corrida (tá no shift)

    float currentMaxMoveSpeed = isTryingToRun ? MARIO_RUN_SPEED : MARIO_WALK_SPEED; // Seleciona qual velocidade ira usar (andando/correndo)    

    // -> Logica de estado e Velocidade Horizontal (Vx)
    // Agachar (para Super e Fire Mario, no chao)
    if(wantsToCrouch && Mario->powerUpState != STATE_SMALL && Mario->actualState!=ACTION_JUMPING && Mario->canJump){
        Mario->actualState = ACTION_CROUCH;
        Mario->speed.x = 0;
    }
    // Se nao quiser agachar, limpa estado agachado e atribui Vx correspondente
    else{
        // Levanta se estava agachado
        if(Mario->actualState == ACTION_CROUCH) Mario->actualState = ACTION_IDLE; 

        // Movimento horizontal
        if(wantsToMoveLeft && !wantsToMoveRight){ // Esquerda
            Mario->speed.x = -currentMaxMoveSpeed;
            Mario->facingRight = false;
            if (Mario->actualState != ACTION_JUMPING) Mario->actualState = ACTION_WALKING; // Troca para anim de andar, caso nao esteja pulando
        }
        else if(!wantsToMoveLeft && wantsToMoveRight){ // Direita
            Mario->speed.x = currentMaxMoveSpeed;
            Mario->facingRight = true;
            if (Mario->actualState != ACTION_JUMPING) Mario->actualState = ACTION_WALKING; // Troca para anim de andar, caso nao esteja pulando
        }
        // Atrito e desaceleracao 
        else{
            // Atrito do chao, caso esteja andando
            if(Mario->canJump){ 
                // Aplica atrito do chão
                Mario->speed.x *= powf(GROUND_FRICTION_COEFF, dt * 60.0f);
            }
            else{
                // Arrasto do ar, caso esteja pulando
                Mario->speed.x *= powf(AIR_FRICTION_COEFF, dt * 60.0f);
            }
            // Para instantaneamente se Vx for muito baixa
            if(fabsf(Mario->speed.x) < STOP_SPEED_THRESHOLD){
                Mario->speed.x = 0.0f;
                if(Mario->actualState == ACTION_WALKING){
                    Mario->actualState = ACTION_IDLE;
                }
            }

        }
    }


    // -> Movimento e colisao no eixo X
    Mario->position.x += Mario->speed.x * dt;
    MarioHitbox(Mario);
    CheckMarioHitboxX(Mario, physPlatforms);


    // -> Pulo e fisica da gravidade
    // Pulo
    if (IsKeyPressed(KEY_SPACE) && Mario->canJump && Mario->actualState != ACTION_CROUCH) {
        Mario->speed.y = -MARIO_JUMP_STRENGTH;
        Mario->actualState = ACTION_JUMPING;
        Mario->canJump = false; // Só pode pular de novo quando tocar o chão
    }
    // Aplica a gravidade
    Mario->speed.y += GRAVITY * dt;
    if (Mario->speed.y > MAX_FALL_SPEED) {
        Mario->speed.y = MAX_FALL_SPEED;
    }


    // -> Movimento e colisao no Eixo Y
    Mario->position.y += Mario->speed.y * dt;
    MarioHitbox(Mario);
    Mario->canJump = false;
    // Checa e corrige a colisao e hitbox do mario
    CheckMarioHitboxY(Mario, physPlatforms, &isOnGround, bumpSound);


    // -> Ajustes finais de estado
    // Se não colidiu com nada embaixo, está caindo
    if (!isOnGround && Mario->actualState != ACTION_JUMPING) {
         Mario->actualState = ACTION_JUMPING;
    }

    // Lógica do FPS da animação de corrida
    if(isTryingToRun){
        Mario->assets.activeSprite->frameSpeed=0.06f;
    } else {
        Mario->assets.activeSprite->frameSpeed=0.1f;
    }

    // Se estiver no chão e parado, garante estado IDLE
    if (isOnGround && Mario->speed.x == 0 && Mario->actualState != ACTION_CROUCH) {
        Mario->actualState = ACTION_IDLE;
    }
}


// Função para inicializar a sprite do Mario com os parâmetros para o Small Mario parado
void InitSprite(MarioSprite_t *sprite, Texture2D texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int currentFrame){
    sprite->spriteSheet = texture; // Carregando a sprite já renderizada
    sprite->sourceRec = original_frame_pos_scale; // Como o sheet será cortado
    sprite->frameSpeed = frameSpeed; // Tempo com que os frames serão alterados
    sprite->frameTimer = frameTimer; // Contador de tempo atual do frame
    sprite->currentFrame = currentFrame; // Frame atual
    sprite->frameWidthCut = original_frame_pos_scale.width;
    sprite->frameHeightCut = original_frame_pos_scale.height;
}

// Função para inicializar a struct Mario
void InitMario(Mario_t *Mario){
    printf("[InitMario] Running\n");

    Mario->position = MARIO_START_POSITION; // Posição inicial
    Mario->speed = (Vector2){0.0f, 0.0f}; // Inicia em repouso (vx, vy = 0)
    Mario->invincible = false; // Inicia "vencível"
    Mario->facingRight = true; // Virado para direita
    Mario->powerUpState = STATE_SMALL; // Estado do mario (normal, super,)
    Mario->actualState = ACTION_IDLE; // Parado
    Mario->lives=3; // Contador de vidas
    Mario->score=0; // Pontuação
    Mario->coins=0; // Quant. de moedas
    Mario->canMove = true; // Booleano para indicar se pode se mover ou não 
    Mario->canJump = false; // Inicia bloquenado pulo
    Mario->deadStarted = false; // Indica que nao comecou a morrer ainda
    Mario->deadSoundTimer = 0.0f; // Timer do som de morte

    // Renderizando os arquivos das sprites
    Mario->assets.superMarioSheet = LoadTexture("assets/textures/mario/supermario.png");
    Mario->assets.smallMarioSheet = LoadTexture("assets/textures/mario/smallmario.png");
    Mario->assets.fireMarioSheet = LoadTexture("assets/textures/mario/firemario.png");

    // Pegando os dados do "Banco de dados" de animações
    Mario->assets.smallMarioAnimDB = InitSmallMarioDB();
    Mario->assets.superMarioAnimDB = InitSuperMarioDB();
    Mario->assets.fireMarioAnimDB = InitFireMarioDB();

    // Iniciando as sprites
    // -> Mario Normal
    InitSprite(
        &Mario->assets.smallMarioSprite, // Ponteiro para a sprite 
        Mario->assets.smallMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    NORMAL_MARIO_FRAME_WIDHT_CUT, NORMAL_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        7); // Frame atual
    // -> Super Mario
    InitSprite(
        &Mario->assets.superMarioSprite, // Ponteiro para a sprite 
        Mario->assets.superMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    SUPER_MARIO_FRAME_WIDHT_CUT, SUPER_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6); // Frame atual
    // -> Fire Mario (placeholder atualmente)
    InitSprite(
        &Mario->assets.fireMarioSprite, // Ponteiro para a sprite 
        Mario->assets.fireMarioSheet, // Ponteiro para a textura carregada
        (Rectangle){0.0f, 0.0f, // Posição da origem dos frames
                    FIRE_MARIO_FRAME_WIDHT_CUT, FIRE_MARIO_FRAME_HEIGHT_CUT}, // Largura e altura de cada frame
        0.1f, // Tempo de cada frame
        0.0f, // Cronometro para cada frame 
        6); // Frame atual

    // -> Iniciando com a sprite do mario normal
    Mario->assets.activeSprite = &Mario->assets.smallMarioSprite;

    // Carregando o audio da morte
    Mario->assets.deathSound = LoadSound("assets/audio/mario_death.mp3");
    
    printf("[InitMario] Finished\n");
}

// Função para mover a sprite de lugar e desenhá-la
void ChangeMarioSpritePosition(Mario_t *Mario, float width_scale, float height_scale){
    MarioSprite_t *sprite = Mario->assets.activeSprite;
    
    // Posição
    // Como o frame começa a ser desenhado no canto superior esquerdo, precisa de offsets
    // (LEMBRA DE ALINHAR COM O CÓDIGO DE COLISÕES)
    float offset_x = width_scale; 
    float offset_y = height_scale;
    // Aplicando o desenho com offset
    sprite->destRec.x = Mario->position.x - offset_x;
    sprite->destRec.y = Mario->position.y - offset_y;
    // Proporção, tem que ser ajustada corretamente depois
    sprite->destRec.width = width_scale;
    sprite->destRec.height = height_scale;

    // Alterna o frame da sprite com base no contador
    sprite->sourceRec.x = (float)(sprite->currentFrame * sprite->frameWidthCut); // Tem que passar como parâmetro pra função, pode variar de uma sprite para outra
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

// Função auxiliar para não permitir um index fora do range na troca de animações
void ConstrainIndex(Mario_t *Mario, FrameRange_t range){
    MarioSprite_t *sprite = Mario->assets.activeSprite;

    if(sprite->currentFrame < range.start){
        sprite->currentFrame = range.start;
    }
    else if(sprite->currentFrame > range.end){
        sprite->currentFrame = range.end;
    }
}

// Função para CONTROLAR a temporização dos frames das sprites
void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range){ // first_frame e end_frame são o intervalo da animação    
    MarioSprite_t *sprite = Mario->assets.activeSprite;

    // Corrigindo o range caso esteja fora do intervalo
    ConstrainIndex(Mario, range);

    // Acumula o tempo que passou desde o ultimo frame
    // Na pratica é um cronometro
    sprite->frameTimer += GetFrameTime();
    if(sprite->frameTimer >= sprite->frameSpeed){ // Verificando se tá na hora de alterar o frame da animação
        sprite->frameTimer = 0; // Reinicia o cronometro

        // Movendo para DIREITA
        if(Mario->facingRight){ 
            sprite->currentFrame++;
            if(sprite->currentFrame>range.end){
                sprite->currentFrame=range.start;
            }
        }
        // Movendo para ESQUERDA
        else{
            sprite->currentFrame--;
            if(sprite->currentFrame<range.start){
                sprite->currentFrame=range.end;
            }
            
        }
        
    }
}


// Função para selecionar qual sprite será manipulada e desenhada
void DrawMario(Mario_t *Mario){
    // Ponteiros para manipular as infos de animação de Mario
    MarioAnimDB_t *currentAnimDB = NULL; 
    AnimData_t *currentAnimData = NULL; 
    
    // Selecionando o banco de dados das animações, conforme o estado atual do Mario e alterando o endereço para a sprite ativa
    switch(Mario->powerUpState){
        case STATE_SMALL:
            currentAnimDB = &Mario->assets.smallMarioAnimDB; // Atualiza o DB
            Mario->assets.activeSprite = &Mario->assets.smallMarioSprite; // Pega o endereço correto para a sprite do estado atual
            break;
        case STATE_SUPER:
            currentAnimDB = &Mario->assets.superMarioAnimDB; // Atualiza o DB
            Mario->assets.activeSprite = &Mario->assets.superMarioSprite; // Pega o endereço correto para a sprite do estado atual
            break;
        case STATE_FIRE:
            currentAnimDB = &Mario->assets.fireMarioAnimDB; // Atualiza o DB
            Mario->assets.activeSprite = &Mario->assets.fireMarioSprite; // Pega o endereço correto para a sprite do estado atual
            break;
    }

    // Selecionando os dados da animação atual
    switch (Mario->actualState) {
        case ACTION_IDLE:
            currentAnimData = &currentAnimDB->idle;
            break;
        case ACTION_WALKING:
            currentAnimData = &currentAnimDB->walking;
            break;
        case ACTION_JUMPING:
            currentAnimData = &currentAnimDB->jumping;
            break;
        case ACTION_SLIDE:
            currentAnimData = &currentAnimDB->slide;
            break;
        case ACTION_CROUCH:
            currentAnimData = &currentAnimDB->crouch;
            break;
        case ACTION_DYING:
            currentAnimData = &currentAnimDB->death;
            break;
        case ACTION_DEAD_ALREADY:
            // Mario tá morto, nao tem animacao
            break;
    }

    // Retorna se nao tiver animacao para exibir (Mario morto)
    if(currentAnimData == NULL){
        return;
    }

    // Usando os dados selecionados para animar e desenhar
    if(currentAnimData->isLooping){ // Se é animação com mais de 1 frame
        FrameRange_t range;

        // Pegando o range da animação
        if(Mario->facingRight){ // Virado para direita
            range = currentAnimData->rightAnimFrames;
        }
        else{ // Virado para esquerda
            range = currentAnimData->leftAnimFrames;
        }
        ChangeSpriteTimer(Mario, range); // Função que verifica o momento de alterar o frame, e o faz quando chega
    }
    else{ // Se é frame fixo
        if(Mario->facingRight){ // Virado para direita
            Mario->assets.activeSprite->currentFrame = currentAnimData->freezedFrameRight;
        }
        else{ // Esquerda
            Mario->assets.activeSprite->currentFrame = currentAnimData->freezedFrameLeft;
        }
    }

    // Se estiver morrendo, ativa a animacao de morte
    if(Mario->actualState == ACTION_DYING){
        deathAnim(Mario, currentAnimData->freezedFrameLeft); // Atualiza a animação de morte
    }

    // Desenha a sprite na tela
    ChangeMarioSpritePosition(
        Mario,
        Mario->assets.activeSprite->frameWidthCut * MARIO_SPRITE_SCALE,
        Mario->assets.activeSprite->frameHeightCut * MARIO_SPRITE_SCALE);
}


// Descarrega os assets do Mario
void UnloadMario(Mario_t *Mario){
    UnloadTexture(Mario->assets.superMarioSheet);
    UnloadTexture(Mario->assets.smallMarioSheet);
    UnloadTexture(Mario->assets.fireMarioSheet);

    UnloadSound(Mario->assets.deathSound);
}