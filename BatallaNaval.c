// Librerías
#include <math.h>
#include <conio.c>
#include <time.h>
#include <ctype.h>
// Mapeo de Teclas
#define ESC 27
#define ENTER 13
#define ARRIBA 72
#define ABAJO 80
#define DERECHA 77
#define IZQUIERDA 75
// Tamaño máximo de
#define ROWS 10
#define COLUMNS 10
#define LIMIT 5
// Parámetros del juego.
#define OCEAN 1
#define USED 2
#define MUSED 3
#define SHP 83
#define MSHP 69
#define HIT 88
#define MISS 79
#define XHIT 120
// Carácter Inicial
#define INITIAL -1
// Distancia entre casillas de la matriz
#define ESP ((int)log10(ROWS * COLUMNS))
// Módulos int
int randRange(int, int);
int verifPlay(int[][COLUMNS], int, int);
int verifOcean(int[][COLUMNS], int, int);
int confirmShips(void);
int putShip(int[][COLUMNS], int, int);
// Módulos void
void genOcean(int[][COLUMNS], int, int);
void showOceanXY(int[][COLUMNS], int, int, int, int, int, int);
void showTimeXY(int, int, int);
void playerTurn(int[][COLUMNS], int, int);
void machineTurn(int[][COLUMNS], int, int);
void copyOcean(int[][COLUMNS], int[][COLUMNS], int, int);
void usedMoves(int[][COLUMNS], int[][COLUMNS], int[][COLUMNS], int, int, int, int);
void machineShip(int[][COLUMNS]);
void setColor(int, int);
void defaultColor(void);
void instructions(void);

int main()
{
   int ships, eneShips, stage, actRow, actCol, key, eneRow = INITIAL, eneCol = INITIAL, rep, game, seg;
   int ocean[ROWS][COLUMNS], oceanShip[ROWS][COLUMNS], tempOcean[ROWS][COLUMNS], eneOcean[ROWS][COLUMNS];

   _setcursortype(0);
   srand(time(NULL));
   gotoxy(24, 5);
   printf("Bienvenidos a juego \"Battle Ship\"");
   gotoxy(37, 7);
   printf("Oc%cano", 130);
   gotoxy(20, 10);
   printf("%cDesea leer las instrucciones%c (s)%c %c (n)o\n\n", 168, 63, 161, 162);
   do
   {
      fflush(stdin);
      key = getch();
   } while (key != 's' && key != 'n');
   if (key == 's')
      instructions();
   else
   {
      gotoxy(20, 10);
      printf("                                                   ");
   }
   do
   {
      ships = eneShips = stage = rep = game = 0, actRow = ROWS / 2, actCol = COLUMNS / 2;
      genOcean(ocean, ROWS, COLUMNS), genOcean(oceanShip, ROWS, COLUMNS), genOcean(tempOcean, ROWS, COLUMNS), genOcean(eneOcean, ROWS, COLUMNS);
      do
      {
         gotoxy(24, 21);
         printf("Tus Barco: %d | Barcos Enemigos: %d", ships, eneShips);
         showOceanXY(ocean, ROWS, COLUMNS, 30, 10, actRow, actCol);
         _setcursortype(0);
         do
         {
            fflush(stdin);
            key = getch();
         } while (key != ESC && key != ARRIBA && key != ABAJO && key != DERECHA && key != IZQUIERDA && key != ENTER);
         // Movimiento.
         if (key == ARRIBA)
            if (actRow > 0)
               actRow--;
            else
               actRow = ROWS - 1;
         if (key == ABAJO)
            if (actRow < ROWS - 1)
               actRow++;
            else
               actRow = 0;
         if (key == IZQUIERDA)
            if (actCol > 0)
               actCol--;
            else
               actCol = COLUMNS - 1;
         if (key == DERECHA)
            if (actCol < COLUMNS - 1)
               actCol++;
            else
               actCol = 0;

         if (stage == 0)
         {
            if (key == ENTER)
               if (putShip(ocean, actRow, actCol) == SHP)
               {
                  if (LIMIT > ships)
                  {
                     ocean[actRow][actCol] = SHP,
                     ships++;
                  }
               }
               else
               {
                  ocean[actRow][actCol] = OCEAN;
                  ships--;
               }
            if (key == ESC && ships == LIMIT)
               if (confirmShips())
                  stage++;
         }
         if (stage == 1)
         {
            for (eneShips = 0; LIMIT > eneShips; eneShips++)
            {
               gotoxy(26, 24);
               printf("Barco enemigo #%d desplegado...", eneShips + 1);
               sleep(1);
               machineShip(ocean);
            }
            system("cls");
            copyOcean(ocean, oceanShip, ROWS, COLUMNS);
            genOcean(ocean, ROWS, COLUMNS);
            stage++;
            seg = time(NULL);
         }
         else if (stage == 2)
         {
            if (key == ENTER)
            {
               if (verifOcean(tempOcean, actRow, actCol))
               {
                  ocean[actRow][actCol] = verifPlay(oceanShip, actRow, actCol);
                  playerTurn(ocean, actRow, actCol);
                  gotoxy(24, 24);
                  if (ocean[actRow][actCol] == XHIT)
                     ships--;
                  if (ocean[actRow][actCol] == HIT)
                     eneShips--;
                  usedMoves(tempOcean, eneOcean, ocean, actRow, actCol, eneRow, eneCol);
                  sleep(1);
                  if (eneShips)
                  {
                     do
                     {
                        eneRow = randRange(0, COLUMNS - 1);
                        eneCol = randRange(0, COLUMNS - 1);
                     } while (!verifOcean(eneOcean, eneRow, eneCol));
                     if (verifOcean(eneOcean, eneRow, eneCol))
                     {
                        if (verifPlay(oceanShip, eneRow, eneCol) != MISS)
                           ocean[eneRow][eneCol] = verifPlay(oceanShip, eneRow, eneCol);
                        machineTurn(ocean, eneRow, eneCol);
                        if (ocean[eneRow][eneCol] == XHIT)
                           ships--;
                        if (ocean[eneRow][eneCol] == HIT)
                           eneShips--;
                        usedMoves(tempOcean, eneOcean, ocean, actRow, actCol, eneRow, eneCol);
                     }
                  }
               }
               showTimeXY(time(NULL) - seg, 35, 22);
               if (!ships || !eneShips)
               {
                  system("cls");
                  showOceanXY(ocean, ROWS, COLUMNS, 30, 10, actRow, actCol);
                  gotoxy(24, 21);
                  printf("Tus Barco: %d | Barcos Enemigos: %d         ", ships, eneShips);
                  gotoxy(33, 23);
                  if (!eneShips)
                     printf("%cHas ganado%c\n", 173, 33);
                  else
                     printf("%cHas perdido%c\n", 173, 33);
                  gotoxy(25, 25);
                  printf("%cVolver a jugar%c (s)%c %c (n)o           ", 168, 63, 161, 162);
                  do
                  {
                     fflush(stdin);
                     key = getch();
                  } while (key != 's' && key != 'n');
                  if (key == 's')
                     game = 1;
                  else
                  {
                     gotoxy(25, 25);
                     printf("                                              ");
                     gotoxy(25, 23);
                     printf("     %cGracias por jugar%c                   ", 173, 33);
                     sleep(1);
                     return 0;
                  }
               }
            }
         }
      } while (!game);
      system("cls");
   } while (!rep);
}
/*
   Función:    genOcean
   Argumentos: int ocean, Matriz a generar.
               int rows, Cantidad de filas de la matriz.
               int columns, Cantidad de columnas de la matriz.

   Objetivo:   Generar la matriz 'ocean' que contiene 'rows' filas y
               'columns' columnas.
   Retorno:    Ninguno.
*/
void genOcean(int ocean[][COLUMNS], int rows, int columns)
{
   for (int rowPos = 0; rowPos < rows; rowPos++)
      for (int colPos = 0; colPos < columns; colPos++)
         ocean[rowPos][colPos] = OCEAN;
   return;
}
/*
   Función:    copyOcean
   Argumentos: int oldMat, Matriz a copiar.
               int newMat, Matriz a recibir valores.
               int rows, Cantidad de filas de la matriz.
               int columns, Cantidad de columnas de la matriz.

   Objetivo:   Copiar los valores de la matriz 'oldMat' en la matriz 'newMat'.
   Retorno:    Ninguno.
*/
void copyOcean(int oldMat[][COLUMNS], int newMat[][COLUMNS], int rows, int columns)
{
   for (int rowPos = 0; rowPos < rows; rowPos++)
      for (int colPos = 0; colPos < columns; colPos++)
         newMat[rowPos][colPos] = oldMat[rowPos][colPos];
   return;
}
/*
   Función:    showOceanXY
   Argumentos: int ocean, Matriz a imprimir.
               int rows, Cantidad de filas de la matriz.
               int columns, Cantidad de columnas de la matriz.
               int posX, Posición del plano horizontal.
               int posY, Posición del plano vertical.
               int actRow, Posición actual fila de la matriz.
               int actCol, Posición actual columna de la matriz.

   Objetivo:   Imprimir la matriz 'ocean' que contiene 'rows' filas y
               'columns' columnas, en las posiciones posX y posY (de la consola) y
               determinar que color va dependiendo de sus valores

   Retorno:     Ninguno.
*/
void showOceanXY(int ocean[][COLUMNS], int rows, int columns, int posX, int posY, int actRow, int actCol)
{
   for (int rowPos = 0; rowPos < rows; rowPos++)
      for (int colPos = 0; colPos < columns; colPos++)
      {
         if (rowPos == actRow && colPos == actCol)
            setColor(LIGHTMAGENTA, LIGHTMAGENTA);
         else if (ocean[rowPos][colPos] == SHP)
            setColor(WHITE, BLUE);
         else if (ocean[rowPos][colPos] == MSHP)
            setColor(WHITE, RED);
         else if (ocean[rowPos][colPos] == OCEAN)
            setColor(CYAN, CYAN);
         else if (ocean[rowPos][colPos] == MISS)
            setColor(WHITE, CYAN);
         else if (ocean[rowPos][colPos] == HIT)
            setColor(RED, CYAN);
         else if (ocean[rowPos][colPos] == XHIT)
            setColor(BLUE, CYAN);
         else
            defaultColor();
         gotoxy(posX + colPos * ESP, posY + rowPos);
         printf("%*c", ESP, ocean[rowPos][colPos]);
      }
   defaultColor();
   return;
}
/*
   Función:    putShip
   Argumentos: int ocean, Matriz a colocar naves.
               int actRow, Posición actual fila de la matriz.
               int actCol, Posición actual columna de la matriz.

   Objetivo:   Colocar o remover naves en la matriz 'ocean'.
   Retorno:    int 'SHP' si era un espacio vació.
               int 'OCEAN' si era una nave.
*/
int putShip(int ocean[][COLUMNS], int actRow, int actCol)
{
   if (verifOcean(ocean, actRow, actCol))
      return SHP;
   else
      return OCEAN;
}
/*
   Función:    machineShip
   Argumentos: int ocean, Matriz a colocar naves.
   Objetivo:   Colocar naves en la matriz 'ocean' de manera aleatoria.
   Retorno:    Ninguno.
*/
void machineShip(int ocean[][COLUMNS])
{
   int actCol, actRow;
   do
   {
      actCol = randRange(0, COLUMNS - 1);
      actRow = randRange(0, COLUMNS - 1);
   } while (ocean[actRow][actCol] != OCEAN);
   ocean[actRow][actCol] = MSHP;
   return;
}
/*
   Función:    verifPlay
   Argumentos: int ocean, matriz a evaluar.
               int actRow, Posición actual fila de la matriz.
               int actCol, Posición actual columna de la matriz.

   Objetivo:   Verificar que hizo el "disparo".
   Retorno:    int HIT, si golpeo una nave de la maquina.
               int XHIT, si golpeo una nave aleada.
               int MISS, si no golpeo a nada.
*/
int verifPlay(int ocean[][COLUMNS], int actRow, int actCol)
{
   if (ocean[actRow][actCol] == MSHP)
      return HIT;
   else if (ocean[actRow][actCol] == SHP)
      return XHIT;
   else
      return MISS;
}
/*
   Función:    verifOcean
   Argumentos: int ocean, matriz a evaluar.
               int actRow, Posición actual fila de la matriz.
               int actCol, Posición actual columna de la matriz.

   Objetivo:   Verificar si las posiciones son validas para realizar la jugada.
   Retorno:    true (int) si se puede jugar.
               false (int) si no se puede jugar.
*/
int verifOcean(int tempOcean[][COLUMNS], int actRow, int actCol)
{
   if (tempOcean[actRow][actCol] == OCEAN)
      return 1;
   else
      return 0;
}
/*
    Función:    copyOcean
    Argumentos: int tampMat, Matriz aliada.
                int eneOcean, Matriz de la maquina.
                int ocean, matriz madre del juego.
                int actRow, Posición actual fila de la matriz.
                int actCol, Posición actual columna de la matriz.
                int eneRow, Posición actual fila de la matriz de la maquina.
                int eneCol, Posición actual columna de la matriz de la maquina.

    Objetivo:   Llenar con USED o MUSED las matrices 'tempOcean' y 'eneOcean', dependiendo del tipo de jugada.
    Retorno:    Ninguno.
 */
void usedMoves(int tempOcean[][COLUMNS], int eneOcean[][COLUMNS], int ocean[][COLUMNS], int actRow, int actCol, int eneRow, int eneCol)
{
   tempOcean[actRow][actCol] = USED;
   eneOcean[eneRow][eneCol] = MUSED;
   // El usuario.
   if (ocean[actRow][actCol] == HIT)
      tempOcean[actRow][actCol] = USED;
   if (ocean[actRow][actCol] == XHIT)
      tempOcean[actRow][actCol] = USED;
   if (ocean[actRow][actCol] == HIT)
      eneOcean[actRow][actCol] = MUSED;
   if (ocean[actRow][actCol] == XHIT)
      eneOcean[actRow][actCol] = MUSED;
   // El enemigo
   if (ocean[eneRow][eneCol] == HIT)
      eneOcean[eneRow][eneCol] = MUSED;
   if (ocean[eneRow][eneCol] == XHIT)
      eneOcean[eneRow][eneCol] = MUSED;
   if (ocean[eneRow][eneCol] == HIT)
      tempOcean[eneRow][eneCol] = USED;
   if (ocean[eneRow][eneCol] == XHIT)
      tempOcean[eneRow][eneCol] = USED;
   return;
}
/*
   Función:    playerTurn
   Argumentos: int ocean, matriz a evaluar.
               int actRow, Posición actual fila de la matriz.
               int actCol, Posición actual columna de la matriz.

   Objetivo:   Informarle al jugar que sucedió con su jugada.
   Retorno:    Ninguno.
*/
void playerTurn(int ocean[][COLUMNS], int actRow, int actCol)
{
   gotoxy(24, 24);
   if (ocean[actRow][actCol] == XHIT)
      printf("%cOh no%c, derribaste tu propio barco :(        ", 173, 33);
   else if (ocean[actRow][actCol] == HIT)
      printf("%cBoom%c %cHas hundido el barco enemigo%c       ", 173, 33, 173, 33);
   else
      printf("%cLo siento, has fallado el disparo%c           ", 173, 33);
   return;
}
/*
   Función:    machineTurn
   Argumentos: int ocean, matriz a evaluar.
               int eneRow, Posición actual fila de la matriz.
               int eneCol, Posición actual columna de la matriz.

   Objetivo:   Informarle al jugar que sucedió con la jugada de la maquina.
   Retorno:    Ninguno.
*/
void machineTurn(int ocean[][COLUMNS], int eneRow, int eneCol)
{
   gotoxy(24, 24);
   if (ocean[eneRow][eneCol] == HIT)
      printf("%cOh no%c, la maquina derribo su propio barco   ", 173, 33);
   else if (ocean[eneRow][eneCol] == XHIT)
      printf("%cBoom%c %cHan hundido tu barco%c                ", 173, 33, 173, 33);
   else
      printf("%cLa maquina ha fallado el disparo%c            ", 173, 33);
   return;
}
/*
   Función:    confirmShips
   Argumentos: void
   Objetivo:   Preguntarle la jugador si desea confirmar la ubicación de las naves colocadas.
   Retorno:    true (int) quiere confirmar;
               false (int) no quiere confirmar.
*/
int confirmShips()
{
   int key;
   gotoxy(18, 24);
   printf("Confirmar la ubicaci%cn de sus naves. (s)%c %c (n)o   ", 162, 161, 162);
   fflush(stdin);
   key = tolower(getch());
   gotoxy(18, 24);
   printf("                                                       ");
   if (key == 's')
      return 1;
   else
      return 0;
}
/*
   Función:    instructions
   Argumentos: Ninguno.
   Objetivo:   Explicarle al jugador como jugar.
   Retorno:    Ninguno.
*/
void instructions()
{
   gotoxy(0, 19);
   printf("1. Debe colocar %d naves en el oc%cano, con la tecla \'ENTER\' podr%c colocar y quitar naves a su gusto.\n", LIMIT, 130, 160);
   printf("2. Una vez colocadas las %d naves, presione la tecla \'ESC\' para confirmar la ubicaci%cn de sus naves.\n", LIMIT, 162);
   printf("3. Dentro del juego, no podr%c ver la ubicaci%cn de las naves, pero cuidado, porque podr%c de derribar sus propias naves.\n", 160, 162, 160);
   printf("4. Presione la tecla \'ENTER\' para revelar el espacio del oc%cano, luego ser%c turno de la maquina.\n", 130, 160);
   printf("5. Ganara quien logre derribar las %d naves del contrincante.\n", LIMIT);
   system("pause");
   system("cls");
   return;
}
/*
   Función:    randRange
   Argumentos: int minLimit, primer valor (rango inferior).
               int maxLimit, segundo valor (rango superior).

   Objetivo:   Crear un número aleatorio con la formula.
   Retorno:    int, un número aleatorio
*/
int randRange(int minLimit, int maxLimit)
{
   return rand() % (maxLimit - minLimit + 1) + minLimit;
}
/*
   Función:    defaultColor
   Argumentos: Ninguno.
   Objetivo:   Establecer los colores de fabrica.
   Retorno:    Ninguno.
*/
void defaultColor()
{
   textcolor(LIGHTGRAY);
   textbackground(BLACK);
   return;
}
/*
   Función:    setColor
   Argumentos: int TC, color del texto.
               int BC, colo del fondo.

   Objetivo:   Establecer los colores elegidos en 'TC' y 'BC'.
   Retorno:    Ninguno.
*/
void setColor(int TC, int BC)
{
   textcolor(TC);
   textbackground(BC);
   return;
}
/*
   Función:    showTimeXY
   Argumentos: int seg, segundos a evaluar.
               int posX, la posición horizontal del plano.
               int posY, la posición vertical del plano.

   Objetivo:   Obtener y mostrar el tiempo al jugador.
   Retorno:    Ninguno.
*/
void showTimeXY(int seg, int posX, int posY)
{
   int min, hour;

   min = (seg / 60);
   seg -= (min * 60);
   hour = (min / 60);
   seg -= (hour * 3600);
   gotoxy(posX, posY);
   setColor(WHITE, BLACK);
   printf("%02d:%02d:%02d", hour, min, seg);
   defaultColor();
   return;
}