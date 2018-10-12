#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define L 16
#define C 32

using namespace std;

void hidecursor()
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

//Nessa função a matriz é percorrida e os números são substituidos
void imprime(int m[L][C], int comida)
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
	cout << endl << endl;
	if (comida == 0)
		cout << "Comida atual: nenhum";
	else
		cout << "Comida atual: " << comida;
}


// Nessa função são executados os comandos do jogo
void comandos(int m[L][C], bool &sair, int local1[], int local2[], int local3[], bool &vazio, int comidaAtualFormiga)
{
	static int x = 1, y = 1;
	char p;

	if (_kbhit())
	{
		p = _getch();

		switch (p)
		{
		case 'w': //cima
			if (m[x - 1][y] == 0)
			{
				m[x][y] = 0;
				x--;
				if (vazio) // verifica se a formiga está ou não com comida
					m[x][y] = 9;
				else
					m[x][y] = 10;
			}
			break;
		case 's': //baixo
			if (m[x + 1][y] == 0)
			{
				m[x][y] = 0;
				x++;
				if (vazio)
					m[x][y] = 9;
				else
					m[x][y] = 10;
			}
			break;
		case 'a': //esquerda
			if (m[x][y - 1] == 0)
			{
				m[x][y] = 0;
				y--;
				if (vazio)
					m[x][y] = 9;
				else
					m[x][y] = 10;
			}
			break;
		case 'd': //direita
			if (m[x][y + 1] == 0)
			{
				m[x][y] = 0;
				y++;
				if (vazio)
					m[x][y] = 9;
				else
					m[x][y] = 10;
			}
			break;
		case 'q': // sair
			sair = true;
			break;
		case 32:  // pega ou deposita comida
			// se alguma posição ao redor da formiga for comida ele pega
			if (m[x + 1][y] == 5 || m[x - 1][y] == 5 ||
				m[x][y + 1] == 5 || m[x][y - 1] == 5   )
			{
				//m[x][y] = 10;
				//vazio = false;
				if (x > 12 && y < 3) // armazem 1 (inicial baixo)
				{

				}
				if (x > 4 && x < 8 &&
					y > 15 && y < 19) // armazem 2 (meio)
				{

				}
				if (x < 4 && y > 28) // armazem 3 (cima)
				{

				}
			}
			break;
		}
		Sleep(100);
	}
}

// função para medir tempo e sortear novo mapa 
void medir_tempo(bool inicio, int &mapa)
{
	static clock_t tempo_inicial = 0, tempo_final = 0;
	static double tempo_total;
	const double TEMPO_MAXIMO = 120000.0;

	if (inicio)
	{
		tempo_inicial = clock();
	}
	else
	{
		tempo_final = clock();
		tempo_total += (tempo_final - tempo_inicial) / (double)CLOCKS_PER_SEC;
		if (tempo_total >= TEMPO_MAXIMO)
		{
			srand(time(NULL));
			mapa = 1 + (rand() % 2);
			tempo_total = 0.0;
		}
	}
}

struct Formiga
{
	int comida = 0;
	bool vazio = true;
};

struct Mapa
{
	int mapa1[L][C] =
	{
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,8,1 },
		{ 1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,2,1 },
		{ 1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1 },
		{ 1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1 },
		{ 1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,7,1,0,1,1,1,1,1,1,1,1,1,0,1 },
		{ 1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,1 },
		{ 1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,0,1,1,1,1,0,1 },
		{ 1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1 },
		{ 1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1 },
		{ 1,6,5,0,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};
	int mapa2[L][C];
	int mapa3[L][C];
	int mapa = 1;
};

struct Armazem
{
	int comida[4] = { 0, 0, 0, 0 };
};

int main()
{
	Formiga formiga;
	Mapa mapa;
	// cada struct do vetor Armazem representa um armazem no jogo
	Armazem local[3];
	bool sair = false;

	hidecursor();
	// aqui inicia o armazem inicial
	local[0].comida[0] = 4;
	local[0].comida[1] = 3;
	local[0].comida[2] = 2;
	local[0].comida[3] = 1;

	while (!sair)
	{
		//medir_tempo(1, mapa);
		if (mapa.mapa == 1)
		{
			imprime(mapa.mapa1, formiga.comida);
			comandos(mapa.mapa1, sair, local[0].comida, local[1].comida, local[2].comida, formiga.vazio, formiga.comida);
		}
		else if (mapa.mapa == 2)
		{
			imprime(mapa.mapa2, formiga.comida);
			comandos(mapa.mapa2, sair, local[0].comida, local[1].comida, local[2].comida, formiga.vazio, formiga.comida);
		}
		else if (mapa.mapa == 3)
		{
			imprime(mapa.mapa3, formiga.comida);
			comandos(mapa.mapa3, sair, local[0].comida, local[1].comida, local[2].comida, formiga.vazio, formiga.comida);
		}
		mgotoxy(0, 0);
		//medir_tempo(0, mapa);
	}
	return 0;
}