// Includes
#include <math.h>
#include <conio.c>
#include <time.h>
#include <ctype.h>
// Key Bindings
#define ESC 27
#define ENTER 13
#define ARRIBA 72
#define ABAJO 80
#define DERECHA 77
#define IZQUIERDA 75
// Max size of...
#define ROWS 10
#define COLUMNS 10
#define LIMIT 5
// Game parameters
#define OCEAN 1
#define USED 2
#define MUSED 3
#define SHP 83
#define MSHP 69
#define HIT 88
#define MISS 79
#define XHIT 120
#define INITIAL -1
// Distance between matrix spaces
#define ESP ((int)log10(ROWS * COLUMNS))
// int modules
int randRange(int, int);
int checkPlay(int[][COLUMNS], int, int);
int checkOcean(int[][COLUMNS], int, int);
int confirmShips(void);
int putShip(int[][COLUMNS], int, int);
// Void modules
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
   printf("Welcome to the game \"Battle Ship\"");
   gotoxy(37, 7);
   printf("Ocean");
   gotoxy(17, 10);
   printf("Would you like to read the instructions%c (y)es or (n)o\n\n", 63);
   do
   {
      fflush(stdin);
      key = getch();
   } while (key != 'y' && key != 'n');
   if (key == 'y')
      instructions();
   else
   {
      gotoxy(17, 10);
      printf("                                                             ");
   }
   do
   {
      ships = eneShips = stage = rep = game = 0, actRow = ROWS / 2, actCol = COLUMNS / 2;
      genOcean(ocean, ROWS, COLUMNS), genOcean(oceanShip, ROWS, COLUMNS), genOcean(tempOcean, ROWS, COLUMNS), genOcean(eneOcean, ROWS, COLUMNS);
      do
      {
         gotoxy(24, 21);
         printf("Your ships: %d | Enemy ships: %d", ships, eneShips);
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
               printf("Enemy's ship #%d deploying...", eneShips + 1);
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
               if (checkOcean(tempOcean, actRow, actCol))
               {
                  ocean[actRow][actCol] = checkPlay(oceanShip, actRow, actCol);
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
                     } while (!checkOcean(eneOcean, eneRow, eneCol));
                     if (checkOcean(eneOcean, eneRow, eneCol))
                     {
                        if (checkPlay(oceanShip, eneRow, eneCol) != MISS)
                           ocean[eneRow][eneCol] = checkPlay(oceanShip, eneRow, eneCol);
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
                  printf("Your ships: %d | Enemy ships: %d           ", ships, eneShips);
                  gotoxy(33, 23);
                  if (!eneShips)
                     printf("%cYou won%c\n", 173, 33);
                  else
                     printf("%cYou lost%c\n", 173, 33);
                  gotoxy(25, 25);
                  printf("Would you like to play again%c (y)es or (n)o           ", 63);
                  do
                  {
                     fflush(stdin);
                     key = getch();
                  } while (key != 'y' && key != 'n');
                  if (key == 'y')
                     game = 1;
                  else
                  {
                     gotoxy(25, 25);
                     printf("                                              ");
                     gotoxy(25, 23);
                     printf("     %cThanks for playing%c                   ", 173, 33);
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
   Argumentos: int ocean, matrix to be generated.
               int rows, matrix's quantity of rows.
               int columns, matrix's quantity of columns.

   Objetivo:   to create the matrix 'ocean' which has  n-'rows' rows and
               n-'columns' columns.
   Retorno:    None.
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
   Argumentos: int oldMat, matrix to be copied.
               int newMat, new matrix copy.
               int rows, matrix's quantity of rows.
               int columns, matrix's quantity of columns.

   Objetivo:   to copy 'oldMat's' values in 'newMat's' ones.
   Retorno:    None.
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
   Argumentos: int ocean, matrix to be printed.
               int rows, matrix's quantity of rows.
               int columns, matrix's quantity of columns.
               int posX, horizontal plane's position.
               int posY, vertical plane's position.
               int actRow, current matrix's row position.
               int actCol, current matrix's columns position.

   Objetivo:   to print the matrix 'ocean' which has  n-'rows' rows and
               n-'columns' columns, in posX and posY positions,
               depending on the value, the color will change.

   Retorno:     None.
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
   Argumentos: int ocean, matrix to be evaluated (putting ships).
               int rows, matrix's quantity of rows.
               int columns, matrix's quantity of columns.

   Objetivo:   to put or remove ships in the matrix 'ocean'.
   Retorno:    int 'SHP' if the position was 'OCEAN'.
               int 'OCEAN' if the position was 'SHP'.
*/
int putShip(int ocean[][COLUMNS], int actRow, int actCol)
{
   if (checkOcean(ocean, actRow, actCol))
      return SHP;
   else
      return OCEAN;
}
/*
   Función:    machineShip
   Argumentos: int eneOcean, enemy's matrix to be evaluated.
   Objetivo:   to put randomly ships in the matrix 'eneOcean'.
   Retorno:    None.
*/
void machineShip(int eneOcean[][COLUMNS])
{
   int actCol, actRow;
   do
   {
      actCol = randRange(0, COLUMNS - 1);
      actRow = randRange(0, COLUMNS - 1);
   } while (eneOcean[actRow][actCol] != OCEAN);
   eneOcean[actRow][actCol] = MSHP;
   return;
}
/*
   Función:    checkPlay
   Argumentos: int ocean, matrix to be evaluated.
               int actRow, current matrix's row position.
               int actCol, current matrix's columns position.

   Objetivo:   to check what happened with the "shot".
   Retorno:    int HIT,  if hit machine's ships.
               int XHIT, if hit player's ships.
               int MISS, if didn't hit anything.
*/
int checkPlay(int ocean[][COLUMNS], int actRow, int actCol)
{
   if (ocean[actRow][actCol] == MSHP)
      return HIT;
   else if (ocean[actRow][actCol] == SHP)
      return XHIT;
   else
      return MISS;
}
/*
   Función:    checkOcean
   Argumentos: int tempOcean, matrix to be evaluated.
               int actRow, current matrix's row position.
               int actCol, current matrix's columns position.

   Objetivo:   to check if the position selected is available to use.
   Retorno:    int true or false
*/
int checkOcean(int tempOcean[][COLUMNS], int actRow, int actCol)
{
   if (tempOcean[actRow][actCol] == OCEAN)
      return 1;
   else
      return 0;
}
/*
    Función:    copyOcean
    Argumentos: int tempMat, player's matrix.
                int eneOcean, machine's matrix.
                int ocean, game's mother matrix.
                int actRow, current player's matrix's row position.
                int actCol, current player's matrix's columns position.
                int eneRow, current machines's matrix's row position.
                int eneCol, current machines's matrix's columns position.

    Objetivo:   to fill with USED or MUSED 'tempOcean' and 'eneOcean' matrix, depending on the last player's game effect.
    Retorno:    None.
 */
void usedMoves(int tempOcean[][COLUMNS], int eneOcean[][COLUMNS], int ocean[][COLUMNS], int actRow, int actCol, int eneRow, int eneCol)
{
   tempOcean[actRow][actCol] = USED;
   eneOcean[eneRow][eneCol] = MUSED;
   // The player.
   if (ocean[actRow][actCol] == HIT)
      tempOcean[actRow][actCol] = USED;
   if (ocean[actRow][actCol] == XHIT)
      tempOcean[actRow][actCol] = USED;
   if (ocean[actRow][actCol] == HIT)
      eneOcean[actRow][actCol] = MUSED;
   if (ocean[actRow][actCol] == XHIT)
      eneOcean[actRow][actCol] = MUSED;
   // The enemy
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
   Argumentos: int ocean, matrix to be evaluated.
               int actRow, current matrix's row position.
               int actCol, current matrix's columns position.

   Objetivo:   to inform to the player what happened with it's move
   Retorno:    None.
*/
void playerTurn(int ocean[][COLUMNS], int actRow, int actCol)
{
   gotoxy(24, 24);
   if (ocean[actRow][actCol] == XHIT)
      printf("%cOh no%c, you destroyed one of your ships :(        ", 173, 33);
   else if (ocean[actRow][actCol] == HIT)
      printf("%cBoom%c %cyou destroyed a enemy's ships%c           ", 173, 33, 173, 33);
   else
      printf("%cSorry, you missed your shot%c                      ", 173, 33);
   return;
}
/*
   Función:     machineTurn
   Argumentos:  int ocean, matrix to be evaluated.
                int actRow, current matrix's row position.
                int actCol, current matrix's columns position.

   Objetivo:    to inform to the player what happened with machine's move
   Retorno:     None.
*/
void machineTurn(int ocean[][COLUMNS], int eneRow, int eneCol)
{
   gotoxy(24, 24);
   if (ocean[eneRow][eneCol] == HIT)
      printf("%cOh no%c, the machine destroyed one of it's ships                  ", 173, 33);
   else if (ocean[eneRow][eneCol] == XHIT)
      printf("%cBoom%c %cthe machine destroyed one of your ships%c                ", 173, 33, 173, 33);
   else
      printf("%cThe machine missed the shot%c                                     ", 173, 33);
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
   printf("Confirm your ship's location (y)es or (n)o   ");
   fflush(stdin);
   key = tolower(getch());
   gotoxy(18, 24);
   printf("                                                       ");
   if (key == 'y')
      return 1;
   else
      return 0;
}
/*
   Función:    instructions
   Argumentos: None.
   Objetivo:   Explain to the player how to play the game.
   Retorno:    None.
*/
void instructions()
{
   gotoxy(0, 19);
   printf("1. You must set %d ships in the ocean, with \'ENTER\' key you can set or remove ships with freedom.\n", LIMIT);
   printf("2. Once %d ships are all set, press \'ESC\' key to confirm the positions of your ships\n", LIMIT);
   printf("3. Once playing, you would not be able to see any ships's location, that includes your own ships, be careful, you could destroy them too\n");
   printf("4. Press \'ENTER\' key to chose a position to be evaluated, then the machine would play.\n");
   printf("5. World win who destroy rival's ships first.\n");
   system("pause");
   system("cls");
   return;
}
/*
   Función:    randRange
   Argumentos: int minLimit, inferior limit.
               int maxLimit, superior limit.

   Objetivo:   to get a random number.
   Retorno:    int, a random number.
*/
int randRange(int minLimit, int maxLimit)
{
   return rand() % (maxLimit - minLimit + 1) + minLimit;
}
/*
   Función:    defaultColor
   Argumentos: None.
   Objetivo:   to set default colors.
   Retorno:    None.
*/
void defaultColor()
{
   textcolor(LIGHTGRAY);
   textbackground(BLACK);
   return;
}
/*
   Función:    setColor
   Argumentos: int TC, text's color.
               int BC, background's color.

   Objetivo:   to set text color 'TC' and background 'BC'.
   Retorno:    None.
*/
void setColor(int TC, int BC)
{
   textcolor(TC);
   textbackground(BC);
   return;
}
/*
   Función:    showTimeXY
   Argumentos: int seg, seconds to be evaluated.
               int posX, horizontal plane's position.
               int posY, vertical plane's position.

   Objetivo:   to get and show the current time lapsed.
   Retorno:    None.
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