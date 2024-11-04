#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t burrinho_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t burrinho_disponivel_cond = PTHREAD_COND_INITIALIZER;
int burrinho_disponivel = 1;
int jogador_solicitante = 0;
int executando = 1; 

//funcao que informa qual jogador deseja solicitar o burrinho
void* input_usuario(void* arg) {
    while (executando) {
        if (burrinho_disponivel) {
            printf("Digite o jogador (1 ou 2) que quer solicitar o burrinho ou 0 para sair: ");
            scanf("%d", &jogador_solicitante);

            if (jogador_solicitante == 1 || jogador_solicitante == 2) {
                pthread_cond_broadcast(&burrinho_disponivel_cond);
            } else if (jogador_solicitante == 0) {
                executando = 0; 
                pthread_cond_broadcast(&burrinho_disponivel_cond);
            } else {
                printf("Entrada invalida! Por favor, insira 1, 2 ou 0 para sair.\n");
            }
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

//funcao que simula o pedido do burrinho (entregador) pelos jogadores
void* requisitar_burrinho(void* jogador_id) {
    int id = *(int*)jogador_id;

    while (executando) {
        pthread_mutex_lock(&burrinho_lock);
        while (burrinho_disponivel == 0 || jogador_solicitante != id) {
            if (!executando) {
                pthread_mutex_unlock(&burrinho_lock);
                pthread_exit(NULL); //sai da thread se executando for 0
            }
            pthread_cond_wait(&burrinho_disponivel_cond, &burrinho_lock);
        }

        if (!executando) { 
            pthread_mutex_unlock(&burrinho_lock);
            pthread_exit(NULL); 
        }

        // O burrinho agora está ocupado
        burrinho_disponivel = 0;
        int tempo_ida = (rand() % 3) + 2;
        int tempo_volta = (rand() % 3) + 2;

        printf("Jogador %d solicitou o burrinho.\n", id);
        printf("Burrinho indo para o Jogador %d. Tempo de viagem: %d segundos.\n", id, tempo_ida);
        sleep(tempo_ida);

        printf("Burrinho entregou o item ao Jogador %d.\n", id);
        printf("Burrinho retornando para a base. Tempo de retorno: %d segundos.\n", tempo_volta);
        sleep(tempo_volta);

        printf("Burrinho esta de volta a base e disponivel novamente.\n");
        burrinho_disponivel = 1;
        jogador_solicitante = 0;

        pthread_cond_broadcast(&burrinho_disponivel_cond);
        pthread_mutex_unlock(&burrinho_lock);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    pthread_t jogador1, jogador2, entrada_usuario;
    int id1 = 1, id2 = 2;

    //threads responsaveis por simular os jogadores
    pthread_create(&entrada_usuario, NULL, input_usuario, NULL);
    pthread_create(&jogador1, NULL, requisitar_burrinho, (void*)&id1);
    pthread_create(&jogador2, NULL, requisitar_burrinho, (void*)&id2);

    //aguardando as threads (simulacao contínua infinita)
    pthread_join(entrada_usuario, NULL);
    pthread_join(jogador1, NULL);
    pthread_join(jogador2, NULL);

    pthread_mutex_destroy(&burrinho_lock);
    pthread_cond_destroy(&burrinho_disponivel_cond);

    return 0;
}
