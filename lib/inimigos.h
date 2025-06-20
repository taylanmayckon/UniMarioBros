#include <raylib.h>
#include <stdbool.h>

#ifndef INIMIGOS_H
#define INIMIGOS_H

#define larguraGoombaTela 40 //multiplo j de 16
#define alturaAndandoGoombaTela 40 //multiplo j de 16
#define alturaAmassadoGoombaTela 20 //multiplo j de 8
#define larguraPlantaTela 40 // multiplo k de 16
#define alturaPlantaTela 60 // multiplo k de 24
#define larguraTartarugaTela 40 // multiplo p de 16
#define alturaTartarugaEmPeTela 60 // multiplo p de 24
#define alturaTartarugaCascoTela 40 // multiplo p de 16
#define alturaBounceMario -8.0f //altura que vai quicar quando matar o goomba ou tartaruga

// 1. Goomba (o cogumelo marrom)
typedef enum EstadoGoomba{
	goombaAndando,
	goombaAmassado,
    goombaCaindo,
} EstadoGoomba;	

typedef struct Goomba {
    Rectangle rect;          // Posição (x,y) e tamanho (largura,altura) do Goomba na tela
    Vector2 velocidade; // Velocidade horizontal e vertical
    float gravidade;  
    bool andandoPraEsquerda; // // true se está andando para a esquerda, false se para a direita
    Rectangle spriteAndando[2];     // Coordenadas do Goomba na spritesheet(imagem com sprites) quando parad 
    Rectangle spriteAmassado;
    int frameAtual;
    int contadorFramesAnimacao;
    int framesParaMudarSprite;
    float temporizadorPisado; //quando tempo ele amassado ate fica ate sumir
    float posicaoXOriginal;
    float distanciaPatrulha;
    EstadoGoomba estadoAtual;
    bool vivo;
} Goomba;

Goomba CriarGoomba(Vector2 posicaoInicial, float distanciaParaPatrulhar);
void DesenharGoomba(Goomba goomba, Texture2D texturaInimigos);
void AtualizarGoomba(Goomba *goomba, Rectangle paredes[], int numParedes);
void ResolverColisoesCenarioGoomba(Goomba *goomba, Rectangle chaoPrincipal, Rectangle plataforma, int alturaTela); // Pouso no chão/plataformas
void ProcessarColisaoGoombaComMario(Goomba *goomba, Rectangle marioRect, float *marioVelY); // Interação Goomba-Mario

// 2. Planta Carnívora
typedef enum EstadoPlantaCarnivora{ //define o estado da planta, para mudança de sprites e temporizador
	plantaEscondida,
    plantaSubindo,
    plantaRevelada,
	plantaDescendo,
} EstadoPlantaCarnivora;

typedef struct PlantaCarnivora {
    Rectangle rect;          // Posição (x,y) e tamanho da planta na tela
    float velocidadeVertical;        // Velocidade de subida/descida
    Rectangle sprite[2];     // Coordenadas na spritesheet aberta e fechada
    float limiteSuperior;    // Posição Y máxima que ela sobe
    float limiteInferior;    // Posição Y mínima que ela desce
	float temporizadorEstado; //Quanto tempo ela fica em cada estado
    int frameAtual;
    int contadorFramesAnimacao;
    int framesParaMudarSprite;
    bool viva;
	EstadoPlantaCarnivora estadoAtual;
} PlantaCarnivora;

PlantaCarnivora CriarPlantaCarnivora(Vector2 posicaoDoCano, int posicaoYTunel, int larguraTunel);
void DesenharPlantaCarnivora(PlantaCarnivora planta, Texture2D texturaInimigos);
void AtualizarPlantaCarnivora(PlantaCarnivora *planta);
void ProcessarColisaoPlantaComMario(PlantaCarnivora *planta, Rectangle marioRect);

// 3. Tartaruga (Pequena)
typedef enum EstadoTartaruga{
	tartarugaAndando,
	tartarugaCascoParado,
    tartarugaCascoRolando,
    tartarugaSaindoDoCasco
} EstadoTartaruga;

typedef struct Tartaruga {
    Rectangle rect;          // Posição (x,y) e tamanho da tartaruga na tela
	Vector2 velocidadeAndando;
	Vector2 velocidadeRolando;  
    float gravidade;     
    bool andandoPraEsquerda; // true se está andando para a esquerda, false se para a direita
	Rectangle spriteAndando[2];
    Rectangle spriteCasco;
    Rectangle spriteCascoSaindo;
	int frameAtual;
    int contadorFramesAnimacao;
    int framesParaMudarSprite;
	float temporizadorCasco;
    float temporizadorSaindoDoCasco;
    float posicaoXOriginal;
    float distanciaPatrulha;
    bool viva;
	EstadoTartaruga estadoAtual;
} Tartaruga;

Tartaruga CriarTarturuga(Vector2 posicaoInicial, float distanciaParaPatrulhar);
void DesenharTartaruga(Tartaruga tartaruga, Texture2D texturaInimigos);
void AtualizarTartaruga(Tartaruga *tartaruga, Rectangle paredes[], int numParedes);
void ResolverColisoesCenarioTartaruga(Tartaruga *tartaruga, Rectangle chaoPrincipal, Rectangle plataforma);
void ProcessarColisaoTartarugaComMario(Tartaruga *tartaruga, Rectangle marioRect, float *marioVelY);
void ProcessarColisaoCascoRolandoComGoomba(Tartaruga *cascoRolando, Goomba *goomba);
#endif //INIMIGOS_H
