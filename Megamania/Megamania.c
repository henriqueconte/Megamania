#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <conio2.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define MAXINIMIGOS 15
// 80 colunas e 24 linhas!!!

typedef struct inimigos
{
    char corpo_inimigo[5];
    int coordx[5];
    int coordy;
    int vivo;
    int atira;

} INIMIGOS;

typedef struct nave
{
    char corpo_nave[6];
    int vivo;
    int coordx_nave;
} NAVE;

void move (char tecla, char Interface_jogo[][80], int *pont_coluna, NAVE nave);
void inicializacao_jogo (char Interface_jogo[][80], NAVE nave);
void inicializacao_status(char Interface_status[][80]);
void imprime_matriz (char Interface_jogo [][80], char Interface_status [][80], int *pontos);
void print_inimigos(char Interface_jogo[][80], int num_inimigos, INIMIGOS array_inimigos[], char direcao);
void moveinimigos(char Interface_jogo[][80], int num_inimigos, INIMIGOS array_inimigos[], char direcao);
void hidecursor(void);
void cria_tiro (char Interface_jogo[][80], int *pont_coluna);
void move_tiro(char Interface_jogo[][80], int *pont_coluna, int ajusta_tiro);
int menu(void);
int ajusta_energia(float diff_energia, int energia, char Interface_status[][80], int ajusta_status);
void pausa_jogo(char Interface_jogo[][80], char Interface_status[][80], int *pontos);
int le_nivel (int cont_nivel, INIMIGOS array_inimigos[10], char *direcao, int *velinimigos, char Interface_jogo[][80], char Interface_status[][80]);
void morte_aos_inimigos (char Interface_jogo[][80], int num_inimigos, INIMIGOS array_inimigos[], int *pont_coluna, int ajusta_tiro, int *flag_tiro, int *pontos);
void pontos_final (char Interface_status[][80], int energia, INIMIGOS array_inimigos[], int *pontos, int num_inimigos);
void tiros_inimigos (char Interface_jogo[][80], INIMIGOS array_inimigos[], int num_inimigos, int *linha_tiro_inim, int *coluna_tiro_inim, int num_aleatorio);
int move_tiro_inim (char Interface_jogo[][80], int coluna_tiro, int *linha_tiro_inim, INIMIGOS array_inimigos[], int num_aleatorio);
int morte_nave(char Interface_jogo[][80], char Interface_status[][80], int *pontos, int *pont_coluna_nave, int coluna_tiro_inim, int *vidas, NAVE nave, int *contvidas, int *sair_jogo, int *volta_menu, int *ajusta_status, int *energia);
int restart(char Interface_jogo[][80], char Interface_status[][80], int *pontos);
void inicializa_variaveis (int *vida, int *energia, int *pontos, int *contvidas, int *sair_jogo, int *ajusta_status);
void vitoria(char Interface_jogo[][80], char Interface_status[][80]);
void musicastarwars(void);
void musicamario(void);

int main ()
{
    // VARIÁVEIS PARA INICIALIZAR: vida, energia, pontos, contvidas

    char Interface_jogo [19][80], Interface_status [5][80];
    char tecla;
    int vida = 4, energia = 40, pause=1;
    int pont_energia, *pont_ajusta_tiro, ajusta_coluna = 79, ajusta_status =62, ajusta_tiro = 16;
    int cont=0, cont_tiro=0, flag_tiro=0, flag_cria_tiro = 0,  menu_opcao = 0, velinimigos = 90, num_inimigos = 0;
    int pontos=0, num_rand = 0, linha_tiro_inim=0, coluna_tiro_inim=0, num_aleatorio = 0, pode_atirar = 0, contvidas = 17;
    int sair_jogo = 1, volta_menu = 0, cont_nivel = 1, todos_inim_mortos, linha, coluna, cont_som = 0;
    char direcao;

    float diff_energia = 0, diff_inimigo = 0, diff_tiro_inim = 0;
    FILE *nivel;
    char nomenivel[20];
    INIMIGOS array_inimigos[MAXINIMIGOS];
    NAVE nave;

    clock_t tempo_atual, tempo_antes;
    tempo_antes = clock();



    hidecursor();    // função que esconde o cursor

    menu_opcao = menu();    // escolha do menu

    if (menu_opcao == 1) // caso tenha sido escolhida a opção "play" dentro do menu, o jogo começa
    {
    while(volta_menu == 0)     // enquanto o jogador não clicar "exit", o jogo continua nesse laço
    {
        if (todos_inim_mortos == 1)     // condição para saber se todos os inimigos estão mortos. Caso estejam, troca de nível
        inicializa_variaveis (&vida, &energia, &pontos, &contvidas, &sair_jogo, &ajusta_status);   // função que inicializa as variáveis

        else
        {
            strcpy(nomenivel, "nivel 1.txt");    // copia o nome do nível à string, de modo a abrir o arquivo texto correto
            cont_nivel = 1;    // indica que está no nível 1
            inicializa_variaveis (&vida, &energia, &pontos, &contvidas, &sair_jogo, &ajusta_status);
            pontos = 0;
        }


        //else
            //inicializa_nivel_2

        //  menu_opcao = menu();

            num_inimigos = le_nivel(cont_nivel, array_inimigos, &direcao, &velinimigos, Interface_jogo, Interface_status);   // determina o número de inimigos

            if(num_inimigos == 0)   // caso o número de inimigos seja 0, sai do jogo, pois não há inimigos
            {
                volta_menu = 2;
            }
            else
            {
                srand(time(NULL));   // inicializa o contador aleatório


                // declarando valores a cada estrutura de inimigo
                for(cont=0; cont <= num_inimigos; cont++)
                {
                    array_inimigos[cont].corpo_inimigo[0] = '(';
                    array_inimigos[cont].corpo_inimigo[1] = 'V';
                    array_inimigos[cont].corpo_inimigo[2] = 'n';
                    array_inimigos[cont].corpo_inimigo[3] = 'V';
                    array_inimigos[cont].corpo_inimigo[4] = ')';
                    array_inimigos[cont].vivo=1;
                    array_inimigos[cont].atira = 0;
                }

                // declarando valores para a nave
                nave.coordx_nave = 40;
                nave.corpo_nave[0] = '(';
                nave.corpo_nave[1] = '-';
                nave.corpo_nave[2] = '0';
                nave.corpo_nave[3] = '-';
                nave.corpo_nave[4] = ')';
                nave.corpo_nave[5] = '_';
                nave.vivo = 1;


                inicializacao_jogo(Interface_jogo, nave); //começa a interface inicial
                inicializacao_status(Interface_status); // começa a interface de status
                imprime_matriz(Interface_jogo, Interface_status, &pontos);  // imprime continuamente a matriz
                print_inimigos(Interface_jogo, num_inimigos, array_inimigos, direcao);   // imprime os inimigos
                hidecursor();  // esconde o cursor para não ficar piscando na tela



                // define a cor do jogo


                // imprime em loop, checando se uma tecla foi pressionada

                while (sair_jogo == 1)   // condição interna de um nível
                {
                    tempo_atual = clock();
                    diff_energia += round ((double) (tempo_atual - tempo_antes) / CLOCKS_PER_SEC*1000);   // conta o tempo decorrido desde a última vez que entrou na função
                    diff_inimigo += round ((double) (tempo_atual - tempo_antes) / CLOCKS_PER_SEC*1000);
                    diff_tiro_inim += round ((double) (tempo_atual - tempo_antes) / CLOCKS_PER_SEC*1000);

                    imprime_matriz(Interface_jogo, Interface_status, &pontos);


                    //condição para se tecla for pressionada
                    if(_kbhit())
                    {
                        tecla = _getch();
                        tecla = tolower(tecla);
                        if(tecla==' ' && flag_tiro == 0)       // caso a tecla apertada seja barra de espaço, e a nave possa atirar, a nave atira
                        {
                            cria_tiro(Interface_jogo, &nave.coordx_nave);      // função que cria um tiro
                            flag_tiro=1;           // flag que impede a criação de outros tiros
                            //flag_cria_tiro = 1;
                        }
                        else if (tecla == 'p')    // pausa o jogo
                        {
                            pausa_jogo(Interface_jogo, Interface_status, &pontos);
                        }
                        else
                            move(tecla, Interface_jogo, &nave.coordx_nave, nave); // chama a função para mover a nave

                    }


                    if(flag_tiro==1) // define se o tiro vai mover ou não
                    {
                        cont_tiro++;
                        if (cont_tiro>=0) // ajusta a velocidade do tiro
                        {
                            move_tiro(Interface_jogo, &nave.coordx_nave, ajusta_tiro);     // função que move o tiro da nave

                            (ajusta_tiro) --;

                            if((ajusta_tiro) == -1)       // a linha do tiro será -1 quando o tiro percorrer toda a matriz
                            {
                                flag_tiro=0;
                            }

                            cont_tiro=0;
                        }

                        morte_aos_inimigos (Interface_jogo, num_inimigos, array_inimigos, &nave.coordx_nave, ajusta_tiro, &flag_tiro, &pontos);  // função que detecta se um inimigo foi morto ou não
                    }
                    else
                        ajusta_tiro = 16;     // reposiciona o parâmetro do tiro na linha 16

                    // ajusta a velocidade dos inimigos

                    if (diff_inimigo > velinimigos)
                    {
                        print_inimigos(Interface_jogo, num_inimigos, array_inimigos, direcao);    // printa os inimigos
                        moveinimigos(Interface_jogo, num_inimigos, array_inimigos, direcao);     // move eles
                        diff_inimigo = 0;         // reseta o contador do tempo

                    }

                    if  (diff_energia >= 1000)        //a cada 1 segundo, a nave perde um de energia
                    {
                        energia = ajusta_energia(diff_energia, energia, Interface_status, ajusta_status);
                        diff_energia = 0;
                        ajusta_status--;           // apaga a representação gráfica da energia a cada 1 segundo
                    }


                    pontos_final (Interface_status, energia, array_inimigos, &pontos, num_inimigos);     // função que multiplica os pontos pela energia restante

                    if (diff_tiro_inim > (velinimigos*20))             // ajusta a velocidade dos tiros dos inimigos
                    {
                        if (pode_atirar == 1)         // permite ou impede os inimigos de atirarem
                        {
                            do
                            {
                                num_aleatorio = (rand()% (num_inimigos));      // gera um número aleatório para escolher qual inimigo atirará
                            }
                            while(array_inimigos[num_aleatorio].vivo == 0);           // procura um número que corresponda a um inimigo vivo
                            pode_atirar = 0;        // impede outros inimigos de atirarem
                        }

                        if (array_inimigos[num_aleatorio].atira == 0 && array_inimigos[num_aleatorio].vivo == 1)   // caso o inimigo possa atirar e esteja vivo, ele atira
                        {
                            tiros_inimigos(Interface_jogo, array_inimigos, num_inimigos, &linha_tiro_inim, &coluna_tiro_inim, num_aleatorio); // cria o tiro do inimigo
                        }
                        diff_tiro_inim = 0;      // zera o tempo entre um tiro e outro
                    }
                    if(pode_atirar == 0)
                        pode_atirar = (move_tiro_inim (Interface_jogo, coluna_tiro_inim, &linha_tiro_inim, array_inimigos, num_aleatorio));

                    {
                        if(linha_tiro_inim == 18)   // caso esteja na última linha do jogo, checa se está colidindo com a nave do jogador ou não
                            if(coluna_tiro_inim == (nave.coordx_nave) || coluna_tiro_inim == (nave.coordx_nave)+1 || coluna_tiro_inim == (nave.coordx_nave)+2 || coluna_tiro_inim == (nave.coordx_nave)-1 || coluna_tiro_inim == (nave.coordx_nave)-2)
                                morte_nave(Interface_jogo, Interface_status, &pontos, &nave.coordx_nave, coluna_tiro_inim, &vida, nave, &contvidas, &sair_jogo, &volta_menu, &ajusta_status, &energia);
                    }                                                            //caso esteja colidindo, mata a nave do jogador

                    // testa se há algum vivo
                    todos_inim_mortos = 1;
                    for(cont = 0; cont < num_inimigos; cont++)
                    {
                        if(array_inimigos[cont].vivo == 1)
                            todos_inim_mortos = 0;
                    }

                    // se não houver, chama o proximo nivel
                    if(todos_inim_mortos == 1)
                    {
                        cont_nivel++;

                        sair_jogo = 2;

                        if(cont_nivel < 4)
                        {
                            Interface_jogo[10][38] = 'L';
                            Interface_jogo[10][39] = 'E';
                            Interface_jogo[10][40] = 'V';           // imprime qual nível será jogado
                            Interface_jogo[10][41] = 'E';
                            Interface_jogo[10][42] = 'L';
                            Interface_jogo[10][43] = ' ';
                            Interface_jogo[10][44] = cont_nivel + '0';
                            imprime_matriz(Interface_jogo, Interface_status, &pontos);

                            for (cont_som = 100; cont_som < 500; cont_som += 20)
                                Beep (cont_som, 50);
                            Sleep(2000);

                            Interface_jogo[20][38] = ' ';
                            Interface_jogo[20][38] = ' ';
                            Interface_jogo[20][38] = ' ';
                            Interface_jogo[20][38] = ' ';
                            Interface_jogo[20][38] = ' ';
                            Interface_jogo[20][38] = ' ';
                            Interface_jogo[20][38] = ' ';
                        }
                    }

                    if (vida == 0)
                        sair_jogo = 0;   // caso o jogador tenha morrido, vai para a opção de jogar novamente

                    if (energia == 0)    // perde uma vida caso a energia seja 0
                        morte_nave(Interface_jogo, Interface_status, &pontos, &nave.coordx_nave, coluna_tiro_inim, &vida, nave, &contvidas, &sair_jogo, &volta_menu, &ajusta_status, &energia);

                    tempo_antes = tempo_atual;      // atualiza o contador do tempo

                    if (sair_jogo == 0)      // chama o gameover
                    {
                        morte_nave(Interface_jogo, Interface_status, &pontos, &nave.coordx_nave, coluna_tiro_inim, &vida, nave, &contvidas, &sair_jogo, &volta_menu, &ajusta_status, &energia);
                    }


                }
            }
        }
    }

    musicastarwars();      // toca música do star wars na saída do jogo
    return 0;
}

//iguala toda a matriz como espaço e cria a nave na coordenada inicial
void inicializacao_jogo (char Interface_jogo[19][80], NAVE nave)
{
    int coluna, linha;

    for (coluna = 0; coluna < 80; coluna++)
    {
        for (linha = 0; linha < 19; linha++)
        {
            Interface_jogo[linha][coluna]= ' ';
        }
    }

    // coordenadas inciais da nave
    Interface_jogo [18][38]= nave.corpo_nave[0];
    Interface_jogo [18][39]= nave.corpo_nave[1];
    Interface_jogo [18][40]= nave.corpo_nave[2];
    Interface_jogo [18][41]= nave.corpo_nave[3];
    Interface_jogo [18][42]= nave.corpo_nave[4];
    Interface_jogo [17][40]= nave.corpo_nave[5];
}

void inicializacao_status (char Interface_status[5][80]) // muda a matriz do status de jogo
{
    int coluna, linha;


    for(coluna=0; coluna<80; coluna++)//cria uma linha de # na linha
        Interface_status[0][coluna]= ' ';


    for(coluna=0; coluna<80; coluna++)
        Interface_status[4][coluna]= ' '; //cria uma linha de # na linha

    for(linha=1; linha<4; linha++) //enquadra a caixa nas linhas entre as
    {
        for(coluna=0; coluna<80; coluna+=79) //bota a # na primeira coluna e na ultima
        {
            Interface_status[linha][coluna]= ' ';
        }
    }

    for (linha=1; linha < 4; linha++) //cria a area dentro do quadrado
    {
        for (coluna=1; coluna<79; coluna++)
            Interface_status[linha][coluna]= ' ';
    }

    Interface_status[2][15]= 'E';
    Interface_status[2][16]= 'n';
    Interface_status[2][17]= 'e';
    Interface_status[2][18]= 'r';
    Interface_status[2][19]= 'g';
    Interface_status[2][20]= 'y';

    Interface_status[2][22]= '[';
    Interface_status[2][63]= ']';

    for (coluna=23; coluna<63; coluna++)
    {
        Interface_status[2][coluna]= '-';
    }


    Interface_status[3][15]= 'ñ';
    Interface_status[3][16]= 'ñ';
    Interface_status[3][17]= 'ñ';

}

void imprime_matriz (char Interface_jogo[19][80], char Interface_status [5][80], int *pontos)   // essa função imprime a matriz inteira atualizada conforme as mudanças ocorridas em outras funçoes
{
    int coluna, linha;

    gotoxy(1, 1);                    // é necessário ir para o xy 1,1 para imprimir a matriz

    textcolor(WHITE);                       //cores da matriz
    textbackground(CYAN);

    for (linha=0; linha<19; linha++)
    {
        for (coluna=0; coluna<80; coluna++)
        {
            printf("%c", Interface_jogo[linha][coluna]);            //imprime o conteúdo atual da matriz
        }
        printf("\n");

    }

    textcolor(WHITE);
    textbackground(BROWN);

    for (linha=0; linha<5; linha ++)
    {
        for (coluna=0; coluna<80; coluna++)
        {
            printf("%c", Interface_status[linha][coluna]);      //imprime o conteúdo atual da matriz
        }
        printf("\n");

    }

    gotoxy(50, 23);
    printf("SCORE: %d", (*pontos));          // imprime o score atual do jogador


}

void move (char tecla, char Interface_jogo [][80], int *pont_coluna, NAVE nave)     // função que serve para mover a nave do jogador
{
    {
        switch (tecla)          //checa qual tecla foi clicada pelo jogador
        {
        case 'a':
            if ((*pont_coluna)>2)           //a coluna mínima em que a nave pode ir é a coluna 2, pois há dois símbolos à esquerda do centro da nave
            {
                (*pont_coluna) --;

                Interface_jogo [18][(*pont_coluna)-2]= nave.corpo_nave[0];
                Interface_jogo [18][(*pont_coluna)-1]= nave.corpo_nave[1];
                Interface_jogo [18][(*pont_coluna)] = nave.corpo_nave[2];   // coloca a nave uma coluna para a esquerda
                Interface_jogo [18][(*pont_coluna)+1]= nave.corpo_nave[3];
                Interface_jogo [18][(*pont_coluna)+2]= nave.corpo_nave[4];
                Interface_jogo [17][(*pont_coluna)]= nave.corpo_nave[5];

                Interface_jogo[18][(*pont_coluna)+3]= ' ';           // deixa em branco onde a nave estava antes do movimento
                Interface_jogo[17][(*pont_coluna)+1]= ' ';

            }
            else
            {};
            break;

        case 'd':
            if ((*pont_coluna)<77)       //a coluna máxima em que a nave pode ir é a coluna 77, pois há dois símbolos à direita do centro da nave
            {
                (*pont_coluna) ++;

                Interface_jogo [18][(*pont_coluna)-2]= nave.corpo_nave[0];
                Interface_jogo [18][(*pont_coluna)-1]= nave.corpo_nave[1];
                Interface_jogo [18][(*pont_coluna)] = nave.corpo_nave[2];       // coloca a nave uma coluna para a direita
                Interface_jogo [18][(*pont_coluna)+1]= nave.corpo_nave[3];
                Interface_jogo [18][(*pont_coluna)+2]= nave.corpo_nave[4];
                Interface_jogo [17][(*pont_coluna)]= nave.corpo_nave[5];

                Interface_jogo[18][(*pont_coluna)-3]= ' ';          // deixa em branco onde a nave estava antes do movimento
                Interface_jogo[17][(*pont_coluna)-1]= ' ';
                break;
            }
        }

    }

}

void print_inimigos (char Interface_jogo[][80], int num_inimigos, INIMIGOS array_inimigos[], char direcao)      // função que posiciona os inimigos em seu lugar inicial
{
    int cont, x0, x1, x2, x3, x4, y;

    for(cont=0; cont<num_inimigos; cont++)
    {
        x0 = array_inimigos[cont].coordx[0];        // atribui cada parte da nave inimiga a uma variável
        x1 = array_inimigos[cont].coordx[1];
        x2 = array_inimigos[cont].coordx[2];
        x3 = array_inimigos[cont].coordx[3];
        x4 = array_inimigos[cont].coordx[4];
        y = array_inimigos[cont].coordy;


        if (direcao == 'R')         // lê, no arquivo texto, a direção em que as naves devem seguir
        {
            Interface_jogo[y][x0-1] =  ' ';

            Interface_jogo[y][x0] =  array_inimigos[cont].corpo_inimigo[0];
            Interface_jogo[y][x1] = array_inimigos[cont].corpo_inimigo[1];
            Interface_jogo[y][x2] = array_inimigos[cont].corpo_inimigo[2];
            Interface_jogo[y][x3] = array_inimigos[cont].corpo_inimigo[3];      // posiciona as naves
            Interface_jogo[y][x4] = array_inimigos[cont].corpo_inimigo[4];
        }
        else if (direcao == 'L')
        {
            Interface_jogo[y][x4+1] =  ' ';

            Interface_jogo[y][x0] =  array_inimigos[cont].corpo_inimigo[0];
            Interface_jogo[y][x1] = array_inimigos[cont].corpo_inimigo[1];
            Interface_jogo[y][x2] = array_inimigos[cont].corpo_inimigo[2];
            Interface_jogo[y][x3] = array_inimigos[cont].corpo_inimigo[3];      // posiciona as naves
            Interface_jogo[y][x4] = array_inimigos[cont].corpo_inimigo[4];
        }
    }

}




void moveinimigos (char Interface_jogo[][80], int num_inimigos, INIMIGOS array_inimigos[], char direcao)        // função que move os inimigos
{
    int cont=0;

    if (direcao == 'R')
    {
        for(cont=0; cont<=num_inimigos; cont++)
        {
            array_inimigos[cont].coordx[0]++;
            array_inimigos[cont].coordx[1]++;   // move os inimigos de acordo com a direção lida no arquivo texto
            array_inimigos[cont].coordx[2]++;
            array_inimigos[cont].coordx[3]++;
            array_inimigos[cont].coordx[4]++;

            if(array_inimigos[cont].coordx[0] == 80)         // os inimigos não podem passar do limite das colunas, então, ao chegar em 80, voltam para 0
            {
                array_inimigos[cont].coordx[0] = 0;
                Interface_jogo[array_inimigos[cont].coordy][79]=' ';
            }
            else if(array_inimigos[cont].coordx[1] == 80)       // move parte por parte da nave inimiga
                array_inimigos[cont].coordx[1] = 0;
            else if(array_inimigos[cont].coordx[2] == 80)
                array_inimigos[cont].coordx[2] = 0;
            else if(array_inimigos[cont].coordx[3] == 80)
                array_inimigos[cont].coordx[3] = 0;
            else if(array_inimigos[cont].coordx[4] == 80)
                array_inimigos[cont].coordx[4] = 0;
        }
    }
    else if (direcao == 'L')
    {
        for(cont=0; cont<=num_inimigos; cont++)
        {
            array_inimigos[cont].coordx[0]--;
            array_inimigos[cont].coordx[1]--;
            array_inimigos[cont].coordx[2]--;    // move os inimigos de acordo com a direção lida no arquivo texto
            array_inimigos[cont].coordx[3]--;
            array_inimigos[cont].coordx[4]--;

            if(array_inimigos[cont].coordx[0] == -1)
                array_inimigos[cont].coordx[0] = 79;
            else if(array_inimigos[cont].coordx[1] == -1)       // move parte por parte da nave inimiga
                array_inimigos[cont].coordx[1] = 79;
            else if(array_inimigos[cont].coordx[2] == -1)
                array_inimigos[cont].coordx[2] = 79;
            else if(array_inimigos[cont].coordx[3] == -1)
                array_inimigos[cont].coordx[3] = 79;
            else if(array_inimigos[cont].coordx[4] == -1)       // os inimigos não podem passar do limite das colunas, então, ao chegar em -1, voltam para 79
            {
                array_inimigos[cont].coordx[4] = 79;
                Interface_jogo[array_inimigos[cont].coordy][0]=' ';
            }

        }
    }
}
void hidecursor(void)   // essa função serve para esconder o cursor, de modo que ele não pisque na tela
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void cria_tiro(char Interface_jogo[][80], int *pont_coluna)     // essa função cria o tiro da nave uma linha acima da própria nave
{
    Interface_jogo[16][*pont_coluna] = '|';
}

void move_tiro (char Interface_jogo[][80], int *pont_coluna, int ajusta_tiro)       // essa função move o tiro criado anteriormente
{
    int flag = 1, cont = 0;
    if(ajusta_tiro == 0)                // checa se já é permitido atirar, pois somente um tiro pode estar ativo por vez
    {
        Interface_jogo[0][*pont_coluna] = ' ';
    }
    else
    {
        if(Interface_jogo[ajusta_tiro][*pont_coluna-1]=='|') //quando a nave anda para a esquerda, o tiro vai uma coluna para a esquerda
        {
            Interface_jogo[(ajusta_tiro)-1][*pont_coluna] = Interface_jogo[ajusta_tiro][*pont_coluna-1];
            Interface_jogo[ajusta_tiro][*pont_coluna-1]= ' ';
        }
        else if(Interface_jogo[ajusta_tiro][*pont_coluna+1]=='|') //quando a nave anda para a direita, o tiro vai uma coluna para a direita
        {
            Interface_jogo[(ajusta_tiro)-1][*pont_coluna] = Interface_jogo[ajusta_tiro][*pont_coluna+1];
            Interface_jogo[ajusta_tiro][*pont_coluna+1]= ' ';
        }
        else
        {
            Interface_jogo[(ajusta_tiro)-1][*pont_coluna] = Interface_jogo[ajusta_tiro][*pont_coluna];
            Interface_jogo[ajusta_tiro][*pont_coluna]= ' ';         // apaga a posição anterior do tiro
        }
    }

}


int menu (void)         // essa função cria o menu inicial, com as opções "play" e "exit"
{
    char interface_menu[24][80], tecla;
    int linha = 0, coluna = 0, ajusta_seta = 10, retorno = 0, cont = 0;



    textbackground(MAGENTA);            // cores do menu
    textcolor (WHITE);

    for (linha = 0; linha < 24; linha++ )
    {
        for (coluna = 0; coluna < 80; coluna ++)
        {
            interface_menu[linha][coluna] = ' ';             // imprime o menu inteiro como espaços em branco
        }
    }

    interface_menu[10][38] = 'P';
    interface_menu[10][39] = 'L';       // opções escritas
    interface_menu[10][40] = 'A';
    interface_menu[10][41] = 'Y';

    interface_menu[12][38] = 'E';
    interface_menu[12][39] = 'X';
    interface_menu[12][40] = 'I';
    interface_menu[12][41] = 'T';


    interface_menu[10][36] = '>';
    interface_menu[10][43] = '<';

    while (retorno == 0)
    {
        gotoxy(1,1);
        interface_menu[5][60] = 'P';
        interface_menu[5][61] = 'r';
        interface_menu[5][62] = 'e';
        interface_menu[5][63] = 's';
        interface_menu[5][64] = 's';
        interface_menu[5][65] = ' ';
        interface_menu[5][66] = 's';
        interface_menu[5][67] = 'p';
        interface_menu[5][68] = 'a';
        interface_menu[5][69] = 'c';
        interface_menu[5][70] = 'e';
        interface_menu[6][60] = ' ';        // informações para o jogador
        interface_menu[6][62] = 't';
        interface_menu[6][63] = 'o';
        interface_menu[6][64] = ' ';
        interface_menu[6][65] = 's';
        interface_menu[6][66] = 'e';
        interface_menu[6][67] = 'l';
        interface_menu[6][68] = 'e';
        interface_menu[6][69] = 'c';
        interface_menu[6][70] = 't';

        for (cont = 58; cont < 75; cont++)
        {
            interface_menu[4][cont] = '*';
            interface_menu[8][cont] = '*';
        }

        for (cont = 4; cont <9; cont++)
        {

            interface_menu[cont][58] = '*';
            interface_menu[cont][74] = '*';
        }

        for (linha=0; linha<24; linha++)
        {
            for (coluna=0; coluna<80; coluna++)
            {
                printf("%c", interface_menu[linha][coluna]);
            }
            printf("\n");
        }
        if(_kbhit())        // kbhit para checar se o usuário pressionou alguma tecla, de modo a definir se a opção escolhida foi "play" ou "exit"
        {
            tecla = _getch();
            tecla = tolower(tecla);
            switch (tecla)
            {
            case 'w':       //se a letra 'w' for clicada, checa se já não está na opção de cima. Caso  não esteja, move a tecla uma linha para cima
                if (ajusta_seta == 12) //selecionar play
                {
                    interface_menu[10][36] = '>';
                    interface_menu[10][43] = '<';
                    interface_menu[12][36] = ' ';
                    interface_menu[12][43] = ' ';
                    ajusta_seta -= 2;
                }
                break;

            case 's':    //se a letra 's' for clicada, checa se já não está na opção de baixo. Caso  não esteja, move a tecla uma linha para cima
                if (ajusta_seta == 10) //selecionar exit
                {
                    interface_menu[10][36] = ' ';
                    interface_menu[10][43] = ' ';
                    interface_menu[12][36] = '>';
                    interface_menu[12][43] = '<';
                    ajusta_seta += 2;

                }
                break;

            case ' ':        // se a barra de espaço for clicada, vê onde está a seta e seleciona essa opção

                if (ajusta_seta == 10)// para play
                {
                    retorno = 1;
                }
                else if (ajusta_seta == 12) // para exit
                {
                    retorno = 2;
                }
            }
        }
    }

    return retorno;

}

int ajusta_energia (float diff_energia, int energia, char Interface_status[][80], int ajusta_status)        // função que diminui a energia conforme o passar do tempo
{
    if (energia > 0)         // a energia pode ir até 0
    {
        energia--;
        Interface_status[2][ajusta_status] = ' ';       // diminui um '-' da energia, para representar seu descrescimento
    }
    return energia;          // retorna o valor para a main para checar se já está em 0
}

void pausa_jogo (char Interface_jogo[19][80], char Interface_status[][80], int *pontos)       // essa função pausa o jogo quando o jogador clica 'p'
{
    int pausado = 0, cont = 0;
    char tecla;

    Interface_jogo[10][37] = 'P';       // imprime a palavra "paused" para indicar que está pausado
    Interface_jogo[10][38] = 'A';
    Interface_jogo[10][39] = 'U';
    Interface_jogo[10][40] = 'S';
    Interface_jogo[10][41] = 'E';
    Interface_jogo[10][42] = 'D';

    imprime_matriz(Interface_jogo, Interface_status, pontos);       // chama a função imprime matriz para imprimir a palavra "pause"

    do
    {
        if(_kbhit())        // checa se o jogador clicou a tecla 'p'. Caso ele tenha clicado, despausa o jogo
        {
            tecla = _getch();
            tecla = tolower(tecla);
            if (tecla == 'p')
                pausado = 1;

        }
    }
    while (pausado == 0);        // a condição

    if (pausado == 1)
    {

        Interface_jogo[10][37] = ' ';
        Interface_jogo[10][38] = ' ';
        Interface_jogo[10][39] = ' ';
        Interface_jogo[10][40] = ' ';    // retira a palavra "paused" da matriz
        Interface_jogo[10][41] = ' ';
        Interface_jogo[10][42] = ' ';

    }

}

int le_nivel (int cont_nivel, INIMIGOS array_inimigos[10], char *direcao, int *velinimigos, char Interface_jogo[][80], char Interface_status[][80]) // essa função lê o arquivo texto para pegar suas informações
{
    int cont = 0;
    int continim = 0;
    int linhainim = 0;
    int flag_le_nivel = 1;
    char linha[81], nomenivel[12];
    FILE *nivel;

    switch(cont_nivel)      // lê em qual nível o jogo está no momento e copia a string para a variável, de modo a abrir o arquivo texto certo
    {
    case 1:
        strcpy(nomenivel, "nivel 1.txt");
        break;
    case 2:
        strcpy(nomenivel, "nivel 2.txt");
        break;
    case 3:
        strcpy(nomenivel, "nivel 3.txt");
        break;
    default:
        flag_le_nivel = 0;
    }

    if(flag_le_nivel)
    {
        nivel = fopen (nomenivel, "r");   // abre o arquivo texto
        if (nivel == NULL)
            printf ("Falha na abertura!\n");
        else
        {
            while (!feof(nivel))
            {
                if (fgets(linha, 256, nivel) != NULL)       // pega uma linha inteira e, a partir dela, lê suas informações
                {

                    for (cont = 0; cont < 81; cont++)
                    {
                        switch(linha[cont])     //checa qual o número existente na primeira linha do arquivo texto, definindo a velocidade dos inimigos
                        {
                        case '1':
                            (*velinimigos) = 90;
                            break;
                        case '2':
                            (*velinimigos) = 75;
                            break;
                        case '3':
                            (*velinimigos) = 40;
                            break;
                        case '4':
                            (*velinimigos) = 20;
                            break;
                        case '5':
                            (*velinimigos) = 0;     // quanto maior o número, menor é o tempo entre uma movimentação inimiga e outra
                            break;
                        }
                        if (linha[cont] == 'R')
                            *direcao = 'R';
                        else if (linha[cont] == 'L')
                            *direcao = 'L';
                        else if (linha[cont] == 'x')
                        {
                            array_inimigos[continim].coordx[0] = cont;
                            array_inimigos[continim].coordx[1] = cont+1;
                            array_inimigos[continim].coordx[2] = cont+2;
                            array_inimigos[continim].coordx[3] = cont+3;
                            array_inimigos[continim].coordx[4] = cont+4;
                            array_inimigos[continim].coordy = linhainim-2; // pula as duas linhas que contém a direção e a velocidade
                            continim++;  // conta o número de inimigos
                        }

                    }
                }
                linhainim++;        // posiciona os inimigos em suas respectivas linhas
            }
            fclose(nivel);       //fecha o arquivo texto do nível
        }

    }
    else
        vitoria(Interface_jogo, Interface_status);  // caso não hajam mais níveis, vai para a cena final de vitória

    return continim;    // retorna o número de inimigos encontrados no arquivo
}


void morte_aos_inimigos (char Interface_jogo[][80], int num_inimigos, INIMIGOS array_inimigos[], int *pont_coluna, int ajusta_tiro, int *flag_tiro, int *pontos)        // função que mata os inimigos
{
    int cont = 0, cont_coord = 0;
    int x, y;

    for(cont=0; cont<=num_inimigos; cont++)         // checa inimigo por inimigo se o tiro está atingindo
    {
        for(cont_coord = 0; cont_coord < 5; cont_coord++)       // checa as 5 partes do inimigo
        {
            if(ajusta_tiro == array_inimigos[cont].coordy && (*pont_coluna) == array_inimigos[cont].coordx[cont_coord] && array_inimigos[cont].vivo == 1)   // checa a coordenada atual do tiro
            {                                                                                                                                               // e compara com os inimigos
                array_inimigos[cont].corpo_inimigo[0] = ' ';
                array_inimigos[cont].corpo_inimigo[1] = ' ';
                array_inimigos[cont].corpo_inimigo[2] = ' ';     // caso a coordenada do tiro seja igual à dos inimigos, o define o corpo inteiro do
                array_inimigos[cont].corpo_inimigo[3] = ' ';     // respectivo inimigo como espaços em branco, e muda seu status para morto
                array_inimigos[cont].corpo_inimigo[4] = ' ';
                array_inimigos[cont].vivo = 0;

                Beep(250, 50);      // som que indica que um inimigo foi morto

                (*pontos) += 20;    // adiciona pontos ao score do jogador

                (*flag_tiro) = 0;        // permite atirar novamente
            }
            else
            {};
        }

    }
}

void pontos_final (char Interface_status[][80], int energia, INIMIGOS array_inimigos[], int *pontos, int numinimigos)   // função que gera a pontuação final do jogador
{
    int cont = 0;

    if((*pontos) == (numinimigos*20))
    {
        (*pontos) += 10 * energia;       // multiplica cada energia sobrando por 10 e adiciona o valor ao score final do jogador
    }
}

void tiros_inimigos (char Interface_jogo[][80], INIMIGOS array_inimigos[], int num_inimigos, int *linha_tiro_inim, int *coluna_tiro_inim, int num_aleatorio)    // função que cria o tiro dos inimigos
{
    array_inimigos[num_aleatorio].atira = 1;        // escolhe um inimigo aleatório para atirar
    Interface_jogo[(array_inimigos[num_aleatorio].coordy)+1][array_inimigos[num_aleatorio].coordx[2]] = 'o';        // cria o tiro do inimigo uma linha abaixo dele
    (*coluna_tiro_inim) = array_inimigos[num_aleatorio].coordx[2];       // salva o valor da coluna atual do tiro
    (*linha_tiro_inim) = array_inimigos[num_aleatorio].coordy+1;         // salva o valor da linha atual do tiro

}

int move_tiro_inim (char Interface_jogo[][80], int coluna_tiro_inim, int *linha_tiro_inim, INIMIGOS array_inimigos[], int num_aleatorio)     // função que move o tiro do inimigo
{
    int pode_atirar = 0;        // flag que define se o próximo inimigo já pode atirar ou não

    if((*linha_tiro_inim) < 18)       // a última linha em que o tiro pode ir é a linha 18, então caso seja menor que 18, soma 1
    {
        Interface_jogo[(*linha_tiro_inim)+1][coluna_tiro_inim] = Interface_jogo[(*linha_tiro_inim)][coluna_tiro_inim];
        Interface_jogo[(*linha_tiro_inim)][coluna_tiro_inim] = ' ';

        (*linha_tiro_inim)++;
    }
    else
    {
        Interface_jogo[(*linha_tiro_inim)][coluna_tiro_inim] = ' ';     // caso o tiro esteja na linha 18, ele desaparece, e os outros inimigos podem atirar novamente
        array_inimigos[num_aleatorio].atira = 0;
        (*linha_tiro_inim) = 0;
        pode_atirar = 1;
    }

    return pode_atirar;    // retorna se o próximo inimigo já pode atirar

}

int morte_nave(char Interface_jogo[][80], char Interface_status[][80], int *pontos, int *pont_coluna_nave, int coluna_tiro_inim, int *vidas, NAVE nave, int *contvidas, int *sair_jogo, int *volta_menu, int *ajusta_status, int *energia)
{
         // essa função define a morta da nave do jogador
    int coluna, linha, cont = 0;

    if((*vidas) > 0)    // caso ainda haja vidas restantes, tira uma vida
    {
        (*vidas)--;
        Interface_status[3][(*contvidas)]= ' ';
        (*contvidas)--;     // parâmetro que faz apagar a representação gráfica das vidas

        for (coluna=23; coluna<63; coluna++)
        {
            Interface_status[2][coluna]= '-';       // imprime novamente a representação gráfica da energia
        }

        *ajusta_status = 62;        // parâmetro que faz apagar a representação gráfica da energia
        *energia = 40;           // parâmetro que define a energia do usuário

        gotoxy(1, 1);

        textcolor(WHITE);
        textbackground(RED);            // muda a cor do jogo para mostrar que o jogador perdeu uma vida

        for (linha=0; linha<19; linha++)
        {
            for (coluna=0; coluna<80; coluna++)
            {
                printf("%c", Interface_jogo[linha][coluna]);
            }
            printf("\n");

        }

        textcolor(WHITE);
        textbackground(BROWN);


        for (linha=0; linha<5; linha ++)
        {
            for (coluna=0; coluna<80; coluna++)
            {
                printf("%c", Interface_status[linha][coluna]);
            }
            printf("\n");

        }



        if(coluna_tiro_inim == (*pont_coluna_nave) || coluna_tiro_inim == (*pont_coluna_nave)+1 || coluna_tiro_inim == (*pont_coluna_nave)+2 || coluna_tiro_inim == (*pont_coluna_nave)-1 || coluna_tiro_inim == (*pont_coluna_nave)-2)
        {

            // condições para determinar se o jogador foi morto por um tiro ou não
            // caso tenha sido atingido por um tiro inimigo, a nave é colocada na posição inicial

            Interface_jogo [18][nave.coordx_nave-2]= ' ';
            Interface_jogo [18][nave.coordx_nave-1]= ' ';
            Interface_jogo [18][nave.coordx_nave]= ' ';
            Interface_jogo [18][nave.coordx_nave+1]= ' ';
            Interface_jogo [18][nave.coordx_nave+2]= ' ';
            Interface_jogo [17][nave.coordx_nave]= ' ';

            Interface_jogo [18][38]= nave.corpo_nave[0];
            Interface_jogo [18][39]= nave.corpo_nave[1];
            Interface_jogo [18][40]= nave.corpo_nave[2];
            Interface_jogo [18][41]= nave.corpo_nave[3];
            Interface_jogo [18][42]= nave.corpo_nave[4];
            Interface_jogo [17][40]= nave.corpo_nave[5];

            (*pont_coluna_nave) = 40;       // retorna o parâmetro da coluna da nave para o início

            gotoxy(50, 23);
            printf("SCORE: %d", (*pontos));     // imprime novamente o score

        }
        for (cont = 800; cont > 100; cont -= 15)
            Beep(cont, 70);         // som para indicar que o jogador perdeu uma vida
    }

    else
    {
                                        // caso o jogador perca mais uma vida, porém já esteja com 0 vidas, o jogo acaba, imprimindo a mensagem de "game over"
        Interface_jogo[10][37] = 'G';
        Interface_jogo[10][38] = 'A';
        Interface_jogo[10][39] = 'M';
        Interface_jogo[10][40] = 'E';
        Interface_jogo[10][41] = ' ';
        Interface_jogo[10][42] = 'O';
        Interface_jogo[10][43] = 'V';
        Interface_jogo[10][44] = 'E';
        Interface_jogo[10][45] = 'R';

        gotoxy(1, 1);

        textcolor(WHITE);
        textbackground(RED);

        for (linha=0; linha<19; linha++)
        {
            for (coluna=0; coluna<80; coluna++)
            {
                printf("%c", Interface_jogo[linha][coluna]);            // imprime a matriz novamente
            }
            printf("\n");

        }

        textcolor(WHITE);
        textbackground(BROWN);

        for (linha=0; linha<5; linha ++)
        {
            for (coluna=0; coluna<80; coluna++)
            {
                printf("%c", Interface_status[linha][coluna]);      // imprime a matriz novamente
            }
            printf("\n");

        }

        gotoxy(50, 23);
        printf("SCORE: %d", (*pontos));


        for (cont = 800; cont > 100; cont -= 15)
            Beep(cont, 70);     // som para indicar que o jogo acabou

        (*sair_jogo) = restart(Interface_jogo, Interface_status, pontos);       // sair_jogo é a condição do while da main, e ele é definido pelo usuário dentro da função restart
        *volta_menu = *sair_jogo;                                           // volta_menu é a condição do while principal, que define se o jogo acabou ou não


    }
    return *contvidas;               // atualiza o contador que define a representação gráfica das vidas
}



int restart(char Interface_jogo[][80], char Interface_status[][80], int *pontos)              // função que dá a opção de jogar novamente ou sair do jogo
{
    int ajusta_seta=12, retorno = 1;
    char tecla;

    Interface_jogo[10][37] = 'R';
    Interface_jogo[10][38] = 'E';
    Interface_jogo[10][39] = 'S';
    Interface_jogo[10][40] = 'T';
    Interface_jogo[10][41] = 'A';
    Interface_jogo[10][42] = 'R';
    Interface_jogo[10][43] = 'T';
    Interface_jogo[10][44] = '?';
    Interface_jogo[10][45] = ' ';

    Interface_jogo[12][40] = 'Y';
    Interface_jogo[12][41] = 'E';
    Interface_jogo[12][42] = 'S';

    Interface_jogo[13][40] = 'N';
    Interface_jogo[13][41] = 'O';

    Interface_jogo[12][39] = '>';
    Interface_jogo[12][43] = '<';


    while(retorno == 1)     // enquanto o valor de "retorno" não se alterar, o laço continua
    {
        imprime_matriz(Interface_jogo, Interface_status, pontos);       // imprime a matriz para atualizar a posição gráfica atual das setas
        if(_kbhit())
        {
            tecla = _getch();
            tecla = tolower(tecla);
            switch (tecla)
            {
            case 'w':
                if (ajusta_seta == 13) //selecionar yes
                {
                    Interface_jogo[12][39] = '>';
                    Interface_jogo[12][43] = '<';
                    Interface_jogo[13][39] = ' ';
                    Interface_jogo[13][43] = ' ';
                    ajusta_seta -= 1;       // atualiza o valor interno da seta
                }
                break;

            case 's':
                if (ajusta_seta == 12) //selecionar no
                {
                    Interface_jogo[12][39] = ' ';
                    Interface_jogo[12][43] = ' ';
                    Interface_jogo[13][39] = '>';
                    Interface_jogo[13][43] = '<';
                    ajusta_seta += 1;   // atualiza o valor interno da seta


                }
                break;

            case ' ':       // caso seja clicado barra de espaço, é selecionado a opção em que a seta está no momento
                if (ajusta_seta == 12)// para play
                {
                    retorno = 0;
                }
                else if (ajusta_seta == 13) // para exit
                {
                    retorno = 2;
                }
                break;

            }
        }
    }


    return retorno;
}

void inicializa_variaveis (int *vida, int *energia, int *pontos, int *contvidas, int *sair_jogo, int *ajusta_status)    // função que inicializa variáveis, o que é necessário quando mudam-se os níveis
{
    *vida = 4;
    *energia = 40;
    //*pontos = *pontos_final();
    *contvidas = 17;
    *sair_jogo = 1;
    *ajusta_status = 62;
}

void vitoria(char Interface_jogo[][80], char Interface_status[][80])    // cena final de vitória, quando o jogador vence os 3 níveis
{
    int coluna, linha;

    for (coluna = 0; coluna < 80; coluna++)
    {
        for (linha = 0; linha < 19; linha++)
        {
            Interface_jogo[linha][coluna]= ' ';
        }
    }

    Interface_jogo[10][37] = 'Y';
    Interface_jogo[10][38] = 'O';
    Interface_jogo[10][39] = 'U';
    Interface_jogo[10][40] = ' ';
    Interface_jogo[10][41] = 'W';  // escreve "YOU WIN" com o fundo preto
    Interface_jogo[10][42] = 'I';
    Interface_jogo[10][43] = 'N';

    Interface_jogo[12][35] = 'p';
    Interface_jogo[12][36] = 'r';
    Interface_jogo[12][37] = 'e';
    Interface_jogo[12][38] = 's';
    Interface_jogo[12][39] = 's';
    Interface_jogo[12][40] = ' ';
    Interface_jogo[12][41] = 's';
    Interface_jogo[12][42] = 'p';
    Interface_jogo[12][43] = 'a';
    Interface_jogo[12][44] = 'c';
    Interface_jogo[12][45] = 'e';
    gotoxy(1, 1);

    textcolor(YELLOW);
    textbackground(BLACK);

    for (linha=0; linha<19; linha++)
    {
        for (coluna=0; coluna<80; coluna++)
        {
            printf("%c", Interface_jogo[linha][coluna]);
        }
        printf("\n");

    }

    textcolor(BLACK);
    textbackground(BLACK);

    for (linha=0; linha<5; linha ++)
    {
        for (coluna=0; coluna<80; coluna++)
        {
            printf("%c", Interface_status[linha][coluna]);
        }
        printf("\n");
    }

    musicamario();   // toca a música do mário

}

void musicastarwars(void)
{

    Beep(300,500);                          // MÚSICA DO STAR WARS --> tocada ao sair do jogo
    Sleep(50);
    Beep(300,500);
    Sleep(50);
    Beep(300,500);
    Sleep(50);

    Beep(250,500);

    Beep(350,250);
    Beep(300,500);
    Sleep(50);

    Beep(250,500);
    Sleep(50);

    Beep(350,250);
    Beep(300,500);
    Sleep(50);


}

void musicamario(void)
{
        Beep (330,100);Sleep(100);    // MÚSICA DO MÁRIO --> tocada ao vencer o jogo
        Beep (330,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (330,100);Sleep(300);
        Beep (392,100);Sleep(700);
        Beep (196,100);Sleep(700);
        Beep (262,300);Sleep(300);
        Beep (196,300);Sleep(300);
        Beep (164,300);Sleep(300);
        Beep (220,300);Sleep(100);
        Beep (246,100);Sleep(300);
        Beep (233,200);
        Beep (220,100);Sleep(300);
        Beep (196,100);Sleep(150);
        Beep (330,100);Sleep(150);
        Beep (392,100);Sleep(150);
        Beep (440,100);Sleep(300);
        Beep (349,100);Sleep(100);
        Beep (392,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(100);
        Beep (247,100);Sleep(500);
        Beep (262,300);Sleep(300);
        Beep (196,300);Sleep(300);
        Beep (164,300);Sleep(300);
        Beep (220,300);Sleep(100);
        Beep (246,100);Sleep(300);
        Beep (233,200);
        Beep (220,100);Sleep(300);
        Beep (196,100);Sleep(150);
        Beep (330,100);Sleep(150);
        Beep (392,100);Sleep(150);
        Beep (440,100);Sleep(300);
        Beep (349,100);Sleep(100);
        Beep (392,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(100);
        Beep (247,100);Sleep(900);
        Beep (392,100);Sleep(100);
        Beep (370,100);Sleep(100);
        Beep (349,100);Sleep(100);
        Beep (311,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (207,100);Sleep(100);
        Beep (220,100);Sleep(100);
        Beep (262,100);Sleep(300);
        Beep (220,100);Sleep(100);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(500);
        Beep (392,100);Sleep(100);
        Beep (370,100);Sleep(100);
        Beep (349,100);Sleep(100);
        Beep (311,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (523,100);Sleep(300);
        Beep (523,100);Sleep(100);
        Beep (523,100);Sleep(1100);
        Beep (392,100);Sleep(100);
        Beep (370,100);Sleep(100);
        Beep (349,100);Sleep(100);
        Beep (311,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (207,100);Sleep(100);
        Beep (220,100);Sleep(100);
        Beep (262,100);Sleep(300);
        Beep (220,100);Sleep(100);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(500);
        Beep (311,300);Sleep(300);
        Beep (296,300);Sleep(300);
        Beep (262,300);Sleep(1300);
        Beep (262,100);Sleep(100);
        Beep (262,100);Sleep(300);
        Beep (262,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(300);
        Beep (330,200);Sleep(50);
        Beep (262,200);Sleep(50);
        Beep (220,200);Sleep(50);
        Beep (196,100);Sleep(700);
        Beep (262,100);Sleep(100);
        Beep (262,100);Sleep(300);
        Beep (262,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(100);
        Beep (330,100);Sleep(700);
        Beep (440,100);Sleep(300);
        Beep (392,100);Sleep(500);
        Beep (262,100);Sleep(100);
        Beep (262,100);Sleep(300);
        Beep (262,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (294,100);Sleep(300);
        Beep (330,200);Sleep(50);
        Beep (262,200);Sleep(50);
        Beep (220,200);Sleep(50);
        Beep (196,100);Sleep(700);
        /*Intro*/
        Beep (330,100);Sleep(100);
        Beep (330,100);Sleep(300);
        Beep (330,100);Sleep(300);
        Beep (262,100);Sleep(100);
        Beep (330,100);Sleep(300);
        Beep (392,100);Sleep(700);
        Beep (196,100);Sleep(700);
        Beep (196,100);Sleep(125);
        Beep (262,100);Sleep(125);
        Beep (330,100);Sleep(125);
        Beep (392,100);Sleep(125);
        Beep (523,100);Sleep(125);
        Beep (660,100);Sleep(125);
        Beep (784,100);Sleep(575);
        Beep (660,100);Sleep(575);
        Beep (207,100);Sleep(125);
        Beep (262,100);Sleep(125);
        Beep (311,100);Sleep(125);
        Beep (415,100);Sleep(125);
        Beep (523,100);Sleep(125);
        Beep (622,100);Sleep(125);
        Beep (830,100);Sleep(575);
        Beep (622,100);Sleep(575);
        Beep (233,100);Sleep(125);
        Beep (294,100);Sleep(125);
        Beep (349,100);Sleep(125);
        Beep (466,100);Sleep(125);
        Beep (587,100);Sleep(125);
        Beep (698,100);Sleep(125);
        Beep (932,100);Sleep(575);
        Beep (932,100);Sleep(125);
        Beep (932,100);Sleep(125);
        Beep (932,100);Sleep(125);
        Beep (1046,675);

}
