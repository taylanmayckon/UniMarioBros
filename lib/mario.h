#ifndef MARIO_H
#define MARIO_H

#include "mario_animdb.h"
#include "platform.h"

// Enum para os estados do Mário (Quando ele pega flor, etc...)
typedef enum{
    STATE_SMALL, // Mario pequeno (padrão)
    STATE_SUPER, // Mario grande
    STATE_FIRE, // Mario que taca bola de fogo
} MarioPowerUpStates_t;

// Enum para as ações do Mario (pular, andar, deslizar...)
typedef enum{
    ACTION_IDLE, // Mario parado
    ACTION_WALKING, // Mario andando
    ACTION_JUMPING, // Mario pulando
    ACTION_SLIDE, // Mario deslizando (parando de correr)
    ACTION_CROUCH, // Mario abaixado
    ACTION_FLAG, // Descendo na bandeira
    ACTION_ENTERING_PIPE, // Entrando no cano
    ACTION_DYING, // Morrendo
    ACTION_DEAD_ALREADY, // Morto de fato
    ACTION_THROW, // Jogar bola de fogo
} MarioActionStates_t;

// Struct para as animações do Mário
typedef struct{
    Texture2D spriteSheet; // Local da textura da animação
    Rectangle sourceRec; // Retangulo de origem (o corte da sprite)
    Rectangle destRec; // Retangulo de destino: pos_x, pos_y, largura, altura
    int currentFrame; // Frame atual (0->frameCount)
    float frameSpeed; // Duração de cada frame (ex: 1s)
    float frameTimer; // Contador para animação atual (conta quantos segundos já foram até o frameSpeed)
    bool revertAnim; // Booleano para inverter animação para tornar mais suave a transição dos frames
    float frameWidthCut; // Largura do corte de cada frame
    float frameHeightCut; // Altura do corte de cada frame
} MarioSprite_t;

// Struct que armazena as informações das ações do Mário
typedef struct{
    // Renderizam os frames do Mario andando, pulando, agachando e deslizando
    Texture2D smallMarioSheet;
    Texture2D superMarioSheet;
    Texture2D fireMarioSheet;
    // Sprites de cada estado do Mario
    MarioSprite_t smallMarioSprite;
    MarioSprite_t superMarioSprite;
    MarioSprite_t fireMarioSprite;
    // Ponteiro para a sprite que será desenhada e animada
    MarioSprite_t *activeSprite; 
    // Banco de dados de cada animação do Mario com base no PowerUp
    MarioAnimDB_t smallMarioAnimDB; // Mario normal
    MarioAnimDB_t superMarioAnimDB; // Super mario
    MarioAnimDB_t fireMarioAnimDB; // Fire mario
    // Som de morte
    Sound deathSound;
} MarioAssets_t;

// Status gerais do Mario
typedef struct{
    int lives; // Contador de vidas
    int score; // Pontuação
    int coins; // Quant. de moedas
    bool invincible; // Quando ele é atacado fica invencível se tiver cogumelo
    bool canMove; // Booleano para indicar se pode se mover ou não 
    bool canJump; // Booleano para indicar que pode pular
    bool facingRight; // Se esta olhando para a direita ou não (1: direita, 0: não)
    bool isOnCave; // Se o Mario esta dentro da caverna ou nao
    bool winningGame; // Animacao de fim de jogo
    bool finished; // Venceu o jogo
    bool gameover; // Perdeu o jogo
} MarioStats_t;

// Variaveis para limitar o controle em ar
typedef struct{
    float maxAirTimeX; // Tempo que pode controlar no ar
    float countAirTimeX; // Contador de tempo no ar
} MarioAirControl_t;

// Variaveis que controlam a morte do Mario
typedef struct{
    bool deadStarted; // Booleano para indicar se a animacao de morte ja comecou ou nao (para indicar quando comeca gravidade)
    bool deadSoundPlayed; // Indica se o som de morte tocou ou nao
    float deadSoundTimer; // Deixa o som de morte tocar todo antes de prosseguir
} MarioDeadControl_t;

// Para controlar a passagem para fase secreta
typedef struct{
    Vector2 previous_position; // Armazena a posicao antes de entrar no cano
    Vector2 cave_coord; // Coordenadas do tp da caverna (x,y)
    Rectangle entrance; // Hitbox da entrada
    Rectangle exit; // Hitbox da saida
} MarioCaveControl_t;

// Struct principal do Mario
typedef struct{
    Vector2 position; // Posição atual do Mario (x, y)
    Vector2 speed; // Velocidade atual do Mario (x, y)
    Rectangle hitbox; // Caixa de colisão do Mario
    MarioStats_t stats; // Status gerais do Mario
    MarioAirControl_t air_control; // Variaveis de controle no ar para o Mario
    MarioDeadControl_t dead_control; // Variaveis de controle da morte do Mario
    MarioCaveControl_t cave_control; // Variaveis de controle da fase secreta
    MarioPowerUpStates_t powerUpState; // Estado atual do Mario (ex: normal, grande)
    MarioActionStates_t actualState; // Ação atual do Mario (parado, correndo, pulando...)
    MarioAssets_t assets; // Todas as animações agrupadas
} Mario_t;

// Struct para as sprites da bandeira
typedef struct{
    Texture2D spriteSheet; // Local da textura da animação
    Rectangle sourceRec; // Retangulo de origem (o corte da sprite)
    Rectangle destRec; // Retangulo de destino: pos_x, pos_y, largura, altura
    float frameWidthCut; // Largura do corte de cada frame
    float frameHeightCut; // Altura do corte de cada frame
} FlagSprite_t;

// Struct para controlar a animação da bandeira
typedef struct{
    FlagSprite_t flag; // Sprite da bandeira que vai deslizar
    FlagSprite_t pilar; // Sprite do pilar da bandeira
    Rectangle hitbox; // Caixa de colisao do pilar
    float speed_y; // Velocidade da parte deslizante
    float flag_end; // Posicao para parar o slide da bandeira
} Flag_t;

// Protótipos de funções:
void UpdateMario(Mario_t *Mario, PhysPlatform_t *physPlatforms, int physPlatCount, Sound bumpSound);
void InitSprite(MarioSprite_t *sprite, Texture2D texture, Rectangle original_frame_pos_scale, float frameSpeed, float frameTimer, int currentFrame);
void InitMario(Mario_t *Mario);
void ChangeMarioSpritePosition(Mario_t *Mario, float width_scale, float height_scale);
void ChangeSpriteTimer(Mario_t *Mario, FrameRange_t range);
void deathAnim(Mario_t *Mario, int frame_index);
void DrawMario(Mario_t *Mario);
void MarioHitbox(Mario_t *Mario);
void UnloadMario(Mario_t *Mario);
void ResetMario(Mario_t *Mario);
// Funcoes para bandeira
void InitFlag(Flag_t *Flag);
void DrawFlag(Flag_t *Flag);
void UpdateFlag(Flag_t *Flag, Mario_t *Mario, PhysPlatform_t *physPlatform);
void UnloadFlag(Flag_t *Flag);

#endif
