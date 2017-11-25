/*============================---(source-start)---============================*/
#include   "asterion.h"



int      nedge  = 0;
int      aedge  = 0;
tEDGE   *ehead  = NULL;
tEDGE   *etail  = NULL;


static FILE *s_file   = NULL;          /* file pointer   */
static char *s_name   = "HTAG.flow";   /* file name      */
static char  s_recd   [MAXLINE] = "";  /* incomming node */



/*====================------------------------------------====================*/
/*===----                      basic file handling                     ----===*/
/*====================------------------------------------====================*/
static void   o___FILE_HANDLE_____o (void) { return; }

char         /*-> open file for reading and prep -----[ leaf   [ge.723.023.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
EDGE_open          ()
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   DEBUG_INPT  yLOG_info    ("s_name"    , s_name);
   s_file = fopen (s_name, "r");
   DEBUG_INPT  yLOG_point   ("s_file"    , s_file);
   --rce;  if (s_file == NULL) {
      DEBUG_INPT  yLOG_note    ("file could not be openned");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_note    ("file successfully opened");
   /*---(complete)-----------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> close file and wrap ----------------[ leaf   [ge.411.011.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
EDGE_close         (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(close file)---------------------*/
   DEBUG_INPT  yLOG_info    ("s_name"    , s_name);
   DEBUG_INPT  yLOG_point   ("s_file"    , s_file);
   if (s_file == NULL) {
      DEBUG_INPT  yLOG_note    ("no file to close");
   } else {
      DEBUG_INPT  yLOG_note    ("close file");
      fclose  (s_file);
   }
   /*---(null file pointer)--------------*/
   s_file = NULL;
   DEBUG_INPT  yLOG_point   ("s_file"    , s_file);
   /*---(complete)-----------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}


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
   char        rc          =    0;
   char*       p           = NULL;          /* strtok() parsing pointer            */
   char       *q           = "(";           /* strtok() delimiters                 */
   tNODE      *x_orig      = NULL;         /* origin node                         */
   tNODE      *x_dest      = NULL;         /* tail of call                        */
   tEDGE      *x_edge      = NULL;         /* new node                            */
   tNODE      *x_layers    [40] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
   int         x_level     = 1;
   char        x_exists    = 0;
   int         x_len       = 0;
   int         x_dups      = 0;
   int         x_bado      = 0;
   int         x_badd      = 0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   rc = EDGE_open ();
   DEBUG_INPT  yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read edges)---------------------*/
   while (1) {
      /*---(read line)-------------------*/
      fgets (s_recd, MAXLINE, s_file);
      if (feof (s_file))                     break;
      ++aedge;
      /*---(name and level)--------------*/
      p         = strtok (s_recd, q);
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
   EDGE_close ();
   rce -= 10;
   /*---(test for trouble)---------------*/
   --rce;  if (nedge <= 0)        return rce;
   /*---(summary)------------------------*/
   printf ("total = %d\n", aedge);
   printf ("dups  = %d\n", x_dups );
   printf ("bado  = %d\n", x_bado );
   printf ("badd  = %d\n", x_badd );
   printf ("nedge = %d\n", nedge);
   /*---(complete)-----------------------*/
   DEBUG_I  printf("EDGE_read     () end\n\n");
   return 0;
}




/*============================----(source-end)----============================*/
