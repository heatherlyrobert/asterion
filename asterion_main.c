/*============================---(source-start)---============================*/
#include    "asterion.h"



int          /*-> main driver ------------------------[ ------ [gn.531.212.97]*/ /*-[01.0000.000.!]-*/ /*-[--.---.---.--]-*/
main               (int a_argc, char *a_argv[])
{
   char        rc          = 0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = yURG_urgs    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_args    (a_argc, a_argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc >= 0)  rc = PROG_final   ();
   if (rc <  0) {
      PROG_end ();
      return -1;
   }
   DEBUG_T  printf("\nconverge : program callback analysis using radial convergence diagramming\n\n");
   if (rc == 0) rc = PROG_begin();
   if (rc == 0) rc = PROG_event();
   PROG_end();                         /* end should run no matter what       */
   return rc;
}



/*============================----(source-end)----============================*/
