/*============================---(source-start)---============================*/

#include   "asterion.h"



char         /*-> drive program startup --------------[ shoot  [gz.210.001.00]*/ /*-[00.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_init          (void)
{
   yCOLOR_diff_scheme (YCOLOR_WHITE);
   return 0;
}

char         /*-> process the command line -----------[ ------ [gz.530.221.83]*/ /*-[02.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_args          (int argc, char *argv[])
{
   char *a = NULL;
   int  i;
   my.file   = "call_graph";
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      /*---(color options)---------------*/
      if      (strcmp (a, "--color-white"       ) == 0)  yCOLOR_diff_scheme (YCOLOR_WHITE);
      else if (strcmp (a, "--color-light"       ) == 0)  yCOLOR_diff_scheme (YCOLOR_LIGHT);
      else if (strcmp (a, "--color-dark"        ) == 0)  yCOLOR_diff_scheme (YCOLOR_DARK );
      else if (strcmp (a, "--color-black"       ) == 0)  yCOLOR_diff_scheme (YCOLOR_BLACK);
      else if (strcmp (a, "--color-start"       ) == 0) {
         if (i + 1 < argc) if (atoi (argv [i + 1]) > 0 ) yCOLOR_diff_start (atoi (argv[++i]));
      }
      else if (strcmp (a, "--color-chaos"       ) == 0) {
         if (i + 1 < argc) if (atoi (argv [i + 1]) > 0 ) yCOLOR_diff_chaos (atoi (argv[++i]));
      }
   }
   return 0;
}

char         /*-> drive program startup --------------[ ------ [gz.530.011.07]*/ /*-[00.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_begin         (void)
{
   /*---(locals)--------------------------------*/
   char        rc          =    0;
   /*---(open window)---------------------------*/
   yXINIT_start (win_title, win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   /*---(load basics)---------------------------*/
   font_load();
   trig_load();
   rc = NODE_read();
   rc = EDGE_read();
   /*---(create texture)------------------------*/
   DRAW_init  ();
   DRAW_main  ();
   mask_small ();
   /*---(ready display)-------------------------*/
   DRAW_resize(win_w, win_h);
   /*---(complete)------------------------------*/
   return 0;
}

char         /*-> drive program startup --------------[ leaf   [gz.210.001.00]*/ /*-[00.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_final         (void)
{
   return 0;
}

char         /*-> process xwindows events ------------[ ------ [gz.UX0.0E1.R6]*/ /*-[04.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_event         ()
{
   int   frames = 0;
   int   fps;
   long  x_start, x_stop;
   long  speed;
   char      x_focus[100] = "";
   /*---(for event)------------------------*/
   XKeyEvent *key_event;
   char       the_key[5];
   int        the_bytes;
   char       done = 0;
   tNODE     *x_node       = nhead;
   char        x_ch        = ' ';
   char        x_mode      = ' ';
   char        rc          = 0;
   while (!done) {
      if (XPending(DISP)) {
         XNextEvent(DISP, &EVNT);
         /*---(start)----------------------------*/
         switch(EVNT.type) {
         case Expose:
            break;

         case ConfigureNotify:
            break;

         case KeyPress:
            key_event  = (XKeyEvent *) &EVNT;
            the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
            if (the_bytes < 1) break;
            x_ch = the_key [0];
            /*---(normal mode)-----------*/
            switch (x_mode) {
            case ' ' :
               switch (x_ch) {
               case 'Q' : done = 1;                                  break;
               case '[' : increment  = STOP;      angle  =   0.0;    break;
               case '{' : increment  = STOP;      angle +=  90.0;    break;
               case '(' : increment  = STOP;      angle +=  20.0;    break;
               case '-' : increment -= INC_SPEED;                    break;
               case '<' : increment  = STOP;      angle += single;   break;
               case '.' : increment  = STOP;      action = 0;        break;
               case '>' : increment  = STOP;      angle -= single;   break;
               case '+' : increment += INC_SPEED;                    break;
               case ')' : increment  = STOP;      angle -=  20.0;    break;
               case '}' : increment  = STOP;      angle -=  90.0;    break;
               case ']' : increment  = STOP;      angle  =   0.0;    break;
               case 'd' :                         zdist -=  20.0;    break;
               case 'D' :                         zdist -= 100.0;    break;
               case 't' :                         zdist +=  20.0;    break;
               case 'T' :                         zdist += 100.0;    break;
               case 'k' :                         ydist -=   5.0;    break;
               case 'K' :                         ydist -=  20.0;    break;
               case 'j' :                         ydist +=   5.0;    break;
               case 'J' :                         ydist +=  20.0;    break;
               case 'l' :                         xdist -=   5.0;    break;
               case 'L' :                         xdist -=  20.0;    break;
               case 'h' :                         xdist +=   5.0;    break;
               case 'H' :                         xdist +=  20.0;    break;
               case '0' : mask_big();    break;
               case '1' : mask_small();  break;
               case '2' : mask_tiny();   break;
               case 'a' : edges = 'b'; strcpy(focus, ""); flen = 0; full_refresh();               break;
               case 's' : edges = 's'; full_refresh();               break;
               case 'e' : edges = 'e'; full_refresh();               break;
               case 'b' : edges = 'b'; full_refresh();               break;
               case ';' : FIND_hintmode (x_mode, x_ch); x_mode = ';';     break;
               case ',' : FIND_filemode (x_mode, x_ch); x_mode = ',';     break;
               default  :                                            break;
               }
               break;
            case ',' :
               rc = FIND_filemode (x_mode, x_ch);
               if (rc != 0)  x_mode = ' ';
               break;
            case ';' :
               rc = FIND_hintmode (x_mode, x_ch);
               if (rc != 0)  x_mode = ' ';
               break;
            case '/' :
               x_mode = ' ';
               break;
               /*> char   temp[10];                                                                                                                     <* 
                *> int    i;                                                                                                                            <* 
                *> if        (search == '/' && x_ch != 13 && x_ch != 27) {                                                                              <* 
                *>    if ((x_ch >= 'a' && x_ch <= 'z') || (x_ch >= 'A' && x_ch <= 'Z') || (x_ch >= '0' && x_ch <= '9') || x_ch == '_' || x_ch == 8) {   <* 
                *>       sprintf(temp, "%c", x_ch);                                                                                                     <* 
                *>       strcat(x_focus, temp);                                                                                                         <* 
                *>    }                                                                                                                                 <* 
                *> } else if (search == ';' && x_ch != 13 && x_ch != 27) {                                                                              <* 
                *>    if ((x_ch >= 'a' && x_ch <= 'z') || x_ch == ';') {                                                                                <* 
                *>       sprintf(temp, "%c", x_ch);                                                                                                     <* 
                *>       strcat(x_focus, temp);                                                                                                         <* 
                *>    }                                                                                                                                 <* 
                *> } else {                                                                                                                             <* 
                *>    switch (search) {                                                                                                                 <* 
                *>    case '/': if (x_focus[0] == '\0') strcpy(focus, "");                                                                              <* 
                *>                 else strncpy(focus, x_focus, 20);                                                                                    <* 
                *>                 flen = strlen(focus);                                                                                                <* 
                *>                 break;                                                                                                               <* 
                *>    case ';': for (i = 0; i < nnode; ++i) {                                                                                           <* 
                *>                 if (x_focus[2] == x_node->hint [0] && x_focus[3] == x_node->hint [1]) {                                              <* 
                *>                    strcpy (focus, x_node->name);                                                                                     <* 
                *>                 }                                                                                                                    <* 
                *>                 x_node = x_node->next;                                                                                               <* 
                *>              }                                                                                                                       <* 
                *>              flen = strlen(focus);                                                                                                   <* 
                *>              break;                                                                                                                  <* 
                *>    }                                                                                                                                 <* 
                *>    alpha    = 0.0;                                                                                                                   <* 
                *>    search   = ' ';                                                                                                                   <* 
                *>    full_refresh ();                                                                                                                  <* 
                *> }                                                                                                                                    <*/
            }
            if (zdist     <   400.0   )  zdist     =  400.0;
            if (zdist     >  2000.0   )  zdist     = 2000.0;
            if (increment >  MAX_SPEED)  increment =  MAX_SPEED;
            if (increment < -MAX_SPEED)  increment = -MAX_SPEED;
            action = 0;
            if (increment != 0.0)  action    = 1;
            break;

         default          :
            break;
         }
      }
      /*---(timer)----------------------------------------*/
      angle -= increment;
      if (angle > 360.0) angle  = angle - 360.0 + increment;
      if (angle <   0.0) angle  = angle + 360.0 - increment;
      x_start = time_stamp();
      DRAW_texture();
      x_stop  = time_stamp();
      ++frames;
      speed = x_stop - x_start;
      if (speed > 0) fps   = 1000 / speed;
      else           fps   = 0;
      /*> printf("%2d) %4d : timer speed=%4ldms so %3dfps\n", EVNT.type, frames, speed, fps);   <*/
      /*> if (action == 1) usleep(20000);                                             <*/
      usleep(10000);
   }
   return 0;
}

char         /*-> drive program closure --------------[ ------ [gz.210.001.01]*/ /*-[00.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_end           (void)
{
   font_delete();
   yXINIT_end();
   return 0;
}



/*============================----(source-end)----============================*/
