#include <iostream>
#include <fstream> //Para leitura e escrita em arquivo
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <allegro5/allegro.h>

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
void imprime(int m[L][C])
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
				cout << (char)958;
			else if (m[i][j] == 2)
				cout << "1";
			else if (m[i][j] == 3)
				cout << "2";
			else if (m[i][j] == 4)
				cout << "3";
			else if (m[i][j] == 5)
				cout << "4";
		}
		cout << "\n";
	}
}

void comandos(int m[L][C], bool &sair)
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
				m[x][y] = 9;
			}
			break;
		case 's': //baixo
			if (m[x + 1][y] == 0)
			{
				m[x][y] = 0;
				x++;
				m[x][y] = 9;
			}
			break;
		case 'a': //esquerda
			if (m[x][y - 1] == 0)
			{
				m[x][y] = 0;
				y--;
				m[x][y] = 9;
			}
			break;
		case 'd': //direita
			if (m[x][y + 1] == 0)
			{
				m[x][y] = 0;
				y++;
				m[x][y] = 9;
			}
			break;
		case 'q':
			sair = true;
		}
	}
}

void atualizar_mapa(int mapa1[L][C], int mapa2[L][C], int mapa3[L][C], int mapa, bool &sair)
{
	if (mapa == 1)
	{
		imprime(mapa1);
		comandos(mapa1, sair);
	}
	else if (mapa == 2)
	{
		imprime(mapa2);
		comandos(mapa2, sair);
	}
	else if (mapa == 3)
	{
		imprime(mapa3);
		comandos(mapa3, sair);
	}
}

void medir_tempo(int a, int &mapa)
{
	static clock_t tempo_inicial = 0, tempo_final = 0;
	static double tempo_total;
	const double TEMPO_MAXIMO = 120000.0;

	if (a == 0)
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

int main()
{
	int mapa1[L][C] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
		{1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1},
		{1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1},
		{1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1},
		{1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1},
		{1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1},
		{1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,0,1,1,1,1,0,1},
		{1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,1},
		{1,0,1,0,0,0,0,0,0,0,0,0,1,1,5,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1},
		{1,0,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,3,1,1,1,0,1,1,1,0,0,0,0,0,0,4,1,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	int mapa2[L][C];
	int mapa3[L][C];
	int mapa = 1;
	bool sair = false;

	hidecursor();

	while (!sair)
	{
		//medir_tempo(0, mapa);
		atualizar_mapa(mapa1, mapa2, mapa3, mapa, sair);
		mgotoxy(0, 0);
		//medir_tempo(1, mapa);
	}
	return 0;
}