
#include"Card/card.h"
#include <stdio.h>
#include "Application/app.h"

static uint16_t AdminPass=6352;
int main()
{
    printf("_______________________________________________________________________\n");
    printf("|_______________________________| WELCOME |____________________________|\n");
    printf("|_______________________| ATM Machine Terminal. |______________________|\n");

    uint16_t userChoose=0;                      // to hold user choice.

    for(; userChoose==0|| userChoose==AdminPass;)
    {
        fflush(stdin);                        // flush the sdtin obejects before calling again.
        printf( " ___|\n");

        if(userChoose==AdminPass)
        {
            listSavedTransactions();          // print in dermial
        }
        else if(userChoose==0)
        {
            appStart();                      // start the ATM Machine Terminal.


        }

        printf( "|_____________________________________________________________________|\n");
        printf( " __| press 0 to continue  | any number -> exit\n");
        printf( " __|admin user enter your PIN to list saved transactions : \n");

        scanf("%d",&userChoose);
    }




    return 0;
}
