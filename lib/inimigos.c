#include <raylib.h>
#include <stdbool.h>
#include "inimigos.h"

#define GOOMBA_VEL_X 2
#define GOOMBA_FRAMES_ANIMACAO 15
#define PLANTA_FRAMES_ANIMACAO 10
#define PLANTA_VEL_Y 0.5
#define TARTARUGA_VEL_X 1
#define TARTARUGA_VEL_CASCO_ROLANDO 8
#define TARTARUGA_FRAMES_ANIMACAO 15
#define TARTARUGA_TEMPO_CASCO_PARADO 5.0f 
#define TARTARUGA_TEMPO_SAINDO_CASCO 1.0f

Goomba CriarGoomba(Vector2 posicaoInicial, float distanciaParaPatrulhar){
    Goomba novoGoomba;
    novoGoomba.rect.x = posicaoInicial.x;
    novoGoomba.rect.y = posicaoInicial.y;
    novoGoomba.rect.width = larguraGoombaTela;
    novoGoomba.estadoAtual = goombaAndando;
    novoGoomba.vivo = true;

    if(novoGoomba.estadoAtual == goombaAndando){
        novoGoomba.rect.height = alturaAndandoGoombaTela;
        novoGoomba.rect.y = posicaoInicial.y;
    }
    else{
        novoGoomba.rect.height = alturaAmassadoGoombaTela;
        novoGoomba.rect.y = posicaoInicial.y + (alturaAndandoGoombaTela - alturaAmassadoGoombaTela);
    }

    novoGoomba.spriteAndando[0] = (Rectangle){0,16,16,16};
    novoGoomba.spriteAndando[1] = (Rectangle){18,16,16,16};
    novoGoomba.spriteAmassado = (Rectangle){36,24,16,8};

    novoGoomba.posicaoXOriginal = posicaoInicial.x;
    novoGoomba.distanciaPatrulha = distanciaParaPatrulhar;
    novoGoomba.andandoPraEsquerda = true;

    if(novoGoomba.andandoPraEsquerda){
        novoGoomba.velocidade.x = -GOOMBA_VEL_X;
    }
    else{
        novoGoomba.velocidade.x = GOOMBA_VEL_X;
    }
    novoGoomba.velocidade.y = 0.0f;
    novoGoomba.gravidade = 0.5;

    novoGoomba.frameAtual = 0;
    novoGoomba.contadorFramesAnimacao = 0;
    novoGoomba.framesParaMudarSprite = GOOMBA_FRAMES_ANIMACAO;

    return novoGoomba;
}

void DesenharGoomba(Goomba goomba, Texture2D texturaInimigos){
    if(!goomba.vivo){
        return;
    }
    Vector2 origem = {0, 0};
    Rectangle spriteParaUsar;

    if(goomba.estadoAtual == goombaAmassado){
    spriteParaUsar = goomba.spriteAmassado;
    goomba.rect.height = alturaAmassadoGoombaTela;
    } 
    else if(goomba.estadoAtual == goombaCaindo){
        spriteParaUsar = goomba.spriteAndando[0];
        if(goomba.velocidade.y > 0){
        spriteParaUsar.height = -spriteParaUsar.height;
        }
    }
    else {
    spriteParaUsar = goomba.spriteAndando[goomba.frameAtual];
    }
    DrawTexturePro(
        texturaInimigos,
        spriteParaUsar,
        goomba.rect,
        origem,
        0,
        WHITE
    );
}

void AtualizarGoomba(Goomba *goomba, Rectangle paredes[], int numParedes){
    //verifica se esta vivo
    if(!goomba->vivo){
        return;
    }

    if(goomba->estadoAtual == goombaAndando){
        goomba->rect.x += goomba->velocidade.x;

    //Colisao com Paredes e Movimento
    for(int i = 0; i < numParedes; i++){
        if(goomba->estadoAtual != goombaCaindo){
        if(CheckCollisionRecs(goomba->rect, paredes[i])){
            if(goomba->velocidade.x > 0){ //esta andando para a direita
                goomba->rect.x = paredes[i].x - goomba->rect.width; //reposiciona na ponta da parede
                goomba->andandoPraEsquerda = true;
                goomba->velocidade.x = -GOOMBA_VEL_X;
            }
            else if(goomba->velocidade.x < 0){
                goomba->rect.x = paredes[i].x + paredes[i].width; //reposiciona na ponta da parede
                goomba->andandoPraEsquerda = false;
                goomba->velocidade.x = GOOMBA_VEL_X;
            }
            break;
        }
    }
}
    
    //Lógica de Movimento Horizontal
    if(goomba->andandoPraEsquerda){
        if(goomba->rect.x <= goomba->posicaoXOriginal - goomba->distanciaPatrulha){
        goomba->rect.x = goomba->posicaoXOriginal - goomba->distanciaPatrulha;
        goomba->andandoPraEsquerda = false;
        goomba->velocidade.x = GOOMBA_VEL_X;
    }
}
    else{
        if(goomba->rect.x >= goomba->posicaoXOriginal + goomba->distanciaPatrulha){
            goomba->rect.x = goomba->posicaoXOriginal + goomba->distanciaPatrulha;
            goomba->andandoPraEsquerda = true; 
            goomba->velocidade.x = -GOOMBA_VEL_X;
    }
}

    
    if(goomba->estadoAtual == goombaAndando){
            goomba->contadorFramesAnimacao++;
            if(goomba->contadorFramesAnimacao >= goomba->framesParaMudarSprite){
                goomba->contadorFramesAnimacao = 0;
                goomba->frameAtual++;
                if(goomba->frameAtual > 1){
                    goomba->frameAtual = 0; //reseta o frame fazendo um loop na animacao
                }
            }
    }
    
}
    goomba->velocidade.y += goomba->gravidade;
    goomba->rect.y += goomba->velocidade.y;

    if(goomba->estadoAtual == goombaAmassado){
    goomba->temporizadorPisado -= GetFrameTime();
        if(goomba->temporizadorPisado <= 0){
            goomba->vivo = false;
        }
    }
}

void ResolverColisoesCenarioGoomba(Goomba *goomba, Rectangle chaoPrincipal, Rectangle plataforma, int alturaTela){
    if(!goomba->vivo){ // Não processa se não estiver vivo
        return;
    }

    bool aterrisouNestaFrame = false;

    // Colisão com o Chão Principal
    if(goomba->estadoAtual != goombaCaindo){
    if(CheckCollisionRecs(goomba->rect, chaoPrincipal)){
        if(goomba->velocidade.y > 0) { // Estava caindo
            // Ajusta a altura do rect ANTES de reposicionar Y
            if(goomba->estadoAtual == goombaAmassado) {
                goomba->rect.height = alturaAmassadoGoombaTela;
            }
            else{
                goomba->rect.height = alturaAndandoGoombaTela;
            }
            goomba->rect.y = chaoPrincipal.y - goomba->rect.height;
            goomba->velocidade.y = 0;
            aterrisouNestaFrame = true;
        }
    }

    // Colisão com Plataformas
    if (!aterrisouNestaFrame){
            if(CheckCollisionRecs(goomba->rect, plataforma)){
                float peDoGoombaFrameAnterior = (goomba->rect.y - goomba->velocidade.y) + goomba->rect.height;
                if(goomba->velocidade.y > 0 && peDoGoombaFrameAnterior <= plataforma.y &&
                    (goomba->rect.x < plataforma.x + plataforma.width &&
                    goomba->rect.x + goomba->rect.width > plataforma.x)){
                    goomba->rect.y = plataforma.y - goomba->rect.height;
                    goomba->velocidade.y = 0;
                    aterrisouNestaFrame = true;
                }
            }
        }
    
    if (aterrisouNestaFrame){
        if(goomba->andandoPraEsquerda){
            goomba->velocidade.x = -GOOMBA_VEL_X;
        } 
        else{
            goomba->velocidade.x = GOOMBA_VEL_X;
        }
    }
    else{
        goomba->velocidade.x = 0;
    }
}

    if(goomba->rect.y > alturaTela){
        goomba->vivo = false;
    }
}

void ProcessarColisaoGoombaComMario(Goomba *goomba, Rectangle marioRect, float *marioVelY) {
    if (!goomba->vivo || goomba->estadoAtual != goombaAndando) {
        return;
    }

    if (CheckCollisionRecs(goomba->rect, marioRect)) {
        float peDoMario = marioRect.y + marioRect.height;
        bool marioEstaCaindo = *marioVelY > 0.1f; 

        bool colisaoDePiso = (marioEstaCaindo &&
                              peDoMario >= goomba->rect.y &&
                              peDoMario < (goomba->rect.y + goomba->rect.height / 2.0f));
        
        if (colisaoDePiso) {
            // --- LÓGICA DE PISÃO NO GOOMBA ---
            goomba->estadoAtual = goombaAmassado;
            goomba->velocidade.x = 0; // Para de andar
            goomba->temporizadorPisado = 0.5f; // Inicia o timer para sumir
            
            
            if (goomba->rect.height == alturaAndandoGoombaTela) { 
                 goomba->rect.y += (alturaAndandoGoombaTela - alturaAmassadoGoombaTela); 
                 goomba->rect.height = alturaAmassadoGoombaTela; 
            }

            *marioVelY = alturaBounceMario; 
            // AQUI VOCE TAMBEM PODE ADICIONAR UM SOM DE PISAO
        } else {
            // --- LÓGICA DE COLISÃO LATERAL OU POR BAIXO (NÃO PISÃO)
            // Aqui chamaria a funcao do Mario para levar dano.
        }
    }
}

PlantaCarnivora CriarPlantaCarnivora(Vector2 posicaoDoCano, int posicaoYTunel, int larguraTunel){
    PlantaCarnivora novaPlanta;
    novaPlanta.rect.x = posicaoDoCano.x + ((larguraTunel - larguraPlantaTela)/2);
    novaPlanta.rect.y = posicaoDoCano.y;
    novaPlanta.rect.width = larguraPlantaTela;
    novaPlanta.rect.height = alturaPlantaTela;
    novaPlanta.estadoAtual = plantaEscondida;

    novaPlanta.sprite[0] = (Rectangle){0,138,16,24};
    novaPlanta.sprite[1] = (Rectangle){18,138,16,24};

    novaPlanta.temporizadorEstado = 1.0f;
    novaPlanta.frameAtual = 0;
    novaPlanta.contadorFramesAnimacao = 0;
    novaPlanta.framesParaMudarSprite = 8;

    novaPlanta.velocidadeVertical = PLANTA_VEL_Y;
    novaPlanta.limiteInferior =  posicaoYTunel;
    novaPlanta.limiteSuperior = posicaoYTunel - alturaPlantaTela;
    novaPlanta.viva = true;

    return novaPlanta;
}

void DesenharPlantaCarnivora(PlantaCarnivora planta, Texture2D texturaInimigos){
    Vector2 origem = {0, 0};
    Rectangle spriteParaUsar;

    spriteParaUsar = planta.sprite[planta.frameAtual];

    DrawTexturePro(
        texturaInimigos,
        spriteParaUsar,
        planta.rect,
        origem,
        0,
        WHITE
    );
}

void AtualizarPlantaCarnivora(PlantaCarnivora *planta){

    if(!planta->viva){
        return;
    }

    switch (planta->estadoAtual){
        case plantaEscondida:
        planta->temporizadorEstado -= GetFrameTime();
        if(planta->temporizadorEstado <= 0){
            planta->estadoAtual = plantaSubindo;
        }
        break;

        case plantaRevelada:
        planta->temporizadorEstado -= GetFrameTime();
        if(planta->temporizadorEstado <= 0){
            planta->estadoAtual = plantaDescendo;
        }
        break;

        case plantaSubindo:
        planta->rect.y -= planta->velocidadeVertical;
        if(planta->rect.y <= planta->limiteSuperior){
            planta->rect.y = planta->limiteSuperior;
            planta->estadoAtual = plantaRevelada;
            planta->temporizadorEstado = 2.0f; //2 segundos parado ate descer
        }
        break;

        case plantaDescendo:
        planta->rect.y += planta->velocidadeVertical;
        if(planta->rect.y >= planta->limiteInferior){
            planta->rect.y = planta->limiteInferior;
            
            planta->temporizadorEstado -= GetFrameTime();
            if(planta->temporizadorEstado <= 0){
            planta->estadoAtual = plantaEscondida;
            planta->temporizadorEstado = 3.0f; //3 segundos parado ate subir
        }
        break;
    
        default:
        break;
    }
} 

    planta->contadorFramesAnimacao++;
    if(planta->contadorFramesAnimacao >= planta->framesParaMudarSprite){
    planta->contadorFramesAnimacao = 0;
    planta->frameAtual++;
        if(planta->frameAtual > 1){
            planta->frameAtual = 0; //reseta o frame fazendo um loop na animacao
        }
    }
}

void ProcessarColisaoPlantaComMario(PlantaCarnivora *planta, Rectangle marioRect) {
    // A planta só causa dano se estiver viva e nao escondida (fora do cano)
    if (!planta->viva || planta->estadoAtual == plantaEscondida){
        return; 
        // Sai da função se a planta não está em um estado de "dano"
    }

    // Verifica se há colisão entre o retângulo da planta e o do Mario
    if (CheckCollisionRecs(planta->rect, marioRect)) {
        // Colisão detectada com a planta
        // AQUI VOCÊ CHAMARIA A FUNÇÃO PARA O MARIO LEVAR DANO
        // Exemplo hipotético, nao sei se da para fazer assim
        // (essa função estaria no código do Mario):
        // MarioLevarDano(&mario);
    }
}

Tartaruga CriarTarturuga(Vector2 posicaoInicial, float distanciaParaPatrulhar){
     Tartaruga novaTartaruga;
    novaTartaruga.rect.x = posicaoInicial.x;
    novaTartaruga.rect.y = posicaoInicial.y; 
    novaTartaruga.rect.width = larguraTartarugaTela;
    novaTartaruga.rect.height = alturaTartarugaEmPeTela; // Altura inicial de 'em pe'
    novaTartaruga.andandoPraEsquerda = true;
    novaTartaruga.estadoAtual = tartarugaAndando;
    novaTartaruga.viva = true;
    
    novaTartaruga.velocidadeAndando.x = -TARTARUGA_VEL_X; 
    novaTartaruga.velocidadeAndando.y = 0.0f; // Começa sem velocidade vertical
    novaTartaruga.velocidadeRolando = (Vector2){0.0f, 0.0f}; 
    novaTartaruga.gravidade = 1.0f; // Gravidade para tartaruga

    // Sprites da Tartaruga (AJUSTE PARA A SUA SPRITESHEET REAL!)
    novaTartaruga.spriteAndando[0] = (Rectangle){0, 112, 16, 24};
    novaTartaruga.spriteAndando[1] = (Rectangle){18, 112, 16, 24};
    novaTartaruga.spriteCasco = (Rectangle){72, 120, 16, 16};   
    novaTartaruga.spriteCascoSaindo = (Rectangle){90, 120, 16, 16}; 

    novaTartaruga.posicaoXOriginal = posicaoInicial.x;
    novaTartaruga.distanciaPatrulha = distanciaParaPatrulhar;
    
    novaTartaruga.frameAtual = 0;
    novaTartaruga.contadorFramesAnimacao = 0;
    novaTartaruga.framesParaMudarSprite = TARTARUGA_FRAMES_ANIMACAO;

    novaTartaruga.temporizadorCasco = 0.0f;          
    novaTartaruga.temporizadorSaindoDoCasco = 0.0f; 

    return novaTartaruga;
}

void DesenharTartaruga(Tartaruga tartaruga, Texture2D texturaInimigos){

    if(!tartaruga.viva){
        return;
    }
    Vector2 origem = {0, 0};
    Rectangle spriteParaUsar;
    
    if(tartaruga.estadoAtual == tartarugaAndando){
        spriteParaUsar = tartaruga.spriteAndando[tartaruga.frameAtual];
        if(!tartaruga.andandoPraEsquerda){ 
            spriteParaUsar.width = -spriteParaUsar.width;
        }
    }
    else if(tartaruga.estadoAtual == tartarugaCascoParado){
        spriteParaUsar = tartaruga.spriteCasco; 
        tartaruga.rect.y = tartaruga.rect.y + 4;
    }
    else if(tartaruga.estadoAtual == tartarugaCascoRolando){
        spriteParaUsar = tartaruga.spriteCasco; 
        tartaruga.rect.y = tartaruga.rect.y + 4;
        if(tartaruga.velocidadeRolando.x < 0){ 
             spriteParaUsar.width = -spriteParaUsar.width;
        }
    }
    else if(tartaruga.estadoAtual == tartarugaSaindoDoCasco){
        spriteParaUsar = tartaruga.spriteCascoSaindo;
        tartaruga.rect.y = tartaruga.rect.y + 2;
    }
    
    DrawTexturePro(
        texturaInimigos,
        spriteParaUsar,
        tartaruga.rect,
        origem,
        0,
        WHITE
    );
}

void AtualizarTartaruga(Tartaruga *tartaruga, Rectangle paredes[], int numParedes){
    if(!tartaruga->viva){
        return;
    }
    float deltaTime = GetFrameTime(); 

    // APLICAR GRAVIDADE (só quando andando)
    if(tartaruga->estadoAtual == tartarugaAndando){ 
        tartaruga->velocidadeAndando.y += tartaruga->gravidade;
        tartaruga->rect.y += tartaruga->velocidadeAndando.y;
    }
    
    switch (tartaruga->estadoAtual){
        case tartarugaAndando:
            // MOVIMENTO HORIZONTAL
            tartaruga->rect.x += tartaruga->velocidadeAndando.x;

            // COLISÃO COM PAREDES E PATRULHA
            for(int i = 0; i < numParedes; i++){
                if(CheckCollisionRecs(tartaruga->rect, paredes[i])){
                    if(tartaruga->velocidadeAndando.x > 0){
                        tartaruga->rect.x = paredes[i].x - tartaruga->rect.width;
                        tartaruga->andandoPraEsquerda = true;
                        tartaruga->velocidadeAndando.x = -TARTARUGA_VEL_X;
                    } else if(tartaruga->velocidadeAndando.x < 0){
                        tartaruga->rect.x = paredes[i].x + paredes[i].width;
                        tartaruga->andandoPraEsquerda = false;
                        tartaruga->velocidadeAndando.x = TARTARUGA_VEL_X;
                    }
                    break;
                }
            }
            if(tartaruga->andandoPraEsquerda){
                if(tartaruga->rect.x <= tartaruga->posicaoXOriginal - tartaruga->distanciaPatrulha){
                    tartaruga->rect.x = tartaruga->posicaoXOriginal - tartaruga->distanciaPatrulha;
                    tartaruga->andandoPraEsquerda = false;
                    tartaruga->velocidadeAndando.x = TARTARUGA_VEL_X;
                }
            } else {
                if(tartaruga->rect.x >= tartaruga->posicaoXOriginal + tartaruga->distanciaPatrulha){
                    tartaruga->rect.x = tartaruga->posicaoXOriginal + tartaruga->distanciaPatrulha;
                    tartaruga->andandoPraEsquerda = true; 
                    tartaruga->velocidadeAndando.x = -TARTARUGA_VEL_X;
                }
            }
            // ANIMAÇÃO ANDANDO
            tartaruga->contadorFramesAnimacao++;
            if(tartaruga->contadorFramesAnimacao >= tartaruga->framesParaMudarSprite){
                tartaruga->contadorFramesAnimacao = 0;
                tartaruga->frameAtual++;
                if(tartaruga->frameAtual > 1){
                    tartaruga->frameAtual = 0;
                }
            }
            break;

        case tartarugaCascoParado:
            // Lógica de Temporizador para sair do casco (Usa GetFrameTime())
            tartaruga->temporizadorCasco -= deltaTime;
            if (tartaruga->temporizadorCasco <= 0) {
                tartaruga->estadoAtual = tartarugaSaindoDoCasco;
                tartaruga->temporizadorSaindoDoCasco = TARTARUGA_TEMPO_SAINDO_CASCO;
                tartaruga->frameAtual = 0;
                tartaruga->contadorFramesAnimacao = 0;
            }
            tartaruga->frameAtual = 0; 
            break;

        case tartarugaCascoRolando:
            // MOVIMENTO DE ROLAMENTO
            tartaruga->rect.x += tartaruga->velocidadeRolando.x; 

            // COLISÃO COM PAREDES QUANDO ROLANDO
            for(int i = 0; i < numParedes; i++){
                if(CheckCollisionRecs(tartaruga->rect, paredes[i])){
                    tartaruga->velocidadeRolando.x *= -1;
                    break;
                }
            }
            tartaruga->frameAtual = 0; 
            break;

        case tartarugaSaindoDoCasco:
            // ANIMAÇÃO DE SAIR DO CASCO (USA GetFrameTime())
            tartaruga->temporizadorSaindoDoCasco -= deltaTime;
            if (tartaruga->temporizadorSaindoDoCasco <= 0) {
                tartaruga->estadoAtual = tartarugaAndando;
                // Ajusta altura e Y ao voltar a andar
                tartaruga->rect.y -= (alturaTartarugaEmPeTela - alturaTartarugaCascoTela); 
                tartaruga->rect.height = alturaTartarugaEmPeTela; 
                
                // Define a direcao ao voltar a andar (mantem a ultima direcao de rolamento se houver)
                if(tartaruga->velocidadeRolando.x > 0){ 
                    tartaruga->velocidadeAndando.x = TARTARUGA_VEL_X; 
                    tartaruga->andandoPraEsquerda = false;
                } else if (tartaruga->velocidadeRolando.x < 0){ 
                    tartaruga->velocidadeAndando.x = -TARTARUGA_VEL_X;
                    tartaruga->andandoPraEsquerda = true;
                } else { // Se estava parado, volta andando na ultima direcao de andar
                    if(tartaruga->andandoPraEsquerda){
                        tartaruga->velocidadeAndando.x = -TARTARUGA_VEL_X;
                    } else {
                        tartaruga->velocidadeAndando.x = TARTARUGA_VEL_X;
                    }
                }
                tartaruga->velocidadeRolando.x = 0; 
                tartaruga->frameAtual = 0; 
                TraceLog(LOG_INFO, "Tartaruga voltou a andar.");
            }
            tartaruga->frameAtual = 0; 
            break;
    }
}

void ResolverColisoesCenarioTartaruga(Tartaruga *tartaruga, Rectangle chaoPrincipal, Rectangle plataforma){
    if(!tartaruga->viva || tartaruga->estadoAtual != tartarugaAndando){ // Só processa se estiver viva E andando
        return;
    }

    bool aterrisouNestaFrame = false;

    // Colisão com o Chão Principal
    if(CheckCollisionRecs(tartaruga->rect, chaoPrincipal)){
        if(tartaruga->velocidadeAndando.y > 0) { // Estava caindo
            tartaruga->rect.y = chaoPrincipal.y - tartaruga->rect.height;
            tartaruga->velocidadeAndando.y = 0;
            aterrisouNestaFrame = true;
        }
    }

    // Colisão com Plataformas (apenas se não aterrisou no chão principal)
    if (!aterrisouNestaFrame){
        if(CheckCollisionRecs(tartaruga->rect, plataforma)){
            float peDaTartarugaFrameAnterior = (tartaruga->rect.y - tartaruga->velocidadeAndando.y) + tartaruga->rect.height;
            if(tartaruga->velocidadeAndando.y > 0 && peDaTartarugaFrameAnterior <= plataforma.y &&
                (tartaruga->rect.x < plataforma.x + plataforma.width &&
                tartaruga->rect.x + tartaruga->rect.width > plataforma.x)){
                
                tartaruga->rect.y = plataforma.y - tartaruga->rect.height;
                tartaruga->velocidadeAndando.y = 0;
                aterrisouNestaFrame = true;
            }
        }
    }
    
    // Se não aterrisou, a gravidade continuará fazendo ela cair em AtualizarTartaruga.
}

void ProcessarColisaoTartarugaComMario(Tartaruga *tartaruga, Rectangle marioRect, float *marioVelY){    
    if(!tartaruga->viva){
        return;
    }

    if(CheckCollisionRecs(tartaruga->rect, marioRect)){

        float peDoMario = marioRect.y + marioRect.height;
        bool marioEstaCaindo = *marioVelY > 0.1f;

        // Condição de pisão: Mario está caindo E Mario está acima ou no topo da tartaruga
        bool colisaoDePiso = (marioEstaCaindo && peDoMario >= tartaruga->rect.y);
        
        if (colisaoDePiso) {
            *marioVelY = alturaBounceMario; // Mario quica

            if (tartaruga->estadoAtual == tartarugaAndando) {
                tartaruga->estadoAtual = tartarugaCascoParado;
                tartaruga->velocidadeAndando.x = 0;
                tartaruga->velocidadeRolando.x = 0;
                tartaruga->temporizadorCasco = TARTARUGA_TEMPO_CASCO_PARADO;
                if (tartaruga->rect.height == alturaTartarugaEmPeTela) {
                    tartaruga->rect.y += (alturaTartarugaEmPeTela - alturaTartarugaCascoTela);
                    tartaruga->rect.height = alturaTartarugaCascoTela;
                }
            }
            else if (tartaruga->estadoAtual == tartarugaCascoParado) {
                // Mario pisou no CASCO PARADO: Chutá-lo para a direita ou esquerda
                float marioCentroXRelativo = (marioRect.x + marioRect.width / 2.0f) - (tartaruga->rect.x);
                float meioDoCasco = tartaruga->rect.width / 2.0f;

                if (marioCentroXRelativo < meioDoCasco) { // Mario pisou na metade ESQUERDA
                    tartaruga->velocidadeRolando.x = TARTARUGA_VEL_CASCO_ROLANDO; // Chuta para a DIREITA
                } 
                else { // Mario pisou na metade DIREITA
                    tartaruga->velocidadeRolando.x = -TARTARUGA_VEL_CASCO_ROLANDO; // Chuta para a ESQUERDA
                }
                tartaruga->estadoAtual = tartarugaCascoRolando;
                tartaruga->temporizadorCasco = 0.0f;
            }
        } else {
            // Colisão LATERAL OU POR BAIXO (NÃO PISÃO)
            if (tartaruga->estadoAtual == tartarugaAndando) {
             //COLISAO LATERAL/POR BAIXO COM TARTARUGA EM PE! Mario deve levar dano
            }
            else if (tartaruga->estadoAtual == tartarugaCascoParado) {
                // COLISÃO LATERAL COM CASCO PARADO: Mario empurra o casco
                tartaruga->estadoAtual = tartarugaCascoRolando;
                if (marioRect.x + marioRect.width / 2 < tartaruga->rect.x + tartaruga->rect.width / 2) {
                    tartaruga->velocidadeRolando.x = TARTARUGA_VEL_CASCO_ROLANDO; // Empurra para a direita
                } else {
                    tartaruga->velocidadeRolando.x = -TARTARUGA_VEL_CASCO_ROLANDO; // Empurra para a esquerda
                }
                tartaruga->temporizadorCasco = 0.0f;
            }
            else if (tartaruga->estadoAtual == tartarugaCascoRolando) {
            }
        }
    }
}

void ProcessarColisaoCascoRolandoComGoomba(Tartaruga *cascoRolando, Goomba *goomba){
    if (!cascoRolando->viva || cascoRolando->estadoAtual != tartarugaCascoRolando ||
        !goomba->vivo || goomba->estadoAtual != goombaAndando) {
        return;
    }

    // Verifica colisão entre o retângulo do casco rolando e o do Goomba
    if (CheckCollisionRecs(cascoRolando->rect, goomba->rect)) {
        goomba->estadoAtual = goombaCaindo; 
        goomba->velocidade.x = 0;             
        goomba->velocidade.y = -8.0f; 
        goomba->gravidade = 0.5f;
        goomba->velocidade.y += goomba->gravidade;
        goomba->rect.y += goomba->velocidade.y;
    }
}
