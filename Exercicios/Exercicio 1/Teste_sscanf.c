#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
   int size;
   char string[20], dtm[100] = "1 Mensagem de Teste pela UART";


   //strcpy( dtm, "Saturday March 25 1989" );
   sscanf( dtm, "%d %s", &size, string);

   printf("%s (%d)\n", string, size);
    
   return(0);
}