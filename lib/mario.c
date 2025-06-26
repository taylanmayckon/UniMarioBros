#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "mario.h"
#include "mario_animdb.h"
#include "platform.h"
#include "collisions.h"
#include "coin.h"

// -> DIMENSOES DAS SPRITES DO MARIO
// (NORMAL MARIO) 
#define NORMAL_MARIO_FRAME_WIDHT_CUT 16.0f
#define NORMAL_MARIO_FRAME_HEIGHT_CUT 16.0f
// (SUPER MARIO) 
#define SUPER_MARIO_FRAME_WIDHT_CUT 16.0f
#define SUPER_MARIO_FRAME_HEIGHT_CUT 30.0f
// (FIRE MARIO) 
#define FIRE_MARIO_FRAME_WIDHT_CUT 16.0f
#define FIRE_MARIO_FRAME_HEIGHT_CUT 31.0f

// -> DIMENSOES DAS SPRITES DA FLAG
#define FLAG_FRAME_WIDTH_CUT 16.0f
#define FLAG_FRAME_HEIGHT_CUT 16.0f
#define PILAR_FRAME_WIDTH_CUT 16.0f
#define PILAR_FRAME_HEIGHT_CUT 96.0f

// Escala que vai ser desenhado na tela 
#define MARIO_SPRITE_SCALE 3.0f
#define FLAG_SPRITE_SCALE 3.0f
#define PILAR_SPRITE_SCALE 3.0f

// Posicao inicial do mario
#define MARIO_START_POSITION (Vector2){260.0f, 445.0f}
// #define MARIO_START_POSITION (Vector2){3000.0f, 0.0f} // Coords do fim de mapa pra depuraçao
#define MARIO_END_POSITION_X 3515.0f

// Constantes de Física e Movimento 
#define MARIO_GROUND_ACCELERATION 320.0f // Aceleração do Mario no chão (pixels/s^2)
#define MARIO_WALK_SPEED 200.0f // Velocidade de caminhada base
#define MARIO_RUN_SPEED 400.0f // Velocidade de corrida
#define MARIO_JUMP_STRENGTH 750.0f // Força inicial do pulo
#define GRAVITY 1400.0f // Aceleração da gravidade (pixels/s^2)
#define MAX_FALL_SPEED 650.0f // Velocidade máxima de queda
#define WALK_GROUND_FRICTION_COEFF 0.94f // Coeficiente de atrito andando (quanto menor, maior o atrito)
#define SLIDE_GROUND_FRICTION_COEFF 0.87f // Coeficiente de atrito deslizando (quanto menor, maior o atrito)
#define AIR_FRICTION_COEFF 0.98f // Coeficiente de atrito no ar (2 anos de aero, se tá no ar tem arrasto)
#define STOP_SPEED_THRESHOLD 10.0f  // Abaixo desta Vx, considera-se parado
#define MARIO_AIR_ACCELERATION 1600.0f // Aceleração que o jogador pode aplicar no ar (pixels/s^2)
// Constantes para controlar a transicao de animacao de andar/correr
#define ANIM_SPEED_WALK 0.15f 
#define ANIM_SPEED_RUN  0.05f 



// Retangulo de colisão do Mario
void MarioHitbox(Mario_t *Mario){
    MarioSprite_t *sprite = Mario->assets.activeSprite; // Sprite ativa
    float width  = sprite->frameWidthCut * MARIO_SPRITE_SCALE; // Largura
    float height = sprite->frameHeightCut * MARIO_SPRITE_SCALE; // Altura

    float x = Mario->position.x; // X do retângulo
    float y = Mario->position.y - height; // Y do retângulo

    Mario->hitbox = (Rectangle){
        x - (MARIO_SPRITE_SCALE * Mario->assets.activeSprite->frameWidthCut) + 7.0f,  // Ajuste X
        y, // Ajuste Y
        width - 15.0f, // Ajuste largura
        height  // Ajuste altura 
    };
}

// Funcao para resetar o Mario para as configurações de fábrica
void ResetMario(Mario_t *Mario){
    MarioStats_t stats_reset;
    MarioDeadControl_t dead_reset;

    Mario->stats.gameover = false;
    // if(Mario->stats.lives > 0){
    //     Mario->stats.gameover = false;
    // }
        
    if(Mario->stats.lives <= 0){
        Mario->stats.lives = 3;
        Mario->actualState = ACTION_IDLE;
        // Mario->stats.gameover = true;
        return;
    }
    

    stats_reset.canJump = true;
    stats_reset.canMove = true;
    stats_reset.facingRight = true;
    stats_reset.invincible = false;
    stats_reset.isOnCave = false;

    dead_reset.deadStarted = false;
    dead_reset.deadSoundTimer = 0.0f;
    dead_reset.deadSoundPlayed = false;

    
    Mario->position = MARIO_START_POSITION; 
    Mario->speed = (Vector2){0.0f, 0.0f}; 
    Mario->powerUpState = STATE_SMALL; 
    Mario->actualState = ACTION_IDLE; 
    Mario->stats = stats_reset;
    Mario->dead_control = dead_reset;
    Mario->air_control.countAirTimeX = 0.0f;
}


// Animação de morte do Mario
void deathAnim(Mario_t *Mario, int death_frame){ 
    MarioSprite_t *sprite = Mario->assets.activeSprite; 
    float dt = GetFrameTime();
    Mario->assets.activeSprite->currentFrame = death_frame;

    // Gravidade de morte (a mesma do movimento de pulo, mas sem colisao)
    Mario->speed.y += GRAVITY * dt;
    if (Mario->speed.y > MAX_FALL_SPEED) {
        Mario->speed.y = MAX_FALL_SPEED;
    }
    Mario->position.y += Mario->speed.y * dt;
}

// Pega inputs do teclado, processa a física e gera outputs para outras libs
void UpdateMario(Mario_t *Mario, PhysPlatform_t *physPlatforms, int physPlatCount, Sound bumpSound) {
    float dt = GetFrameTime(); 
    bool isOnGround = false; // Assume que inicialmente o Mario est[a no ar no inicio de cada frame
    // printf("(%.2f, %.2f)\n", Mario->position.x, Mario->position.y);


    // Considera que está morto sempre que some da tela sem estar na caverna
    if(Mario->position.y > 700.0f && !Mario->stats.isOnCave && Mario->actualState != ACTION_ENTERING_PIPE){ // O fim da tela + uns quebrados
        Mario->stats.gameover = false;
        Mario->actualState = ACTION_DYING; // Se o Mario caiu, ele está no processo de morrer
        
        if (!Mario->dead_control.deadSoundPlayed) {
            PlaySound(Mario->assets.deathSound);
            Mario->dead_control.deadSoundPlayed = true;
            Mario->dead_control.deadSoundTimer = 0.0f;
        }
        
        Mario->dead_control.deadSoundTimer += GetFrameTime();

        if (Mario->dead_control.deadSoundTimer >= 4.0f) {
            Mario->stats.lives--;
            // Mario->actualState = ACTION_DEAD_ALREADY; // AGORA sim, ele está pronto para o reset
            ResetMario(Mario);
        }
        return; 
    }

    // Sinaliza que tem que travar o movimento na morte
    if(Mario->actualState == ACTION_DYING){
        if(!Mario->dead_control.deadStarted){
            Mario->speed.y = -MARIO_JUMP_STRENGTH; // Aplica o impulso inicial da morte
            Mario->dead_control.deadStarted = true; // Sinaliza que comecou animacao de morte agora
            // Verifica se pode tocar o som e sinaliza que ja tocou o mesmo
            if(!Mario->dead_control.deadSoundPlayed){
                PlaySound(Mario->assets.deathSound);
                Mario->dead_control.deadSoundPlayed = true;
                Mario->dead_control.deadSoundTimer = 0.0f;
            }
        }
        Mario->stats.canMove = false;
    }

    // Animacao de finalizar o jogo
    if(Mario->stats.winningGame){
        Mario->position.x += MARIO_WALK_SPEED*dt/2;
        Mario->speed.y += GRAVITY * dt;
        Mario->position.y += Mario->speed.y * dt;
        MarioHitbox(Mario);
        // Checa e corrige a colisao e hitbox do mario
        CheckMarioHitboxY(Mario, physPlatforms, &isOnGround, bumpSound);
        if(Mario->position.x >= MARIO_END_POSITION_X){
            Mario->stats.winningGame = false;
            Mario->stats.finished = true;
            Mario->actualState=ACTION_IDLE;
        }
    }

    // Isso é pra processar quando pegar cogumelo, morrer, pegar bandeira...
    if(!Mario->stats.canMove){
        Mario->speed.x = 0; // Para o slide
        return;
    }

    // Leitura de inputs do teclado
    bool wantsToMoveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool wantsToMoveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool wantsToCrouch = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    bool isTryingToRun = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT); 

    // Detecta animacao de entrar no cano da caverna
    if(CheckCollisionRecs(Mario->cave_control.entrance, Mario->hitbox) && wantsToCrouch){
        Mario->actualState = ACTION_ENTERING_PIPE;
        Mario->cave_control.previous_position = Mario->position;
    }

    // Tratamento da passagem para fase secreta
    if(Mario->actualState==ACTION_ENTERING_PIPE){
        Mario->position.y += 0.3f; 
        MarioHitbox(Mario);
        if(!CheckCollisionRecs(Mario->cave_control.entrance, Mario->hitbox)){
            Mario->actualState = ACTION_IDLE;
            Mario->stats.facingRight = false;
            Mario->position = Mario->cave_control.cave_coord;
            Mario->stats.isOnCave = true;
        }
        return; // Retorno pra nao processar fisica e afins
    }

    // Detecta se deve voltar para a fase normal
    if(CheckCollisionRecs(Mario->cave_control.exit, Mario->hitbox) && Mario->stats.isOnCave){
        Mario->position = Mario->cave_control.previous_position;
        Mario->stats.isOnCave = false;
        // Volta pulando
        Mario->stats.canJump = false;
        Mario->speed.y = -MARIO_JUMP_STRENGTH;
        Mario->speed.x = 0.0f;
        Mario->actualState = ACTION_JUMPING;
    }

    // Modifica pra fazer aumento gradual de velocidade de andar até correr na vel max
    float currentMoveSpeed = isTryingToRun ? MARIO_RUN_SPEED : MARIO_WALK_SPEED;   

    // -> Logica de estado e Velocidade Horizontal (Vx)
    // Agachar (para Super e Fire Mario, no chao)
    if(wantsToCrouch && Mario->powerUpState != STATE_SMALL && Mario->actualState!=ACTION_JUMPING && Mario->stats.canJump){
        Mario->actualState = ACTION_CROUCH;
        Mario->speed.x = 0;
    }
    // Atualiza Vx, caso nao agache
    else{
        // Vx no chao
        if(Mario->stats.canJump){
            // Reseta o timer de controle no ar
            Mario->air_control.countAirTimeX = 0.0f;

            if(Mario->actualState == ACTION_CROUCH) Mario->actualState = ACTION_IDLE;  // Levanta se estava agachado

            // Detecta se deve ativar o slide
            bool isTryingToSlide = (wantsToMoveLeft && Mario->speed.x > STOP_SPEED_THRESHOLD) || (wantsToMoveRight && Mario->speed.x < -STOP_SPEED_THRESHOLD) ;

            // Aciona slide
            if(isTryingToSlide && Mario->stats.canJump) Mario->actualState = ACTION_SLIDE;

            // Trata o slide
            if(Mario->actualState == ACTION_SLIDE){
                // Desaceleracao por atrito
                Mario->speed.x *= powf(SLIDE_GROUND_FRICTION_COEFF, dt * 60.0f);
                // Limitante inferior de Vx
                if(fabsf(Mario->speed.x) < STOP_SPEED_THRESHOLD){
                    Mario->speed.x = 0.0f;
                    if(Mario->actualState == ACTION_WALKING){
                        Mario->actualState = ACTION_IDLE;
                    }
                }
            }

            // Movimento horizontal
            else if(wantsToMoveLeft && !wantsToMoveRight){ // Esquerda
                // Incrementa aceleracao
                Mario->speed.x -= MARIO_GROUND_ACCELERATION*dt;
                // Limita para a Vmax atual
                if(Mario->speed.x < -currentMoveSpeed){
                    Mario->speed.x = -currentMoveSpeed;
                }
                Mario->stats.facingRight = false;
                if (Mario->actualState != ACTION_JUMPING) Mario->actualState = ACTION_WALKING; // Troca para anim de andar, caso nao esteja pulando
            }
            else if(!wantsToMoveLeft && wantsToMoveRight){ // Direita
                // Incrementa aceleracao
                Mario->speed.x += MARIO_GROUND_ACCELERATION*dt;
                // Limita para a Vmax atual
                if(Mario->speed.x > currentMoveSpeed){
                    Mario->speed.x = currentMoveSpeed;
                }
                Mario->stats.facingRight = true;
                if (Mario->actualState != ACTION_JUMPING) Mario->actualState = ACTION_WALKING; // Troca para anim de andar, caso nao esteja pulando
            }
            // Atrito e desaceleracao 
            else{
                // Aplica atrito do chão
                Mario->speed.x *= powf(WALK_GROUND_FRICTION_COEFF, dt * 60.0f);
                // Limitante inferior de Vx
                if(fabsf(Mario->speed.x) < STOP_SPEED_THRESHOLD){
                    Mario->speed.x = 0.0f;
                    if(Mario->actualState == ACTION_WALKING){
                        Mario->actualState = ACTION_IDLE;
                    }
                }
            }
        }

        // Vx no ar
        else{
            // Aplica aceleracao no ar
            if(Mario->air_control.countAirTimeX < Mario->air_control.maxAirTimeX){
                Mario->air_control.countAirTimeX += dt;

                if(wantsToMoveLeft) {
                    Mario->speed.x -= MARIO_AIR_ACCELERATION * dt;
                }
                if(wantsToMoveRight) {
                    Mario->speed.x += MARIO_AIR_ACCELERATION * dt;
                }
            }

            // Limitando velocidade
            if(Mario->speed.x > currentMoveSpeed){
                Mario->speed.x = currentMoveSpeed;
            }
            if(Mario->speed.x < -currentMoveSpeed){
                Mario->speed.x = -currentMoveSpeed;
            }
            
            Mario->speed.x *= powf(AIR_FRICTION_COEFF, dt * 60.0f);
        }
    }


    // -> Movimento e colisao no eixo X
    Mario->position.x += Mario->speed.x * dt;
    MarioHitbox(Mario);
    CheckMarioHitboxX(Mario, physPlatforms);


    // -> Pulo e fisica da gravidade
    // Pulo
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) && Mario->stats.canJump && Mario->actualState != ACTION_CROUCH) {
        Mario->speed.y = -MARIO_JUMP_STRENGTH;
        Mario->actualState = ACTION_JUMPING;
        Mario->stats.canJump = false; // Só pode pular de novo quando tocar o chão
    }
    // Gravidade
    Mario->speed.y += GRAVITY * dt;
    if (Mario->speed.y > MAX_FALL_SPEED) {
        Mario->speed.y = MAX_FALL_SPEED;
    }


    // -> Movimento e colisao no Eixo Y
    Mario->position.y += Mario->speed.y * dt;
    MarioHitbox(Mario);
    Mario->stats.canJump = false;
    // Checa e corrige a colisao e hitbox do mario
    CheckMarioHitboxY(Mario, physPlatforms, &isOnGround, bumpSound);


    // -> Ajustes finais de estado
    // Se não colidiu com nada embaixo, está caindo
    if (!isOnGround && Mario->actualState != ACTION_JUMPING) {
         Mario->actualState = ACTION_JUMPING;
    }

    // Lógica do FPS da animação de corrida
    if(Mario->actualState == ACTION_WALKING){
        float current_speed = fabsf(Mario->speed.x);

        float sprite_fps = current_speed/MARIO_RUN_SPEED;
        if(sprite_fps > 1.0f){
            sprite_fps = 1.0f;
        }
        Mario->assets.activeSprite->frameSpeed = ANIM_SPEED_WALK - (sprite_fps * (ANIM_SPEED_WALK - ANIM_SPEED_RUN));
    }

    // Evita um bug do estado IDLE
    if (isOnGround && Mario->speed.x == 0 && Mario->actualState != ACTION_CROUCH) {
        Mario->actualState = ACTION_IDLE;
    }
}


// Funçao para inicializar cada sprite
void InitSprite(MarioSprite_t *sprite, Texture2D texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int currentFrame){
    sprite->spriteSheet = texture; // Arquivo da sprite
    sprite->sourceRec = original_frame_pos_scale; // Frame de corte
    sprite->frameSpeed = frameSpeed; // dt de passagem dos frames
    sprite->frameTimer = frameTimer; // cronometro de passagem dos frames
    sprite->currentFrame = currentFrame; // index do frame atual
    // x/y do corte
    sprite->frameWidthCut = original_frame_pos_scale.width; 
    sprite->frameHeightCut = original_frame_pos_scale.height;
}

// Função para inicializar a struct Mario
void InitMario(Mario_t *Mario){
    Mario->position = MARIO_START_POSITION; 
    Mario->speed = (Vector2){0.0f, 0.0f}; 
    
    Mario->powerUpState = STATE_SMALL; 
    Mario->actualState = ACTION_IDLE;

    Mario->stats.invincible = false;
    Mario->stats.facingRight = true; 
    Mario->stats.lives=3; 
    Mario->stats.score=0;
    Mario->stats.coins=0;
    Mario->stats.canMove = true; 
    Mario->stats.canJump = false; 
    Mario->stats.isOnCave = false;
    Mario->stats.winningGame = false;
    Mario->stats.finished = false;

    Mario->dead_control.deadStarted = false; 
    Mario->dead_control.deadSoundTimer = 0.0f; 

    Mario->air_control.maxAirTimeX = 0.3f; 
    Mario->air_control.countAirTimeX = 0.0f; 

    Mario->cave_control.cave_coord = (Vector2){1850.0f, 400.0f};
    Mario->cave_control.entrance = (Rectangle){1467.0f, 320.0f, MARIO_SPRITE_SCALE*17.0f, MARIO_SPRITE_SCALE*12.0f};
    Mario->cave_control.exit = (Rectangle){1860.0f, 368.0f, MARIO_SPRITE_SCALE*17.0f, MARIO_SPRITE_SCALE*24.0f};

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
        &Mario->assets.smallMarioSprite,
        Mario->assets.smallMarioSheet, 
        (Rectangle){0.0f, 0.0f,
                    NORMAL_MARIO_FRAME_WIDHT_CUT, NORMAL_MARIO_FRAME_HEIGHT_CUT}, 
        0.1f, 
        0.0f, 
        7); 
    // -> Super Mario
    InitSprite(
        &Mario->assets.superMarioSprite, 
        Mario->assets.superMarioSheet, 
        (Rectangle){0.0f, 0.0f, 
                    SUPER_MARIO_FRAME_WIDHT_CUT, SUPER_MARIO_FRAME_HEIGHT_CUT}, 
        0.1f,
        0.0f, 
        6);
    // -> Fire Mario 
    InitSprite(
        &Mario->assets.fireMarioSprite, 
        Mario->assets.fireMarioSheet, 
        (Rectangle){0.0f, 0.0f, 
                    FIRE_MARIO_FRAME_WIDHT_CUT, FIRE_MARIO_FRAME_HEIGHT_CUT}, 
        0.1f, 
        0.0f, 
        6); 

    // -> Iniciando com a sprite do mario normal
    Mario->assets.activeSprite = &Mario->assets.smallMarioSprite;

    // Audio da morte
    Mario->assets.deathSound = LoadSound("assets/audio/mario_death.mp3");
}

// Função para desenhar a sprite em outro lugar
void ChangeMarioSpritePosition(Mario_t *Mario, float width_scale, float height_scale){
    MarioSprite_t *sprite = Mario->assets.activeSprite;
    
    // Posição
    float offset_x = width_scale; 
    float offset_y = height_scale;
    // Aplicando o desenho com offset
    sprite->destRec.x = Mario->position.x - offset_x;
    sprite->destRec.y = Mario->position.y - offset_y;
    // Proporção, tem que ser ajustada corretamente depois
    sprite->destRec.width = width_scale;
    sprite->destRec.height = height_scale;

    // Alterna o frame da sprite com base no contador
    sprite->sourceRec.x = (float)(sprite->currentFrame * sprite->frameWidthCut); 
    Vector2 origin = {0, 0}; // Ponto de origem para rotação/escala 
    
    // Finalmente desenhando
    DrawTexturePro(
        sprite->spriteSheet,
        sprite->sourceRec, 
        sprite->destRec,
        origin,
        0.0f,
        WHITE 
    );
}

// Limita o range dos frames dinamicos
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
void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range){ 
    MarioSprite_t *sprite = Mario->assets.activeSprite;
    ConstrainIndex(Mario, range);

    // Acumula o tempo que passou desde o ultimo frame
    sprite->frameTimer += GetFrameTime();
    if(sprite->frameTimer >= sprite->frameSpeed){ 
        sprite->frameTimer = 0; 

        // Movendo para DIREITA
        if(Mario->stats.facingRight){ 
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
    
    // Selecionando o banco de dados das animações
    // Atualiza o DB e pega o endereço correto para a sprite do estado atual
    switch(Mario->powerUpState){
        case STATE_SMALL:
            currentAnimDB = &Mario->assets.smallMarioAnimDB; 
            Mario->assets.activeSprite = &Mario->assets.smallMarioSprite; 
            break;
        case STATE_SUPER:
            currentAnimDB = &Mario->assets.superMarioAnimDB; 
            Mario->assets.activeSprite = &Mario->assets.superMarioSprite; 
            break;
        case STATE_FIRE:
            currentAnimDB = &Mario->assets.fireMarioAnimDB; 
            Mario->assets.activeSprite = &Mario->assets.fireMarioSprite; 
            break;
    }

    // Adquire as infos da animação atual
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
        case ACTION_FLAG:
            currentAnimData = &currentAnimDB->flag;
            break;
        case ACTION_ENTERING_PIPE:
            currentAnimData = &currentAnimDB->pipe;
            break;
        case ACTION_DEAD_ALREADY:
            // Mario tá morto, nao tem animacao
            break;
    }

    // Retorna se nao tiver animacao para exibir (Mario morto)
    if(currentAnimData == NULL){
        return;
    }

    if(currentAnimData->isLooping){ // Se é animação com mais de 1 frame
        FrameRange_t range;

        // Pegando o range da animação
        if(Mario->stats.facingRight){ // Virado para direita
            range = currentAnimData->rightAnimFrames;
        }
        else{ // Virado para esquerda
            range = currentAnimData->leftAnimFrames;
        }
        ChangeSpriteTimer(Mario, range); 
    }
    else{ // Se é frame fixo
        if(Mario->stats.facingRight){ // Virado para direita
            Mario->assets.activeSprite->currentFrame = currentAnimData->freezedFrameRight;
        }
        else{ // Esquerda
            Mario->assets.activeSprite->currentFrame = currentAnimData->freezedFrameLeft;
        }
    }

    // Se estiver morrendo, ativa a animacao de morte
    if(Mario->actualState == ACTION_DYING){
        deathAnim(Mario, currentAnimData->freezedFrameLeft);
    }

    // Desenha a sprite na tela
    ChangeMarioSpritePosition(
        Mario,
        Mario->assets.activeSprite->frameWidthCut * MARIO_SPRITE_SCALE,
        Mario->assets.activeSprite->frameHeightCut * MARIO_SPRITE_SCALE);

    // Desenhar hitox para depuracao
    // DrawRectangleLines(Mario->hitbox.x, Mario->hitbox.y, Mario->hitbox.width, Mario->hitbox.height, RED); 
    // DrawRectangleLinesEx(Mario->cave_control.entrance, 2, RED);
    // DrawRectangleLinesEx(Mario->cave_control.exit, 2, RED);
}


// Função que inicializa a bandeira de fim de jogo
void InitFlag(Flag_t *Flag){
    // Iniciando sprite do pilar da bandeira
    Flag->pilar.spriteSheet = (Texture2D)LoadTexture("assets/textures/items/flag_pilar.png");
    Flag->pilar.sourceRec = (Rectangle){0.0f, 0.0f, PILAR_FRAME_WIDTH_CUT, PILAR_FRAME_HEIGHT_CUT};
    Flag->pilar.destRec = (Rectangle){3150.0f, 99.0f, PILAR_FRAME_WIDTH_CUT*PILAR_SPRITE_SCALE, PILAR_FRAME_HEIGHT_CUT*PILAR_SPRITE_SCALE};
    Flag->pilar.frameWidthCut = Flag->pilar.sourceRec.width;
    Flag->pilar.frameHeightCut = Flag->pilar.sourceRec.height;

    // Caixa de colisao para o pilar do fim de mapa
    float hitbox_offset_x = 18.0f;
    float hitbox_offset_y = 66.0f;
    Flag->hitbox.x = Flag->pilar.destRec.x + hitbox_offset_x;
    Flag->hitbox.y = Flag->pilar.destRec.y + hitbox_offset_y;
    Flag->hitbox.width = 4*PILAR_SPRITE_SCALE;
    Flag->hitbox.height = Flag->pilar.destRec.height - hitbox_offset_y;

    // Iniciando sprite da bandeira
    Flag->flag.spriteSheet = (Texture2D)LoadTexture("assets/textures/items/flag_flag.png");
    Flag->flag.sourceRec = (Rectangle){0.0f, 0.0f, FLAG_FRAME_WIDTH_CUT, FLAG_FRAME_HEIGHT_CUT};
    Flag->flag.destRec = (Rectangle){3150.0f, 99.0f, FLAG_FRAME_WIDTH_CUT*FLAG_SPRITE_SCALE, FLAG_FRAME_HEIGHT_CUT*FLAG_SPRITE_SCALE};
    Flag->flag.frameWidthCut = Flag->flag.sourceRec.width;
    Flag->flag.frameHeightCut = Flag->flag.sourceRec.height;

    // Offset pra corrigir posicao da bandeira
    Flag->flag.destRec.x -= 25.0f;
    Flag->flag.destRec.y += 28.0f;
    Flag->speed_y = 4.0f; // Velocidade da parte deslizante

    // Ajustado no modo "força bruta"
    Flag->flag_end = Flag->flag.destRec.y - 77.0f + PILAR_FRAME_HEIGHT_CUT*PILAR_SPRITE_SCALE;
}

// Funçao que atualiza a bandeira
void DrawFlag(Flag_t *Flag){
    FlagSprite_t pilar = Flag->pilar;
    FlagSprite_t flag = Flag->flag;

    Vector2 origin = {0, 0};

    // Desenha pilar
    DrawTexturePro(
        pilar.spriteSheet,
        pilar.sourceRec,
        pilar.destRec,
        origin,
        0.0f,
        WHITE
    );

    // Desenha a bandeira
    DrawTexturePro(
        flag.spriteSheet,
        flag.sourceRec,
        flag.destRec,
        origin,
        0.0f,
        WHITE
    );

    // Desenha hitbox do pilar para depuracao
    // DrawRectangleLinesEx(Flag->hitbox, 2, RED);
}

// Atualiza interacao do mario com a bandeira de fim de jogo
void UpdateFlag(Flag_t *Flag, Mario_t *Mario, PhysPlatform_t *physPlatform){
    // Logica de fim de jogo
    if(CheckCollisionRecs(Flag->hitbox, Mario->hitbox) && (Flag->flag.destRec.y < Flag->flag_end)){
        Mario->stats.facingRight = true;
        Mario->actualState = ACTION_FLAG;
        Mario->stats.canMove = false;
        Flag->flag.destRec.y += Flag->speed_y;
        
        // Checa se o Mario chegou no chao enquanto descia
        CheckFlagEndCollision(Mario, physPlatform);
        
        // Fim da descida de bandeira
        if(Flag->flag.destRec.y > Flag->flag_end){
            Flag->flag.destRec.y = Flag->flag_end;
            Mario->actualState = ACTION_WALKING;
            Mario->stats.winningGame = true;
            AddScoreAtPosition((Vector2){Flag->flag.destRec.x, Flag->flag.destRec.y - 10.0f}, 1000);
            Mario->stats.score += 1000;
        }
    }
    DrawFlag(Flag);
}

// Descarrega os assets do Mario
void UnloadMario(Mario_t *Mario){
    UnloadTexture(Mario->assets.superMarioSheet);
    UnloadTexture(Mario->assets.smallMarioSheet);
    UnloadTexture(Mario->assets.fireMarioSheet);

    UnloadSound(Mario->assets.deathSound);
}

// Descarrega os assets da bandeira
void UnloadFlag(Flag_t *Flag){
    UnloadTexture(Flag->flag.spriteSheet);
    UnloadTexture(Flag->pilar.spriteSheet);
}