/*
DESCRIÇÃO: Jogo criado para a disciplina de Algoritmos e Programação, referente à M2.

AUTORES: Carlos Eduardo de Bobra Machado;
		 Caio Gonzaga Bernils;
		 Mateus Luis Ropke Lauer.
*/

// BIBLIOTECAS DO SISTEMA
//
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
//
// BIBLIOTECAS DO PROGRAMA
//

//
// CONSTANTES

#define L 16
#define C 32
#define TEMPO_MAXIMO 45.0

using namespace std;

void esconderCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void mgotoxy(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ static_cast<short>(x), static_cast<short>(y) });
}



/*---------------------------------------------------------------------------------------------------*/



/*
CAIO:
	- Implementar um menu para escolher jogo difícil ou fácil:
		- Seria interessante a implementação de um menu com as tleclas W e S para selecionar as
		  opções do menu, assim combinando com o resto do jogo;
		- O menu pode ter intruções dizendo: "Use as teclas W e S para movimentar o menu" e
		  "Use as teclas W, A, S e D para movimentar a formiga";
		- O menu pode ser rodado no laço principal usando a variável booleana controle.menu;
	- Criar mais dois mapas:
		- Só alterar os caminhos, os armazéns precisam estar no mesmo local;
		- Colocar os dois outros mapas criados dentro do "para" pra poder preencher as outras matrizes.
*/
// PARTE DO CÓDIGO ESCRITA POR CAIO
//
// Responsável por preencher as matrizes
void preencherMapas(int m1[L][C], int m2[L][C], int m3[L][C])
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
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1 },
		{ 1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1 },
		{ 1,6,5,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};
	int mapa2[L][C];
	int mapa3[L][C];

	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < C; j++)
		{
			m1[i][j] = mapa1[i][j];
			m2[i][j] = mapa1[i][j]+2;
			m3[i][j] = mapa1[i][j]+1;
		}

	}
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CAIO



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR CARLOS
//
// Nessa função a matriz é percorrida e os números são substituidos
void imprimir(int m[L][C], int comida, int a1[], int a2[], int a3[], double tempo)
{
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < C; j++)
		{
			if (m[i][j] == 1)
				cout << (char)178;
			else if (m[i][j] == 0)
				cout << " ";
			else if (m[i][j] == 9)
				cout << (char)667;
			else if (m[i][j] == 10)
				cout << "O";
			else if (m[i][j] == 6)
				cout << "1";
			else if (m[i][j] == 7)
				cout << "2";
			else if (m[i][j] == 8)
				cout << "3";
			else if (m[i][j] == 5)
				cout << (char)254;
		}
		cout << endl;
	}

	/*-------------- HUD -----------------*/

	cout << endl;
	cout << "Tempo ate o terremoto: " << (int)tempo << "   " << endl;
	if (comida == 0)
		cout << "Comida atual da formiga: nenhum\n\n";
	else
		cout << "Comida atual da formiga: " << comida << "         " << endl << endl;
	cout << "Armazem 1 (BAIXO):\nP1: " << a1[0] << " | P2: " << a1[1] << " | P3: " << a1[2] << " | P4: " << a1[3] << endl << endl;
	cout << "Armazem 2  (MEIO):\nP1: " << a2[0] << " | P2: " << a2[1] << " | P3: " << a2[2] << " | P4: " << a2[3] << endl << endl;
	cout << "Armazem 3  (CIMA):\nP1: " << a3[0] << " | P2: " << a3[1] << " | P3: " << a3[2] << " | P4: " << a3[3] << endl << endl;

	/*------------------------------------*/
}

// Verifica se a formiga está ou não com comida
void formigaAtual(int m[L][C], int x, int y, bool vazio)
{
	if (vazio)
		m[x][y] = 9;
	else
		m[x][y] = 10;
}

// Inicializa o armazém inicial
void iniciarArmazem(int comida[])
{
	int i, j = 4;
	for (i = 0; i < 4; i++)
	{
		comida[i] = j;
		j--;
	}
}

// Nessa função verifica-se se vai retirar ou colocar a comida no armazém
void verificarArmazem(int &comida, int local[], int m[L][C], int x, int y, bool &vazio, bool &fim)
{
	int i;
	bool s = false;
	if (comida == 0)
	{
		for (i = 3; i >= 0; i--)
		{
			if (local[i] != 0)
			{
				comida = local[i];
				local[i] = 0;
				m[x][y] = 10;
				vazio = false;
				i = -1;
			}
		}
	}
	else
	{
		for (i = 0; i <= 3; i++)
		{
			if (local[i] == 0)
			{
				if (i == 0)
					s = true;
				else if (comida < local[i - 1])
					s = true;
				else if (comida > local[i - 1])
					fim = true;

				if (s)
				{
					local[i] = comida;
					comida = 0;
					m[x][y] = 9;
					vazio = true;
				}
				i = 4;
			}
		}
	}
}

// Nessa função são executados os comandos do jogo
void lerComandos(int m[L][C], bool &sair, int local1[], int local2[], int local3[], bool &vazio, int &comidaAtualFormiga, bool &fim)
{
	static int x = 1, y = 1;
	char tecla;

	if (_kbhit())
	{
		tecla = _getch();

		switch (tecla)
		{
		case 'w': //cima
			if (m[x - 1][y] == 0)
			{
				m[x][y] = 0;
				x--;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 's': //baixo
			if (m[x + 1][y] == 0)
			{
				m[x][y] = 0;
				x++;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 'a': //esquerda
			if (m[x][y - 1] == 0)
			{
				m[x][y] = 0;
				y--;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 'd': //direita
			if (m[x][y + 1] == 0)
			{
				m[x][y] = 0;
				y++;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 27: // sair
			sair = true;
			break;
		case 32:  // pega ou deposita comida
			// se alguma posição ao redor da formiga for comida ela pega
			if (m[x + 1][y] == 5 || m[x - 1][y] == 5 || m[x][y + 1] == 5 || m[x][y - 1] == 5   )
			{
				if (x > 10 && y < 5) // armazem 1 (inicial baixo)
				{
					verificarArmazem(comidaAtualFormiga, local1, m, x, y, vazio, fim);
				}
				if (x > 5 && x < 10 && y > 14 && y < 20) // armazem 2 (meio)
				{
					verificarArmazem(comidaAtualFormiga, local2, m, x, y, vazio, fim);
				}
				if (x < 5 && y > 25) // armazem 3 (cima)
				{
					verificarArmazem(comidaAtualFormiga, local3, m, x, y, vazio, fim);
				}
			}
			break;
		}
		Sleep(50);
	}
}

// Verificam de o jogador venceu
bool venceuDificil(int a[])
{
	int i;
	for (i = 0; i < 4; i++)
		if (a[i] == 0)
			return false;
	return true;
}

bool venceuFacil(int a[])
{
	if (a[0] == 4)
		return true;
	return false;
}
//-------------------------------

// Mostram a tela de Game Over
bool fimJogo()
{
	system("cls");
	cout << "VOCE COLOCOU A COMIDA MAIOR EM CIMA DA MENOR E AGORA TODO MUNDO VAI MORRER DE FOME!!!\n\n";
	system("pause");
	return true;
}

bool venceuJogo()
{
	system("cls");
	cout << "VOCE VENCEU O JOGO, PARABENS!!!\n\n";
	system("pause");
	return true;
}
//-------------------------------
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CARLOS



/*---------------------------------------------------------------------------------------------------*/



/*
MATEUS:
	- Criar uma função/método que realize a troca do mapa;
	- Criar uma variável para mostrar o cronometro na tela.
*/
// PARTE DO CÓDIGO ESCRITA POR MATEUS
//
// função para medir tempo e sortear novo mapa 
void medirTempo(bool inicio, int &mapa, clock_t &tempoInicial, clock_t &tempoFinal, double &tempoTotal)
{
	if (inicio)
	{
		tempoInicial = clock();
	}
	else
	{
		tempoFinal = clock();
		tempoTotal -= (tempoFinal - tempoInicial) / (double)CLOCKS_PER_SEC;
		if (tempoTotal < 0)
		{
			srand(time(NULL));
			mapa = rand() % 2;
			tempoTotal = TEMPO_MAXIMO;
		}
	}
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR MATEUS



/*---------------------------------------------------------------------------------------------------*/



// TANTO A FUNÇÃO MAIN COMO SUAS VARIÁVEIS FORAM MANIPULADAS POR TODO O GRUPO
//

/*-------------------------------- STRUCTS ------------------------------------*/
struct Formiga
{
	int comidaAtual = 0;
	bool vazio = true;
};

struct Mapa
{
	int m[L][C];
};

struct Armazem
{
	int lugares[4] = {0, 0, 0, 0};
};

struct Controle
{
	bool menu = false,
		 sair = false,
		 fim = false,
		 facil = true;

	clock_t tempoInicial = 0, 
			tempoFinal = 0;

	double tempoTotal = TEMPO_MAXIMO;

	int mapaAtual = 0;
};
/*-----------------------------------------------------------------------------*/

int main()
{
	/*-------------------------------- VARIÁVEIS ------------------------------*/
	Mapa mapa[3];
	Armazem armazem[3]; // cada struct do vetor representa um armazem no jogo
	Formiga formiga;
	Controle controle;
	/*-------------------------------------------------------------------------*/

	/*------------------------------ INICIALIZAÇÃO ----------------------------*/
	esconderCursor();
	preencherMapas(mapa[0].m, mapa[1].m, mapa[2].m);
	iniciarArmazem(armazem[0].lugares);
	//setlocale(LC_ALL, "Portuguese");
	//AO TENTAR USAR O SETLOCALE BUGOU TODOS OS CARACTERES DO MAPA
	//POR ISSO AS PALAVRAS ESTÃO SEM ACENTO
	/*-------------------------------------------------------------------------*/

	while (!controle.sair)
	{
		medirTempo(true, controle.mapaAtual, controle.tempoInicial, controle.tempoFinal, controle.tempoTotal);
		if (controle.menu)
		{

		}
		else
		{
			imprimir(mapa[controle.mapaAtual].m, formiga.comidaAtual, armazem[0].lugares, armazem[1].lugares, armazem[2].lugares, controle.tempoTotal);
			lerComandos(mapa[controle.mapaAtual].m, controle.sair, armazem[0].lugares, armazem[1].lugares, armazem[2].lugares, formiga.vazio, formiga.comidaAtual, controle.fim);
		}
		mgotoxy(0, 0);
		if (controle.fim)
			controle.sair = fimJogo();
		if (controle.facil)
			if (venceuFacil(armazem[2].lugares))
				controle.sair = venceuJogo();
		else
			if (venceuDificil(armazem[2].lugares))
				controle.sair = venceuJogo();
		medirTempo(false, controle.mapaAtual, controle.tempoInicial, controle.tempoFinal, controle.tempoTotal);
	}
	return 0;
}
//
//
