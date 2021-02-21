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

#define TRUE        1
#define FALSE       0
#define MAX        80
#define MAX_DATE   10

#define RIGHT   77
#define LEFT    75
#define ENTER   13
#define ESC     27
#define BKSP     8
#define SPACE   ' '
#define POINT   46

#define MIN_ASCII    33
#define MAX_ASCII   126


#define INI_X    1
#define INI_Y    1

#define TEXT_COLOR      BLUE
#define BCKGRND_COLOR   LIGHTGRAY

#define DTC   WHITE
#define DBC   BLACK

#define OPT_1    1
#define OPT_2    2
#define OPT_3    3

/* Prototipos de función */

void selectOption(int*);
void textFieldRequirements(int*, int*, char*);
void numericFieldRequirements(int*, int*);
void captureText(char*, int, int, char);
void showField(char*, int, int, int, int);
void setColor(int, int);
void colorDefault();

int main()
{
   int type_field, length;
   char* text;

   while (TRUE)
   {
      selectOption(&type_field);
      system("cls");

      /* Pidiendo requerimientos según la opción seleccionada */

      if (type_field == OPT_1) // campo tipo texto
      {
         int bool_space;
         char* pattern;

         pattern = (char*)malloc(MAX*sizeof(char));

         textFieldRequirements(&length, &bool_space, pattern);

         if (pattern != NULL)
            pattern = (char*)realloc(pattern, strlen(pattern)*sizeof(char));

         text = (char*)calloc(length, sizeof(char));

         captureText(text, length, bool_space, pattern);

         free(pattern);
      }
      else if (type_field == OPT_2) // campo tipo fecha
      {
         length = MAX_DATE;
      }
      else if (type_field == OPT_3) // campo tipo numérico
      {
         int digits, precision;

         numericFieldRequirements(&digits, &precision);
      }

      system("cls");
      free(text);
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
                 int* flag       : indicará si se restringe a solo un espacio
                 char* pattern   : guardará un patrón aceptado (si se indica)
   Objetivo    : capturar los requerimientos para el campo de texto
   Retorno     : ---
*/
void textFieldRequirements(int* lenght, int* flag, char* pattern)
{
   char key;

   do { // validando tamaño del texto

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

   // capturando patrón aceptado (no obligatorio)

   fflush(stdin);
   printf("\n\nIntroduzca el patr%cn aceptado\n(Dejar vac%co para no establecer) : ", 162, 161);
   gets(pattern);

   return;
}

/*
   Función     : numericFieldRequirements
   Arrgumentos : int* max_digits : cantidad de dígitos aceptada
                 int* precision  : cantidad de dígitos después del punto
   Objetivo    : capturar los requerimientos para el campo numérico
   Retorno     : ---
*/
void numericFieldRequirements(int* max_digits, int* precision)
{
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
   Función     : captureText
   Arrgumentos : char* str    : cadena de texto a capturar
                 int n        : longitud de "str"
                 int flag     : indica si se restringe a solo un espacio
                 char pattern : patrón de texto aceptado
   Objetivo    : capturar campo de tipo texto
   Retorno     : ---
*/
void captureText(char* str, int n, int flag, char pattern)
{
   int index = 0;
   char key;

   system("cls");
   _setcursortype(100);

   do {

      showField(str, index, n, INI_X+9, INI_Y+5);

      do{
         key = getch();
      }while (!(key >= MIN_ASCII && key <= MAX_ASCII) && key != ENTER && key != ESC && key != RIGHT
              && key != LEFT && key != POINT && key != SPACE && key != BKSP);

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
            else if (index < n)
            {
               *(str+index) = key;
               index++;
            }
         }
      }

   } while (key != ENTER && key != ESC);

   if (key != ESC)
      *(str+index) = NULL;

   return;
}

/*
   Función     : showField
   Arrgumentos : char* str : cadena de texto
                 int pos   : posición del cursor
                 int n     : longitud de "str"
                 int x     : posición en x (columnas)
                 int y     : posición en y (filas)
   Objetivo    : mostrar el campo de texto
   Retorno     : ---
*/
void showField(char* str, int pos, int n, int x, int y)
{
   int index;

   gotoxy(x, y);
   printf("[");
   setColor(TEXT_COLOR, BCKGRND_COLOR);

   for (index = 0; index < n; index++)
   {
      gotoxy(x+index+1, y);

      if (*(str+index))
         printf("%c", *(str+index));
      else
         printf(" ");
   }

   colorDefault();
   gotoxy(x+index, y);
   printf("]");
   gotoxy(x+pos+1, y);

   return;
}

/*
   Función     : setColor
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
   Función     : colorDeafult
   Arrgumentos : ---
   Objetivo    : establecer los colores de texto y fondo predeterminados
   Retorno     : ---
*/
void colorDefault()
{
   setColor(DTC, DBC);
}





















































