/*============================---(source-start)---============================*/

#include   "asterion.h"



int       nnode   = 0;
tNODE    *nhead   = NULL;
tNODE    *ntail   = NULL;



tNODE*       /*-> create a new node ------------------[ leaf   [gp.960.121.20]*/ /*-[11.0000.013.!]-*/ /*-[--.---.---.--]-*/
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

char         /*-> append node to list ----------------[ leaf   [gz.320.101.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
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

tNODE*       /*-> find a specific node ---------------[ leaf   [gp.320.121.20]*/ /*-[02.0000.103.!]-*/ /*-[--.---.---.--]-*/
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

char         /*-> read internal function list --------[ ------ [gc.C71.065.B2]*/ /*-[02.0004.102.!]-*/ /*-[--.---.---.--]-*/
NODE_read          (void)
{
   DEBUG_I  printf("NODE_read     () begin\n");
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_recd      [MAXLINE] = "";  /* record from stdin              */
   int         x_len       = 0;             /* uncleaned string len           */
   char*       p           = NULL;          /* strtok() parsing pointer       */
   char       *q           = " ";           /* strtok() delimiters            */
   char        x_hint      [5] = "";        /* temp for hint                  */
   tNODE      *x_curr      = NULL;          /* new node                       */
   char        x_sfile       [30]   = "";     /* saved name of input file       */
   char       *x_hint1     = "abcdefghijklmnopqrstuvwxyz";
   char       *x_hint2     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   /*---(read first)---------------------*/
   fgets  (x_recd, MAXLINE, stdin);
   --rce;  if (feof (stdin))      return rce;
   /*---(clear blank lines)--------------*/
   --rce;  while (1) {
      x_len = strlen (x_recd);
      if (x_recd[0] != '\n')      break;
      fgets (x_recd, MAXLINE, stdin);
      if (feof (stdin))           return rce;
   }
   /*---(read functions)-----------------*/
   --rce;
   while (1) {
      /*---(read line)-------------------*/
      fgets (x_recd, MAXLINE, stdin);
      if (feof (stdin))                     return rce;
      /*---(filter)----------------------*/
      if (x_recd [0] == '\n')               break;
      /*---(handle hint)-----------------*/
      p        = strtok (x_recd, q);
      x_len    = strlen (p);
      if (x_len < 2)                        return rce - 1;
      if (strchr (x_hint1, p[0]) == NULL)   return rce - 2;
      if (strchr (x_hint2, p[1]) == NULL)   return rce - 3;
      strcpy (x_hint, p);
      /*---(handle name)-----------------*/
      p        = strtok (NULL  , q);
      x_len    = strlen (p);
      if (x_len < 4)                        return rce - 4;
      /*---(create node)-----------------*/
      x_curr   = NODE_create (p);
      NODE_append (x_curr);
      /*---(add hint)--------------------*/
      x_curr->id  = ++nnode;
      strcpy (x_curr->l, x_hint);
      /*---(get file name)---------------*/
      p = strtok(NULL, q);
      p = strtok(NULL, q);
      p = strtok(NULL, q);
      p = strtok(NULL, q);
      if (strcmp (x_sfile, p) != 0) {
         strcpy (x_sfile, p);
         ++nfile;
      }
      x_curr->file = nfile;
      /*---(prepare for next)------------*/
      DEBUG_I  printf("   (%3d) %s\n", x_curr->id, x_curr->s);
   }
   rce -= 10;
   /*---(test for trouble)---------------*/
   --rce;  if (nnode <= 0)        return rce;
   /*---(prep stats)---------------------*/
   single = 360.0 / nnode;
   /*---(complete)-----------------------*/
   DEBUG_I  printf("NODE_read     () end\n\n");
   return 0;
}







/*============================----(source-end)----============================*/
