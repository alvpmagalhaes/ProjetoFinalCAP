//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sobre o programa: O programa tem o objetivo de criar/lêr um arquivo binário com informações sobre um jogo da  //
//                  velha. O programa utiliza procedimentos para otimizar o código, recebe os dados da rodada   //
//                  pela URL do navegador, e imprime o HTML de resposta para o navegador.                       //
//                                                                                                              //
// Objetivo do programa: Rodar com sucesso o jogo da velha proposto pelos autores do mesmo.                     //
//                                                                                                              //
//                                                                                                              //
// Autores: Thiago de Moraes Teixeira   RA: 760667                                                              //
//          Álvaro        RA:                                                                                   //
//                                                                                                              //
// Data: 10/07/2019                                                                                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//BIBLIOTECAS UTILIZADAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FUNÇÕES E PROCEDIMENTOS UTILIZADOS NO PROGRAMA
void cria_arquivo();
void preenche_tabuleiro(int casas, int vetor[casas]);

//PROGRAMA
int main() {

    //MENSAGEM QUE SERÁ ENVIADA PARA O HTML COM O RESULTADO DE CADA ACONTECIMENTO
    char resposta[515];

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

    //VÁRIAVEL PARA UTILIZAR O ARQUIVO
    FILE *arquivo;

    //DECLARA O TABULEIRO EM FORMATO DE VETOR
    int tabuleiro[9];
    //PREENCHE O TABULEIRO COM 0 'ZEROS' PARA INICIAR
    preenche_tabuleiro(9, tabuleiro);

    //ABRE O ARQUIVO
    arquivo = fopen("JogoDaVelha.txt", "r+b");

    //SE O ARQUIVO NÃO EXISTIR, ELE É CRIADO
    if(arquivo == NULL) {
        //CRIA O ARQUIVO E O PREENCHE COM AS INFORMAÇÕES DE JOGADA E COM O TABULEIRO
        cria_arquivo();
        arquivo = fopen("JogoDaVelha.txt", "r+b");

        fwrite(&round, 1, sizeof(int), arquivo);
        fwrite(tabuleiro, 9, sizeof(int), arquivo);
    }
    else{
        //SE O ARQUIVO JÁ EXISTIR, APENAS LÊ AS INFORMAÇÕES DO JOGO ATUAL
        fread(&player, 1, sizeof(int), arquivo);
        fread(tabuleiro, 9, sizeof(int), arquivo);

        //RECEBE OS DAOS DA URL
        dados = getenv("QUERY_STRING");

        //RELATA SE HOUVER ALGUM ERRO
        if(dados == NULL)
            snprintf(resposta, sizeof resposta, "Erro na leitura da URL");
        else{
            //LÊ QUAL É O JOGADOR DA RODADA E JÁ FAZ A ALTERAÇÃO PARA A PRÓXIMA
            if(player==0 || player==2)
                player = 1;
            else
                player = 2;

            //GRAVA O JOGADOR DA RODADA NO ARQUIVO
            fseek(arquivo, 0, SEEK_SET);
            fwrite(&player, 1, sizeof(int), arquivo);

            //PREVINE ERROS, SE O PARÂMETRO VIER INVÁLIDO NA URL
            if(casa < 1)
                casa = 1;
            if(casa > 9)
                casa = 9;

            //GRAVA O JOGADOR DA PRÓXIMA RODADA
            fseek(arquivo, (2+casa)* sizeof(int), SEEK_SET);
            fwrite(&round, 1, sizeof(int), arquivo);

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
            }
            //JOGADOR 'X' GANHOU
            if(winner == 2){
                snprintf(resposta, sizeof(resposta), "X - foi o vencedor desse round!!");
            }
        }

        //ZERA O TABULEIRO PARA A PROXIMA PARTIDA
        i = 0;
        while(i < 9){
            tabuleiro[i] = 0;
            i += 1;
        }

        //PASSA O TABULEIRO ZRADO PARA O ARQUIVO
        fseek(arquivo, sizeof(int), SEEK_SET);
        fwrite(tabuleiro, 9, sizeof(int), arquivo);
    }

    //IMPRIMINDO OS RESULTADOS EM HTML
    printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
    printf("<html>");
        printf("<head>");
            printf("<meta charset=\"utf-8\">");
            printf("<title>Jogo da velha - CGI + Arquivos</title>");
            printf("<link rel=\"stylesheet\" href=\"../style.css\">");
        printf("</head>");
        printf("<body class=\"prox%d\">", player); // Para o CSS "saber" quem vai jogar e mudar o fundo do placar, aplica-se a classe (prox1 / prox2) � tag body e utiliza-se o seletor ("body.prox1 > a > div#placar > div#o" / "body.prox2 > a > div#placar > div#x")
            printf("<a href=\"?\">"); // Ao utilizar um link sem definir nenhum endere�o, ele reabrir� o mesmo arquivo. Ao colocar "?", ele abre o mesmo arquivo sem nenhum par�metro de URL
                printf("<div id=\"placar\">");
                    printf("<div id=\"o\">");
                        printf("<span>O</span>");
                        printf("<br>");
                    printf("</div>");
                    printf("<div id=\"x\">");
                        printf("<span>X</span>");
                        printf("<br>");
                    printf("</div>");
                printf("</div>");
            printf("</a>");
            printf("<div id=\"jogo\">");
                /*
                    Utiliza-se links para reabrir o arquivo e passando por par�metro
                    qual � o bloco a ser aplicado e de quem � a jogada. As classes
                    "cell0", "cell1" e "cell2" servem para o CSS exibir uma imagem
                    no fundo do bloco de quem fez a jogada naquele bloco.
                */
                j = 0;
                while(j < 9){

                    printf("<a");
                    if (tabuleiro[j] == 0) printf(" href=\"?bloco=%d&proxJogada=%d\"", j+1, player); // Se ningu�m tiver jogado, insere a refer�ncia de quais s�o os par�metros para efetivar a jogada naquele bloco
                    printf(">");
                    printf("<div class=\"cell%d\"></div>", tabuleiro[j]); // Imprime 0 se ningu�m tiver jogado, 1 se a jogada for O e 2 se for X
                    printf("</a>");
                    j += 1;
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

    arquivo = fopen("JogoDaVelha.txt", "w+b");
    fclose(arquivo);
}

/* PROCEDIMENTO
 * NOME: preenche_tabuleiro
 * OBJETIVO: Preencher o vetor do tabuleiro com zeros para iniciar
 * PARÂMETROS FORMAIS:
 *      - int casas = Número de casas do tabuleiro
 *      - int vetor[casas] = Vetor já criado com o tamanho exato
 */
void preenche_tabuleiro(int casas, int vetor[casas]){
    //CONTADORES
    int i = 0;

    //PREENCHE O TABULEIRO
    while(i < 9) {
        vetor[i] = 0;
        i += 1;
    }
}
