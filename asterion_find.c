/*============================---(source-start)---============================*/
#include   "asterion.h"

static     char    s_type    = '-';
char               s_file    [  5]  = "";
char               s_hint    [  5]  = "";
char               s_hintold [  5]  = "";
static     char    s_search  [100]  = "";
static     char    s_fileid  = -1;


char         /*-> tbd --------------------------------[ leaf   [fz.630.031.70]*/ /*-[03.0000.023.!]-*/ /*-[--.---.---.--]-*/
FIND__filter         (void)
{
   int         i           = 0;
   tNODE      *x_node      = NULL;
   char        x_hint      = '-';
   x_node  = nhead;
   if (strlen (s_hint) == 4)  x_hint = 'y';
   for (i = 0; i < nnode; ++i) {
      /*---(default)---------------------*/
      x_node->show = 'f';
      /*---(check file)------------------*/
      if (s_fileid >= 0) {
         if (x_node->file != s_fileid) {
            if (x_node->show == 'f')  x_node->show = '-';
         }
      }
      /*---(check hint)------------------*/
      if (x_hint == 'y') {
         if (s_hint [2] != x_node->hint [0] || s_hint[3] != x_node->hint [1]) {
            if (x_node->show == 'f')  x_node->show = 'y';
         }
      }
      /*---(next)------------------------*/
      x_node = x_node->next;
   }
   full_refresh ();
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [fz.A61.051.20]*/ /*-[02.0000.013.!]-*/ /*-[--.---.---.--]-*/
FIND__partition      (void)
{
   tNODE      *x_node      = NULL;
   int         x_count     = 0;
   int         i           = 0;
   float       x_arc       = 0.0;
   float       x_lead      = 0.0;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(count nodes)--------------------*/
   x_node  = nhead;
   for (i = 0; i < nnode; ++i) {
      /*---(default)---------------------*/
      if (x_node->show != '-') {
         ++x_count;
      }
      /*---(next)------------------------*/
      x_node = x_node->next;
   }
   /*---(place nodes)--------------------*/
   DEBUG_USER   yLOG_value   ("x_count"   , x_count);
   x_arc   = 360.0 / (x_count + 1);
   DEBUG_USER   yLOG_double  ("x_arc"     , x_arc);
   x_lead  = x_arc;
   x_node  = nhead;
   for (i = 0; i < nnode; ++i) {
      /*---(default)---------------------*/
      switch (x_node->show) {
      case '-' :
         x_node->lead   = 0.0;
         x_node->tail   = x_lead + x_arc;
         x_node->arc    = x_arc;
         break;
      case 'y' : case 'f' :
      default  :
         x_node->lead   = x_lead;
         x_node->tail   = x_lead + x_arc;
         x_node->arc    = x_arc;
         x_lead        += x_arc;
         break;
      }
      /*---(next)------------------------*/
      x_node = x_node->next;
   }
   full_refresh ();
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.KA6.247.C2]*/ /*-[02.0000.102.!]-*/ /*-[--.---.---.--]-*/
FIND_filemode        (char a_major, char a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_len       =    0;
   char        t           [5]   = "";
   char        x_fileid    =   -1;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_value   ("a_major"   , a_major);
   DEBUG_USER   yLOG_value   ("a_minor"   , a_minor);
   /*---(prepare)------------------------*/
   DEBUG_USER   yLOG_info    ("s_file"    , s_file);
   x_len = strlen (s_file);
   DEBUG_USER   yLOG_value   ("x_len"     , x_len);
   /*---(first call)---------------------*/
   --rce;  if (a_major != ',' && a_minor == ',') {
      DEBUG_USER   yLOG_note    ("first call, initialize");
      strcpy (s_file, ",");
      s_fileid = -1;
      alpha = 0.5;
      DEBUG_USER   yLOG_info    ("s_file"    , s_file);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(defense)------------------------*/
   --rce;  if (a_major != ',') {
      DEBUG_USER   yLOG_note    ("incorrect major, failed");
      strcpy (s_file, "");
      s_fileid = -1;
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_file"    , s_file);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(escaping out)-------------------*/
   if (a_minor == 10 || a_minor == 27) {
      DEBUG_USER   yLOG_note    ("return/escape, quit");
      strcpy (s_file, "");
      s_fileid = -1;
      FIND__filter ();
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_file"    , s_file);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(too small)----------------------*/
   --rce;  if (x_len <  1) {
      DEBUG_USER   yLOG_note    ("s_file too small");
      strcpy (s_file, "");
      s_fileid = -1;
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_file"    , s_file);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(too large)----------------------*/
   --rce;  if (x_len >  2) {
      DEBUG_USER   yLOG_note    ("s_file too large");
      strcpy (s_file, "");
      s_fileid = -1;
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_file"    , s_file);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(second char)--------------------*/
   --rce;  if (x_len == 1) {
      DEBUG_USER   yLOG_note    ("file identifier");
      if (strchr ("-,0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", a_minor) == NULL) {
         DEBUG_USER   yLOG_note    ("invalid char");
         strcpy (s_file, "");
         s_fileid = -1;
         alpha = 0.0;
         DEBUG_USER   yLOG_info    ("s_file"    , s_file);
         DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      sprintf (t, "%c", a_minor);
      strcat  (s_file, t);
      if      (a_minor == '-')    x_fileid = -1;
      else if (a_minor == ',')    x_fileid = -1;
      else if (a_minor < '9')     x_fileid = a_minor - '0';
      else                        x_fileid = a_minor - 'A' + 10;
      if (x_fileid <= nfile)  s_fileid = x_fileid;
      DEBUG_USER   yLOG_info    ("s_file"    , s_file);
      DEBUG_USER   yLOG_value   ("s_fileid"  , s_fileid);
   }
   /*---(enact)--------------------------*/
   DEBUG_USER   yLOG_note    ("filter now");
   FIND__filter    ();
   FIND__partition ();
   alpha = 0.0;
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 1;
}

char         /*-> tbd --------------------------------[ leaf   [fz.630.031.70]*/ /*-[03.0000.023.!]-*/ /*-[--.---.---.--]-*/
FIND__hintrelative   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         i           = 0;
   tNODE      *x_node      = NULL;
   char        x_found     = '-';
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
   DEBUG_USER   yLOG_char    ("s_hint[1]" , s_hint [1]);
   if (s_hint [1] == '[') {
      DEBUG_USER   yLOG_note    ("find first hint");
      sprintf (s_hint, ";;%s", nhead->hint);
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   if (s_hint [1] == ']') {
      DEBUG_USER   yLOG_note    ("find last hint");
      sprintf (s_hint, ";;%s", ntail->hint);
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   --rce;  if (s_hint [1] == '<' || s_hint [1] == '>') {
      DEBUG_USER   yLOG_note    ("<> hint, start search");
      if (strcmp (s_hintold, "") == 0) {
         sprintf (s_hintold, ";;%s", nhead->hint);
      }
      x_node  = nhead;
      /*---(search)----------------------*/
      for (i = 0; i < nnode; ++i) {
         /*---(filter)-------------------*/
         if (s_hintold [2] != x_node->hint [0] || s_hintold [3] != x_node->hint [1]) {
            x_node = x_node->next;
            continue;
         }
         /*---(found)--------------------*/
         DEBUG_USER   yLOG_info    ("found"     , x_node->hint);
         x_found = 'y';
         break;
      }
      if (x_found == '-') {
         DEBUG_USER   yLOG_note    ("current not found");
         DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (s_hint [1] == '<') {
         DEBUG_USER   yLOG_note    ("look for previous");
         if (x_node->prev == NULL) {
            DEBUG_USER   yLOG_note    ("previous is null");
            DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce -1);
            return rce - 1;
         }
         sprintf (s_hint, ";;%s", x_node->prev->hint);
         DEBUG_USER   yLOG_note    ("s_hint"    , s_hint);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      if (s_hint [1] == '>') {
         DEBUG_USER   yLOG_note    ("look for next");
         if (x_node->next == NULL) {
            DEBUG_USER   yLOG_note    ("next is null");
            DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce -2);
            return rce - 2;
         }
         sprintf (s_hint, ";;%s", x_node->next->hint);
         DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return 0;
      }
   }
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [ge.MA8.23#.A1]*/ /*-[02.0000.102.!]-*/ /*-[--.---.---.--]-*/
FIND_hintmode        (char a_major, char a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_len       =    0;
   char        t           [5]   = "";
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_value   ("a_major"   , a_major);
   DEBUG_USER   yLOG_value   ("a_minor"   , a_minor);
   /*---(prepare)------------------------*/
   DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
   x_len = strlen (s_hint);
   DEBUG_USER   yLOG_value   ("x_len"     , x_len);
   /*---(first call)---------------------*/
   --rce;  if (a_major != ';' && a_minor == ';') {
      DEBUG_USER   yLOG_note    ("first call, initialize");
      strcpy (s_hint, ";");
      alpha = 0.5;
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(defense)------------------------*/
   --rce;  if (a_major != ';') {
      DEBUG_USER   yLOG_note    ("incorrect major, failed");
      strcpy (s_hint, "");
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(escaping out)-------------------*/
   if (a_minor == 13 || a_minor == 27) {
      DEBUG_USER   yLOG_note    ("clear hint focus");
      strcpy (s_hint, "");
      FIND__filter ();
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(second call)--------------------*/
   if (a_minor == ';') {
      DEBUG_USER   yLOG_note    ("second semi-colon");
      strcat (s_hint, ";");
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(relative moves)-----------------*/
   if (strchr ("[<>]", a_minor) != NULL) {
      DEBUG_USER   yLOG_note    ("relative hint");
      sprintf (t, "%c", a_minor);
      strcat  (s_hint, t);
      rc = FIND__hintrelative ();
      if (rc < 0) {
         strcpy (s_hint, "");
         alpha = 0.0;
         DEBUG_USER   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      strcpy  (s_hintold, s_hint);
      FIND__filter ();
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(too small)----------------------*/
   --rce;  if (x_len <  2) {
      DEBUG_USER   yLOG_note    ("s_hint too small");
      strcpy (s_hint, "");
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(too large)----------------------*/
   --rce;  if (x_len >  3) {
      DEBUG_USER   yLOG_note    ("s_hint too large");
      strcpy (s_hint, "");
      alpha = 0.0;
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(first char)---------------------*/
   --rce;  if (x_len == 2) {
      DEBUG_USER   yLOG_note    ("first hint character");
      if (strchr ("abcdefghijklmnopqrstuvwxyz", a_minor) == NULL) {
         DEBUG_USER   yLOG_note    ("invalid char");
         strcpy (s_hint, "");
         alpha = 0.0;
         DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
         DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      sprintf (t, "%c", a_minor);
      strcat  (s_hint, t);
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(second char)--------------------*/
   --rce;  if (x_len == 3) {
      DEBUG_USER   yLOG_note    ("second hint character");
      if (strchr ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", a_minor) == NULL) {
         DEBUG_USER   yLOG_note    ("invalid char");
         strcpy (s_hint, "");
         alpha = 0.0;
         DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
         DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      sprintf (t, "%c", a_minor);
      strcat  (s_hint, t);
      strcpy  (s_hintold, s_hint);
      DEBUG_USER   yLOG_info    ("s_hint"    , s_hint);
   }
   /*---(enact)--------------------------*/
   DEBUG_USER   yLOG_note    ("filter now");
   FIND__filter ();
   alpha = 0.0;
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 1;
}

char         /*-> tbd --------------------------------[ leaf   [gz.210.201.00]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
FIND_searchmode      (char a_major, char a_minor)
{
   return 0;
}
