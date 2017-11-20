/*============================---(source-start)---============================*/

#include   "asterion.h"



char         /*-> process the command line -----------[ leaf   [gz.430.221.50]*/ /*-[01.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_args          (int argc, char *argv[])
{
   char *a = NULL;
   int  i;
   my.file   = "call_graph";
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if      (strncmp(a, "-",      5) == 0)     my.file     = "stdin";
      else if (strncmp(a, "@m",     5) == 0)     debug_top   = debug_mem   = 'y';
      else if (strncmp(a, "@i",     5) == 0)     debug_top   = debug_input = 'y';
      else if (strncmp(a, "@g",     5) == 0)     debug_top   = debug_graph = 'y';
      else                                       my.file     = a;
   }
   /*> printf ("file = %s\n", my.file);                                               <*/
   return 0;
}

char         /*-> drive program startup --------------[ ------ [gz.530.021.03]*/ /*-[00.0000.101.!]-*/ /*-[--.---.---.--]-*/
PROG_begin         (void)
{
   /*---(locals)--------------------------------*/
   char        rc          =    0;
   /*---(open window)---------------------------*/
   yXINIT_start(win_title, win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   /*---(load basics)---------------------------*/
   font_load();
   trig_load();
   rc = NODE_read();

   printf ("nnode = %d\n", nnode);
   printf ("rc    = %d\n", rc);


   EDGE_read();
   /*---(create texture)------------------------*/
   draw_init();
   draw_main();
   mask_big();
   /*---(ready display)-------------------------*/
   draw_resize(win_w, win_h);
   /*---(complete)------------------------------*/
   return 0;
}

char         /*-> process xwindows events ------------[ ------ [gz.RV0.0E1.Q3]*/ /*-[05.0000.101.!]-*/ /*-[--.---.---.--]-*/
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
            if (search == ' ') {
               switch (the_key[0]) {
               case 'Q': done = 1;                                  break;
               case '[': increment  = STOP;      angle  =   0.0;    break;
               case '{': increment  = STOP;      angle +=  90.0;    break;
               case '(': increment  = STOP;      angle +=  20.0;    break;
               case '-': increment -= INC_SPEED;                    break;
               case '<': increment  = STOP;      angle += single;   break;
               case '.': increment  = STOP;      action = 0;        break;
               case ',': increment  = PLAY;                         break;
               case '>': increment  = STOP;      angle -= single;   break;
               case '+': increment += INC_SPEED;                    break;
               case ')': increment  = STOP;      angle -=  20.0;    break;
               case '}': increment  = STOP;      angle -=  90.0;    break;
               case ']': increment  = STOP;      angle  =   0.0;    break;
               case 'd':                         zdist -=  20.0;    break;
               case 'D':                         zdist -= 100.0;    break;
               case 't':                         zdist +=  20.0;    break;
               case 'T':                         zdist += 100.0;    break;
               case 'k':                         ydist -=   5.0;    break;
               case 'K':                         ydist -=  20.0;    break;
               case 'j':                         ydist +=   5.0;    break;
               case 'J':                         ydist +=  20.0;    break;
               case 'l':                         xdist -=   5.0;    break;
               case 'L':                         xdist -=  20.0;    break;
               case 'h':                         xdist +=   5.0;    break;
               case 'H':                         xdist +=  20.0;    break;
               case '0': mask_big();    break;
               case '1': mask_small();  break;
               case '2': mask_tiny();   break;
               case '/': alpha = 0.8; search = '/'; x_focus[0] = '\0'; break;
               case ';': alpha = 0.8; search = ';'; strcpy(x_focus, ";") ; break;
               case 'a': edges = 'b'; strcpy(focus, ""); flen = 0; full_refresh();               break;
               case 's': edges = 's'; full_refresh();               break;
               case 'e': edges = 'e'; full_refresh();               break;
               case 'b': edges = 'b'; full_refresh();               break;
               default :                                            break;
               }
            } else {
               char   ch  = the_key[0];
               char   temp[10];
               tNODE *curr = nhead;
               int    i;
               if        (search == '/' && ch != 13 && ch != 27) {
                  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_' || ch == 8) {
                     sprintf(temp, "%c", ch);
                     strcat(x_focus, temp);
                  }
               } else if (search == ';' && ch != 13 && ch != 27) {
                  if ((ch >= 'a' && ch <= 'z') || ch == ';') {
                     sprintf(temp, "%c", ch);
                     strcat(x_focus, temp);
                  }
               } else {
                  switch (search) {
                  case '/': if (x_focus[0] == '\0') strcpy(focus, "");
                               else strncpy(focus, x_focus, 20);
                               flen = strlen(focus);
                               break;
                  case ';': for (i = 0; i < nnode; ++i) {
                               if (x_focus[2] == curr->l[0] && x_focus[3] == curr->l[1]) {
                                  strcpy(focus, curr->s);
                               }
                               curr = curr->n;
                            }
                            flen = strlen(focus);
                            break;
                  }
                  alpha    = 0.0;
                  search   = ' ';
                  full_refresh ();
               }
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
      draw_texture();
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
