#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>  
#include <termios.h>  

#define SALIR 0
#define JUGAR 1
#define GOL 2

typedef struct
{
    int x;
    int y;
}pos;

typedef struct
{
    int j;
    int n;
}goles;

void bienvenida(void);
void generarCampo(char**campo);
void imprimirCampo(char** campo, goles* marcador);
void swap (char* a, char* b);
int realizarAccion(char** campo, pos *j, pos *n);
int verGol(char** campo, goles* marcador, pos *j, pos *n);
void correr(char**campo, pos*j, char movj);
int patear(char** campo, pos*j);
int pelotaAdyacente(char** campo, pos*j);
void verReglas(void);
char getch(void);
void limpiarPantalla(void) ;

int main(void)
{
    int i, estado=JUGAR;
    pos *j=malloc(sizeof(pos)), *n=malloc(sizeof(pos));
    char** campo=malloc(50*sizeof(char*));
    goles *marcador=malloc(sizeof(goles));
    bienvenida();
    for( i=0 ; i<50 ; i++ )
        campo[i]=malloc(30*sizeof(char));
    marcador->j=0;
    marcador->n=0;
    generarCampo(campo);
    campo[17][15]='J';
    campo[31][15]='N';
    campo[24][15]='O';
    j->x=17;
    j->y=15;
    n->x=31;
    n->y=15;
    while(estado!=SALIR)
    {
        imprimirCampo(campo, marcador);
        estado=realizarAccion(campo, j, n);
        if( estado==GOL )
            estado=verGol(campo, marcador, j, n);
    }
}


void bienvenida(void)
{
    printf("  _______  __   __  _______  _______  _______  ___      \n");
    printf(" |       ||  | |  ||       ||  _    ||       ||   |     \n");
    printf(" |    ___||  | |  ||_     _|| |_|   ||   _   ||   |     \n");
    printf(" |   |___ |  |_|  |  |   |  |       ||  | |  ||   |     \n");
    printf(" |    ___||       |  |   |  |  _   | |  |_|  ||   |___  \n");
    printf(" |   |    |       |  |   |  | |_|   ||       ||       | \n");
    printf(" |___|    |_______|  |___|  |_______||_______||_______| \n");
    printf("  _______  _______  _______  ___  ___  \n");
    printf(" |   _   ||       ||       ||   ||   | \n");
    printf(" |  |_|  ||  _____||       ||   ||   | \n");
    printf(" |       || |_____ |       ||   ||   | \n");
    printf(" |       ||_____  ||      _||   ||   | \n");
    printf(" |   _   | _____| ||     |_ |   ||   | \n");
    printf(" |__| |__||_______||_______||___||___| \n");

    printf("\nby Gonzalo Nardini.\n\n");
    printf("\nPresiona una tecla para continuar...");
    getch();
}

int realizarAccion(char** campo, pos* j, pos* n)
{
    int i, k;
    static char movj, movn;
    switch(tolower(getch()))
    {
        case 't':
            if( campo[j->x-1][j->y]==' ' || ( campo[j->x-1][j->y]=='O' &&  campo[j->x-2][j->y]==' ') )
            {
                if( campo[j->x-1][j->y]==' ' )
                {
                    swap( &campo[j->x][j->y], &campo[j->x-1][j->y] );
                    j->x--;
                }
                else if( campo[j->x-1][j->y]=='O' )
                {
                    swap( &campo[j->x-1][j->y], &campo[j->x-2][j->y] );
                    swap( &campo[j->x][j->y], &campo[j->x-1][j->y] );
                    j->x--;
                }
            }
            movj='w';
            break;
        case 'g':
            if( campo[j->x+1][j->y]==' ' || ( campo[j->x+1][j->y]=='O' && campo[j->x+2][j->y]==' ' ) )
            {
                if( campo[j->x+1][j->y]==' ' )
                {
                    swap( &campo[j->x][j->y], &campo[j->x+1][j->y] );
                    j->x++;
                }
                else if( campo[j->x+1][j->y]=='O' )
                {
                    swap( &campo[j->x+1][j->y], &campo[j->x+2][j->y] );
                    swap( &campo[j->x][j->y], &campo[j->x+1][j->y] );
                    j->x++;
                }
                movj='s';
                return JUGAR;
            }
            if( campo[j->x+1][j->y]=='O' && j->x==47 && j->y>11 && j->y<19)
            {
                swap( &campo[j->x+1][j->y], &campo[j->x+2][j->y] );
                swap( &campo[j->x][j->y], &campo[j->x+1][j->y] );
                campo[j->x][j->y]=' ';
                movj='s';
                return GOL;
            }
            movj='s';
            break;
        case 'f':
            if( campo[j->x][j->y-1]==' ' || ( campo[j->x][j->y-1]=='O' && campo[j->x][j->y-2]==' ' ) )
            {
                if( campo[j->x][j->y-1]==' ' )
                {
                    swap( &campo[j->x][j->y], &campo[j->x][j->y-1] );
                    j->y--;
                }
                else if( campo[j->x][j->y-1]=='O' )
                {
                    swap( &campo[j->x][j->y-1], &campo[j->x][j->y-2] );
                    swap( &campo[j->x][j->y], &campo[j->x][j->y-1] );
                    j->y--;
                }
            }
            movj='a';
            break;
        case 'h':
            if( campo[j->x][j->y+1]==' ' || ( campo[j->x][j->y+1]=='O' && campo[j->x][j->y+2]==' ' ) )
            {
                if( campo[j->x][j->y+1]==' ' )
                {
                    swap( &campo[j->x][j->y], &campo[j->x][j->y+1] );
                    j->y++;
                }
                else if( campo[j->x][j->y+1]=='O' )
                {
                    swap( &campo[j->x][j->y+1], &campo[j->x][j->y+2] );
                    swap( &campo[j->x][j->y], &campo[j->x][j->y+1] );
                    j->y++;
                }
            }
            movj='d';
            break;
        case 'a':
            if( campo[j->x][j->y+1]=='O' )
            {
                swap( &campo[j->x][j->y], &campo[j->x][j->y+1] );
                j->y++;
            }
            else if( campo[j->x][j->y-1]=='O' )
            {
                swap( &campo[j->x][j->y], &campo[j->x][j->y-1] );
                j->y--;
            }
            else if( campo[j->x+1][j->y]=='O' )
            {
                swap( &campo[j->x][j->y], &campo[j->x+1][j->y] );
                j->x++;
            }
            else if( campo[j->x-1][j->y]=='O' )
            {
                swap( &campo[j->x][j->y], &campo[j->x-1][j->y] );
                j->x--;
            }
            break;
        case 's':
            correr(campo, j, movj);
            break;

            case '5':
            if( campo[n->x-1][n->y]==' ' || ( campo[n->x-1][n->y]=='O' &&  campo[n->x-2][n->y]==' ') )
            {
                if( campo[n->x-1][n->y]==' ' )
                {
                    swap( &campo[n->x][n->y], &campo[n->x-1][n->y] );
                    n->x--;
                }
                else if( campo[n->x-1][n->y]=='O' )
                {
                    swap( &campo[n->x-1][n->y], &campo[n->x-2][n->y] );
                    swap( &campo[n->x][n->y], &campo[n->x-1][n->y] );
                    n->x--;
                }
                movn='w';
                return JUGAR;
            }
            if( campo[n->x-1][n->y]=='O' && n->x==2 && n->y>11 && n->y<19)
            {
                swap( &campo[n->x-1][n->y], &campo[n->x-2][n->y] );
                swap( &campo[n->x][n->y], &campo[n->x-1][n->y] );
                campo[n->x][n->y]=' ';
                movn='w';
                return GOL;
            }
            movn='w';
            break;
        case '2':
            if( campo[n->x+1][n->y]==' ' || ( campo[n->x+1][n->y]=='O' && campo[n->x+2][n->y]==' ' ) )
            {
                if( campo[n->x+1][n->y]==' ' )
                {
                    swap( &campo[n->x][n->y], &campo[n->x+1][n->y] );
                    n->x++;
                }
                else if( campo[n->x+1][n->y]=='O' )
                {
                    swap( &campo[n->x+1][n->y], &campo[n->x+2][n->y] );
                    swap( &campo[n->x][n->y], &campo[n->x+1][n->y] );
                    n->x++;
                }
            }
            movn='s';
            break;
        case '1':
            if( campo[n->x][n->y-1]==' ' || ( campo[n->x][n->y-1]=='O' && campo[n->x][n->y-2]==' ' ) )
            {
                if( campo[n->x][n->y-1]==' ' )
                {
                    swap( &campo[n->x][n->y], &campo[n->x][n->y-1] );
                    n->y--;
                }
                else if( campo[n->x][n->y-1]=='O' )
                {
                    swap( &campo[n->x][n->y-1], &campo[n->x][n->y-2] );
                    swap( &campo[n->x][n->y], &campo[n->x][n->y-1] );
                    n->y--;
                }
            }
            movn='a';
            break;
        case '3':
            if( campo[n->x][n->y+1]==' ' || ( campo[n->x][n->y+1]=='O' && campo[n->x][n->y+2]==' ' ) )
            {
                if( campo[n->x][n->y+1]==' ' )
                {
                    swap( &campo[n->x][n->y], &campo[n->x][n->y+1] );
                    n->y++;
                }
                else if( campo[n->x][n->y+1]=='O' )
                {
                    swap( &campo[n->x][n->y+1], &campo[n->x][n->y+2] );
                    swap( &campo[n->x][n->y], &campo[n->x][n->y+1] );
                    n->y++;
                }
            }
            movn='d';
            break;

        case 0xE0:
        switch(getch())
        {
        case 75:
            if( campo[n->x][n->y+1]=='O' )
            {
                swap( &campo[n->x][n->y], &campo[n->x][n->y+1] );
                n->y++;
            }
            else if( campo[n->x][n->y-1]=='O' )
            {
                swap( &campo[n->x][n->y], &campo[n->x][n->y-1] );
                n->y--;
            }
            else if( campo[n->x+1][n->y]=='O' )
            {
                swap( &campo[n->x][n->y], &campo[n->x+1][n->y] );
                n->x++;
            }
            else if( campo[n->x-1][n->y]=='O' )
            {
                swap( &campo[n->x][n->y], &campo[n->x-1][n->y] );
                n->x--;
            }
            break;
        case 80:
            correr(campo, n, movn);
            break;
        case 77:
            return patear(campo, n);
            break;
        }
        case 'd':
            return patear(campo, j);
            break;
        case 'y':
            verReglas();
            break;
        case 'm':
            return SALIR;
        case 'p':
            for( i=0 ; i<50 ; i++ )
            {
                for( k=0 ; k<30 ; k++ )
                {
                    if( campo[i][k]=='N' )
                    {
                        n->x=i;
                        n->y=k;
                    }
                    else if( campo[i][k]=='J' )
                    {
                        j->x=i;
                        j->y=k;
                    }
                }
            }
            break;
    }
    return JUGAR;
}

int verGol(char** campo, goles* marcador, pos *j, pos *n)
{
    int i;

    for( i=12 ; i<19 ; i++ )
    {
        if( campo[0][i]=='O' )
        {
            printf("\a");
            marcador->n++;
            imprimirCampo(campo, marcador);
            generarCampo(campo);
            campo[17][15]='J';
            campo[31][15]='N';
            campo[18][15]='O';
            j->x=17;
            j->y=15;
            n->x=31;
            n->y=15;
            printf("\n\nGOL DEL JUGADOR N\nPresione Enter para continuar: ");
            while(getchar()!='\n');
            return JUGAR;
        }
        else if( campo[49][i]=='O' )
        {
            printf("\a");
            marcador->j++;
            imprimirCampo(campo, marcador);
            generarCampo(campo);
            campo[17][15]='J';
            campo[31][15]='N';
            campo[30][15]='O';
            j->x=17;
            j->y=15;
            n->x=31;
            n->y=15;
            printf("\n\nGOL DEL JUGADOR J\nPresione Enter para continuar: ");
            while(getchar()!='\n');
            return JUGAR;
        }
    }
}

void generarCampo(char**campo)
{
    int i, j;

    for( i=0 ; i<30 ; i++ )
        campo[0][i]='-';
    for( i=0 ; i<30 ; i++ )
        campo[49][i]='-';
    for( i=1 ; i<49 ; i++ )
        campo[i][0]='|';
    for( i=1 ; i<49 ; i++ )
        campo[i][29]='|';
    for( i=12 ; i<=18 ; i++ )
        campo[0][i]='*';
    for( i=12 ; i<=18 ; i++ )
        campo[49][i]='*';
    campo[0][11]='/';
    campo[0][19]='\\';
    campo[49][11]='\\';
    campo[49][19]='/';

    for( i=1 ; i<49 ; i++ )
        for( j=1 ; j<29 ; j++ )
            campo[i][j]=' ';
}

void imprimirCampo(char** campo, goles* marcador)
{
    int i, j;
    limpiarPantalla();
    for( i=0 ; i<47 ; i++ )
    {
        printf("\n");
        for( j=0 ; j<30 ; j++ )
        {
            printf("%c",campo[i][j]);
            //color(campo[i][j]);
        }
    }
    printf("\n");
    for( j=0 ; j<30 ; j++ )
            printf("%c",campo[i][j]);
    printf("by\n");
    i++;
    for( j=0 ; j<30 ; j++ )
            printf("%c",campo[i][j]);
    printf("Gonzalo\n");
    i++;
    for( j=0 ; j<30 ; j++ )
            printf("%c",campo[i][j]);
    printf("Nardini\n");

    printf("\n\nMARCADOR: ");
    printf("\nJugador J: %d",marcador->j);
    printf("\nJugador N: %d",marcador->n);
    printf("\n\nPresione la Y para ver las reglas y los comandos posibles.");
}

void limpiarPantalla() 
{
	#ifdef WINDOWS
		system("cls");
	#else
		system ("clear");
	#endif
}

void correr(char**campo, pos*j, char mov)
{
    if(mov=='w' && j->x>1)
    {
        if( campo[j->x-2][j->y]==' ' && campo[j->x-1][j->y]!='N' && campo[j->x-1][j->y]!='J' )
        {
            swap( &campo[j->x][j->y], &campo[j->x-2][j->y] );
            j->x-=2;
        }
        else if( campo[j->x-1][j->y]==' ' )
        {
            swap( &campo[j->x][j->y], &campo[j->x-1][j->y] );
            j->x-=1;
        }
    }
    else if(mov=='s' && j->x<48)
    {
        if( campo[j->x+2][j->y]==' ' && campo[j->x+1][j->y]!='N' && campo[j->x+1][j->y]!='J' )
        {
            swap( &campo[j->x][j->y], &campo[j->x+2][j->y] );
            j->x+=2;
        }
        else if( campo[j->x+1][j->y]==' ' )
        {
            swap( &campo[j->x][j->y], &campo[j->x+1][j->y] );
            j->x+=1;
        }
    }
    else if(mov=='a' && j->y>1 && campo[j->x][j->y-1]!='|')
    {
        if( campo[j->x][j->y-2]==' ' && campo[j->x][j->y-1]!='N' && campo[j->x][j->y-1]!='J' )
        {
            swap( &campo[j->x][j->y], &campo[j->x][j->y-2] );
            j->y-=2;
        }
        else if( campo[j->x][j->y-1]==' ' )
        {
            swap( &campo[j->x][j->y], &campo[j->x][j->y-1] );
            j->y-=1;
        }
    }
    else if(mov=='d' && j->y<48 && campo[j->x][j->y+1]!='|')
    {
        if( campo[j->x][j->y+2]==' ' && campo[j->x][j->y+2]!='N' && campo[j->x][j->y+1]!='J' )
        {
            swap( &campo[j->x][j->y], &campo[j->x][j->y+2] );
            j->y+=2;
        }
        else if( campo[j->x][j->y-1]==' ' )
        {
            swap( &campo[j->x][j->y], &campo[j->x][j->y+1] );
            j->y+=1;
        }
    }
}

int patear(char** campo, pos*j)
{
    int i, k;
    if( campo[j->x+1][j->y]=='O' )
    {
        for( i=2 ; i<7 ; i++ )
        {
            if( campo[j->x+i][j->y]==' ' )
                swap( &campo[j->x+i][j->y], &campo[j->x+i-1][j->y] );
            else if( campo[j->x+i][j->y]=='*' )
            {
                swap( &campo[j->x+i][j->y], &campo[j->x+i-1][j->y] );
                campo[j->x+i-1][j->y]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x-1][j->y]=='O' )
    {
        for( i=2 ; i<7 ; i++ )
        {
            if( campo[j->x-i][j->y]==' ' )
                swap( &campo[j->x-i][j->y], &campo[j->x-i+1][j->y] );
            else if( campo[j->x-i][j->y]=='*' )
            {
                swap( &campo[j->x-i][j->y], &campo[j->x-i+1][j->y] );
                campo[j->x-i+1][j->y]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x][j->y-1]=='O' )
    {
        for( i=2 ; i<7 ; i++ )
        {
            if( campo[j->x][j->y-i]==' ' )
                swap( &campo[j->x][j->y-i], &campo[j->x][j->y-i+1] );
            else if( campo[j->x][j->y-i]=='*' )
            {
                swap( &campo[j->x][j->y-i], &campo[j->x][j->y-i+1] );
                campo[j->x][j->y-i+1]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x][j->y+1]=='O' )
    {
        for( i=2 ; i<7 ; i++ )
        {
            if( campo[j->x][j->y+i]==' ' )
                swap( &campo[j->x][j->y+i], &campo[j->x][j->y+i-1] );
            else if( campo[j->x][j->y+i]=='*' )
            {
                swap( &campo[j->x][j->y+i], &campo[j->x][j->y+i-1] );
                campo[j->x][j->y+i-1]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x+1][j->y+1]=='O' )
    {
        for( i=2, k=2 ; i<7 ; i++, k++ )
        {
            if( campo[j->x+k][j->y+i]==' ' )
                swap( &campo[j->x+k][j->y+i], &campo[j->x+k-1][j->y+i-1] );
            else if( campo[j->x+k][j->y+i]=='*' )
            {
                swap( &campo[j->x+k][j->y+i], &campo[j->x+k-1][j->y+i-1] );
                campo[j->x+k-1][j->y+i-1]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x+1][j->y-1]=='O' )
    {
        for( i=2, k=2 ; i<7 ; i++, k++ )
        {
            if( campo[j->x+k][j->y-i]==' ' )
                swap( &campo[j->x+k][j->y-i], &campo[j->x+k-1][j->y-i+1] );
            else if( campo[j->x+k][j->y-i]=='*' )
            {
                swap( &campo[j->x+k][j->y-i], &campo[j->x+k-1][j->y-i+1] );
                campo[j->x+k-1][j->y-i+1]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x-1][j->y-1]=='O' )
    {
        for( i=2, k=2 ; i<7 ; i++, k++ )
        {
            if( campo[j->x-k][j->y-i]==' ' )
                swap( &campo[j->x-k][j->y-i], &campo[j->x-k+1][j->y-i+1] );
            else if( campo[j->x-k][j->y-i]=='*' )
            {
                swap( &campo[j->x-k][j->y-i], &campo[j->x-k+1][j->y-i+1] );
                campo[j->x-k+1][j->y-i+1]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    else if( campo[j->x-1][j->y+1]=='O' )
    {
        for( i=2, k=2 ; i<7 ; i++, k++ )
        {
            if( campo[j->x-k][j->y+i]==' ' )
                swap( &campo[j->x-k][j->y+i], &campo[j->x-k+1][j->y+i-1] );
            else if( campo[j->x-k][j->y+i]=='*' )
            {
                swap( &campo[j->x-k][j->y+i], &campo[j->x-k+1][j->y+i-1] );
                campo[j->x-k+1][j->y+i-1]=' ';
                return GOL;
            }
            else
                return JUGAR;
        }
    }
    return JUGAR;
}

int pelotaAdyacente(char** campo, pos*j)
{
    if( campo[j->x+1][j->y]=='O' || campo[j->x][j->y+1]=='O' || campo[j->x-1][j->y]=='O' || campo[j->x][j->y-1]=='O' )
        return 1;
    return 0;
}

void swap (char* a, char* b)
{
	char aux;
	aux=*a;
	*a=*b;
	*b=aux;
}

void verReglas(void)
{
    limpiarPantalla();
    printf("\n\nEl objetivo del juego es muy simple: Meter gol.");
    printf("\nAl intentar caminar por ensima de la pelota, el jugador la empujará.");
    printf("\nAl apretar el botón para cambiar de lugar con la pelota, si el jugador y la");
    printf("\npelota están en posiciones adyacentes, cambiarán de lugar.");
    printf("\nAl apretar el boton para correr, el jugador se moverá dos posiciones en la");
    printf("\núltima dirección en la que se haya movido.");
    printf("\nAl correr, no se puede llevar la pelota con uno.");
    printf("\nAl patear la pelota, si no hay ningún jugador adelante, esta se moverá 5");
    printf("\nespacios en la dirección que haya sido pateada.");
    printf("\n\nComandos para el jugador J:");
    printf("\nMovimiento: TFGH");
    printf("\nCambiar posición con la pelota: A");
    printf("\nCorrer: S");
    printf("\nPatear: D");
    printf("\n\nComandos para el jugador N:");
    printf("\nMovimiento: 5123");
    printf("\nCambiar posición con la pelota: Flechita izquierda");
    printf("\nCorrer: Flechita Abajo");
    printf("\nPatear: Flechita Derecha");
    printf("\n\nPresione la M para salir.");
    printf("\n\nPresione la P para destrabar el juego.");
    printf("\n\nPresione Enter para volver al partido: ");
    while(getchar()!='\n');
}
/*
void color(char c)
{
  HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
  WORD wOldColorAttrs;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

  GetConsoleScreenBufferInfo(h, &csbiInfo);
  wOldColorAttrs = csbiInfo.wAttributes;

    if( c=='J' )
    {
        SetConsoleTextAttribute ( h, FOREGROUND_RED | FOREGROUND_INTENSITY );
        printf ( "%c",c );
    }
    else if( c=='N' )
    {
        SetConsoleTextAttribute ( h, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
        printf ( "%c",c );
    }
    else if( c=='O' )
    {
        SetConsoleTextAttribute ( h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
        printf ( "%c",c );
    }
    else if( c=='-' || c=='_' || c=='|'  )
    {
        SetConsoleTextAttribute ( h, FOREGROUND_RED | FOREGROUND_GREEN );
        printf ( "%c",c );
    }
    else
        printf("%c",c);

  SetConsoleTextAttribute ( h, wOldColorAttrs);
}



*/

// Google Search
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }







