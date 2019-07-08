

//BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cria_arquivo();
void preenche_tabuleiro(int casas, int vetor[casas]);

int main() {

    char resposta[515];
    char *dados = NULL;
    int round = 0;
    int i, j, k;
    int casa = 0;
    int player = 0;
    int winner = 0;
    int full = 0;
    FILE *arquivo;

    int tabuleiro[9];
    preenche_tabuleiro(9, tabuleiro);


    arquivo = fopen("JogoDaVelha.txt", "r+b");

    if(arquivo == NULL) {
        cria_arquivo();
        arquivo = fopen("JogoDaVelha.txt", "r+b");

        fwrite(&round, 1, sizeof(int), arquivo);
        fwrite(tabuleiro, 9, sizeof(int), arquivo);
    }
    else{

        fread(&player, 1, sizeof(int), arquivo);

        fseek(arquivo, sizeof(int), SEEK_SET);
        fread(tabuleiro, 9, sizeof(int), arquivo);

        dados = getenv("QUERY_STRING");

        if(dados == NULL){
            snprintf(resposta, sizeof resposta, "Erro na leitura da URL");
        }
        else{

            if(player==0 || player==2)
                player = 1;
            else
                player = 2;

            fseek(arquivo, 0, SEEK_SET);
            fwrite(&player, 1, sizeof(int), arquivo);

            if(casa < 1)
                casa = 1;
            if(casa > 9)
                casa = 9;

            fseek(arquivo, (2+casa)* sizeof(int), SEEK_SET);
            fwrite(&round, 1, sizeof(int), arquivo);

            fseek(arquivo, (2+casa)* sizeof(int), SEEK_SET);
            fread(&tabuleiro[casa - 1], 1, sizeof(int), arquivo);

        }

    }

    if((tabuleiro[0]==1 && tabuleiro[1]==1 && tabuleiro[2] == 1) || (tabuleiro[3]==1 && tabuleiro[4]==1 && tabuleiro[5] ==1)
        || (tabuleiro[6]==1 && tabuleiro[7]==1 && tabuleiro[8] == 1) || (tabuleiro[0]==1 && tabuleiro[3]==1 && tabuleiro[6] == 1)
            || (tabuleiro[1]==1 && tabuleiro[4]==1 && tabuleiro[7] == 1) || (tabuleiro[2]==1 && tabuleiro[5]==1 && tabuleiro[8] == 1)
                || (tabuleiro[0]==1 && tabuleiro[4]==1 && tabuleiro[8] == 1) || (tabuleiro[6]==1 && tabuleiro[4]==1 && tabuleiro[2] == 1)){
        winner = 1;
    }

    if((tabuleiro[0]==2 && tabuleiro[1]==2 && tabuleiro[2] == 2) || (tabuleiro[3]==2 && tabuleiro[4]==2 && tabuleiro[5] ==2)
        || (tabuleiro[6]==2 && tabuleiro[7]==2 && tabuleiro[8] == 2) || (tabuleiro[0]==2 && tabuleiro[3]==2 && tabuleiro[6] == 2)
            || (tabuleiro[1]==2 && tabuleiro[4]==2 && tabuleiro[7] == 2) || (tabuleiro[2]==2 && tabuleiro[5]==2 && tabuleiro[8] == 2)
                || (tabuleiro[0]==2 && tabuleiro[4]==2 && tabuleiro[8] == 2) || (tabuleiro[6]==2 && tabuleiro[4]==2 && tabuleiro[2] == 2)){
        winner = 2;
    }

    i = 0;
    while(i < 9) {
        if (tabuleiro[i] != 0)
            full += 1;
        i += 1;
    }

    if((full == 9) || (winner != 0)){
        if(winner == 0){
            snprintf(resposta, sizeof(resposta), "Deu velha!!");
        }
        else{
            if(winner == 1){
                snprintf(resposta, sizeof(resposta), "O foi o vencedor desse round!!");
            }
            else if(winner == 2){
                snprintf(resposta, sizeof(resposta), "X foi o vencedor desse round!!");
            }
        }

        i = 0;
        while(i < 9){
            tabuleiro[i] = 0;
            i += 1;
        }

        fseek(arquivo, sizeof(int), SEEK_SET);
        fwrite(tabuleiro, 9, sizeof(int), arquivo);
    }

    // Imprimindo os resultados e leituras do arquivo
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
                i = 0;
                while(i < 9){

                    printf("<a");
                    if (tabuleiro[i] == 0) printf(" href=\"?bloco=%d&proxJogada=%d\"", i+1, player); // Se ningu�m tiver jogado, insere a refer�ncia de quais s�o os par�metros para efetivar a jogada naquele bloco
                    printf(">");
                    printf("<div class=\"cell%d\"></div>", tabuleiro[i]); // Imprime 0 se ningu�m tiver jogado, 1 se a jogada for O e 2 se for X
                    printf("</a>");
                    i += 1;
                }
            printf("</div>");
            printf("<div id=\"log\">%s</div>", resposta);
        printf("</body>");
    printf("</html>");

    fclose(arquivo);
    return 0;
}


void cria_arquivo(){

    FILE *arquivo;
    arquivo = fopen("JogoDaVelha.txt", "w+b");
    fclose(arquivo);
}

void preenche_tabuleiro(int casas, int vetor[casas]){
    //CONTADORES
    int i = 0;

    //PREENCHE O TABULEIRO
    while(i < 9) {
        vetor[i] = 0;
        i += 1;
    }
}
