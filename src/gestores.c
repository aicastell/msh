#include "gestores.h"

/****************************************************************
* Rutina de tratamiento de la interrupcion software SIGALARM    *
****************************************************************/
void gestor_SIGALRM()
{
    printf("\nAlarm clock.\n" );
    printf("Aborting %s... \n", shNAME );
    exit(0);

}/* gestor_SIGALRM */

