/*
   Manejador de campo programado en C.

   Permite escoger entre tres opciones:

   1- Campo de texto
   2- Campo de fecha
   3- Campo numérico

   Autor : Naoki Nakao
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.c>

/* Macros */

#define TRUE    1
#define FALSE   0
#define MAX    80

#define RIGHT   77
#define LEFT    75
#define ENTER   13
#define ESC     27
#define BKSP     8
#define SPACE   32

#define INI_X    1
#define INI_Y    1

#define OPT_1    1
#define OPT_2    2
#define OPT_3    3

/* Prototipos de función */

int main()
{
   int option;

   // seleccionando tipo de campo
   do {
      gotoxy(INI_X+10, INI_Y);
      printf("VALIDADOR DE CAMPO");

      gotoxy(INI_X, INI_Y+2);
      printf("Opciones:\n");
      printf("1- Texto\n");
      printf("2- Fecha\n");
      printf("3- Num%crico\n", 130);
      printf("\nDigite el n%cmero de la opci%cn : ", 163, 162);
      scanf("%d", &option);
      clrscr();
   } while (option != OPT_1 && option != OPT_2 && option != OPT_3);

   return 0;
}

















































