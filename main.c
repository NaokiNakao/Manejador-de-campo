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

void selectOption(int*);
void textFieldRequirements(int*, int*, char*);

int main()
{
   int type_field;

   selectOption(&type_field);

   /* Pidiendo requerimientos según la opción seleccionada */

   if (type_field == OPT_1) // campo tipo texto
   {
      int length, bool_space;
      char pattern;

      textFieldRequirements(&length, &bool_space, &pattern);
   }
   else if (type_field == OPT_2)
   {

   }
   else if (type_field == OPT_3)
   {

   }

   return 0;
}

/*
   Función     : selectOption
   Arrgumentos : int* option : referencia para almacenar la opción seleccionada
   Objetivo    : capturar y validar la opción
   Retorno     : ---
*/
void selectOption(int* option)
{
   do {
      gotoxy(INI_X+10, INI_Y);
      printf("VALIDADOR DE CAMPO");

      gotoxy(INI_X, INI_Y+2);
      printf("Opciones:\n");
      printf("1- Texto\n");
      printf("2- Fecha\n");
      printf("3- Num%crico\n", 130);
      printf("\nDigite el n%cmero de la opci%cn : ", 163, 162);
      scanf("%d", option);
      clrscr();
   } while (*option != OPT_1 && *option != OPT_2 && *option != OPT_3);

   return;
}

/*
   Función     : textFieldRequirements
   Arrgumentos : int* lenght     : almacenará el tamaño del texto
                 int* bool_space : indicará si se restringe a solo un espacio
                 char* pattern   : guardará un patrón aceptado (si se indica)
   Objetivo    : capturar los requerimientos para el campo de texto
   Retorno     : ---
*/
void textFieldRequirements(int* lenght, int* bool_sapce, char* pattern)
{
   char key;

   gotoxy(INI_X, INI_Y+2);

   do { // validando tamaño del texto

      printf("Tama%co del campo [1-70] : ", 164);
      scanf("%d", lenght);

      if ((*lenght) <= 0 || (*lenght) > MAX-10)
         printf("[ERROR] : tama%co no v%clido.\n\n", 164, 160);

   } while (*lenght <= 0 || *lenght > MAX-10);

   do { // especificando si se limita a solo un espacio entre letras

      printf("\n%cRestringido a solo un espacio entre letras?\nS(i) o N(o): ", 168);
      key = toupper(getch());

   } while (key != 'S' && key != 'N');

   if (key == 'S')
      *bool_sapce = TRUE;
   else
      *bool_sapce = FALSE;

   // capturando patrón aceptado

   pattern = (char*)malloc((*lenght)*sizeof(char));
   fflush(stdin);
   printf("\n\nIntroduzca el patr%cn aceptado\n(Dejar vac%co para no establecer) : ", 162, 161);
   gets(pattern);

   if (pattern != NULL)
   {
      int lenght_pattern = strlen(pattern);
      pattern = (char*)realloc(pattern, lenght_pattern*sizeof(char)+1);
      pattern[lenght_pattern+1] = NULL;
   }

   return;
}



















































