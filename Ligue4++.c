#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#define RED     "\x1b[31m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BOLD    "\x1b[1m"
#define CYAN    "\x1b[36m"
#define MAGENTA "\x1b[35m"
#define RESET   "\x1b[0m"
#define GOLD    "\x1b[33m"
#define SILVER  "\x1b[37m"
#define BRONZE  "\x1b[31m"

#define EXPL_ATIVA_X 'A'
#define EXPL_ATIVA_O 'B'
#define EXPL_DESAT_X 'X'
#define EXPL_DESAT_O 'O'
#define portal '@'

// Variavel que controla o jogador atual 
char jogador = 'X';

// Matriz que representa o tabuleiro (6 linhas x 7 colunas) 
char tab[6][7];

// para guardar o nome do jogador 1 e 2
char nomeJogador1[50];
char nomeJogador2[50];

// Variaveis de controle do jogo 
int ganhador = 0;
int linha, coluna;
int rodada = 0;
int i,j;
int comunsX = 21;
int comunsO = 21;

// fichas do jogador x
int explosivasX = 0;
int portalX = 0;

// fichas do jogador o
int explosivasO = 0;
int portalO = 0;


// funcao de delay para melhor visualizacao
void DelayAutomatico() {
#ifdef _WIN32
    Sleep(2000);
#else
    sleep(2);
#endif
}

//Funcao para limpar a tela
void LimparTela() {
    #ifdef _WIN32
        system("cls");   // Comando para Windowns
    #else
        system("clear"); // Comando para Linux
    #endif
}

// funcao para exibir tabuleiro ela mostra o tabuleiro na tela 
void ExibeTabuleiro() {
    printf("\n");
    printf(BLUE "  +---------------------------+" RESET "\n");
    
	//percorre as linhas
    for (i = 0; i < 6; i++) {
        printf(BLUE "  ¦" RESET);
        
		//percorre as colunas 
        for (j = 0; j < 7; j++) {
            if (tab[i][j] == 'X'){ 
                printf(" " YELLOW "x" RESET BLUE " ¦" RESET); 
            }
            
            else if (tab[i][j] == 'O'){
                printf(" " CYAN "o" RESET BLUE " ¦" RESET);
        	}
        	
            else if (tab[i][j] == portal){
                printf(" " MAGENTA "@ " RESET BLUE "¦" RESET);
        	}
        	
            else if(tab[i][j] == EXPL_ATIVA_X){
            	printf(" " RED "A " RESET BLUE "¦" RESET);
			}
			
			else if(tab[i][j] == EXPL_ATIVA_O){
				printf(" " RED "B " RESET BLUE "¦" RESET);
			}
			
            else{
                printf("   " BLUE "¦" RESET);
        	}
        }
        printf("\n");
        if (i < 5) printf(BLUE "  ¦---+---+---+---+---+---+---¦" RESET "\n");
    }
    // Numeracao das colunas
    printf(BLUE "  +---------------------------+" RESET "\n");
    printf(BOLD "    1   2   3   4   5   6   7" RESET "\n\n");
}

// funcao para trocar jogador, ela vai alternar entre X e O 
void TrocarJogador() {
    if (jogador == 'X') {
        jogador = 'O';
    } else {
        jogador = 'X';
    }
}

// funcao para atualizar o numero de fichas especias de cada jogador e maquina
void AtualizarFichas(){
// a cada cinco rodadas dara aos jogodores as fichas portais e explosivas 
    if(rodada > 0 && rodada % 5 == 0){
    	portalX++;
    	portalO++;
    	explosivasX++;
    	explosivasO++;
	}
}

//funcao para ter aplicacao da gravidade apos uma explosao
void AplicarGravidade() {
	int col,lin;
    for ( col = 0; col < 7; col++) {
        for ( lin = 5; lin > 0; lin--) {
            if (tab[lin][col] == ' ') {
                int k = lin - 1;
                while (k >= 0 && tab[k][col] == ' ') k--;
                if (k >= 0) {
                    tab[lin][col] = tab[k][col];
                    tab[k][col] = ' ';
                }
            }
        }
    }
}

// funcao da ficha portal
void FichaPortal(int linha, int coluna){
	    //Vai por a ficha portal
	    tab[linha][coluna] = portal;
	    
	    //Atulizacao de tela
	    LimparTela();
	    ExibeTabuleiro();
	    DelayAutomatico();
	    
        // Se nao houver apoio 
        if(linha< 5 && tab[linha+1][coluna] != ' ') {
        	tab[linha + 1][coluna] = ' ';
        }
        
        //Remove a ficha portal
    	tab[linha][coluna] = ' ';  
		    	
    	// Atulizacao de tela
	    LimparTela();
	    ExibeTabuleiro();
   }
   
//Funcao vai ser quem vai destruir tudo!!!
void Explodir(int linha, int coluna){
	
	//vai apagar a ficha explosiva da posicao dela
    tab[linha][coluna] = ' ';
    
    //vai percorre todas as posicoes ao redor da explosiva
    for(i = -1;i<=1;i++){
        for(j = -1; j<=1;j++){
        	
        	//Calculo das novas linhas e colunas da vizinhaca
            int nl = linha + i;
            int nc = coluna +j;
            
            // Se a posicao for fora do tabuleiro, vai ignorar e continuar o loop
            if(nl<0||nl>5||nc<0||nc>6){
                continue;
            }
            // Caso tenha outras fichas explosivas atividas, vai igual dominor, explodindo tudo
            if(tab[nl][nc] == EXPL_ATIVA_X || tab[nl][nc] == EXPL_ATIVA_O){
            	tab[nl][nc] = ' ';
                Explodir(nl,nc);
            }
            // Caso contrario vai apenas explodir qualquer ficha comun
            else{
                tab[nl][nc] = ' ';
            }
        }
    }
    //Atualizacao de tela
    AplicarGravidade();
    LimparTela();
    ExibeTabuleiro();
}

// vai por uma ficha explosiva
void PorExplosiva(int linha, int coluna, char jogador){
    
    // ver qual jogado atual
    if(jogador == 'X'){
    // se for o jogado X vai por uma ficha explosiva dele
        tab[linha][coluna] =    EXPL_ATIVA_X;
    }
    
    //caso contrario vai por do jogado O
    else{
        tab[linha][coluna] =    EXPL_ATIVA_O;
    }
    
}

//vericaficao da ficha abaixo da explosiva
void VerExplosivaDeBaixo(int linha, int coluna, char jogador){

    // se a ficha caiu na ultima linha, nï¿½o tem o que verificar
    if(linha == 5){
        return;
    }    
    
    // vai guarda quem a ficha de baixo
      char abaixo = tab[linha + 1][coluna];
    
    //se tiver uma ficha explosiva ativa do jogado X na posicao abaixo
   if(abaixo == EXPL_ATIVA_X){
   	
   	// se quem jogou foi o adversario O, ela vai explodi
       if(jogador == 'O'){
           Explodir(linha + 1, coluna);
       }
       // agora se quem jogou foi o jogado X, ela vai ser uma ficha comum
       else{
           tab[linha + 1][coluna] = EXPL_DESAT_X;
       }
   }
   
       //se tiver uma ficha explosiva ativa do jogado O na posicao abaixo
   else if(abaixo == EXPL_ATIVA_O){
   	
   	// se quem jogou foi o adversario X, ela vai explodi
       if(jogador == 'X'){
           Explodir(linha + 1, coluna);
       }
       // agora se quem jogou foi o jogado O, ela vai ser uma ficha comum
       else{
           tab[linha + 1][coluna] = EXPL_DESAT_O;
       }
   }
}

// Dados dos jogares, aparecem quando iniciada uma partida em qualquer modo
void dadoDoJogo(){
	printf(BOLD "  RODADA: %d" RESET "\n", rodada);
    printf("  +-------------------------------------------------------+\n");
    printf("  ¦ JOGADOR: %-16s ¦ JOGADOR: %-16s ¦\n", nomeJogador1, nomeJogador2);
    printf("  +---------------------------+---------------------------¦\n");
    printf("  ¦ " YELLOW "Comuns: %-2d" RESET "                ¦ " BLUE "Comuns: %-2d" RESET "                ¦\n", comunsX, comunsO);
    printf("  ¦ " MAGENTA "Portais: %-2d" RESET "               ¦ " MAGENTA "Portais: %-2d" RESET "               ¦\n", portalX, portalO);
    printf("  ¦ " RED "Explosivas: %-2d" RESET "            ¦ " RED "Explosivas: %-2d" RESET "            ¦\n", explosivasX, explosivasO);
    printf("  +-------------------------------------------------------+\n");
}

// Funcao jogada humana, ela vai ver o tipo de ficha e colocar na coluna indicada 
void JogadaHumana() {
	
	//vai iniciar com o tipo de ficha comum
	int TFicha = 1;
	
	// vai pergunta ao jogador X qual ficha ele quer usar
	if (jogador == 'X' && (portalX > 0 || explosivasX > 0)) {
    	printf(BOLD "\n  +---------------- SELECIONE SUA FICHA ----------------+" RESET "\n");
		printf("  ¦                                                     ¦\n");
		printf("  ¦" YELLOW BOLD "[1]" RESET " Ficha Comum " MAGENTA BOLD "[2]" RESET " Ficha Portal " RED BOLD "[3]" RESET " Ficha Explosiva ¦\n");
		printf("  ¦                                                     ¦\n");
		printf(BOLD "  +-----------------------------------------------------+" RESET "\n");
		printf(BOLD "  Sua escolha: " RESET);
    	scanf("%d", &TFicha);
    	
    		// Caso o jogado digite um valo diferente de 1 e 3 ele vai por a ficha comun automaticamente
    		if(TFicha != 1 && TFicha!=2 && TFicha != 3){
    			printf("Você digitou um numero invalido, ganhara uma ficha comun.\n");
    			getchar();
    			TFicha = 1;
			}
			
			//Caso o jogador X tente usar um ficha portal sem ter nenhuma, ele irá receber um ficha comum
			else if(TFicha == 2 && ((jogador == 'X' && portalX == 0) || (jogador == 'O' && portalO == 0))){
					printf("\nVocê não tem ficha portal! Ganhara uma ficha comum.\n");
					getchar();
					TFicha = 1;
			}
	
			//Caso o jogador X tente usar um ficha explosiva sem ter nenhuma, ele irá receber um ficha comum
			else if(TFicha == 3 && ((jogador == 'X' && explosivasX == 0) || (jogador == 'O' && explosivasO == 0))){
					printf("\nVocê não tem ficha explosiva! Ganhara uma ficha comum.\n");
					getchar();
					TFicha = 1;
			}		
 	}
	// vai pergunta ao jogador O qual ficha ele quer usar
	else if (jogador == 'O' && (portalO > 0 || explosivasO > 0)) {
			printf(BOLD "\n  +---------------- SELECIONE SUA FICHA ----------------+" RESET "\n");
			printf("  ¦                                                     ¦\n");
			printf("  ¦" YELLOW BOLD "[1]" RESET " Ficha Comum " MAGENTA BOLD "[2]" RESET " Ficha Portal " RED BOLD "[3]" RESET " Ficha Explosiva ¦\n");
			printf("  ¦                                                     ¦\n");
			printf(BOLD "  +-----------------------------------------------------+" RESET "\n");
			printf(BOLD "  Sua escolha: " RESET); 
   				scanf("%d", &TFicha);
    	
    		// Caso o jogado digite um valo diferente de 1 e 3 ele vai por a ficha comun automaticamente
    		if(TFicha != 1 && TFicha!=2 && TFicha != 3){
    			printf("Você digitou um numero invalido, ganhara um comun.\n");
    			getchar();
    			TFicha = 1;
			}
		
			//Caso o jogador O tente usar um ficha explosiva sem ter nenhuma, ele irá receber um ficha comum
			else if(TFicha == 2 && (jogador == 'X' && portalX == 0) || (jogador == 'O' && portalO == 0)){
					printf("\nVocê não tem ficha portal! Ganhará uma comum.\n");
					getchar();
					TFicha = 1;
				}
			
			//Caso o jogador X tente usar um ficha explosiva sem ter nenhuma, ele irá receber um ficha comum
			else if(TFicha == 3 && (jogador == 'X' && explosivasX == 0) || (jogador == 'O' && explosivasO == 0)){
					printf("\nVocê não tem ficha explosiva! Ganhará uma comum.\n");
					getchar();
					TFicha = 1;
				}
	}

    //Vai solicita a coluna ao jogador X e depois ao jogador O
    if(jogador == 'X'){
        printf(YELLOW "\n%s" RESET ", informe a coluna da sua jogada: ",nomeJogador1);
    }
    else{
        printf(BLUE "\n%s" RESET ", informe a coluna da sua jogada: ",nomeJogador2);
    }
    
    int leuNumero = scanf("%d", &coluna);

    // Enquanto nao ler um numero (leuNumero == 0) ou o numero for invalido (<1 ou >7)
    while (leuNumero == 0 || coluna < 1 || coluna > 7) {
        
        // Se scanf retornou 0, significa que tem simbolo travando o buffer
        if(leuNumero == 0) {
            printf("Entrada invalida! Por favor, digite apenas NUMEROS: ");
            while(getchar() != '\n'); // Limpa o buffer 
        } 
        else {
            // Se leu um numero, mas ele esta fora do range 1-7
            printf("Coluna invalida! Digite um valor entre 1 e 7: ");
        }
        
        // Tenta ler novamente
        leuNumero = scanf("%d", &coluna);
    }
    
    //decrementa os espacos das colunas
    coluna--;

	// limite do tabuleiro
    linha = 5;

    // Procura a primeira posicao vazia de baixo para cima 
    while (linha >= 0 && tab[linha][coluna] != ' ') {
        linha--;
    }
    
	// verificacao para ver se a coluna ta cheia
    if (linha < 0) {
        printf("Essa coluna esta cheia. Informe outra coluna:\n");
        JogadaHumana();
        return;
    }

	// tipo de ficha comun
	if (TFicha == 1) {
		
		//jogador X
		if(jogador == 'X' && comunsX > 0){
			tab[linha][coluna] = jogador;	
    		VerExplosivaDeBaixo(linha,coluna,jogador);
			comunsX--;
		}
		
		//jogador O
		else if(jogador == 'O' && comunsO > 0){
			tab[linha][coluna] = jogador;
    		VerExplosivaDeBaixo(linha,coluna,jogador);
			comunsO--;
		}
	}

	// tipo de ficha portal
	else if (TFicha == 2) {
		
			//jogador X
  	  		if (jogador == 'X' && portalX > 0){	
  	  			FichaPortal(linha, coluna);
				portalX--;
			}
			
			//jogador O
			else if(jogador == 'O' && portalO > 0){
				FichaPortal(linha, coluna);
				portalO--;	
			}
	}	
	
	// tipo de ficha explosiva
	else if(TFicha == 3){
	    
	    // jogador X
	    if(jogador == 'X' && explosivasX > 0){
	    	PorExplosiva(linha,coluna,jogador);
       		VerExplosivaDeBaixo(linha,coluna,jogador);	
	        explosivasX--;
	    }
	    
	    //jogador O
	    else if(jogador == 'O' && explosivasO > 0){
	    	PorExplosiva(linha,coluna,jogador);
        	VerExplosivaDeBaixo(linha,coluna,jogador);
        	explosivasO--;
		}
	}
	
	// Incrementa o numero de jogadas 
    rodada++;
    
	// vai atualizar as fichas especias
	AtualizarFichas();
  
}

// funcao para a maquina poder jogar as fichas especias, portal e explosivas
int fichaMaquina(){
	int chance = rand() % 100;
	
	// caso tenha a ficha portal a chance de cair, tem 30% de cair na portal
	if(portalO>0 && chance <30){
		return 2;
	}
	
	// caso tenha a ficha explosiva a chance de cair, tem 30% de cair na esplosiva
	else if(explosivasO > 0 && chance < 60){
		return 3;
	}
	
	//ficha comum
	else{
		return 1;
	}
}

//Funcao para a Maquina jogar 
void JogadaAutomatica(){
    
    //Para saber se o computador jogou
    int jogou = 0;
    
    // chamanado a funcao de escolher a ficha da maquina
    int TFicha = fichaMaquina();
    while(!jogou){
        int coluna = rand()%7; //Escolhe uma coluna aleatoria entre 1 e 7
    
    //Simulador de gravidade
        for(linha = 5; linha >=0;linha--){
            if(tab[linha][coluna] == ' '){  
				
				// ficha comum
				if(TFicha == 1){
                	if(jogador == 'X'){
                		tab[linha][coluna] = jogador;
                		VerExplosivaDeBaixo(linha,coluna,jogador);
                		comunsX--;
					}
					else if (jogador == 'O'){
						tab[linha][coluna] = jogador;
                		VerExplosivaDeBaixo(linha,coluna,jogador);
						comunsO--;
					}
					
				}     
				 
				 //ficha portal                          	
				else if(TFicha == 2 && portalO > 0){
					FichaPortal(linha,coluna);
					portalO--;
				}
				
				//ficha explosivas
				else if(TFicha == 3 && explosivasO > 0){
					PorExplosiva(linha,coluna,jogador);
					VerExplosivaDeBaixo(linha,coluna,jogador);
					explosivasO--;
				}
				
				else{
					tab[linha][coluna] = jogador;
						if(jogador == 'X' && comunsX > 0){
                			comunsX--;
						}
						else{
							if(comunsO > 0){
								comunsO--;
							}
						}
						
				}
                jogou = 1; 
                rodada++;  
				AtualizarFichas();                     
                                     
                break;
            }
        }
    }
}



//Funcao vencer(verificar se houve um vencedor ou empate)
int vencer() {
    int i, j;

    // Verificao horizontal 
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 4; j++) {
            if (
                tab[i][j] != ' ' &&
                tab[i][j] == tab[i][j+1] &&
                tab[i][j] == tab[i][j+2] &&
                tab[i][j] == tab[i][j+3] 
            ) {
                ganhador = 1;
                return 1;
            }
        }
    }

    // Verificao vertical 
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 7; j++) {
            if (tab[i][j] != ' ' &&
                tab[i][j] == tab[i+1][j] &&
                tab[i][j] == tab[i+2][j] &&
                tab[i][j] == tab[i+3][j]) {
                ganhador = 1;
                return 1;
            }
        }
    }

    // Verificao da diagonal principal 
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            if (tab[i][j] != ' ' &&
                tab[i][j] == tab[i+1][j+1] &&
                tab[i][j] == tab[i+2][j+2] &&
                tab[i][j] == tab[i+3][j+3]) {
                ganhador = 1;
                return 1;
            }
        }
    }

    // Verificao da diagonal secundaria
    for (i = 3; i < 6; i++) {
        for (j = 0; j < 4; j++) {
            if (tab[i][j] != ' ' &&
                tab[i][j] == tab[i-1][j+1] &&
                tab[i][j] == tab[i-2][j+2] &&
                tab[i][j] == tab[i-3][j+3]) {
                ganhador = 1;
                return 1;
            }
        }
    }
	
    // Verificao de empate 
    if (comunsX == 0 && comunsO == 0) {
        ganhador = 2;
        return 2;
    }
	
    return 0;
}

//Funcao para limpar Tabuleir ela reinicia o tabuleiro para uma nova partida    
void LimpaTabuleiro() {
    int i, j;

    // Preenche o tabuleiro com espaï¿½os vazios 
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 7; j++) {
            tab[i][j] = ' ';
        }
    }
    
    //Vai zerar todos os dados antes de uma nova partida
    rodada = 0;
    ganhador = 0;
    comunsX = 21;
	comunsO = 21;
	portalX = 0;
	portalO = 0;
	explosivasX = 0;
	explosivasO = 0;
}




//Menu inical
int MenuPrincipal(){
	
	LimparTela();
    int menu;
    
    printf("\x1b[34m" "==========================================\n");
    printf("\x1b[1m\x1b[33m");
    printf("     _      ___  ____ _   _  _____ \n");
    printf("    | |    |_ _ |  _ | | | || ____|\n");
    printf("    | |     | | | | _| | | ||  _|  \n");
    printf("    | |___  | | | |_| | |_| || |___ \n");
    printf("    |_____||___| \\____|\\___/ |_____|\n");
    printf("            Q U A T R O ++           \n");
    printf("\x1b[0m\x1b[34m" "==========================================\n\n" "\x1b[0m");

    printf("\x1b[1m  [1]\x1b[0m Iniciar novo jogo\n");
    printf("\x1b[1m  [2]\x1b[0m Hall dos Campeões\n");
    printf("\x1b[1m  [3]\x1b[0m Como Jogar?\n");
    printf("\x1b[31m  [0] Sair\n\n" "\x1b[0m");

    printf("\x1b[34m" "==========================================\n" "\x1b[0m");
    printf(" Escolha uma opção: ");
    
    
    // Leitura da opcao escolhida e tratamento caso digite algo diferente do esperado
    int lerMenu = scanf("%d", &menu);
   
    while(lerMenu == 0 || menu < 0 || menu > 3) {
        if(lerMenu == 0) {
            //joga fora as letras digitadas erradas
            fflush(stdin); 
            printf("Entrada invalida! Digite apenas NUMEROS(1, 2, 3 ou 0): ");
        } else {
            printf("Entrada invalida! Digite apenas NUMEROS(1, 2, 3 ou 0): ");
        }
        // Tenta ler novamente
        lerMenu = scanf("%d", &menu);
    }
    
	getchar();
    return menu;
}

//Menu para escolher modo de jogo
int ModoJogo(){
	
	LimparTela();
    int modo;
    
    printf("\n" CYAN "  +----------------------------------------+" RESET "\n");
    printf(CYAN "  ¦" RESET BOLD "             MODO DE JOGO               " RESET CYAN "¦" RESET "\n");
    printf(CYAN "  ¦----------------------------------------¦" RESET "\n");
    printf("  ¦                                        ¦\n");
    
    // opcao 1
    printf("  ¦  " GREEN BOLD "[1]" RESET  BOLD "    Jogador vs Jogador" RESET "             ¦\n");
    printf("  ¦      " GREEN "---- Clássico local ----" RESET "          ¦\n");
    printf("  ¦                                        ¦\n");

    // opcao 2
    printf("  ¦  " YELLOW BOLD "[2]" RESET  BOLD "    Jogador vs Máquina" RESET "             ¦\n");
    printf("  ¦      " YELLOW "---- Desafie a CPU ----" RESET "           ¦\n");
    printf("  ¦                                        ¦\n");

    // opcao 3
    printf("  ¦  " MAGENTA BOLD "[3]" RESET  BOLD "    Máquina vs Máquina" RESET "             ¦\n");
    printf("  ¦      " MAGENTA "---- Modo Automático ----" RESET "         ¦\n");
    printf("  ¦                                        ¦\n");

    printf(CYAN "  +----------------------------------------+" RESET "\n");
    printf(BOLD "\n  Escolha o modo de jogo: " RESET);
    
    
    // Leitura da opcao escolhida e tratamento caso digite algo diferente do esperado
    int leu = scanf("%d", &modo);

    
    while(leu == 0 || modo < 1 || modo > 3) {
        
        
        if(leu == 0) {
            //joga fora as letras digitadas erradas
            fflush(stdin); 
            printf("Entrada invalida! Digite apenas NUMEROS: ");
        } else {
            printf("Opcao invalida! Digite 1, 2 ou 3: ");
        }
        
        // Tenta ler novamente
        leu = scanf("%d", &modo);
    }
    
    getchar(); // Consome o enter 
    return modo;
}

//Funcao para escanear os nomes dos jogadores
void DefinirNomes(int modo){
    
    if(modo == 1){
        printf(YELLOW "  Digite o nome do jogador 1: " RESET);
        fgets(nomeJogador1, sizeof(nomeJogador1), stdin);
        nomeJogador1[strcspn(nomeJogador1, "\n")] = '\0';
    
        printf(BLUE "  Digite o nome do jogador 2: " RESET);
        fgets(nomeJogador2, sizeof(nomeJogador2), stdin);
        nomeJogador2[strcspn(nomeJogador2, "\n")] = '\0';
    }
    
    else if (modo == 2) {
        printf(YELLOW "  Digite o nome do Jogador: " RESET);
        fgets(nomeJogador1, sizeof(nomeJogador1), stdin);
        nomeJogador1[strcspn(nomeJogador1, "\n")] = '\0'; 
        strcpy(nomeJogador2, "Computador");
    }

    else if (modo == 3) {
        strcpy(nomeJogador1, "Alpha");
        strcpy(nomeJogador2, "Beta");
    }   
}

// criacao dos dados do HALL
typedef struct {
    char nome[50];
    int jogadas;
}Campeao;

// funcao para registra no arquivo hall.txt os campeoes
void RegistroDoHall(char nome[], int jogadas) {
    FILE *arquivo = fopen("hall.txt", "a"); // cria o arquivo
    
    // verificacao de possivel erro de criacao
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo do HALL\n");
        return;
    }
    
    // registro no arquivo
    fprintf(arquivo,"%s %d\n",nome,jogadas);
    fclose(arquivo);
}

// funcao de exibicao/ler do arquivo hall para um vetor
void ExibiHall(){
    FILE *arquivo = fopen("hall.txt", "r");
            int i,j;

    if(arquivo == NULL){
        printf("\nNenhunm campeao registrado ainda");
        return;
    }
    
    Campeao campeoes[100];
    int total = 0;
    
    // leitura do arquivo
    while(total < 100 && fscanf(arquivo,"%s %d", campeoes[total].nome, &campeoes[total].jogadas) != EOF){
        total++;
    }  
  
  fclose(arquivo);
  
  // ordendacao do menor numero de jogadas
  for(i=0; i< total -1; i++){
      for(j = i + 1; j < total; j++){
          if(campeoes[i].jogadas > campeoes[j].jogadas){
              Campeao temp = campeoes[i];
              campeoes[i] = campeoes[j];
              campeoes[j] = temp;
          }
      }
  }
  
  // vai exibir somente os tops 3 
  int limite;
  
  if(total < 3){
    limite = total;
  }
  
  else{
    limite = 3;
  }
// Exibicao do hall da fama  
  printf("\n" CYAN "  +----------------------------------------+" RESET "\n");
    printf(CYAN "  ¦" RESET BOLD "            HALL DOS CAMPEÕES           " RESET CYAN "¦" RESET "\n");
    printf(CYAN "  ¦----------------------------------------¦" RESET "\n");
    printf("  ¦                                        ¦\n");
    
  for(i = 0; i< limite; i++){
  	
	if(i == 0){
    	printf("  ¦  " GOLD "1º " RESET " %-15s " GOLD "¦" RESET " %2d jogadas      ¦\n", campeoes[i].nome, campeoes[i].jogadas);
	}
	
    else if(i == 1){
    	printf("  ¦  " SILVER "2º " RESET " %-15s " SILVER "¦" RESET " %2d jogadas      ¦\n", campeoes[i].nome, campeoes[i].jogadas);
	}
	
    else if(i == 2){
    	printf("  ¦  " BRONZE "3º " RESET " %-15s " BRONZE "¦" RESET " %2d jogadas      ¦\n", campeoes[i].nome, campeoes[i].jogadas);
	}
    printf("  ¦                                        ¦\n");
    printf(CYAN "  +----------------------------------------+" RESET "\n");
}
  }
  
// Exibicao do manual
void ManualDoJogo(){

	printf("\n" BLUE "  +------------------------------------------------------+" RESET "\n");
    printf(BLUE "  ¦" RESET BOLD "                COMO JOGAR: LIGUE 4++                 " RESET BLUE "¦" RESET "\n");
    printf(BLUE "  +------------------------------------------------------+" RESET "\n\n");

    printf(YELLOW BOLD "  [ OBJETIVO ]" RESET "\n");
    printf("    Alinhe 4 fichas da sua cor na horizontal, vertical\n");
    printf("    ou diagonal em um tabuleiro de 7x6.\n\n");
    
	printf(GREEN BOLD "  [ REGRAS DO JOGO ]" RESET "\n\n");
	printf("    Sera declarado empate quando ambos os jogadores\n");
	printf("    nao tiverem mais fichas comnuns.\n\n");
	printf("    Caso o tente usar uma ficha que nao tenha ou digite um valor\n");
	printf("    na escolha da ficha, ele ira recebe uma ficha comum automaticamente.\n\n");
	
	
    printf(CYAN BOLD "  [ TIPOS DE FICHAS ]" RESET "\n\n");
    
    // Ficha Comum
    printf("  " BOLD "   Ficha Comum:" RESET "\n");
    printf("    A base do jogo. Sem efeitos especiais.\n\n");

    // Ficha Explosiva
    printf("  " RED BOLD "   Ficha Explosiva:" RESET "\n");
    printf("    Ganha a cada 5 rodadas. Elimina todas as fichas\n");
    printf("    ao seu redor (3x3), mas ela so sera atividade quando \n");
	printf("    uma ficha comnun adversaria cai sobre ela.\n\n");
	
    // Ficha Portal
    printf("  " BLUE BOLD "   Ficha Portal:" RESET "\n");
    printf("    Ganha a cada 5 rodadas. Remove a ficha que estiver\n");
    printf("    diretamente abaixo dela e desaparece em seguida.\n\n");

    printf(BLUE "  " "------------------------------------------------------" RESET "\n");    
    }

//Funcao Main    
int main() {
    
    setlocale(LC_ALL,"Portuguese_Brazil"); //Define o idoma
    srand(time(NULL)); 
    
    int menu; //menu main
    int modo; //seletor de modo
    
    // Loop infinito para permitir varias partidas 
    while (1) {
        menu = MenuPrincipal();
    
        if(menu == 1){
        	modo = ModoJogo();
            fflush(stdin); // Limpa o buffer antes de ler nomes
            DefinirNomes(modo);
            LimpaTabuleiro();
            ExibeTabuleiro();
            jogador = 'X';
        
            //Loop da partida(so sai quando alguem ganha ou empata)
            while (ganhador != 1 && ganhador != 2) {
            	
            	////Atualiza o tabueleiro na tela
                LimparTela();
                ExibeTabuleiro();
                dadoDoJogo();
            
                //Modo 1: Jogador X Jogador
                if(modo == 1){
                    JogadaHumana();
                    dadoDoJogo();
                }
            
                //Modo 2: Jogador vs Computador 
                else if(modo==2){
                    if(jogador=='X'){
                        JogadaHumana();
                    }
                
                    else{
                        printf("\n Computador pensando...\n");
                        JogadaAutomatica();
                        DelayAutomatico();
                    }
                
                }
            
                //Modo 3: Computador vs Computador 
                else if (modo == 3){
                	
                	if(jogador == 'X'){
                		printf(YELLOW "\nVez de %s jogar\n" RESET, nomeJogador1);
					} else {
						printf(BLUE "\nVez de %s jogar \n" RESET, nomeJogador2);
					}
                    JogadaAutomatica();
                    DelayAutomatico();
                }
            
                //Atualiza o tabueleiro na tela 
                LimparTela();
                ExibeTabuleiro();
                vencer();

                // So troca o jogador se ninguem ganhou 
                if (ganhador == 0){
                    TrocarJogador();
                }
            }
            
            // se tiver algum ganhado, fara a verificacao quem ganhou
            if(ganhador == 1){
            	
            	// Caso for o jogador X
                if(jogador == 'X'){
                    printf(YELLOW " PARABENS %s! VOCE VENCEU!\n" RESET, nomeJogador1);
                    RegistroDoHall(nomeJogador1,rodada);
                }
                // Caso for o jogador O
                else {
                    printf(BLUE " PARABENS %s! VOCE VENCEU!\n" RESET, nomeJogador2);
                    RegistroDoHall(nomeJogador2,rodada);
                }
            }
            
            // Se ninguem ganhou deu empate
            else if(ganhador == 2) {
                printf(GREEN "O JOGO EMPATOU! As fichas comuns acabaram " RESET);
            }
            
            //Quando finalizar, vai voltar ao menu
   			printf("\n   " BOLD "[ Pressione ENTER para voltar ]" RESET);
         	fflush(stdin);
            getchar(); // Pausa o jogo
          
    }
    
    //Opcao de menu
    else if(menu == 2){
    	LimparTela();
        ExibiHall();
   		printf("\n      " BOLD "[ Pressione ENTER para voltar ]" RESET);
        getchar();  // Pausa o jogo
    }
    
    else if(menu == 3){
    	LimparTela();
    	ManualDoJogo();
   		printf("\n" BOLD "           [ Pressione ENTER para voltar ]" RESET);
        getchar();  // Pausa o jogo
	}
    //Encerra o programa 
    else if(menu == 0){
        printf("Encerrando o jogo....\n");
        break;
    }
    // Opcao invalida do menu 
    else{
        printf("Opcao invalida\n");
    	printf("\n" BOLD "[ Pressione ENTER para voltar ]" RESET);
        fflush(stdin);
        getchar(); // Pausa o jogo
    }
    }

    //Finalza o programa 
    return 0;
}

