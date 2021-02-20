/*
   Manejador de campo programado en C.

   Permite escoger entre tres opciones:

   1- Campo de texto
   2- Campo de fecha
   3- Campo num�rico

   Autor : Naoki Nakao
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.c>

/* Macros */

#define TRUE        1
#define FALSE       0
#define MAX        80
#define MAX_DATE   10

#define RIGHT   77
#define LEFT    75
#define ENTER   13
#define ESC     27
#define BKSP     8
#define SPACE   32
#define POINT   46

#define INI_X    1
#define INI_Y    1

#define FIELD_COLOR    WHITE
#define TEXT_COLOR     BLUE
#define CURSOR_COLOR   BLACK

#define OPT_1    1
#define OPT_2    2
#define OPT_3    3

/* Prototipos de funci�n */

void selectOption(int*);
void textFieldRequirements(int*, int*, char*);
void numericFieldRequirements(int*, int*);
void captureField(char*, int, int, int);
void showField(char*, int, int, int, int);
void setColor(int, int);
void validateTextField(char*, int, int, char*);

int main()
{
   int type_field, length;
   char* text;

   while (TRUE)
   {
      selectOption(&type_field);

      /* Pidiendo requerimientos seg�n la opci�n seleccionada */

      if (type_field == OPT_1) // campo tipo texto
      {
         int bool_space;
         char* pattern;

         pattern = (char*)malloc(MAX*sizeof(char));

         textFieldRequirements(&length, &bool_space, pattern);

         if (pattern != NULL)
            pattern = (char*)realloc(pattern, strlen(pattern)*sizeof(char));

         text = (char*)calloc(length, sizeof(char));

         captureField(text, length, 10, 10);

         free(text);
         free(pattern);
      }
      else if (type_field == OPT_2) // campo tipo fecha
      {
         length = MAX_DATE;
      }
      else if (type_field == OPT_3) // campo tipo num�rico
      {
         int digits, precision;

         numericFieldRequirements(&digits, &precision);
      }
   }

   return 0;
}

/*
   Funci�n     : selectOption
   Arrgumentos : int* option : referencia para almacenar la opci�n seleccionada
   Objetivo    : capturar y validar la opci�n
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
   Funci�n     : textFieldRequirements
   Arrgumentos : int* lenght     : almacenar� el tama�o del texto
                 int* flag       : indicar� si se restringe a solo un espacio
                 char* pattern   : guardar� un patr�n aceptado (si se indica)
   Objetivo    : capturar los requerimientos para el campo de texto
   Retorno     : ---
*/
void textFieldRequirements(int* lenght, int* flag, char* pattern)
{
   char key;

   gotoxy(INI_X, INI_Y+2);

   do { // validando tama�o del texto

      printf("Tama%co del campo [1-70] : ", 164);
      scanf("%d", lenght);

      if (*lenght <= 0 || *lenght > MAX-10)
         printf("[ERROR] : tama%co no v%clido.\n\n", 164, 160);

   } while (*lenght <= 0 || *lenght > MAX-10);

   do { // especificando si se limita a solo un espacio entre letras

      printf("\n%cRestringido a solo un espacio entre letras?\nS(i) o N(o): ", 168);
      key = toupper(getch());

   } while (key != 'S' && key != 'N');

   if (key == 'S')
      *flag = TRUE;
   else
      *flag = FALSE;

   // capturando patr�n aceptado (no obligatorio)

   fflush(stdin);
   printf("\n\nIntroduzca el patr%cn aceptado\n(Dejar vac%co para no establecer) : ", 162, 161);
   gets(pattern);

   return;
}

/*
   Funci�n     : numericFieldRequirements
   Arrgumentos : int* max_digits : cantidad de d�gitos aceptada
                 int* precision  : cantidad de d�gitos despu�s del punto
   Objetivo    : capturar los requerimientos para el campo num�rico
   Retorno     : ---
*/
void numericFieldRequirements(int* max_digits, int* precision)
{
   gotoxy(INI_X, INI_Y+2);

   do {

      printf("Cantidad m%cxima de d%cigitos : ", 160, 161);
      scanf("%d", max_digits);

      if (*max_digits <= 0 || *max_digits > MAX-10)
         printf("[ERROR] : cantidad no v%clida.\n\n", 160);

   } while (*max_digits <= 0 || *max_digits > MAX-10);

   do {

      printf("\nCantidad de d%cgitos despu%cs del punto : ", 161, 130);
      scanf("%d", precision);

      if (*precision < 0 || *precision >= *max_digits)
         printf("[ERROR] : cantidad no v%clida.\n\n", 160);

   } while (*precision < 0 || *precision >= *max_digits);

   return;
}

/*
   Funci�n     : captureField
   Arrgumentos : char* str : cadena de texto
                 int n     : longitud de "str"
                 int x     : posici�n en x (columnas)
                 int y     : posici�n en y (filas)
   Objetivo    : controlar la captura del texto
   Retorno     : ---
*/
void captureField(char* str, int n, int x, int y)
{
   int index = 0;
   char key;

   clrscr();
   _setcursortype(100);

   do {

      showField(str, index, n, x, y);

      do {
         key = getch();
      } while ( !((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))
               && key != ENTER && key != ESC && key != BKSP && key != SPACE && key != POINT
               && key != RIGHT && key != LEFT);

      if (key == RIGHT)
      {
         if (index < n-1)
            index++;
      }
      else if (key == LEFT)
      {
         if (index > 0)
            index--;
      }
      else
      {
         if (key != ENTER && key != ESC)
         {
            if (key == BKSP)
            {
               if (index)
               {
                  index--;
                  *(str+index) = NULL;
               }
            }
            else
            {
               if (index < n)
               {
                  *(str+index) = key;
                  index++;
               }
            }
         }
      }

   } while (key != ENTER && key != ESC);

   if (key != ESC)
      *(str+index) = NULL;

   return;
}

/*
   Funci�n     : showField
   Arrgumentos : char* str : cadena de texto
                 int pos   : posici�n del cursor
                 int n     : longitud de "str"
                 int x     : posici�n en x (columnas)
                 int y     : posici�n en y (filas)
   Objetivo    : mostrar el campo de texto
   Retorno     : ---
*/
void showField(char* str, int pos, int n, int x, int y)
{
   int index;

   setColor(BLUE, LIGHTGRAY);

   for (index = 0; index < n; index++)
   {
      gotoxy(x+index+1, y);

      if (*(str+index))
         printf("%c", *(str+index));
      else
         printf(" ");
   }

   colorDefault();
   gotoxy(x+pos+1, y);

   return;
}

/*
   Funci�n     : setColor
   Arrgumentos : int text_color       : color del texto
                 int background_color : color de fondo
   Objetivo    : cambiar el color del texto y del fondo
   Retorno     : ---
*/
void setColor(int text_color, int background_color)
{
   textcolor(text_color);
   textbackground(background_color);
}

/*
   Funci�n     : colorDeafult
   Arrgumentos : ---
   Objetivo    : establecer los colores de texto y fondo predeterminados
   Retorno     : ---
*/
void colorDefault()
{
   setColor(LIGHTGRAY, BLACK);
}

/*
   Funci�n     : validateTextField
   Arrgumentos : char* str     : cadena de texto a validar
                 int n         : longitud de "str"
                 int flag      : indica si se restringe a un solo espacio
                 char* pattern : contiene el patr�n aceptado
   Objetivo    : manejar la entrada de datos en el campo tipo texto
   Retorno     : ---
*/
void validateTextField(char* str, int n, int flag, char* pattern)
{

}



















































