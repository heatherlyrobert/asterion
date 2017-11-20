/*============================---(source-start)---============================*/

#include   "asterion.h"



int      nedge  = 0;
tEDGE   *ehead  = NULL;
tEDGE   *etail  = NULL;



tEDGE*       /*-> create a new edge ------------------[ leaf   [gp.420.011.00]*/ /*-[10.0000.011--]-*/ /*-[--.---.---.--]-*/
EDGE_create        (void)
{
   /*---(locals)-------------------------*/
   tEDGE    *temp      = NULL;         /* new node                            */
   /*---(create)-------------------------*/
   temp      = (tEDGE *) malloc(sizeof(tEDGE));
   /*---(initialize)---------------------*/
   temp->s     = NULL;
   temp->e     = NULL;
   temp->l     = 0;
   temp->num   = 0;
   temp->p     = NULL;
   temp->n     = NULL;
   /*---(complete)-----------------------*/
   return temp;
}

char         /*-> append node to the end -------------[ leaf   [gz.320.101.20]*/ /*-[01.0000.011--]-*/ /*-[--.---.---.--]-*/
EDGE_append        (tEDGE  *a_edge)
{
   if (ehead == NULL) {
      ehead     = a_edge;
      etail     = a_edge;
   } else {
      etail->n  = a_edge;
      a_edge->p = etail;
      etail     = a_edge;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> calculate the indent ---------------[ leaf   [gc.430.141.10]*/ /*-[01.0000.011--]-*/ /*-[--.---.---.--]-*/
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

char         /*-> find a specific edge ---------------[ leaf   [gc.310.212.20]*/ /*-[01.0000.011--]-*/ /*-[--.---.---.--]-*/
EDGE_find          (tNODE *a_source, tNODE *a_end)
{
   tEDGE    *curr   = ehead;
   while (curr != NULL) {
      if (curr->s == a_source && curr->e == a_end) return 1;
      curr  = curr->n;
   }
   return 0;
}

char         /*-> establish call list ----------------[ leaf   [gc.E91.0A3.G0]*/ /*-[02.0004.011--]-*/ /*-[--.---.---.--]-*/
EDGE_read          (void)
{
   DEBUG_I  printf("EDGE_read     () begin\n");
   /*---(locals)--------------------------------*/
   char      recd [MAXLINE] = "";      /* record from stdin                   */
   int       len       = 0;            /* uncleaned string len                */
   char*     p         = NULL;         /* strtok() parsing pointer            */
   char      q[5]      = "(";          /* strtok() delimiters                 */
   tNODE    *parent    = NULL;         /* parent node                         */
   tNODE    *found     = NULL;         /* tail of call                        */
   tEDGE    *curr      = NULL;         /* new node                            */
   tNODE    *layer[40] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
   int       level     = 1;
   char      exists    = 0;
   /*---(read first)---------------------*/
   fgets(recd, MAXLINE, stdin);
   if (feof(stdin))     return 0;
   /*---(clear blanks)-------------------*/
   while (1) {
      len = strlen(recd);
      if (recd[0] != '\n') break;
      fgets(recd, MAXLINE, stdin);
      if (feof(stdin))     return 0;
   }
   /*---(read functions)-----------------*/
   while (1) {
      /*---(name and level)--------------*/
      p         = strtok(recd, q);
      level     = EDGE_level(p);
      if (level > 39) {
         fgets(recd, MAXLINE, stdin);
         if (feof(stdin))     break;
         if (recd[0] == '\n') break;
         continue;
      }
      strltrim (p, ySTR_BOTH, 100);
      /*---(endpoints)-------------------*/
      found     = NODE_find(p);
      parent    = layer[level - 1];
      exists    = EDGE_find (parent, found);
      /*---(add edge)--------------------*/
      if (parent != NULL && found != NULL && exists == 0) {
         if (strcmp (found->s, parent->s) == 0)  ++parent->r;
         curr      = EDGE_create();
         EDGE_append(curr);
         curr->s   = parent;
         curr->e   = found;
         curr->l   = level;
         ++found->ins;
         DEBUG_I  printf("   %-20s (%2d) %-20s   #=%2d\n", parent->s, level, found->s, parent->c);
         ++parent->c;
         curr->num = parent->c;
      } else if (strncmp(p, "printf", 6) == 0) {
         if (parent != NULL) ++parent->cli;
      } else if (strncmp(p, "gl", 2) == 0) {
         if (parent != NULL) ++parent->glx;
      }
      layer [level] = found;
      /*---(prepare next)----------------*/
      fgets(recd, MAXLINE, stdin);
      if (feof(stdin))     break;
      if (recd[0] == '\n') break;
   }
   /*---(complete)-----------------------*/
   DEBUG_I  printf("EDGE_read     () end\n\n");
   return 0;
}




/*============================----(source-end)----============================*/
