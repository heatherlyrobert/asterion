/*============================---(source-start)---============================*/

#include   "asterion.h"



int       nnode   = 0;
tNODE    *nhead   = NULL;
tNODE    *ntail   = NULL;

static FILE *s_file   = NULL;          /* file pointer   */
static char *s_name   = "HTAG.tags";   /* file name      */
static char  s_recd   [MAXLINE] = "";  /* incomming node */



/*====================------------------------------------====================*/
/*===----                      basic file handling                     ----===*/
/*====================------------------------------------====================*/
static void   o___FILE_HANDLE_____o (void) { return; }

char         /*-> open file for reading and prep -----[ leaf   [ge.723.023.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
NODE_open          ()
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
NODE_close         (void)
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

char         /*-> set colors -------------------------[ leaf   [ge.411.011.20]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
NODE_color         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tNODE      *x_node      = nhead;
   /*---(walk nodes)---------------------*/
   /*> yCOLOR_diff_first ();                                                          <*/
   while (x_node != NULL) {
      x_node->color = yCOLOR_diff_next ();
      x_node        = x_node->next;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> assign arc to nodes ----------------[ leaf   [gz.530.031.10]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
NODE_place_default   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   tNODE      *x_node      = nhead;
   float       x_arc       = 0.0;
   float       x_lead      = 0.0;
   /*---(prepare)------------------------*/
   x_arc   = 360.0 / nnode;
   /*---(walk nodes)---------------------*/
   while (x_node != NULL) {
      /*---(position)----------*/
      x_node->lead = x_lead;
      x_node->arc  = x_arc;
      x_node->tail = x_lead + x_arc;
      /*---(next)--------------*/
      x_lead      += x_arc;
      x_node       = x_node->next;
      /*---(done)--------------*/
   }
   /*---(complete)-----------------------*/
   return 0;
}

tNODE*       /*-> create a new node ------------------[ ------ [gp.A70.121.21]*/ /*-[11.0000.013.!]-*/ /*-[--.---.---.--]-*/
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
   strcpy (temp->hint, "");
   strcpy (temp->name, "");
   strcpy (temp->desc, "");
   temp->ins    = 0;
   temp->nchild = 0;
   temp->nheir  = 0;
   temp->color  = yCOLOR_diff_next ();
   temp->cli    = 0;
   temp->glx    = 0;
   temp->show   = 'f';
   temp->prev   = NULL;
   temp->next   = NULL;
   temp->r      = 0;
   temp->lead   = 0;
   temp->tail   = 0;
   temp->arc    = 0;
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
   strncpy(temp->name, a_name,  20);
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
      ntail->next  = a_node;
      a_node->prev = ntail;
      ntail        = a_node;
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
      if (strcmp(curr->name, a_name) == 0) {
         found = curr;
         break;
      }
      curr  = curr->next;
   }
   return found;
}

char         /*-> check incomming record -------------[ ------ [ge.FA1.0B7.B3]*/ /*-[02.0001.102.!]-*/ /*-[--.---.---.--]-*/
NODE_filter        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       = 0;             /* uncleaned string len           */
   char       *x_hint1     = "abcdefghijklmnopqrstuvwxyz";
   char       *x_hint2     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(line types)---------------------*/
   --rce;  if (s_recd [  0] == '\0')                      return -(rce);
   --rce;  if (s_recd [  0] == '\n')                       return -(rce);
   --rce;  if (s_recd [  0] == ' ' )                       return -(rce);
   --rce;  if (s_recd [  0] == '#' )                       return -(rce);
   /*---(hint)---------------------------*/
   --rce;  if (strchr (x_hint1, s_recd [  0]) == NULL)     return -(rce);
   --rce;  if (strchr (x_hint2, s_recd [  1]) == NULL)     return -(rce);
   /*---(gap)----------------------------*/
   --rce;  if (strncmp (s_recd +   2, "  "      , 2) != 0) return -(rce);
   /*---(prefix of name)-----------------*/
   --rce;  if (strchr  (x_hint2, s_recd [  4]) == NULL)    return -(rce);
   --rce;  if (strchr  (x_hint2, s_recd [  5]) == NULL)    return -(rce);
   /*---(field break)--------------------*/
   --rce;  if (strncmp (s_recd +  46, "#1#"     , 3) != 0) return -(rce);
   /*---(field break)--------------------*/
   --rce;  if (strncmp (s_recd +  92, "function", 8) != 0) return -(rce);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> read internal function list --------[ ------ [ge.FA1.0B7.B3]*/ /*-[02.0001.102.!]-*/ /*-[--.---.---.--]-*/
NODE_read          (void)
{
   DEBUG_I  printf("NODE_read     () begin\n");
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       = 0;             /* uncleaned string len           */
   char*       p           = NULL;          /* strtok() parsing pointer       */
   char       *q           = " ";           /* strtok() delimiters            */
   char        x_hint      [5] = "";        /* temp for hint                  */
   tNODE      *x_curr      = NULL;          /* new node                       */
   char        x_sfile       [30]   = "";     /* saved name of input file       */
   char       *x_hint1     = "abcdefghijklmnopqrstuvwxyz";
   char       *x_hint2     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   int         x_lines     = 0;
   int         x_total     = 0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   rc = NODE_open ();
   DEBUG_INPT  yLOG_value   ("rc"        , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read functions)-----------------*/
   --rce;  while (1) {
      /*---(read line)-------------------*/
      fgets (s_recd, MAXLINE, s_file);
      if (feof (s_file))                    break;
      ++x_lines;
      /*> printf ("\n%s", s_recd);                                                    <*/
      /*---(filter)----------------------*/
      rc = NODE_filter ();
      if (rc > 0)                           continue;
      if (rc < 0)                           break;
      ++x_total;
      /*> printf ("   GOOD\n");                                                       <*/
      /*---(handle hint)-----------------*/
      p        = strtok (s_recd, q);
      x_len    = strlen (p);
      if (x_len < 2)                        return rce - 1;
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
      strcpy (x_curr->hint, x_hint);
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
      DEBUG_I  printf("   (%3d) %s\n", x_curr->id, x_curr->name);
   }
   NODE_close ();
   rce -= 10;
   /*---(test for trouble)---------------*/
   --rce;  if (nnode <= 0)        return rce;
   /*---(prep stats)---------------------*/
   single = 360.0 / nnode;
   NODE_place_default ();
   /*---(summary)------------------------*/
   printf ("lines = %d\n", x_lines);
   printf ("total = %d\n", x_total);
   printf ("nnode = %d\n", nnode);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}







/*============================----(source-end)----============================*/
