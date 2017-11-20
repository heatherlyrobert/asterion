/*============================---(source-start)---============================*/
#include    "asterion.h"



int          /*-> main driver ------------------------[ ------ [gn.321.211.23]*/ /*-[00.0000.000.!]-*/ /*-[--.---.---.--]-*/
main               (int a_argc, char *a_argv[])
{
   int rc = 0;
   rc = PROG_args(a_argc, a_argv);
   DEBUG_T  printf("\nconverge : program callback analysis using radial convergence diagramming\n\n");
   if (rc == 0) rc = PROG_begin();
   if (rc == 0) rc = PROG_event();
   PROG_end();                         /* end should run no matter what       */
   return rc;
}



/*============================----(source-end)----============================*/
