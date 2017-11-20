/*============================---(source-start)---============================*/

#include   "asterion.h"



int       nnode   = 0;
tNODE    *nhead   = NULL;
tNODE    *ntail   = NULL;



tNODE*       /*-> create a new node ------------------[ leaf   [gp.960.121.20]*/ /*-[11.0000.011--]-*/ /*-[--.---.---.--]-*/
NODE_create        (char *a_name)
{
   /*---(locals)-------------------------*/
   tNODE    *temp      = NULL;         /* new node                            */
   int       len       = 0;            /* uncleaned string len                */
   /*---(create)-------------------------*/
   temp      = (tNODE *) malloc(sizeof(tNODE));
   /*---(initialize)---------------------*/
   temp->id    = 0;
   temp->type  = 0;
   temp->file  = 0;
   strcpy(temp->l, "");
   strcpy(temp->s, "");
   strcpy(temp->d, "");
   temp->ins   = 0;
   temp->c     = 0;
   temp->t     = 0;
   temp->cli   = 0;
   temp->glx   = 0;
   temp->f     = 1;
   temp->p     = NULL;
   temp->n     = NULL;
   temp->r     = 0;
   /*---(parse name)------------------*/
   temp->type  = 'f';
   len = strlen(a_name);
   if (strncmp(a_name, "o___", 4) == 0) { 
      ++a_name;
      --len;
      temp->type  = '-';
   }
   if (strncmp(a_name + len - 4, "___o", 4) == 0) { 
      --len;
      a_name [len] = '\0';
   }
   strltrim (a_name, ySTR_BOTH, 100);
   strncpy(temp->s, a_name,  20);
   /*---(complete)-----------------------*/
   return temp;
}

char         /*-> append node to list ----------------[ leaf   [gz.320.101.20]*/ /*-[01.0000.011--]-*/ /*-[--.---.---.--]-*/
NODE_append        (tNODE  *a_node)
{
   if (nhead == NULL) {
      nhead     = a_node;
      ntail     = a_node;
   } else {
      ntail->n  = a_node;
      a_node->p = ntail;
      ntail     = a_node;
   }
   /*---(complete)-----------------------*/
   return 0;
}

tNODE*       /*-> find a specific node ---------------[ leaf   [gp.320.121.20]*/ /*-[02.0000.011--]-*/ /*-[--.---.---.--]-*/
NODE_find          (char *a_name)
{
   tNODE    *curr   = nhead;
   tNODE    *found  = NULL;
   while (curr != NULL) {
      if (strcmp(curr->s, a_name) == 0) {
         found = curr;
         break;
      }
      curr  = curr->n;
   }
   return found;
}

char         /*-> read internal function list --------[ leaf   [gc.C71.065.B0]*/ /*-[02.0004.011--]-*/ /*-[--.---.---.--]-*/
NODE_read          (void)
{
   DEBUG_I  printf("NODE_read     () begin\n");
   /*---(locals)--------------------------------*/
   char      recd [MAXLINE] = "";      /* record from stdin                   */
   int       len       = 0;            /* uncleaned string len                */
   char*     p         = NULL;         /* strtok() parsing pointer            */
   char      q[5]      = " ";          /* strtok() delimiters                 */
   tNODE    *curr      = NULL;         /* new node                            */
   char      sfile[30] = "";           /* saved name of input file            */
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
      /*---(create node)-----------------*/
      p         = strtok(recd, q);
      if (p[0] == '_' || strncmp(p, "o___", 4) == 0) {
         fgets(recd, MAXLINE, stdin);
         if (recd[0] == '\n') break;
         if (feof(stdin))     return 0;
         continue;
      }
      curr      = NODE_create(p);
      NODE_append(curr);
      curr->id  = ++nnode;
      curr->l[0]  = 'a' + (nnode / 26);
      curr->l[1]  = 'a' + (nnode % 26);
      curr->l[2]  = '\0';
      /*---(get file name)---------------*/
      p = strtok(NULL, q);
      p = strtok(NULL, q);
      p = strtok(NULL, q);
      if (strcmp(sfile, p) != 0) {
         strcpy(sfile, p);
         ++nfile;
      }
      curr->file = nfile;
      /*---(prepare for next)------------*/
      DEBUG_I  printf("   (%3d) %s\n", curr->id, curr->s);
      /*---(read next)-------------------*/
      fgets(recd, MAXLINE, stdin);
      if (recd[0] == '\n') break;
      if (feof(stdin))     return 0;
   }
   single = 360.0 / nnode;
   /*---(complete)-----------------------*/
   DEBUG_I  printf("NODE_read     () end\n\n");
   return 0;
}







/*============================----(source-end)----============================*/
