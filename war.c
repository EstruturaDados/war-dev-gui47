// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define TAM_NOME 50
#define TAM_COR 20

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_NOME];
    char corExercito[TAM_COR];
    int numTropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao, const char *corJogador);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *atacante, Territorio *defensor, const char *corJogador);
int sortearMissao(void);
int verificarVitoria(const Territorio *mapa, int idMissao, const char *corJogador);

// Função utilitária:
void limparBufferEntrada(void);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");
    
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand((unsigned int)time(NULL));
    
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: Falha ao alocar memoria para o mapa!\n");
        return 1;
    }
    
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa);
    
    // - Define a cor do jogador e sorteia sua missão secreta.
    char corJogador[TAM_COR] = "Azul";
    int missaoJogador = sortearMissao();
    
    int opcao;
    int venceu = 0;

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    do {
        // - A cada iteração, exibe o mapa, a missão e o menu de ações.
        printf("\n");
        printf("============================================\n");
        printf("           ESTADO ATUAL DO MAPA\n");
        printf("============================================\n");
        exibirMapa(mapa);
        
        printf("\n--- Sua Missao Secreta ---\n");
        exibirMissao(missaoJogador, corJogador);
        
        exibirMenuPrincipal();
        
        // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            if (feof(stdin)) {
                opcao = 0; // Exit gracefully on EOF
                printf("\n");
                break;
            }
            limparBufferEntrada();
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }
        limparBufferEntrada();
        
        switch (opcao) {
            //   - Opção 1: Inicia a fase de ataque.
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            
            //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
            case 2:
                if (verificarVitoria(mapa, missaoJogador, corJogador)) {
                    printf("\n*** PARABENS! Voce completou sua missao e VENCEU o jogo! ***\n");
                    venceu = 1;
                } else {
                    printf("\nSua missao ainda nao foi cumprida. Continue jogando!\n");
                }
                break;
            
            //   - Opção 0: Encerra o jogo.
            case 0:
                printf("\nEncerrando o jogo. Ate a proxima!\n");
                break;
            
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }
        
        // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
        if (opcao != 0 && !venceu) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
        
    } while (opcao != 0 && !venceu);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(void) {
    Territorio *mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio *mapa) {
    // Território 1
    strncpy(mapa[0].nome, "Brasil", TAM_NOME - 1);
    mapa[0].nome[TAM_NOME - 1] = '\0';
    strncpy(mapa[0].corExercito, "Azul", TAM_COR - 1);
    mapa[0].corExercito[TAM_COR - 1] = '\0';
    mapa[0].numTropas = 3;
    
    // Território 2
    strncpy(mapa[1].nome, "Argentina", TAM_NOME - 1);
    mapa[1].nome[TAM_NOME - 1] = '\0';
    strncpy(mapa[1].corExercito, "Verde", TAM_COR - 1);
    mapa[1].corExercito[TAM_COR - 1] = '\0';
    mapa[1].numTropas = 2;
    
    // Território 3
    strncpy(mapa[2].nome, "Chile", TAM_NOME - 1);
    mapa[2].nome[TAM_NOME - 1] = '\0';
    strncpy(mapa[2].corExercito, "Vermelho", TAM_COR - 1);
    mapa[2].corExercito[TAM_COR - 1] = '\0';
    mapa[2].numTropas = 4;
    
    // Território 4
    strncpy(mapa[3].nome, "Colombia", TAM_NOME - 1);
    mapa[3].nome[TAM_NOME - 1] = '\0';
    strncpy(mapa[3].corExercito, "Verde", TAM_COR - 1);
    mapa[3].corExercito[TAM_COR - 1] = '\0';
    mapa[3].numTropas = 2;
    
    // Território 5
    strncpy(mapa[4].nome, "Peru", TAM_NOME - 1);
    mapa[4].nome[TAM_NOME - 1] = '\0';
    strncpy(mapa[4].corExercito, "Azul", TAM_COR - 1);
    mapa[4].corExercito[TAM_COR - 1] = '\0';
    mapa[4].numTropas = 3;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio *mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal(void) {
    printf("\n============================================\n");
    printf("              MENU PRINCIPAL\n");
    printf("============================================\n");
    printf("  1 - Atacar\n");
    printf("  2 - Verificar Missao\n");
    printf("  0 - Sair\n");
    printf("============================================\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio *mapa) {
    printf("%-5s | %-15s | %-12s | %-8s\n", "ID", "Territorio", "Exercito", "Tropas");
    printf("------+-----------------+--------------+---------\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-5d | %-15s | %-12s | %-8d\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].corExercito, 
               mapa[i].numTropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int idMissao, const char *corJogador) {
    switch (idMissao) {
        case 0:
            printf("Missao: Destruir o exercito Verde (conquistar todos os territorios verdes).\n");
            break;
        case 1:
            printf("Missao: Conquistar 3 territorios para o exercito %s.\n", corJogador);
            break;
        default:
            printf("Missao desconhecida.\n");
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    int idAtacante, idDefensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    exibirMapa(mapa);
    
    printf("\nDigite o numero do territorio ATACANTE (1 a %d): ", NUM_TERRITORIOS);
    if (scanf("%d", &idAtacante) != 1) {
        limparBufferEntrada();
        printf("Entrada invalida.\n");
        return;
    }
    limparBufferEntrada();
    
    printf("Digite o numero do territorio DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
    if (scanf("%d", &idDefensor) != 1) {
        limparBufferEntrada();
        printf("Entrada invalida.\n");
        return;
    }
    limparBufferEntrada();
    
    // Validações básicas
    if (idAtacante < 1 || idAtacante > NUM_TERRITORIOS || 
        idDefensor < 1 || idDefensor > NUM_TERRITORIOS) {
        printf("Erro: Territorios invalidos! Escolha numeros entre 1 e %d.\n", NUM_TERRITORIOS);
        return;
    }
    
    if (idAtacante == idDefensor) {
        printf("Erro: Um territorio nao pode atacar a si mesmo!\n");
        return;
    }
    
    // Ajusta para índices do array (0-based)
    Territorio *atacante = &mapa[idAtacante - 1];
    Territorio *defensor = &mapa[idDefensor - 1];
    
    // Verifica se o atacante pertence ao jogador
    if (strcmp(atacante->corExercito, corJogador) != 0) {
        printf("Erro: Voce so pode atacar a partir de territorios do exercito %s!\n", corJogador);
        return;
    }
    
    // Verifica se o defensor é do mesmo exército
    if (strcmp(defensor->corExercito, corJogador) == 0) {
        printf("Erro: Voce nao pode atacar seu proprio territorio!\n");
        return;
    }
    
    // Verifica se o atacante tem tropas suficientes
    if (atacante->numTropas < 2) {
        printf("Erro: O territorio atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }
    
    simularAtaque(atacante, defensor, corJogador);
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio *atacante, Territorio *defensor, const char *corJogador) {
    printf("\n>>> BATALHA: %s (%s) vs %s (%s) <<<\n", 
           atacante->nome, atacante->corExercito,
           defensor->nome, defensor->corExercito);
    
    // Rola os dados
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;
    
    printf("Dado do Atacante: %d\n", dadoAtaque);
    printf("Dado do Defensor: %d\n", dadoDefesa);
    
    // Compara os resultados (empate favorece o atacante conforme README)
    if (dadoAtaque >= dadoDefesa) {
        printf("Resultado: ATACANTE VENCE!\n");
        defensor->numTropas--;
        printf("O defensor perdeu 1 tropa. Tropas restantes em %s: %d\n", 
               defensor->nome, defensor->numTropas);
        
        // Se o defensor perdeu todas as tropas, território é conquistado
        if (defensor->numTropas <= 0) {
            printf("\n*** TERRITORIO CONQUISTADO! ***\n");
            printf("%s agora pertence ao exercito %s!\n", defensor->nome, corJogador);
            
            // Atualiza o dono do território
            strncpy(defensor->corExercito, corJogador, TAM_COR - 1);
            defensor->corExercito[TAM_COR - 1] = '\0';
            
            // Move uma tropa do atacante para o território conquistado
            defensor->numTropas = 1;
            atacante->numTropas--;
            
            printf("1 tropa foi movida de %s para %s.\n", atacante->nome, defensor->nome);
        }
    } else {
        printf("Resultado: DEFENSOR VENCE!\n");
        atacante->numTropas--;
        printf("O atacante perdeu 1 tropa. Tropas restantes em %s: %d\n", 
               atacante->nome, atacante->numTropas);
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio *mapa, int idMissao, const char *corJogador) {
    switch (idMissao) {
        case 0: {
            // Missão: Destruir o exército Verde (não pode haver nenhum território verde)
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].corExercito, "Verde") == 0) {
                    return 0; // Ainda existe território verde
                }
            }
            return 1; // Todos os territórios verdes foram conquistados
        }
        
        case 1: {
            // Missão: Conquistar 3 territórios para o exército do jogador
            int territoriosConquistados = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].corExercito, corJogador) == 0) {
                    territoriosConquistados++;
                }
            }
            return (territoriosConquistados >= 3) ? 1 : 0;
        }
        
        default:
            return 0;
    }
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
