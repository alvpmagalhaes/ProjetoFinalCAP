/********************************************************************/
/*	Universidade Federal de São Carlos - UFSCar 
/*	Centro de Ciências Exatas e Tecnologia - CCET
/*	Departamento de Computação - DC
/*	Construção de Algoritmos e Programação

/* Objetivo: 
/*	Autor: Alvaro Pedro Magalhães RA: 587818
/*	Data: 

/********************************************************************/

#include <stdio.h>
#include <stdlib.h>


int main(){
	
    
	char* dados = NULL;
    char resposta[512] = {'\n'};

    FILE *arquivo;
	
	// Abre o arquivo já criado
	arquivo = fopen("jogoVelha.bin", "r+b");
	
	if (arquivo == NULL) {
		
		// Se o arquivo não existir, tenta criá-lo
		arquivo = fopen("jogoDaVelha.bin", "w+b");
		
		if (arquivo == NULL) {
			
			snprintf(resposta, sizeof resposta, "O arquivo não pôde ser criado/lido.");
		
		} else {
			
			// Ao conseguir criar o arquivo, insere todos os campos com o valor 0
			fwrite(placar, 2, sizeof(int), arquivo);
			fwrite(&proxJogada, 1, sizeof(int), arquivo);
			fwrite(blocos, 9, sizeof(int), arquivo);
			fflush(lf);
			
		}

    }    

    int cont_jogadas,l,c, vez = 0,i,j;
    do {
        cont_jogadas = 1;
        for(i=0;i<=2;i++){
            for(j=0;j<=2;j++){
                casas[i][j] = ' ';
            }
        }
        do
        {
            if(vez%2==0){
                printf("Jogador X\n");
            }else
            {
                printf("Jogador O\n");
            }
            
            printf("Digite a linha: ");
            scanf("%d", &l);
            printf("Digite a coluna: ");
            scanf("%d", &c);

            if(l < 1 || c < 1 || l > 3 || c > 3){
                //jogada inválida
                l = 0;
                c = 0;
            }else if(casas[l-1][c-1] != ' '){
                //também jogada inválida
                l = 0;
                c = 0;
            }else{
                if(vez%2==0){
                    casas[l-1][c-1] = 'X';
                }else{
                    casas[l-1][c-1] = 'O';
                }
                vez++;
                cont_jogadas++;
            }   

            if(casas[0][0]=='X' && casas[0][1]=='X' && casas[0][2] == 'X'){ cont_jogadas = 11;}
            if(casas[1][0]=='X' && casas[1][1]=='X' && casas[1][2] == 'X'){ cont_jogadas = 11;}
            if(casas[2][0]=='X' && casas[2][1]=='X' && casas[2][2] == 'X'){ cont_jogadas = 11;}
            if(casas[0][0]=='X' && casas[1][0]=='X' && casas[2][0] == 'X'){ cont_jogadas = 11;}
            if(casas[0][1]=='X' && casas[1][1]=='X' && casas[2][1] == 'X'){ cont_jogadas = 11;}
            if(casas[0][2]=='X' && casas[1][2]=='X' && casas[2][2] == 'X'){ cont_jogadas = 11;}
            if(casas[0][0]=='X' && casas[1][1]=='X' && casas[2][2] == 'X'){ cont_jogadas = 11;}
            if(casas[0][2]=='X' && casas[1][1]=='X' && casas[2][0] == 'X'){ cont_jogadas = 11;}

            if(casas[0][0]=='O' && casas[0][1]=='O' && casas[0][2] == 'O'){ cont_jogadas = 12;}
            if(casas[1][0]=='O' && casas[1][1]=='O' && casas[1][2] == 'O'){ cont_jogadas = 12;}
            if(casas[2][0]=='O' && casas[2][1]=='O' && casas[2][2] == 'O'){ cont_jogadas = 12;}
            if(casas[0][0]=='O' && casas[1][0]=='O' && casas[2][0] == 'O'){ cont_jogadas = 12;}
            if(casas[0][1]=='O' && casas[1][1]=='O' && casas[2][1] == 'O'){ cont_jogadas = 12;}
            if(casas[0][2]=='O' && casas[1][2]=='O' && casas[2][2] == 'O'){ cont_jogadas = 12;}
            if(casas[0][0]=='O' && casas[1][1]=='O' && casas[2][2] == 'O'){ cont_jogadas = 12;}
            if(casas[0][2]=='O' && casas[1][1]=='O' && casas[2][0] == 'O'){ cont_jogadas = 12;}

        } while (cont_jogadas<=9);

        if(cont_jogadas == 10){
            printf("Jogo Empatou\n");
        }if(cont_jogadas==11){
            printf("Vencedor X\n");
        }if(cont_jogadas==12){
            printf("Vencedor O\n");
        }

        printf("Deseja jogar Novamente?[S-N]\n");
        scanf("%s", &res);
    }while(res=='s');


	return 0;
}