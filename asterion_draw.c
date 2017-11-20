/*============================---(source-start)---============================*/
#include   "asterion.h"



char   win_title[]     = "converge - radial convergence visualization";

/*---(sizes)--------------s--------------*/
int       win_h     =  1000;                /* window height                  */
int       win_w     =  1000;                /* window width                   */
int       tex_h     =  1300;                /* texture height                 */
int       tex_w     =  1300;                /* texture width                  */

/*---(opengl objects)--------------------*/
uint      tex       =     0;                /* texture for image              */
uint      fbo       =     0;                /* framebuffer                    */
uint      depth     =     0;                /* depth buffer                   */

/*---(fonts)-----------------------------*/
char  *face_sm  = "verdana_sm";
char  *face_bg  = "verdana";
int    txf_sm;
int    txf_bg;


int       spaces    = 0;          /* number of radial spaces required         */

float     single    = 0.0;
float     angle     = 0.0;
float     increment = PLAY;
char      action    = 1;

char      debug_top    = 'n';
char      debug_mem    = 'n';
char      debug_input  = 'n';
char      debug_graph  = 'n';

char      file_format = 'f';
char      nfile     = 0;            /* number of input file                */

float     xdist       =    0.0;
float     ydist       =    0.0;
float     zdist       = 1000.0;

char      search      = ' ';
char      focus [200] = "";
uint      flen        = 0;
float     alpha       = 0.0;
char      edges       = 'b';
char      mask        = 'b';

/*----------------------------------------------------------------------------*/





/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char         /*-> tbd --------------------------------[ leaf   [gz.320.021.00]*/ /*-[01.0000.011--]-*/ /*-[--.---.---.--]-*/
trig_load (void)
{
   int   i;
   float d;
   for (i = 0; i < 3600; ++i) {
      d   = (float) i / 10;
      my_sin[i] = sin(d * DEG2RAD);
      my_cos[i] = cos(d * DEG2RAD);
   }
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [gz.430.021.06]*/ /*-[00.0000.031--]-*/ /*-[--.---.---.--]-*/
mask_big   (void)
{
   Pixmap    bounds    = XCreatePixmap(DISP, BASE, win_w, win_h, 1);
   GC        gc        = XCreateGC(DISP, bounds, 0, NULL);
   XSetForeground(DISP, gc, 0);
   XFillRectangle(DISP, bounds, gc, 0, 0, win_w, win_h);
   XSetForeground(DISP, gc, 1);
   /*> XFillArc(DISP, bounds, gc, win_w / 2, win_h / 2, win_w / 2, win_h / 2, 0, 360 * 64);   <*/
   XFillArc(DISP, bounds, gc, 0, 0, win_w, win_h, 0, 360 * 64);
   XShapeCombineMask(DISP, BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(complete)-----------------------*/
   XFreePixmap(DISP, bounds);
   xdist = 0.0;
   ydist = 0.0;
   zdist = 1000.0;
   mask  = 'b';
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [gz.430.021.06]*/ /*-[00.0000.011--]-*/ /*-[--.---.---.--]-*/
mask_small (void)
{
   Pixmap    bounds    = XCreatePixmap(DISP, BASE, win_w, win_h, 1);
   GC        gc        = XCreateGC(DISP, bounds, 0, NULL);
   XSetForeground(DISP, gc, 0);
   XFillRectangle(DISP, bounds, gc, 0, 0, win_w, win_h);
   XSetForeground(DISP, gc, 1);
   /*> XFillArc(DISP, bounds, gc, win_w / 2, win_h / 2, win_w / 2, win_h / 2, 0, 360 * 64);   <*/
   XFillArc(DISP, bounds, gc, 125, 125, 750, 750, 0, 360 * 64);
   XShapeCombineMask(DISP, BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(complete)-----------------------*/
   XFreePixmap(DISP, bounds);
   xdist = 0.0;
   ydist = 0.0;
   zdist = 1300.0;
   mask  = 's';
   return 0;
}

char         /*-> tbd --------------------------------[ ------ [gz.430.021.06]*/ /*-[00.0000.021--]-*/ /*-[--.---.---.--]-*/
mask_tiny  (void)
{
   Pixmap    bounds    = XCreatePixmap(DISP, BASE, win_w, win_h, 1);
   GC        gc        = XCreateGC(DISP, bounds, 0, NULL);
   XSetForeground(DISP, gc, 0);
   XFillRectangle(DISP, bounds, gc, 0, 0, win_w, win_h);
   XSetForeground(DISP, gc, 1);
   /*> XFillArc(DISP, bounds, gc, win_w / 2, win_h / 2, win_w / 2, win_h / 2, 0, 360 * 64);   <*/
   XFillArc(DISP, bounds, gc, 250, 250, 500, 500, 0, 360 * 64);
   XShapeCombineMask(DISP, BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(complete)-----------------------*/
   XFreePixmap(DISP, bounds);
   xdist = 0.0;
   ydist = 0.0;
   zdist = 2000.0;
   mask  = 't';
   return 0;
}

long         /*-> tbd --------------------------------[ leaf   [gn.420.021.00]*/ /*-[00.0000.011--]-*/ /*-[--.---.---.--]-*/
time_stamp()                      /* PURPOSE : timestamp in microseconds      */
{
   /* second
    * millisecond  ms  0.001 sec
    * microsecond  us  0.000001 sec
    * nanosecond   ns  0.000000001 sec
    * picosecond   ps  0.000000000001 sec
    */
   long             tint = 0;
   struct timeval   tv;
   gettimeofday(&tv, NULL);
   tint += (int) (tv.tv_sec * 1000);
   tint += (int) tv.tv_usec / 1000;
   return tint;

}

char         /*-> tbd --------------------------------[ ------ [gz.430.001.31]*/ /*-[00.0000.011--]-*/ /*-[--.---.---.--]-*/
full_refresh       (void)
{
   texture_free();
   font_delete();
   yXINIT__gdestroy();
   yXINIT__gsetup();
   font_load();
   draw_init();
   draw_main();
   if      (mask == 'b') mask_big();
   else if (mask == 's') mask_big();
   else                  mask_tiny();
   draw_resize(win_w, win_h);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char         /*-> tbd --------------------------------[ leaf   [gz.420.001.20]*/ /*-[01.0000.021--]-*/ /*-[--.---.---.--]-*/
font_load(void)
{
   txf_sm = yFONT_load(face_sm);
   if (txf_sm < 0) {
      fprintf(stderr, "Problem loading %s\n", face_sm);
      exit(1);
   }
   txf_bg = yFONT_load(face_bg);
   if (txf_bg < 0) {
      fprintf(stderr, "Problem loading %s\n", face_bg);
      exit(1);
   }
   return 0;
}


char         /*-> tbd --------------------------------[ leaf   [gz.210.001.00]*/ /*-[00.0000.021--]-*/ /*-[--.---.---.--]-*/
font_delete(void)
{
   yFONT_free(txf_sm);
   yFONT_free(txf_bg);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                             opengl                           ----===*/
/*====================------------------------------------====================*/
static void      o___OPENGL__________________o (void) {;}

char         /*-> tbd --------------------------------[ leaf   [gz.970.031.10]*/ /*-[02.0080.011--]-*/ /*-[--.---.---.--]-*/
draw_texture (void)
{
   char  temp[100];
   float w = 0;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef( xdist,  ydist,   0.0);
   glPushMatrix(); {
      glTranslatef(   0.0,    0.0, -zdist);
      glRotatef(angle, 0.0, 0.0, 1.0);
      glBindTexture   (GL_TEXTURE_2D, tex);
      glBegin(GL_POLYGON);
      glTexCoord2f( 0.00, 1.00);
      glVertex3f( -400,  400, 0.0);
      glTexCoord2f( 1.00, 1.00);
      glVertex3f(  400,  400, 0.0);
      glTexCoord2f( 1.00, 0.00);
      glVertex3f(  400, -400, 0.0);
      glTexCoord2f( 0.00, 0.00);
      glVertex3f( -400, -400, 0.0);
      glEnd();
      glBindTexture   (GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(shade if not-focused)-----------*/
   float    z = -5.0;
   if (alpha != 0.0) {
      glColor4f (0.0f, 0.0f, 0.0f, alpha);
      glBegin(GL_POLYGON); {
         glVertex3f   ( -400,   400,    z);
         glVertex3f   (  400,   400,    z);
         glVertex3f   (  400,  -400,    z);
         glVertex3f   ( -400,  -400,    z);
      } glEnd();
   }
   /*---(force the redraw)-----------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.BA0.071.00]*/ /*-[02.0040.011--]-*/ /*-[--.---.---.--]-*/
draw_back          (void)
{
   /*> printf("drawing\n");                                                           <*/
   float d   = 0.0;
   int   r = 200;
   float rad;
   float x, y;
   int   rc = 0;
   char    temp[100];
   /*---(center)---------------------------*/
   r = 240;
   glBindTexture(GL_TEXTURE_2D, 0);
   glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
   glBegin(GL_POLYGON); {
      for (d = 0; d <= 365; d += 5) {
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x = (r * sin(rad));
         y = (r * cos(rad));
         glVertex3f(x, y,  20.0);
      }
   } glEnd();
   glBegin(GL_LINE_STRIP); {
      for (d = 0; d <= 365; d += 5) {
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x = (r * sin(rad));
         y = (r * cos(rad));
         glVertex3f(x, y,  22.0);
      }
   } glEnd();
   r = 245;
   glBegin(GL_POLYGON); {
      for (d = 0; d <= 365; d += 5) {
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x = (r * sin(rad));
         y = (r * cos(rad));
         glVertex3f(x, y,  -5.0);
      }
   } glEnd();
   glBegin(GL_LINE_STRIP); {
      for (d = 0; d <= 365; d += 5) {
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x = (r * sin(rad));
         y = (r * cos(rad));
         glVertex3f(x, y,  -5.0);
      }
   }glEnd();
   /*---(show radial)----------------------*/
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   /*---(complete)-------------------------*/
   return 0;
}

char  gcolor = 0;

char         /*-> tbd --------------------------------[ leaf   [gz.450.201.10]*/ /*-[00.0000.021--]-*/ /*-[--.---.---.--]-*/
color_set          (char a_base, float a_alpha)
{
   switch (a_base) {
   case 0 : glColor4f (1.0f, 0.0f, 0.0f, a_alpha); break;
   case 1 : glColor4f (0.3f, 0.7f, 0.0f, a_alpha); break;
   case 2 : glColor4f (0.0f, 0.1f, 0.0f, a_alpha); break;
   case 3 : glColor4f (0.0f, 0.7f, 0.3f, a_alpha); break;
   case 4 : glColor4f (0.0f, 0.3f, 0.7f, a_alpha); break;
   case 5 : glColor4f (0.7f, 0.3f, 0.0f, a_alpha); break;
   case 6 : glColor4f (0.0f, 0.0f, 1.0f, a_alpha); break;
   case 7 : glColor4f (0.7f, 0.0f, 0.3f, a_alpha); break;
   case 8 : glColor4f (0.3f, 0.0f, 0.7f, a_alpha); break;
   default: glColor4f (0.1f, 0.1f, 0.1f, a_alpha); break;
   }
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.WQ0.0C1.D0]*/ /*-[04.0060.011--]-*/ /*-[--.---.---.--]-*/
draw_edges         (void)
{
   int   r = 15 + 90 * 2.5;
   float d;
   float rad;
   float x, y;
   float arc = 360.0 / nnode;
   float     i    = 0;                       /* loop iterator                 */
   char      xcolor = 0;
   tEDGE    *curr = ehead;
   char    xbegins = 0;
   char    xends   = 0;
   char      msg[200];
   /*> printf ("focus (%2d) <<%s>>\n", flen, focus);                                  <*/
   while (curr != NULL) {
      if (flen > 0) {
         xbegins = (strstr(curr->s->s, focus) == NULL) ? 0 : 1;
         xends   = (strstr(curr->e->s, focus) == NULL) ? 0 : 1;
         /*> printf ("(%c) f=<<%-20.20s>>, s=<<%-20.20s>> (%d),  e=<<%-20.20s>> (%d)\n",   <* 
          *>       edges, focus, curr->s->s, xbegins, curr->e->s, xends);                  <*/
         if        (edges == 's' &&  xbegins != 1) {
            curr = curr->n; continue;
         } else if (edges == 'e' &&  xends   != 1) {
            curr = curr->n; continue;
         } else if (edges == 'b' && (xends   != 1 && xbegins != 1)) {
            curr = curr->n; continue;
         }
      }
      /*> printf ("drawing %s\n", curr->s->s);                                        <*/
      glLineWidth(2.0);
      if        (curr->l <= 2) {
         glLineWidth(4.0);
      } else if (curr->l <= 3) {
         glLineWidth(2.0);
      } else if (curr->l <= 4) {
         glLineWidth(1.0);
      } else if (curr->l <= 6) {
         glLineWidth(1.0);
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0xAAAA);
      } else {
         glLineWidth(1.0);
         glEnable(GL_LINE_STIPPLE);
         glLineStipple(1, 0xA0A0);
      }
      if (curr->s != curr->e) {
         /*---(center point)----------------*/
         ctrl[1][0] =   0;
         ctrl[1][1] =   0;
         ctrl[1][2] =  40;
         /*---(start)-----------------------*/
         d   = (curr->s->id + (curr->s->t / 2.0)) * arc;
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x  = (r * sin(rad));
         y  = (r * cos(rad));
         ctrl[0][0] =   x;
         ctrl[0][1] =   y;
         ctrl[0][2] =  40;
         /*---(end)-------------------------*/
         d   = (curr->e->id + (curr->e->t / 2.0)) * arc;
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x  = (r * sin(rad));
         y  = (r * cos(rad));
         ctrl[2][0] =   x;
         ctrl[2][1] =   y;
         ctrl[2][2] =  40;
         /*---(draw)------------------------*/
         xcolor = curr->s->id % 9;
         color_set (xcolor, 1.0);
         glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrl[0][0]);
         glEnable(GL_MAP1_VERTEX_3);
         glBegin(GL_LINE_STRIP); {
            for (i = 0.0; i <= 1.01; i += 0.05) {
               glEvalCoord1f(i);
            }
         } glEnd();
         glDisable(GL_MAP1_VERTEX_3);
      } else {
         glDisable(GL_LINE_STIPPLE);
         /*---(start)-----------------------*/
         d   = (curr->s->id + (curr->s->t / 2.0)) * arc;
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x   = (r * sin(rad));
         y   = (r * cos(rad));
         ctrlr[0][0] =   x;
         ctrlr[0][1] =   y;
         ctrlr[0][2] =  40;
         ctrlr[3][0] =   x;
         ctrlr[3][1] =   y;
         ctrlr[3][2] =  40;
         d  -= 10;
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x   = ((r - 40) * sin(rad));
         y   = ((r - 40) * cos(rad));
         ctrlr[1][0] =   x;
         ctrlr[1][1] =   y;
         ctrlr[1][2] =  40;
         d  += 20;
         rad = ((float) (d) / 360.0) * (2 * 3.1415927);
         x   = ((r - 40) * sin(rad));
         y   = ((r - 40) * cos(rad));
         ctrlr[2][0] =   x;
         ctrlr[2][1] =   y;
         ctrlr[2][2] =  40;
         /*---(draw)------------------------*/
         xcolor = curr->s->id % 9;
         color_set (xcolor, 1.0);
         glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlr[0][0]);
         glEnable(GL_MAP1_VERTEX_3);
         glBegin(GL_LINE_STRIP); {
            for (i = 0.0; i <= 1.01; i += 0.05) {
               glEvalCoord1f(i);
            }
         } glEnd();
         glDisable(GL_MAP1_VERTEX_3);
      }
      glDisable(GL_LINE_STIPPLE);
      /*---(dot)-------------------------*/
      d   = (curr->e->id + (curr->e->t / 2.0)) * arc;
      rad = ((float) (d) / 360.0) * (2 * 3.1415927);
      x  = (r * sin(rad));
      y  = (r * cos(rad));
      glPointSize(10.0);
      glBegin(GL_POINTS);
      glVertex3f(   x,   y, 50.0);
      glEnd();
      /*---(edge number)-----------------*/
      if (edges == 's') {
         glPushMatrix(); {
            glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
            d   = (curr->e->id + (curr->e->t / 2.0)) * arc;
            glRotatef ( 90 - d, 0.0f, 0.0f, 1.0f);
            glTranslatef (r - 30, 0.0, 40.0);
            snprintf (msg, 200, "%d", curr->num);
            yFONT_print(txf_sm,  8, YF_MIDCEN, msg);
         } glPopMatrix();
      }
      /*---(edge number)-----------------*/
      /*> glPushMatrix(); {                                                           <* 
       *>    glColor4f (0.0f, 0.0f, 0.0f, 1.0f);                                      <* 
       *>    d   = (curr->e->id + (curr->e->t / 2.0)) * arc;                          <* 
       *>    glRotatef ( 90 - d, 0.0f, 0.0f, 1.0f);                                   <* 
       *>    glTranslatef (r, 0.0, 20.0);                                             <* 
       *>    glColor4f (0.8f, 0.8f, 0.8f, 1.0f);                                      <* 
       *>    glBegin(GL_POLYGON); {                                                   <* 
       *>       glVertex3f( -15.0f,   -5.0f,   0.0f);                                 <* 
       *>       glVertex3f( -10.0f,    0.0f,   0.0f);                                 <* 
       *>       glVertex3f( -15.0f,    5.0f,   0.0f);                                 <* 
       *>       glVertex3f( -50.0f,    0.0f,   0.0f);                                 <* 
       *>    } glEnd();                                                               <* 
       *> } glPopMatrix();                                                            <*/
      /*---(prepare for next)------------*/
      curr = curr->n;
   }
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.#Y0.0H1.J0]*/ /*-[05.00#0.011--]-*/ /*-[--.---.---.--]-*/
draw_slices        (void)
{
   /*---(locals)---------------------------*/
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   int   r = 0;
   float rad;
   float x, y;
   float x1, y1;
   float x2, y2;
   float d1, d2;
   float d   = 0.0;
   float     i    = 0;                       /* loop iterator                 */
   float alpha = 0.0f;
   float arc = 360.0 / nnode;
   tNODE    *curr = nhead;
   int       lfile = 0;
   char    new_type = 'n';
   while (curr != NULL) {
      /*---(establish color group)-------*/
      gcolor = curr->id % 9;
      if (curr->n == NULL && gcolor == 1) gcolor = 4;
      /*---(level based settings)--------*/
      /*> r   = 25 + 2.5 * 90;                                                        <*/
      r   = 25 + 2.5 * 90;
      alpha = 0.8f;
      /*---(level based settings)--------*/
      d1  = (curr->id - 0.40) * arc;
      d2  = (curr->id + curr->t + 0.40) * arc;
      d   = (curr->id + (curr->t / 2.0)) * arc;
      glPushMatrix(); {
         /*---(file line)----------------*/
         color_set (curr->file % 9, 0.3);
         if (curr->file == lfile) d1  = (curr->id - 0.60) * arc;
         glBegin(GL_POLYGON); {
            glVertex3f(   0.0f,    0.0f, -15.0f);
            for (i = d1; i <= d2; i += 1) {
               rad = ((float) (i) / 360.0) * (2 * 3.1415927);
               x  = ((r + 15) * sin(rad));
               y  = ((r + 15) * cos(rad));
               glVertex3f(     x,      y, -15.0f);
            }
            rad = ((float) (d2) / 360.0) * (2 * 3.1415927);
            x  = ((r + 15) * sin(rad));
            y  = ((r + 15) * cos(rad));
            glVertex3f(     x,      y, -15.0f);
            glVertex3f(   0.0f,    0.0f, -15.0f);
         } glEnd();
         lfile = curr->file;
         /*---(function)-----------------*/
         glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
         if (curr->type == 'f') color_set (gcolor, alpha);
         d1  = (curr->id - 0.40) * arc;
         glBegin(GL_POLYGON); {
            glVertex3f(   0.0f,    0.0f,   0.0f);
            for (i = d1; i <= d2; i += 1) {
               rad = ((float) (i) / 360.0) * (2 * 3.1415927);
               x  = (r * sin(rad));
               y  = (r * cos(rad));
               glVertex3f(     x,      y,   0.0f);
            }
            rad = ((float) (d2) / 360.0) * (2 * 3.1415927);
            x  = (r * sin(rad));
            y  = (r * cos(rad));
            glVertex3f(     x,      y,   0.0f);
            glVertex3f(   0.0f,    0.0f,   0.0f);
         } glEnd();
         glBegin(GL_LINE_STRIP); {
            glVertex3f(   0.0f,    0.0f,   5.0f);
            for (i = d1; i <= d2; i += 1) {
               rad = ((float) (i) / 360.0) * (2 * 3.1415927);
               x  = (r * sin(rad));
               y  = (r * cos(rad));
               glVertex3f(     x,      y,   5.0f);
            }
            rad = ((float) (d2) / 360.0) * (2 * 3.1415927);
            x  = (r * sin(rad));
            y  = (r * cos(rad));
            glVertex3f(     x,      y,     5.0f);
            glVertex3f(   0.0f,    0.0f,   5.0f);
         } glEnd();
         /*---(separator)----------------*/
         if (curr->type == 'f') {
            glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
            d1  = (curr->id - 0.60) * arc;
            glBegin(GL_POLYGON); {
               glVertex3f(   0.0f,    0.0f, -10.0f);
               for (i = d1; i <= d2; i += 1) {
                  rad = ((float) (i) / 360.0) * (2 * 3.1415927);
                  x  = ((r - 1) * sin(rad));
                  y  = ((r - 1) * cos(rad));
                  glVertex3f(     x,      y, -10.0f);
               }
               rad = ((float) (d2) / 360.0) * (2 * 3.1415927);
               x  = ((r - 1) * sin(rad));
               y  = ((r - 1) * cos(rad));
               glVertex3f(      x,       y, -10.0f);
               glVertex3f(   0.0f,    0.0f, -10.0f);
            } glEnd();
         }
         /*---(move to edge)-------------*/
         glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
         glRotatef( 90 - d, 0.0f, 0.0f, 1.0f);
         glTranslatef( r +  5,  0.0, 20.0);
         /*---(recursive)----------------*/
         if (curr->r > 0) {
            glPushMatrix(); {
               glTranslatef( -5.0,  0.0,   5.0);
               glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
               glPointSize(10.0);
               glBegin(GL_POINTS);
               glVertex3f( 0.0, 0.0,  0.0);
               glEnd();
               glPointSize( 1.0);
            } glPopMatrix();
         }
         /*---(output marker)------------*/
         if (curr->type == '-') {
            glColor4f (0.4f, 0.4f, 0.4f, 1.0f);
            glBegin(GL_POLYGON); {
               glVertex3f( -15.0f,   -5.0f,  80.0f);
               glVertex3f( -10.0f,    0.0f,  80.0f);
               glVertex3f( -15.0f,    5.0f,  80.0f);
               glVertex3f( -25.0f,    0.0f,  80.0f);
            } glEnd();
            glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_STRIP); {
               glVertex3f( -15.0f,   -5.0f,  80.0f);
               glVertex3f( -10.0f,    0.0f,  80.0f);
               glVertex3f( -15.0f,    5.0f,  80.0f);
               glVertex3f( -25.0f,    0.0f,  80.0f);
               glVertex3f( -15.0f,   -5.0f,  80.0f);
            } glEnd();
         } else {
            if (curr->c > 0) {
               if      (curr->c > 5) glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
               else if (curr->c > 2) glColor4f (1.0f, 1.0f, 0.2f, 1.0f);
               else                  glColor4f (0.2f, 1.0f, 0.2f, 1.0f);
               glBegin(GL_POLYGON); {
                  glVertex3f( -15.0f,   -5.0f,  80.0f);
                  glVertex3f( -10.0f,    0.0f,  80.0f);
                  glVertex3f( -15.0f,    5.0f,  80.0f);
                  glVertex3f( -25.0f,    0.0f,  80.0f);
               } glEnd();
               glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
               glBegin(GL_LINE_STRIP); {
                  glVertex3f( -15.0f,   -5.0f,  80.0f);
                  glVertex3f( -10.0f,    0.0f,  80.0f);
                  glVertex3f( -15.0f,    5.0f,  80.0f);
                  glVertex3f( -25.0f,    0.0f,  80.0f);
                  glVertex3f( -15.0f,   -5.0f,  80.0f);
               } glEnd();
            }
         }
         /*---(input marker)-------------*/
         if (curr->ins > 1) {
            glPushMatrix(); {
               glTranslatef( -5.0,  0.0,  0.0);
               if      (curr->ins > 5) glColor4f (1.0f, 0.2f, 0.2f, 1.0f);
               else if (curr->ins > 2) glColor4f (1.0f, 1.0f, 0.2f, 1.0f);
               else                    glColor4f (0.2f, 1.0f, 0.2f, 1.0f);
               glBegin(GL_POLYGON); {
                  glVertex3f(  -9.0f,    4.0f,  80.0f);
                  glVertex3f(  -5.0f,    0.0f,  80.0f);
                  glVertex3f(  -9.0f,   -4.0f,  80.0f);
                  glVertex3f( -13.0f,    0.0f,  80.0f);
               } glEnd();
               glBegin(GL_LINE_STRIP); {
                  glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
                  glVertex3f(  -9.0f,    4.0f,  80.0f);
                  glVertex3f(  -5.0f,    0.0f,  80.0f);
                  glVertex3f(  -9.0f,   -4.0f,  80.0f);
                  glVertex3f( -13.0f,    0.0f,  80.0f);
                  glVertex3f(  -9.0f,    4.0f,  80.0f);
               } glEnd();
            } glPopMatrix();
         }
         /*---(label)--------------------*/
         glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
         if (nnode < 100) {
            /*> glRotatef(-65    , 0.0f, 0.0f, 1.0f);                                 <*/
            glTranslatef(  2.0,  0.0,  0.0);
            yFONT_print(txf_sm,  10, YF_MIDCEN, curr->l);
            glTranslatef( 12.0,  0.0,  0.0);
            yFONT_print(txf_sm,  10, YF_MIDLEF, curr->s);
         } else if (nnode < 150) {
            /*> glRotatef(-65    , 0.0f, 0.0f, 1.0f);                                 <*/
            glTranslatef(  2.0,  0.0,  0.0);
            yFONT_print(txf_sm,  8, YF_MIDCEN, curr->l);
            glTranslatef( 12.0,  0.0,  0.0);
            yFONT_print(txf_sm,  8, YF_MIDLEF, curr->s);
         } else {
            glTranslatef(  2.0,  0.0,  0.0);
            yFONT_print(txf_sm,  8, YF_MIDCEN, curr->l);
            glTranslatef( 12.0,  0.0,  0.0);
            yFONT_print(txf_sm,  8, YF_MIDLEF, curr->s);
         }
      } glPopMatrix();
      curr = curr->n;
   }
   return 0;
}

char         /*-> create a texture -------------------[ leaf   [gz.640.001.00]*/ /*-[00.0000.011--]-*/ /*-[--.---.---.--]-*/
texture_create     (void)
{
   /*---(generate)-----------------------*/
   glGenFramebuffersEXT         (1, &fbo);
   glGenTextures                (1, &tex);
   glGenRenderbuffersEXT        (1, &depth);
   /*---(bind)---------------------------*/
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  fbo);
   glBindTexture                (GL_TEXTURE_2D,       tex);
   /*---(settings)-----------------------*/
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexEnvi                    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri              (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, GL_TRUE);
   /*---(copy)---------------------------*/
   glTexImage2D                 (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glFramebufferTexture2DEXT    (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
   /*---(depth)--------------------------*/
   glBindRenderbufferEXT        (GL_RENDERBUFFER_EXT, depth);
   glRenderbufferStorageEXT     (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, tex_w, tex_h);
   glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
   /*---(unbind)-------------------------*/
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> clear the texture and framebuffer --[ leaf   [gz.311.001.00]*/ /*-[00.0000.011--]-*/ /*-[--.---.---.--]-*/
texture_free       (void)
{
   DEBUG_G  printf("texture_free        () beg\n");
   /*---(generate)-----------------------*/
   glDeleteTextures                (1, &tex);
   glDeleteRenderbuffersEXT        (1, &depth);
   glDeleteFramebuffersEXT         (1, &fbo);
   /*---(complete)-----------------------*/
   DEBUG_G  printf("texture_free        () end\n");
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.B63.021.00]*/ /*-[00.0000.021--]-*/ /*-[--.---.---.--]-*/
draw_main          (void)
{
   /*---(begin)--------------------------*/
   DEBUG_G  printf("draw_main()   : begin...\n");
   /*---(locals)-------------------------*/
   float     xwidth  = tex_w  / 4.0;
   float     xheight = tex_h  / 4.0;
   /*---(create objects)-----------------*/
   DEBUG_G  printf("draw_main()   : to enter texture create...\n");
   texture_create();
   DEBUG_G  printf("draw_main()   : done with texture create...\n");
   /*> tex = texture_create  ();                                                      <* 
    *> fbo = framebuf_create ();                                                      <*/
   /*---(setup)--------------------------*/
   /*> glBindTexture   (GL_TEXTURE_2D, 0);                                            <*/
   DEBUG_G  printf("draw_main()   : basic opengl setup...\n");
   glViewport(0,  0, tex_w, tex_h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(  -xwidth,   xwidth,  -xheight,   xheight, -500.0,  500.0);
   glMatrixMode(GL_MODELVIEW);
   DEBUG_G  printf("draw_main()   : bind framebuffer...\n");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  fbo);
   /*---(draw)---------------------------*/
   DEBUG_G  printf("draw_main()   : clear...\n");
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);       /* nice medium grey            */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   DEBUG_G  printf("draw_main()   : draw_back...\n");
   draw_back  ();
   DEBUG_G  printf("draw_main()   : draw_slices...\n");
   draw_slices();
   DEBUG_G  printf("draw_main()   : draw_edges...\n");
   draw_edges ();
   DEBUG_G  printf("draw_main()   : unbind framebuffer...\n");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   DEBUG_G  printf("draw_main()   : done drawing...\n");
   /*---(mipmaps)------------------------*/
   glBindTexture   (GL_TEXTURE_2D, tex);
   /*> printf("draw_first()   : after_main...\n");                                    <*/
   /*> glBindTexture   (GL_TEXTURE_2D, tex);                                          <*/
   /* compressed rgba was only about 1/4 the size !!!! */
   /*> glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, tex_w, tex_h, 0);            <*/
   /*> glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, 0, 0, tex_w, tex_h, 0);   <*/
   DEBUG_G  printf("draw_main()   : gen mipmaps...\n");
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*---(complete)-----------------------*/
   /*> printf("draw_first()   : end...\n");                                           <*/
   DEBUG_G  printf("draw_main()   : end\n");
   return 0;
}

char         /*-> tbd --------------------------------[ shoot  [gz.440.001.00]*/ /*-[00.0000.021--]-*/ /*-[--.---.---.--]-*/
draw_init(void)
{
   glShadeModel (GL_SMOOTH);
   glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
   glClearDepth (1.0f);
   glEnable     (GL_TEXTURE_2D);    /* NEW */
   glEnable     (GL_DEPTH_TEST);
   glEnable     (GL_ALPHA_TEST);
   glEnable     (GL_BLEND);
   glAlphaFunc  (GL_GEQUAL, 0.0125);
   glBlendFunc  (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint       (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /*> glEnable     (GL_POLYGON_SMOOTH);                                              <*/
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glHint       (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glFlush      ();
   return 0;
}

char         /*-> tbd --------------------------------[ leaf   [gz.430.221.10]*/ /*-[00.0000.021--]-*/ /*-[--.---.---.--]-*/
draw_resize(uint a_w, uint a_h)
{
   if (a_h == 0) a_h = 1;
   float   xwidth  = win_w  / 2.0;
   float   xheight = win_h / 2.0;
   glViewport(0,  0, win_w, win_h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   /*> glOrtho(  -xwidth,   xwidth,  -xheight,   xheight, -500.0,  500.0);            <*/
   gluPerspective(45.0f, (GLfloat) win_w / (GLfloat) win_h, 0.1f, 10000.0f);
   glMatrixMode(GL_MODELVIEW);
   /*> glTranslatef( 0.0,  0.0, -500.0);                                              <*/
   return 0;
}




/*============================----(source-end)----============================*/
