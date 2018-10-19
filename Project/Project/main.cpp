/*
DESCRIÇÃO: Jogo criado para a disciplina de Algoritmos e Programação, referente à A1M2.
AUTORES: Carlos Eduardo de Borba Machado;
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

// CONSTANTES
//
#define L 16
#define C 32
#define TEMPO_MAXIMO 4.0
//

using namespace std;

/*-------------------------------- STRUCTS ------------------------------------*/
struct Formiga
{
	int comidaAtual = 0;
	bool vazio = true;
};

typedef struct Mapa
{
	int m[L][C];
};

struct Controle
{
	bool menu  = true,
		 facil = true,
		 fim   = false,
		 sair  = false;

	clock_t tempoInicial = 0,
			tempoFinal   = 0;

	double tempoTotal    = TEMPO_MAXIMO,
		   tempoExecucao = 0;

	int mapaAtual = 0;
};
/*-----------------------------------------------------------------------------*/

// Oculta o cursor do console
void esconderCursor(void)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

// Muda a posição do cursor do console (no caso desse jogo ele coloca na posição inicial para que o mapa possa ser atualizado a cada "frame" se é que da para chamar assim kk)
void setarCursor(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ static_cast<short>(x), static_cast<short>(y) });
}



/*---------------------------------------------------------------------------------------------------*/




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
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1 },
		{ 1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1 },
		{ 1,6,5,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1 },
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
		{ 1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,0,0,0,0,1 },
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
void menu(bool &facil, bool &sair, bool &menu)
{
	static bool opcao = false;
	char tecla;

	if (_kbhit())
	{
		tecla = _getch();

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
			sair = true;
			break;
		case 32: // seleciona
			if (opcao)
				facil = false;
			else
				facil = true;
			menu = false;
			break;
		}
	}
	//----------------- MENU --------------------- //

	cout << "********************************" << endl;
	cout << "*                              *" << endl;
	cout << "*                              *" << endl;
	if (!opcao)
	cout << "*           * Facil *          *" << endl;
	else
	cout << "*             Facil            *" << endl;
	cout << "*                              *" << endl;
	if (opcao)
	cout << "*          * Dificil *         *" << endl;
	else
	cout << "*            Dificil           *" << endl;
	cout << "*                              *" << endl;
	cout << "*                              *" << endl;
	cout << "*          Instrucoes:         *" << endl;
	cout << "*                              *" << endl;
	cout << "* O deposito esta desmoronando,*" << endl;
	cout << "*    retire todas as comidas   *" << endl;
	cout << "*  do dep. 1 e leve para o 3.  *" << endl;
	cout << "*  As comidas possuem tamanho, *" << endl;
	cout << "*  as maiores sempre em baixo. *" << endl;
	cout << "*                              *" << endl;
	cout << "*  Movimentacao - Teclas WASD  *" << endl;
	cout << "*     Pegar comida - Barra     *" << endl;
	cout << "*    Pressione ESC para sair   *" << endl;
	cout << "*     Desligue o Caps Lock     *" << endl;
	cout << "*                              *" << endl;
	cout << "********************************";
	//-------------------------------------------- //
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CAIO



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR CARLOS
//
// Nessa função a matriz é percorrida e os números são substituidos
void imprimir(int m[L][C], int comida, int a[3][4], double tempo)
{
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < C; j++)
		{
			if (m[i][j] == 1)        // parede
				cout << (char)178;
			else if (m[i][j] == 0)   // caminho
				cout << " ";
			else if (m[i][j] == 9)   // personagem vazio
				cout << (char)667;
			else if (m[i][j] == 10)  // personagem cheio
				cout << "O";
			else if (m[i][j] == 6)   // armazém 1
				cout << "1";
			else if (m[i][j] == 7)   // armazém 2
				cout << "2";
			else if (m[i][j] == 8)   // armazém 3
				cout << "3";
			else if (m[i][j] == 5)   // local de comida
				cout << (char)254;
		}
		cout << endl;
	}

	/*---------------------------- HUD -------------------------------*/

	cout << endl;
	cout << "Tempo ate o terremoto: " << (int)tempo << "   " << endl;
	if (comida == 0)
		cout << "Comida atual da formiga: nenhuma \n\n";
	else
		cout << "Comida atual da formiga: " << comida << "         " << endl << endl;
	cout << "Armazem 1 (BAIXO):\nP1: " << a[0][0] << " | P2: " << a[0][1] << " | P3: " << a[0][2] << " | P4: " << a[0][3] << endl << endl;
	cout << "Armazem 2  (MEIO):\nP1: " << a[1][0] << " | P2: " << a[1][1] << " | P3: " << a[1][2] << " | P4: " << a[1][3] << endl << endl;
	cout << "Armazem 3  (CIMA):\nP1: " << a[2][0] << " | P2: " << a[2][1] << " | P3: " << a[2][2] << " | P4: " << a[2][3] << endl << endl;

	/*----------------------------------------------------------------*/
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
void verificarArmazem(int &comida, int local[3][4], int a, int m[L][C], int x, int y, bool &vazio, bool &fim)
{
	int i;
	bool s = false;

	if (comida == 0) // se a formiga não tiver comida, significa que ela vai pegar do armazém
	{
		for (i = 3; i >= 0; i--)
		{
			if (local[a][i] != 0)
			{
				comida = local[a][i];
				local[a][i] = 0;
				m[x][y] = 10;
				vazio = false;
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
				else if (comida < local[a][i - 1])
					s = true;
				else if (comida > local[a][i - 1])
					fim = true;

				if (s)
				{
					local[a][i] = comida;
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
void lerComandos(int m[L][C], bool &sair, int local[3][4], bool &vazio, int &comidaAtualFormiga, bool &fim, int &x, int &y)
{
	char tecla;

	if (_kbhit())
	{
		tecla = _getch();

		switch (tecla)
		{
		case 'w': // cima
			if (m[x - 1][y] == 0)
			{
				m[x][y] = 0;
				x--;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 's': // baixo
			if (m[x + 1][y] == 0)
			{
				m[x][y] = 0;
				x++;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 'a': // esquerda
			if (m[x][y - 1] == 0)
			{
				m[x][y] = 0;
				y--;
				formigaAtual(m, x, y, vazio);
			}
			break;
		case 'd': // direita
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
			if (m[x + 1][y] == 5 || m[x - 1][y] == 5 || m[x][y + 1] == 5 || m[x][y - 1] == 5)
			{
				if (x > 10 && y < 5) // armazem 1 (inicial baixo)
					verificarArmazem(comidaAtualFormiga, local, 0, m, x, y, vazio, fim);
				if (x > 5 && x < 10 && y > 14 && y < 20) // armazem 2 (meio)
					verificarArmazem(comidaAtualFormiga, local, 1, m, x, y, vazio, fim);
				if (x < 5 && y > 25) // armazem 3 (cima final)
					verificarArmazem(comidaAtualFormiga, local, 2, m, x, y, vazio, fim);
			}
			break;
		}
		Sleep(50);
	}
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

// Mostram a tela de Game Over
bool fimJogo(void)
{
	system("cls");
	cout << "VOCE COLOCOU A COMIDA MAIOR EM CIMA DA MENOR E AGORA TODO MUNDO VAI MORRER DE FOME!!!\n\n";
	system("pause");
	return true;
}

bool venceuJogo(bool tempo)
{
	system("cls");
	cout << "VOCE VENCEU O JOGO, PARABENS!!!\n\n";
	cout << "O tempo total foi: " << tempo << endl << endl;
	system("pause");
	return true;
}
//-------------------------------
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CARLOS



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR MATEUS
//
// função para medir tempo e sortear novo mapa 
void medirTempo(bool inicio, int &mapa, clock_t &tempoInicial, clock_t &tempoFinal, double &tempoTotal, double &tempoExecucao, Mapa m[], int &x, int &y)
{
	int aux = mapa;
	bool carregando = false;
	if (inicio)
	{
		tempoInicial = clock();
	}
	else
	{
		tempoFinal = clock();
		tempoTotal -= (tempoFinal - tempoInicial) / (double)CLOCKS_PER_SEC;
		tempoExecucao -= (tempoFinal - tempoInicial) / (double)CLOCKS_PER_SEC;
		if (tempoTotal < 0)
		{
			for (int i = 0; i < L; i++) 
			{
				for (int j = 0; j < C; j++) 
				{
					if (m[mapa].m[i][j] == 9) 
					{
						x = i;
						y = j;
						m[mapa].m[i][j] = 0;
						carregando = false;
					}
					if (m[mapa].m[i][j] == 10) 
					{
						x = i;
						y = j;
						m[mapa].m[i][j] = 0;
						carregando = true;
					}
				}
			}
			srand(time(NULL));

			mapa = rand() % 3;

			tempoTotal = TEMPO_MAXIMO;

			if (carregando) 
			{
				if (m[mapa].m[x][y] == 0)
					m[mapa].m[x][y] = 10;
				else if (m[mapa].m[x + 1][y] == 0)
					m[mapa].m[x + 1][y] = 10;
				else if (m[mapa].m[x - 1][y] == 0)
					m[mapa].m[x - 1][y] = 10;
				else if (m[mapa].m[x][y + 1] == 0)
					m[mapa].m[x][y + 1] = 10;
				else if (m[mapa].m[x][y - 1] == 0)
					m[mapa].m[x][y - 1] = 10;
			}
			else 
			{
				if (m[mapa].m[x][y] == 0)
					m[mapa].m[x][y] = 9;
				else if (m[mapa].m[x + 1][y] == 0)
					m[mapa].m[x + 1][y] = 9;
				else if (m[mapa].m[x - 1][y] == 0)
					m[mapa].m[x - 1][y] = 9;
				else if (m[mapa].m[x][y + 1] == 0)
					m[mapa].m[x][y + 1] = 9;
				else if (m[mapa].m[x][y - 1] == 0)
					m[mapa].m[x][y - 1] = 9;
			}
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
	Mapa mapa[3];
	Formiga formiga;
	Controle controle;
	int armazem[3][4];
	int x = 1, y = 1;
	/*-------------------------------------------------------------------------*/

	/*------------------------------ INICIALIZAÇÃO ----------------------------*/
	esconderCursor();
	preencherMapas(mapa);
	iniciarArmazem(armazem);
	//setlocale(LC_ALL, "Portuguese");
	//AO TENTAR USAR O SETLOCALE BUGOU TODOS OS CARACTERES DO MAPA
	//POR ISSO AS PALAVRAS ESTÃO SEM ACENTO
	/*-------------------------------------------------------------------------*/

	/*------------------------------ LOOP PRINCIPAL ---------------------------*/
	while (!controle.sair)
	{
		if (controle.menu)
		{
			menu(controle.facil, controle.sair, controle.menu);
			if (!controle.menu)
				system("cls");
		}
		else
		{
			medirTempo(true, controle.mapaAtual, controle.tempoInicial, controle.tempoFinal, controle.tempoTotal, controle.tempoExecucao, mapa, x, y);
			imprimir(mapa[controle.mapaAtual].m, formiga.comidaAtual, armazem, controle.tempoTotal);
			lerComandos(mapa[controle.mapaAtual].m, controle.sair, armazem, formiga.vazio, formiga.comidaAtual, controle.fim, x, y);
			medirTempo(false, controle.mapaAtual, controle.tempoInicial, controle.tempoFinal, controle.tempoTotal, controle.tempoExecucao, mapa, x, y);
		}
		setarCursor(0, 0);
		if (controle.fim)
			controle.sair = fimJogo();
		if (controle.facil && venceuFacil(armazem))
			controle.sair = venceuJogo(controle.tempoExecucao);
		else if (!controle.facil && venceuDificil(armazem))
			controle.sair = venceuJogo(controle.tempoExecucao);
	}
	/*-------------------------------------------------------------------------*/
	return 0;
}
//
//