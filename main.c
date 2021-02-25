/*
   Manejador de campo programado en C.

   Permite escoger entre tres opciones:

   1- Campo de texto
   2- Campo de fecha
   3- Campo numérico

   Autor : Naoki Nakao
   Fecha : 25/02/2021
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
#define DM30       30
#define DM31       31
#define FEB         2

#define RIGHT   77
#define LEFT    75
#define ENTER   13
#define ESC     27
#define BKSP     8
#define SPACE   ' '
#define POINT   46
#define BAR     32
#define SLASH   47

#define MIN_ASCII    33
#define MAX_ASCII   126


#define INI_X    1
#define INI_Y    1
#define LOGS     INI_Y+8

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
void captureTextField(char*, int, int, char*);
int validSep(char*, int);
int strEnd(char*);
int correctPattern(char*, char*);
void captureDateField(char*, int);
int validDate(char*);
void captureNumericField(char*, int, int);
void showField(char*, int, int, int, int);
void setColor(int, int);
void colorDefault();
void clearLine(int);
void warningMessage(char *);
void showText(char*, char*);

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

         pattern = (char*)calloc(MAX, sizeof(char));

         textFieldRequirements(&length, &bool_space, pattern);

         if (pattern != NULL)
            pattern = (char*)realloc(pattern, strlen(pattern)*sizeof(char));

         text = (char*)calloc(length, sizeof(char));

         captureTextField(text, length, bool_space, pattern);

         free(pattern);
      }
      else if (type_field == OPT_2) // campo tipo fecha
      {
         length = MAX_DATE;
         text = (char*)calloc(length, sizeof(char));
         captureDateField(text, length);
      }
      else if (type_field == OPT_3) // campo tipo numérico
      {
         int precision;

         numericFieldRequirements(&length, &precision);
         text = (char*)calloc(length, sizeof(char));
         captureNumericField(text, length, precision);
      }

      free(text);
      system("cls");
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
   do { // validando máxima cantidad de dígitos

      printf("Cantidad m%cxima de d%cgitos : ", 160, 161);
      scanf("%d", max_digits);

      if (*max_digits <= 0 || *max_digits > MAX-10)
         printf("[ERROR] : cantidad no v%clida.\n\n", 160);

   } while (*max_digits <= 0 || *max_digits > MAX-10);

   do { // validando cantidad de dígitos después del punto

      printf("\nCantidad de d%cgitos despu%cs del punto : ", 161, 130);
      scanf("%d", precision);

      if (*precision < 0 || *precision >= *max_digits)
         printf("[ERROR] : cantidad no v%clida.\n\n", 160);

   } while (*precision < 0 || *precision >= *max_digits);

   return;
}

/*
   Función     : captureTextField
   Arrgumentos : char* str    : cadena de texto a capturar
                 int n        : longitud de "str"
                 int flag     : indica si se restringe a solo un espacio
                 char pattern : patrón de texto aceptado
   Objetivo    : capturar campo de tipo texto
   Retorno     : ---
*/
void captureTextField(char* str, int n, int flag, char* pattern)
{
   int index = 0, last = index;
   char key, temp[n];

   system("cls");
   _setcursortype(100);

   gotoxy(INI_X+9, INI_Y+3);
   printf("Intoduzca el texto:");

   do {

      showField(str, index, n, INI_X+9, INI_Y+5);

      do{
         key = getch();
      }while (!(key >= MIN_ASCII && key <= MAX_ASCII) && key != ENTER && key != ESC && key != RIGHT
              && key != LEFT && key != POINT && key != SPACE && key != BKSP && key != BAR);

      if (key == RIGHT)
      {
         if (index < n-1 && index < last)
         {
            index++;
         }
      }
      else if (key == LEFT)
      {
         if (index > 0)
            index--;
      }
      else if (key == BAR)
      {
         // se verifica que los caracteres no se salgan del margen
         // permitido al presionar la barra espaciadora
         if (index < n-1 && strEnd(str) < n)
         {
            strcpy(temp, str+index);
            strcpy(str+index, " ");
            strcpy(str+index+1, temp);
            index++;
            last++;
         }
      }
      else if (key == ENTER)
      {
         // verificando que el espaciado sea correcto si se restringe
         // a solo en espacio entre letras
         if (flag)
         {
            for (int i = 0; *(str+i); i++)
            {
               if (!validSep(str, i))
               {
                  key = NULL;
                  warningMessage("texto restringido a un espacio entre letras.");
                  break;
               }
            }
         }
         else if (pattern)
         {
            if (!correctPattern(str, pattern))
            {
               key = NULL;
               warningMessage("el texto no coincide con el patron.");
               continue;
            }
         }
      }
      else
      {
         if (key != ENTER && key != ESC)
         {
            if (key == BKSP)
            {
               if (index)
               {
                  strcpy(temp, str+index);
                  index--;
                  strcpy(str+index, temp);
                  last--;
               }
            }
            else if (index < n)
            {
               if (flag)
               {
                  if (!validSep(str, index))
                     continue;
               }
               *(str+index) = key;
               index++;

               if (index > last)
                  last = index;
            }
         }
      }

      *(str+last) = NULL;

   } while (key != ENTER && key != ESC);

   if (key != ESC)
      showText(str, "Texto digitado:");

   return;
}

/*
   Función     : validSep
   Arrgumentos : char* str : cadena de texto
                 int pos   : posición del cursor
   Objetivo    : confirmar si la separación entre letras es válida
   Retorno     : (int) 1 si la separación es válida; (int) 0 en caso contrario
*/
int validSep(char* str, int pos)
{
   if ( !(strncmp(str+pos-2, "  ", 2)) || !(strncmp(str+pos, "  ", 2)) )
      return FALSE;
   else
      return TRUE;
}

/*
   Función     : strEnd
   Arrgumentos : char* str : cadena de texto
   Objetivo    : encontrar la posición del caracter NULL
   Retorno     : (int) count : posición del caracter NULL
*/
int strEnd(char* str)
{
   int count = 0;

   while (*(str+count))
      count++;

   return count;
}

/*
   Función     : captureDateField
   Arrgumentos : char* str1 : cadena de texto 1
                 char* str2 : cadena de texto 2
   Objetivo    : comparar "str1" con "str2"
   Retorno     : (int) 1 si son iguales; (int) 0 en caso contrario
*/
int correctPattern(char* str1, char* str2)
{
   int index;

   for (index = 0; *(str2+index); index++)
   {
      if (*(str1+index) != *(str2+index))
         return FALSE;
   }

   return TRUE;
}

/*
   Función     : captureDateField
   Arrgumentos : char* str    : cadena de texto a capturar
                 int n        : longitud de "str"
   Objetivo    : capturar campo de tipo fecha
   Retorno     : ---
*/
void captureDateField(char* str, int n)
{
   int index = 0, last = index;
   char key;

   _setcursortype(100);

   gotoxy(INI_X+9, INI_Y+1);
   printf("Formato dd/mm/yyyy Ejemplo -> 02/10/2002");
   gotoxy(INI_X+9, INI_Y+2);
   printf("Intoduzca la fecha:");

   do {

      showField(str, index, n, INI_X+9, INI_Y+4);

      do {
         key = getch();
      } while (!(key >= '0' && key <= '9') && key != SLASH && key != ENTER && key != ESC
               && key != RIGHT && key != LEFT && key != BKSP);

      if (key == RIGHT)
      {
         if (index < n-1 && index < last)
            index++;
      }
      else if (key == LEFT)
      {
         if (index > 0)
            index--;
      }
      else if (key == ENTER)
      {
         if (!validDate(str))
         {
            key = NULL;
            warningMessage("fecha o formato incorrecto.");
            continue;
         }
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
                  last--;
               }
            }
            else if (index < n)
            {
               *(str+index) = key;
               index++;

               if (index > last)
                  last = index;
            }
         }
      }

      *(str+last) = NULL;

   } while (key != ENTER && key != ESC);

   if (key != ESC)
      showText(str, "Fecha digitada:");

   return;
}

/*
   Función     : validDate

   Arrgumentos : char* date : fecha en cadena de texto
   Objetivo    : validar que la fecha y su formato sean correctos
   Retorno     : (int) 1 si la fecha es correcta; (int) 0 en caso contrario
*/
int validDate(char* date)
{
   int index, count;

   // validando la posición de los slash
   if (*(date+2) != SLASH && *(date+5) != SLASH)
      return FALSE;

   // validando la cantidad de números
   for (index = 0, count = 0; index < MAX_DATE; index++)
   {
      if (*(date+index) >= '0' && *(date+index) <= '9')
         count++;
   }

   if (count != 8) return FALSE;

   char temp[MAX_DATE-6];
   int d_months[] = {DM31, DM30-2, DM31, DM30, DM31, DM30, DM31, DM31, DM30, DM31, DM30, DM31},
       day, month, year;

   strncpy(temp, date, 2);
   day = strtol(temp, NULL, 10);
   strncpy(temp, date+3, 2);
   month = strtol(temp, NULL, 10);
   strncpy(temp, date+6, 4);
   year = strtol(temp, NULL, 10);

   // validando la cantidad de días en los meses
   if (month <= 12 && month > 0 && day > 0)
   {
      if (month == FEB)
      {
         // validando año bisiesto
         if ( day == DM30-1 && !(year % 4) )
            return TRUE;
      }

      if (day > d_months[month-1])
         return FALSE;
   }
   else
      return FALSE;

   return TRUE;
}

/*
   Función     : captureNumericField
   Arrgumentos : char str*     : cadena de texto
                 int digits    : cantidad de dígitos permitidos en "str"
                 int precision : cantidad de números después del punto
   Objetivo    : capturar campo de tipo numérico
   Retorno     : ---
*/
void captureNumericField(char* str, int digits, int precision)
{
   int index = 0, n = digits, last = index, flag = FALSE;
   char key;

   system("cls");
   _setcursortype(100);

   gotoxy(INI_X+9, INI_Y+2);
   printf("Intoduzca el n%cmero:", 163);

   do {

      showField(str, index, n, INI_X+9, INI_Y+4);

      do {
         key = getch();
      } while (!(key >= '0' && key <= '9') && (key != POINT && !flag) && key != ENTER
               && key != ESC && key != RIGHT && key != LEFT && key != BKSP);

      if (key == RIGHT)
      {
         if (index < n-1 && index < last)
            index++;
      }
      else if (key == LEFT)
      {
         if (index > 0)
            index--;
      }
      else if (key == POINT)
      {
         if (!flag)
         {
            *(str+index) = key;
            index++;

            if (index > last)
               last = index;
         }
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
                  last--;
               }
            }
            else if (index < n)
            {
               *(str+index) = key;
               index++;

               if (index > last)
                  last = index;
            }
         }
      }

      flag = FALSE;

      for (int count = 0; *(str+count); count++)
      {
         if (*(str+count) == POINT)
         {
            flag = TRUE;
            if ( (n - (count+1)) > precision )
            {
               n = count+precision+1;
               clearLine(INI_Y+4);
            }
            break;
         }
      }

      *(str+last) = NULL;

      if (flag)
         continue;
      else
         n = digits;

   } while (key != ENTER && key != ESC);

   if (key != ESC)
      showText(str, "Numero digitado:");

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
   gotoxy(x+index+1, y);
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

   return;
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

   return;
}

/*
   Función     : clearLine
   Arrgumentos : int line : línea de la consola
   Objetivo    : borrar la línea "line"
   Retorno     : ---
*/
void clearLine(int line)
{
   for (int index = INI_X; index <= MAX; index++)
   {
      gotoxy(INI_X+index, line);
      printf(" ");
   }

   return;
}

/*
   Función    : warningMessage
   Argumentos : char* message : cadena de texto que contiene el mensaje
   Objetivo   : mostrar el mensaje de error "message"
   Retorno    : ---
*/
void warningMessage(char* message)
{
   gotoxy(INI_X, LOGS);
   setColor(WHITE, RED);
   printf("ADVERTENCIA:");
   colorDefault();
   printf(" %s", message);
   getch();
   clearLine(LOGS);

   return;
}

/*
   Función    : showtext
   Argumentos : char* str     : cadena de texto a mostrar
                char* message : cadena de texto que contiene el mensaje
   Objetivo   : mostrar en pantalla "str"
   Retorno    : ---
*/
void showText(char* str, char* message)
{
   gotoxy(INI_X, LOGS);
   setColor(WHITE, BLUE);
   printf("%s", message);
   colorDefault();
   printf(" %s", str);
   gotoxy(INI_X, LOGS+2);
   printf("Presione cualquier tecla.");
   getch();

   return;
}




















































