#include <iostream>
#include <fstream> //Para leitura e escrita em arquivo
#include <windows.h>
#include<conio.h>

#define L 16
#define C 34

using namespace std;

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void mgotoxy(int x,int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{x,y});
}

//Nessa função a matriz é percorrida e os números são substituidos
void imprime(int m[L][C]){
    for (int i=0;i<L;i++) {
        for (int j=0;j<C;j++) {
            if(m[i][j]==1){
                cout<<(char)178;
            }
            else if(m[i][j]==0){
                cout<<" ";
            }
            else if (m[i][j]==9){
                cout<<"f";
            } else {
                cout<<"A";
            }

        }
        cout<<"\n";
    }
}




void move_formiga(int m[L][C]){
    static int x=1,y=1;
    char p;

     if ( _kbhit() ){
        p = getch();

        m[x][y]=0;

        switch(p)
        {
            case 'w': x--; break;      //cima
            case 's': x++; break;     //baixo
            case 'a': y--; break;  //esquerda
            case 'd': y++; break;   //direita
        }
        m[x][y]=9;
        //Sleep(100);
        mgotoxy(0,0);
        imprime(m);
     }


}

int main()
{
    int m[L][C]={
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1},
        {1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1},
        {1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1},
        {1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1},
        {1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1},
        {1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1},
        {1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1},
        {1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,0,1,1,1,1,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,0,1},
        {1,0,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,3,1,0,1,1,1,1,1,1,1,1,1,1,1,4,1,1,1,1,1,1,1,1},
    };
    hidecursor();

    imprime(m);

    while(true){
        move_formiga(m);
    }
    return 0;
}
