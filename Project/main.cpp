/*
	DESCRIÇÃO: Jogo criado para a disciplina de Algoritmos e Programação.
	AUTORES:
		Carlos Eduardo de Borba Machado;
		Caio Gonzaga Bernils;
		Mateus Luis Ropke Lauer.
*/

// BIBLIOTECAS
//
#include <iostream>
#include <time.h>
#include <conio.h>
#include <graphics.h>
//

// CONSTANTES
//
#define L 16
#define C 32
#define TEMPO_MAXIMO 45.0
//

/*-------------------------------- STRUCTS ------------------------------------*/

typedef struct Controle {
	bool menu = false, jogar = true, pausa = false, sair = false, facil = true;
	clock_t tempoInicial = 0, tempoFinal = 0;
	double tempoTotal = TEMPO_MAXIMO, tempoExecucao = 0;
	int mapaAtual = 0;
};

typedef struct Formiga {
	int comidaAtual = 0;
	bool vazio = true;
	int x = 1, y = 1;
};

typedef struct Mapa {
	int m[L][C];
};

/*-----------------------------------------------------------------------------*/




// PARTE DO CÓDIGO ESCRITA POR CAIO
//
// Responsável por preencher as matrizes
void preencherMapas(Mapa m[])
{
	int mapa1[L][C] =
	{
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,8,1 },
		{ 1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,5,1 },
		{ 1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1 },
		{ 1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1 },
		{ 1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,7,1,0,1,1,1,1,1,1,1,1,1,0,1 },
		{ 1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,0,5,1,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,1 },
		{ 1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1 },
		{ 1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1,1 },
		{ 1,6,5,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,1,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};
	int mapa2[L][C] =
	{
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,5,8,1 },
		{ 1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,0,1,1,1,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,1,1,1,1,1,1,1,1,1,1,1,0,1 },
		{ 1,0,1,1,1,1,1,1,0,1,0,1,1,0,1,1,0,1,5,0,0,1,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,0,1,1,1,1,1,1 },
		{ 1,1,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,1 },
		{ 1,0,1,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1 },
		{ 1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1 },
		{ 1,6,5,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};
	int mapa3[L][C] =
	{
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,8,1 },
		{ 1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,5,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1 },
		{ 1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1 },
		{ 1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,1,0,1,1,1,1,1,1,1,1,1,0,1 },
		{ 1,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,0,5,1,0,1,1,1,0,0,0,0,0,0,0,1 },
		{ 1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1 },
		{ 1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,0,1 },
		{ 1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,1 },
		{ 1,6,5,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};

	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < C; j++)
		{
			m[0].m[i][j] = mapa1[i][j];
			m[1].m[i][j] = mapa2[i][j];
			m[2].m[i][j] = mapa3[i][j];
		}

	}
}

// Apresenta a tela de menu ao jogador
void menu(Controle &c)
{
	static bool opcao = false;
	char tecla;

	tecla = getch();

	switch (tecla)
	{
	case 'w': //cima
		if (opcao)
			opcao = false;
		break;
	case 's': //baixo
		if (!opcao)
			opcao = true;
		break;
	case 27: // sair
		c.sair = true;
		break;
	case 32: // seleciona
		if (opcao)
			c.facil = false;
		else
			c.facil = true;
		c.menu = false;
		break;
	}
	//----------------- MENU --------------------- //

	//LÓGICA MENU
	
	//-------------------------------------------- //
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CAIO



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR CARLOS
//
// Verifica se a formiga está ou não com comida
void formigaAtual(int m[L][C], Formiga f)
{
	if (f.vazio)
		m[f.x][f.y] = 9;
	else
		m[f.x][f.y] = 10;
}

// Inicializa o armazém inicial
void iniciarArmazem(int armazen[3][4])
{
	int i, j, aux = 4;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (i == 0)
			{
				armazen[i][j] = aux;
				aux--;
			}
			else
			{
				armazen[i][j] = 0;
			}
		}
	}
}

// Nessa função verifica-se se vai retirar ou colocar a comida no armazém
void verificarArmazem(Formiga &f, int local[3][4], int a, int m[L][C])
{
	int i;
	bool s = false;

	if (f.comidaAtual == 0) // se a formiga não tiver comida, significa que ela vai pegar do armazém
	{
		for (i = 3; i >= 0; i--)
		{
			if (local[a][i] != 0)
			{
				f.comidaAtual = local[a][i];
				local[a][i] = 0;
				m[f.x][f.y] = 10;
				f.vazio = false;
				i = -1;
			}
		}
	}
	else // se já tiver ela vai colocar no armazém
	{
		for (i = 0; i <= 3; i++)
		{
			if (local[a][i] == 0)
			{
				if (i == 0)
					s = true;
				else if (f.comidaAtual < local[a][i - 1])
					s = true;

				if (s)
				{
					local[a][i] = f.comidaAtual;
					f.comidaAtual = 0;
					m[f.x][f.y] = 9;
					f.vazio = true;
				}
				i = 4;
			}
		}
	}
}

// Nessa função são executados os comandos do jogo
void lerComandos(int m[L][C], int armazem[3][4], Controle &c, Formiga &f) {
	
	char tecla = getch();
	switch (tecla)
	{
	case 'w': // cima
		if (m[f.x - 1][f.y] == 0)
		{
			m[f.x][f.y] = 0;
			f.x--;
			formigaAtual(m, f);
		}
		break;
	case 's': // baixo
		if (m[f.x + 1][f.y] == 0)
		{
			m[f.x][f.y] = 0;
			f.x++;
			formigaAtual(m, f);
		}
		break;
	case 'a': // esquerda
		if (m[f.x][f.y - 1] == 0)
		{
			m[f.x][f.y] = 0;
			f.y--;
			formigaAtual(m, f);
		}
		break;
	case 'd': // direita
		if (m[f.x][f.y + 1] == 0)
		{
			m[f.x][f.y] = 0;
			f.y++;
			formigaAtual(m, f);
		}
		break;
	case 27: // sair
		c.sair = true;
		break;
	case 32:  // pega ou deposita comida
		// se alguma posição ao redor da formiga for comida ela pega
		if (m[f.x + 1][f.y] == 5 || m[f.x - 1][f.y] == 5 || m[f.x][f.y + 1] == 5 || m[f.x][f.y - 1] == 5)
		{
			if (f.x > 10 && f.y < 5) // armazem 1 (inicial baixo)
				verificarArmazem(f, armazem, 0, m);
			if (f.x > 5 && f.x < 10 && f.y > 14 && f.y < 20) // armazem 2 (meio)
				verificarArmazem(f, armazem, 1, m);
			if (f.x < 5 && f.y > 25) // armazem 3 (cima final)
				verificarArmazem(f, armazem, 2, m);
		}
		break;
	}
	Sleep(0);
}

// Nessa função a matriz é percorrida e os números são substituidos
void imprimir(int m[L][C], int a[3][4], Controle c, Formiga f)
{
	int l = 0, t = 25;
	int x = 0, y = 100;
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < C; j++)
		{
			if (m[i][j] == 1){         // parede
				setfillstyle(1, GREEN);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 0){   // caminho
				setfillstyle(1, BROWN);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 9){   // personagem vazio
				setfillstyle(1, BLUE);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 10){  // personagem cheio
				setfillstyle(1, RED);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 6){   // armazém 1
				setfillstyle(1, WHITE);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 7){   // armazém 2
				setfillstyle(1, WHITE);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 8){   // armazém 3
				setfillstyle(1, WHITE);
				bar(l+x, l+y, t+x, t+y);
			}else if (m[i][j] == 5){   // local de comida
				setfillstyle(1, WHITE);
				bar(l+x, l+y, t+x, t+y);
			}
			x += 25;
		}
		y += 25;
		x = 0;
	}

	/*---------------------------- HUD -------------------------------*/
	//PROVAVELMENTE SERÁ RETIRADO
	/*
	cout << endl;
	cout << "Tempo ate o terremoto: " << (int)c.tempoTotal << "   " << endl;
	if (f.comidaAtual == 0)
		cout << "Comida atual da formiga: nenhuma \n\n";
	else
		cout << "Comida atual da formiga: " << f.comidaAtual << "         " << endl << endl;
	cout << "Armazem 1 (BAIXO):\nP1: " << a[0][0] << " | P2: " << a[0][1] << " | P3: " << a[0][2] << " | P4: " << a[0][3] << endl << endl;
	cout << "Armazem 2  (MEIO):\nP1: " << a[1][0] << " | P2: " << a[1][1] << " | P3: " << a[1][2] << " | P4: " << a[1][3] << endl << endl;
	cout << "Armazem 3  (CIMA):\nP1: " << a[2][0] << " | P2: " << a[2][1] << " | P3: " << a[2][2] << " | P4: " << a[2][3] << endl << endl;
	*/
	/*----------------------------------------------------------------*/
}

// Verificam se o jogador venceu
bool venceuDificil(int a[3][4])
{
	int i;

	for (i = 0; i < 4; i++)
		if (a[2][i] == 0)
			return false;
	return true;
}

bool venceuFacil(int a[3][4])
{
	if (a[2][0] == 4)
		return true;
	return false;
}
//-------------------------------

bool venceuJogo(double tempo)
{
	//LÓGICA VENCEU
	/*
	system("cls");
	cout << "VOCE VENCEU O JOGO, PARABENS!!!\n\n";
	cout << "O tempo total foi: " << tempo << endl << endl;
	system("pause");
	*/
	return true;
}
//-------------------------------
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CARLOS



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR MATEUS
//
// Muda a posição da formiga para não ser soterrada
void mudarPosicao(Formiga &f, Mapa m[], Controle c, int a)
{
	if (m[c.mapaAtual].m[f.x][f.y] == 0)
	{
		m[c.mapaAtual].m[f.x][f.y] = a;
	}
	else if (m[c.mapaAtual].m[f.x + 1][f.y] == 0)
	{
		m[c.mapaAtual].m[f.x + 1][f.y] = a;
		f.x++;
	}
	else if (m[c.mapaAtual].m[f.x - 1][f.y] == 0)
	{
		m[c.mapaAtual].m[f.x - 1][f.y] = a;
		f.x--;
	}
	else if (m[c.mapaAtual].m[f.x][f.y + 1] == 0)
	{
		m[c.mapaAtual].m[f.x][f.y + 1] = a;
		f.y++;
	}
	else if (m[c.mapaAtual].m[f.x][f.y - 1] == 0)
	{
		m[c.mapaAtual].m[f.x][f.y - 1] = a;
		f.y--;
	}
}
// função para medir tempo e sortear novo mapa 
void medirTempo(bool inicio, Controle &c, Mapa m[], Formiga &f)
{
	if (inicio)
	{
		c.tempoInicial = clock();
	}
	else
	{
		int aux = c.mapaAtual;

		c.tempoFinal = clock();
		c.tempoTotal -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;
		c.tempoExecucao -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;

		if (c.tempoTotal < 0)
		{
			for (int i = 0; i < L; i++)
			{
				for (int j = 0; j < C; j++)
				{
					if (m[c.mapaAtual].m[i][j] == 9)
						m[c.mapaAtual].m[i][j] = 0;
					if (m[c.mapaAtual].m[i][j] == 10)
						m[c.mapaAtual].m[i][j] = 0;
				}
			}

			srand(time(NULL));

			while (c.mapaAtual == aux)
				c.mapaAtual = rand() % 3;

			c.tempoTotal = TEMPO_MAXIMO;

			if (f.vazio)
				mudarPosicao(f, m, c, 9);
			else
				mudarPosicao(f, m, c, 10);
		}
	}
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR MATEUS



/*---------------------------------------------------------------------------------------------------*/



// TANTO A FUNÇÃO MAIN COMO SUAS VARIÁVEIS FORAM MANIPULADAS POR TODO O GRUPO
//
int main(void)
{
	/*-------------------------------- VARIÁVEIS ------------------------------*/
	Mapa m[3];
	Formiga f;
	Controle c;
	int armazem[3][4];
	/*-------------------------------------------------------------------------*/

	/*------------------------------ INICIALIZAÇÃO ----------------------------*/
	initwindow(805, 505, "The Ant Quest");
	preencherMapas(m);
	iniciarArmazem(armazem);
	/*-------------------------------------------------------------------------*/

	/*------------------------------ LOOP PRINCIPAL ---------------------------*/
	while (!c.sair)
	{
		if (c.menu)
		{
			menu(c);
		}
		else if (c.jogar)
		{
			medirTempo(true, c, m, f);
			imprimir(m[c.mapaAtual].m, armazem, c, f);
			lerComandos(m[c.mapaAtual].m, armazem, c, f);
			medirTempo(false, c, m, f);
		}
		else if (c.pausa)
		{
			
		}
		
		if (c.facil && venceuFacil(armazem))
		{
			c.sair = venceuJogo(c.tempoExecucao);
		}
		else if (!c.facil && venceuDificil(armazem))
		{
			c.sair = venceuJogo(c.tempoExecucao);
		}
	}
	/*-------------------------------------------------------------------------*/
	closegraph();
	return 0;
}
//
//
