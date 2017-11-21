/*============================---(source-start)---============================*/

#include   "asterion.h"



int      nedge  = 0;
tEDGE   *ehead  = NULL;
tEDGE   *etail  = NULL;



tEDGE*       /*-> create a new edge ------------------[ leaf   [gp.420.011.00]*/ /*-[10.0000.013.!]-*/ /*-[--.---.---.--]-*/
EDGE_create        (void)
{
   /*---(locals)-------------------------*/
   tEDGE    *temp      = NULL;         /* new node                            */
   /*---(create)-------------------------*/
   temp      = (tEDGE *) malloc(sizeof(tEDGE));
   /*---(initialize)---------------------*/
   temp->beg   = NULL;
   temp->end   = NULL;
   temp->lvl   = 0;
   temp->seq   = 0;
   temp->prev  = NULL;
   temp->next  = NULL;
   /*---(complete)-----------------------*/
   return temp;
}

char         /*-> append node to the end -------------[ leaf   [gz.320.101.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
EDGE_append        (tEDGE  *a_edge)
{
   if (ehead == NULL) {
      ehead     = a_edge;
      etail     = a_edge;
   } else {
      etail->next  = a_edge;
      a_edge->prev = etail;
      etail     = a_edge;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> calculate the indent ---------------[ leaf   [gc.430.141.10]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
EDGE_level         (char   *a_name)
{
   /*---(locals)-------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       len       = 0;            /* uncleaned string len                */
   int       indent    = 0;
   int       level     = 0;
   /*---(calculate)----------------------*/
   len = strlen(a_name);
   for (i = 0; i < len; ++i) {
      if (a_name[i] != ' ') break;
      ++indent;
   }
   level = (indent / 4) + 1;
   /*---(complete)-----------------------*/
   return level;
}

char         /*-> find a specific edge ---------------[ leaf   [gc.310.212.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
EDGE_find          (tNODE *a_source, tNODE *a_end)
{
   tEDGE    *x_edge   = ehead;
   while (x_edge != NULL) {
      if (x_edge->beg == a_source && x_edge->end == a_end) return 1;
      x_edge  = x_edge->next;
   }
   return 0;
}

char         /*-> establish call list ----------------[ ------ [ge.IC1.0F2.H5]*/ /*-[02.0001.102.!]-*/ /*-[--.---.---.--]-*/
EDGE_read          (void)
{
   DEBUG_I  printf("EDGE_read     () begin\n");
   /*---(locals)--------------------------------*/
   char        rce         =  -10;
   char        x_recd      [MAXLINE] = "";  /* record from stdin                   */
   char*       p           = NULL;          /* strtok() parsing pointer            */
   char       *q           = "(";           /* strtok() delimiters                 */
   tNODE      *x_orig      = NULL;         /* origin node                         */
   tNODE      *x_dest      = NULL;         /* tail of call                        */
   tEDGE      *x_edge      = NULL;         /* new node                            */
   tNODE      *x_layers    [40] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
   int         x_level     = 1;
   char        x_exists    = 0;
   int         x_len       = 0;
   int         x_total     = 0;
   int         x_dups      = 0;
   int         x_bado      = 0;
   int         x_badd      = 0;
   /*---(read edges)---------------------*/
   while (1) {
      /*---(read line)-------------------*/
      fgets (x_recd, MAXLINE, stdin);
      if (feof (stdin))                     break;
      /*---(filter)----------------------*/
      if (x_recd [0] == '\n') {
         if (x_total <= 0)                  continue;
         else                               break;
      }
      ++x_total;
      /*---(name and level)--------------*/
      p         = strtok (x_recd, q);
      x_len     = strlen (p);
      x_level   = EDGE_level (p);
      if (x_level <  0)                       continue;
      if (x_level > 39)                       continue;
      strltrim (p, ySTR_BOTH, 100);
      /*---(endpoints)-------------------*/
      x_orig    = x_layers [x_level - 1];
      x_dest    = NODE_find(p);
      x_exists  = EDGE_find (x_orig, x_dest);
      /*---(filter)----------------------*/
      if (x_exists == 1) {
         ++x_dups;
         continue;
      }
      if (x_dest == NULL) {
         ++x_badd;
         continue;
      }
      x_layers [x_level] = x_dest;
      if (x_orig == NULL) {
         ++x_bado;
         continue;
      }
      /*---(add edge)--------------------*/
      if (x_orig != NULL && x_dest != NULL && x_exists == 0) {
         if (strcmp (x_dest->name, x_orig->name) == 0)  ++x_orig->r;
         x_edge    = EDGE_create();
         EDGE_append(x_edge);
         x_edge->beg   = x_orig;
         x_edge->end   = x_dest;
         x_edge->lvl   = x_level;
         ++x_dest->ins;
         DEBUG_I  printf("   %-20s (%2d) %-20s   #=%2d\n", x_orig->name, x_level, x_dest->name, x_orig->nchild);
         ++x_orig->nchild;
         x_edge->seq = x_orig->nchild;
         ++nedge;
      } else if (strncmp(p, "printf", 6) == 0) {
         if (x_orig != NULL) ++x_orig->cli;
      } else if (strncmp(p, "gl", 2) == 0) {
         if (x_orig != NULL) ++x_orig->glx;
      }
      /*---(prepare for next)------------*/
   }
   rce -= 10;
   /*---(test for trouble)---------------*/
   --rce;  if (nedge <= 0)        return rce;
   /*---(summary)------------------------*/
   printf ("total = %d\n", x_total);
   printf ("dups  = %d\n", x_dups );
   printf ("bado  = %d\n", x_bado );
   printf ("badd  = %d\n", x_badd );
   printf ("nedge = %d\n", nedge);
   /*---(complete)-----------------------*/
   DEBUG_I  printf("EDGE_read     () end\n\n");
   return 0;
}




/*============================----(source-end)----============================*/
