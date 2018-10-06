#include <iostream>
#include <fstream> //Para leitura e escrita em arquivo
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
				cout << (char)423;
			else if (m[i][j] == 3)
				cout << (char)975;
			else if (m[i][j] == 4)
				cout << "A";
			else if (m[i][j] == 5)
				cout << "A";
		}
		cout << "\n";
	}
}

void move_formiga(int m[L][C])
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
		}
	}
}

void atualizar_mapa(int mapa1[L][C], int mapa2[L][C], int mapa3[L][C], int mapa)
{
	if (mapa == 1)
	{
		imprime(mapa1);
		move_formiga(mapa1);
	}
	else if (mapa == 2)
	{
		imprime(mapa2);
		move_formiga(mapa2);
	}
	else if (mapa == 3)
	{
		imprime(mapa3);
		move_formiga(mapa3);
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
			tempo_total = 0;
		}
	}
}

void ler_matriz(int mapa1[L][C], int mapa2[L][C], int mapa3[L][C])
{
	ifstream arquivoMapa[3];
	int i, j;

	arquivoMapa[0].open("mapa1.txt");
	//arquivoMapa[1].open("mapa2.txt");
	//arquivoMapa[2].open("mapa3.txt");

	for (i = 0; i < L; i++)
	{
		for (j = 0; j < C; j++)
		{
			arquivoMapa[0] >> mapa1[L][C];
			//arquivoMapa[1] >> mapa2[L][C];
			//arquivoMapa[2] >> mapa3[L][C];
		}
	}
}

int main()
{
	int mapa1[L][C], mapa2[L][C], mapa3[L][C];
	int mapa = 1;

	ler_matriz(mapa1, mapa2, mapa3);

	hidecursor();

	while (true)
	{
		medir_tempo(0, mapa);
		atualizar_mapa(mapa1, mapa2, mapa3, mapa);
		mgotoxy(0, 0);
		medir_tempo(1, mapa);
	}
	return 0;
}