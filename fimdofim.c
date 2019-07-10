//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sobre o programa: O programa tem o objetivo de criar/lêr um arquivo binário com informações sobre um jogo da  //
//                  velha. O programa utiliza procedimentos para otimizar o código, recebe os dados da rodada   //
//                  pela URL do navegador, e imprime o HTML de resposta para o navegador.                       //
//                                                                                                              //
// Objetivo do programa: Rodar com sucesso o jogo da velha proposto pelos autores do mesmo.                     //
//                                                                                                              //
//                                                                                                              //
// Autores: Thiago de Moraes Teixeira   RA: 760667                                                              //
//          Álvaro Pedro Magalhães      RA: 587818                                                              //
//                                                                                                              //
// Data: 10/07/2019                                                                                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//BIBLIOTECAS UTILIZADAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FUNÇÕES E PROCEDIMENTOS UTILIZADOS NO PROGRAMA
void cria_arquivo();
void preenche(int casas, int vetor[casas]);

//PROGRAMA
int main() {

    //MENSAGEM QUE SERÁ ENVIADA PARA O HTML COM O RESULTADO DE CADA ACONTECIMENTO
    char resposta[50] = {'\n'};

    //CONTADORES
    int i, j;

    //VÁRIAVEL PARA RECEBER OS DADOS DA URL
    char *dados = NULL;

    //VÁRIAVEIS PARA CONTROLAR O JOGO
    int round = 0;    //RODADA
    int casa = 0;     //CASA ESCOLHIDA PELO JOGADOR
    int player = 0;   //JOGADOR DA VEZ
    int winner = 0;   //VENCEDOR
    int full = 0;     //SE O TABULEIRO ESTIVER COMPLETO
    int score[2];     //VERIRIFCAR O PLACAR DE ROUNDS PARA CADA JOGADOR

    //VÁRIAVEL PARA UTILIZAR O ARQUIVO
    FILE *arquivo;

    //DECLARA O TABULEIRO EM FORMATO DE VETOR
    int tabuleiro[9];
    //PREENCHE O TABULEIRO E O SCORE COM 0 'ZEROS' PARA INICIAR
    preenche(9, tabuleiro);
    preenche(2, score);

    //ABRE O ARQUIVO
    arquivo = fopen("JogoDaVelha.bin", "r+b");

    //SE O ARQUIVO NÃO EXISTIR, ELE É CRIADO
    if(arquivo == NULL) {
        //CRIA O ARQUIVO E O PREENCHE COM AS INFORMAÇÕES DE JOGADA E COM O TABULEIRO
        cria_arquivo();
        arquivo = fopen("JogoDaVelha.bin", "r+b");

        //SE O ARQUIVO NÃO PUDER SER CRIADO
        if(arquivo == NULL)
            snprintf(resposta, sizeof resposta, "Erro ao criar o arquivo!");
        else {
            snprintf(resposta, sizeof resposta, "INÍCIO");
            fwrite(score, 2, sizeof(int), arquivo);
            fwrite(&round, 1, sizeof(int), arquivo);
            fwrite(tabuleiro, 9, sizeof(int), arquivo);
            fflush(arquivo);
        }
    }
    else{
        //SE O ARQUIVO JÁ EXISTIR, APENAS LÊ AS INFORMAÇÕES DO JOGO ATUAL UTILIZA FSEEKS PARA ASEGURAR A LOCALIZAÇÃO DO DADO
        fseek(arquivo, 0, SEEK_SET);
        fread(score, 2, sizeof(int), arquivo);

        fseek(arquivo, 2* sizeof(int), SEEK_SET);
        fread(&player, 1, sizeof(int), arquivo);

        fseek(arquivo, 3* sizeof(int), SEEK_SET);
        fread(tabuleiro, 9, sizeof(int), arquivo);

        //RECEBE OS DAOS DA URL
        dados = getenv("QUERY_STRING");

        //RELATA SE HOUVER ALGUM ERRO
        if(dados == NULL)
            snprintf(resposta, sizeof resposta, "Erro na leitura da URL");
        else if(sscanf(dados, "casa=%d&round=%d", &casa, &round) != 2){
            snprintf(resposta, sizeof resposta, "INÍCIO");
            score[0] = 0;
            score[1] = 0;
            fseek(arquivo, 0, SEEK_SET);
            fwrite(score, 2, sizeof(int), arquivo);
            fflush(arquivo);
        }
        else{
            //LÊ QUAL É O JOGADOR DA RODADA E JÁ FAZ A ALTERAÇÃO PARA A PRÓXIMA
            if(player==0 || player==2)
                player = 1;
            else
                player = 2;

            //GRAVA O JOGADOR DA RODADA NO ARQUIVO
            fseek(arquivo, 2* sizeof(int), SEEK_SET);
            fwrite(&player, 1, sizeof(int), arquivo);
            fflush(arquivo);

            //PREVINE ERROS, SE O PARÂMETRO VIER INVÁLIDO NA URL
            if(casa < 1)
                casa = 1;
            if(casa > 9)
                casa = 9;

            //GRAVA A RODADA NO ARQUIVO
            fseek(arquivo, (2+casa)* sizeof(int), SEEK_SET);
            fwrite(&round, 1, sizeof(int), arquivo);
            fflush(arquivo);

            //ATUALIZA O TABULEIRO COM A JOGADA FEITA
            fseek(arquivo, (2+casa)* sizeof(int), SEEK_SET);
            fread(&tabuleiro[casa - 1], 1, sizeof(int), arquivo);

        }

    }

    //VERIFICA TODAS AS POSSIBILIDADES DE GANHAR DO JOGADOR 'O'
    if((tabuleiro[0]==1 && tabuleiro[1]==1 && tabuleiro[2] == 1) || (tabuleiro[3]==1 && tabuleiro[4]==1 && tabuleiro[5] ==1)
        || (tabuleiro[6]==1 && tabuleiro[7]==1 && tabuleiro[8] == 1) || (tabuleiro[0]==1 && tabuleiro[3]==1 && tabuleiro[6] == 1)
            || (tabuleiro[1]==1 && tabuleiro[4]==1 && tabuleiro[7] == 1) || (tabuleiro[2]==1 && tabuleiro[5]==1 && tabuleiro[8] == 1)
                || (tabuleiro[0]==1 && tabuleiro[4]==1 && tabuleiro[8] == 1) || (tabuleiro[6]==1 && tabuleiro[4]==1 && tabuleiro[2] == 1)){
        winner = 1;
    }

    //VERIFICA TODAS AS POSSIBILIDADES DE GANHAR DO JOGADOR 'X'
    if((tabuleiro[0]==2 && tabuleiro[1]==2 && tabuleiro[2] == 2) || (tabuleiro[3]==2 && tabuleiro[4]==2 && tabuleiro[5] ==2)
        || (tabuleiro[6]==2 && tabuleiro[7]==2 && tabuleiro[8] == 2) || (tabuleiro[0]==2 && tabuleiro[3]==2 && tabuleiro[6] == 2)
            || (tabuleiro[1]==2 && tabuleiro[4]==2 && tabuleiro[7] == 2) || (tabuleiro[2]==2 && tabuleiro[5]==2 && tabuleiro[8] == 2)
                || (tabuleiro[0]==2 && tabuleiro[4]==2 && tabuleiro[8] == 2) || (tabuleiro[6]==2 && tabuleiro[4]==2 && tabuleiro[2] == 2)){
        winner = 2;
    }

    //VERIFICA QUAIS CASAS DO TABULEIRO JÁ FORAM USADAS
    i = 0;
    while(i < 9) {
        if (tabuleiro[i] != 0)
            full += 1;
        i += 1;
    }

    //HOUVE ALGUM GANHADOR OU O TABULEIRO ESTÁ CHEIO
    if((full == 9) || (winner != 0)){
        //NÃO HOUVE GANHADOR - DEU EMPATE
        if(winner == 0){
            snprintf(resposta, sizeof(resposta), "Deu velha!!");
        }
        //HOUVE GANHADOR
        else{
            //JOGADOR 'O' GANHOU
            if(winner == 1){
                snprintf(resposta, sizeof(resposta), "O - foi o vencedor desse round!!");
                score[0] += 1;
            }
            //JOGADOR 'X' GANHOU
            if(winner == 2){
                snprintf(resposta, sizeof(resposta), "X - foi o vencedor desse round!!");
                score[1] += 1;
            }

            fseek(arquivo, 0, SEEK_SET);
            fwrite(score, 2, sizeof(int), arquivo);
            fflush(arquivo);
        }

        //ZERA O TABULEIRO PARA A PROXIMA PARTIDA
        i = 0;
        while(i < 9){
            tabuleiro[i] = 0;
            i += 1;
        }

        //PASSA O TABULEIRO ZERADO PARA O ARQUIVO
        fseek(arquivo, 3*sizeof(int), SEEK_SET);
        fwrite(tabuleiro, 9, sizeof(int), arquivo);
        fflush(arquivo);
    }

    //IMPRIMINDO OS RESULTADOS EM HTML
    printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
    printf("<html>");
        printf("<head>");
            printf("<meta charset=\"utf-8\">");
            printf("<title>Jogo da velha</title>");
            printf("<link rel=\"stylesheet\" href=\"../style.css\">");
        printf("</head>");
        printf("<body class=\"prox%d\">", player);
            printf("<a href=\"?\">");
                printf("<div id=\"placar\">");
                    printf("<div id=\"o\">");
                        printf("<span>O</span>");
                        printf("<br>");
                        printf("<span>%d</span>", score[0]);
                    printf("</div>");
                    printf("<div id=\"x\">");
                        printf("<span>X</span>");
                        printf("<br>");
                        printf("<span>%d</span>", score[1]);
                    printf("</div>");
                printf("</div>");
            printf("</a>");
            printf("<div id=\"jogo\">");
                for(j=0; j<9; j++){
                    printf("<a");
                    if (tabuleiro[j] == 0)
                        printf(" href=\"?casa=%d&round=%d\"", j + 1, player);
                    printf(">");
                        printf("<div class=\"cell%d\"></div>", tabuleiro[j]);
                    printf("</a>");
                }
            printf("</div>");
            printf("<div id=\"log\">%s</div>", resposta);
        printf("</body>");
    printf("</html>");

    fclose(arquivo);
    return 0;
}

/* PROCEDIMENTO
 * NOME: cria_arquivo
 * OBJETIVO: Criar o arquivo que será usado no programa
 */
void cria_arquivo(){

    //ARQUIVO QUE SERÁ CRIADO
    FILE *arquivo;

    arquivo = fopen("JogoDaVelha.bin", "w+b");
    fclose(arquivo);
}

/* PROCEDIMENTO
 * NOME: preenche_tabuleiro
 * OBJETIVO: Preencher o vetor do tabuleiro com zeros para iniciar
 * PARÂMETROS FORMAIS:
 *      - int casas = Número de casas do tabuleiro
 *      - int vetor[casas] = Vetor já criado com o tamanho exato
 */
void preenche(int casas, int vetor[casas]){
    //CONTADORES
    int i = 0;

    //PREENCHE O TABULEIRO
    while(i < casas) {
        vetor[i] = 0;
        i += 1;
    }
}
