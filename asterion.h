/*===[[ START HDOC ]]=========================================================*/
/*===[[ HEADER ]]=============================================================*

 *   focus         : (IG) infographics
 *   niche         : (cg) call_graph
 *   application   : converge
 *   purpose       : fast, light tool to visualize program call graphs
 *
 *   base_system   : gnu/linux  (because it's powerful, universal, and hackable)
 *   lang_name     : gnu/ansi-c (because it's right, just, best, and universal)
 *   dependencies  : X11, opengl, yXINIT, yFONT
 *   size goal     : tiny (less than 1,000 slocL)
 *
 *   author        : the_heatherlys
 *   created       : 2010-11
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*---[[ DECISION : SHOULD WE BUILD IT ]]--------------------------------------*

 *   PROBLEM (pr)
 *      program flow can be very hard to grasp, especially when trying to get an
 *      overall picture and follow the flow lines in a reasonably complex set
 *      of source files
 *
 *   OBJECTIVES (o)
 *      - see the entire flow in a reasonably constrained space (all at once)
 *      - able to use resulting diagram to "tell the story" of the program
 *      - focus on internal functions, not external libraries our glibc
 *      - be able to see the function names clearly as labels
 *      - attempt to preserve the function ordering/grouping from source files
 *      - clearly call out orphan/vestigal functions (never called)
 *      - clearly call out functions that don't call other functions (leaves)
 *      - clearly show functions that call many other functions (big drivers)
 *      - use colors and textures to make lines easier to follow
 *      - only use common source analyis tools to create input (ctags, cflow)
 *      - accomodate non-linear, event-driven programs as well
 *      - keep under 1,000 lines of code, maybe 2,000 if really good ;)
 *
 *   ALTERNATIVES and CONSEQUENCES (ac)
 *   
 *      1) well-known integrated IDE
 *         - built-in tools requiring no effort to use
 *         - no ability to adapt to my style of coding
 *         - limited to fairly typical node/edge graphs
 *         - then, locked into a full tool including editor
 *         - zero learning opportunity
 *
 *      2) common scripts and graphing libraries
 *         - mostly pre-built and requiring limited effort to use
 *         - limited ability to adapt to my style of coding
 *         - must bring down and use special libraries
 *         - limited learning opportunity outside configuration
 *
 *      3) build a custom program
 *         - gotta do the coding myself from the ground up
 *         - must setup, maintain, and port it myself
 *         - huge learning opportunity and chance to press my tools harder
 *         - can be kept very tailored and small for me
 *         - great chance to experiment on various display styles
 *
 *   JUDGEMENT
 *      surprise, surpise.  we are going to go with building a custom solution
 *      as it just won't be that much code, we can press our librariies to
 *      harden them down, we want to experiment, and its a huge learning
 *      opportunity both coding and graphically
 *
 *
 *
 *
 *   information graphics are consistent visual shorthand techniques used to
 *   communicate critical, complex, or unwieldy data in unique, stylized, and
 *   simplified ways allowing knowledge to be transferred and digested quickly
 *   and clearly.  these techniques can be separated into charts, diagrams,
 *   graphs, tables, maps, and lists.
 *
 *   cgraph is a 
 *
 *
 *
 *
 *   rtree is a visualization tool for large tree structures which places them
 *   in a radial format for compact viewing and navigation.
 *
 *   variations...
 *      - vtree     : sideways, vertical tree
 *      - htree     : normal tree view
 *      - rtree     : radial tree
 *      - stree     : sunburst tree
 *      - itree     : icicle tree
 *      - ctree     : cone tree
 *      - dtree     : donut tree (two level radial)
 *
 *   rational limitations...
 *      - only handle 3 levels of tree at one time (rest are ignored)
 *      - only display the first 12 characters of names (visual consistency)
 *
 *   will not...
 *      - interactively build or edit them
 *
 *   input format...
 *      - pure and plain ascii text file
 *      - only name field or delimited by proper ascii field separator
 *      - blank lines allowed between root branches (none inside these branches)
 *      - first root branch format determines file layout (spaces, numbers, ...)
 *      - typically follows a three-space indent per level model
 *      - comments allowed if '#' in first column
 *
 *
 *
 *   there are many, stable, accepted, existing programs and utilities have
 *   been built by better programmers and are likely superior in speed, size,
 *   capability, and reliability; BUT, i would not have learned nearly as much
 *   using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the underlying idea is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get messy ;)
 *
 */
/*===((END DOC))==============================================================*/




/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YASTERION
#define YASTERION loaded

/*---(heatherly libraries)-------------------------*/
#include   <yX11.h>                    /* heatherly xlib/glx setup            */
#include   <yFONT.h>                   /* opengl texture-mapped fonts         */
#include   <ySTR.h>                    /* heatherly string handling           */
#include   <yCOLOR.h>                  /* heatherly color library             */
#include   <yURG.h>                    /* heatherly urgent handling           */

/*---(X11/opengl libraries)------------------------*/
#include   <GL/gl.h>                   /* main header for opengl              */
#include   <GL/glx.h>                  /* for xlib/opengl integration         */
#include   <GL/glu.h>
#include   <X11/extensions/shape.h>    /* xwindows shape extension            */

/*---(glibc libraries)-----------------------------*/
#include   <stdlib.h>
#include   <stdio.h>
#include   <string.h>
#include   <math.h>
#include   <sys/time.h>
#include   <unistd.h>
#include   <time.h>


/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "v0.5h"
#define VER_TXT   "built file filtering, very nice"


/*===[[ window semi-constants ]]==============================================*/
extern  char   win_title[];            /* window title                        */
extern  int    win_h;            /* full window height                  */
extern  int    win_w;             /* full window width                   */

typedef struct cNODE tNODE;
struct cNODE {
   int       id;                  /* id number                                */
   char      type;                /* type -- function or separator            */
   int       file;                /* file number                              */
   char      hint       [  5];    /* label                                    */
   char      name       [ 21];    /* short name                               */
   char      desc       [200];    /* description                              */
   int       ins;                 /* count of places that call this function  */
   int       nchild;              /* count of direct children                 */
   int       nheir;               /* count of total decendents                */
   int       color;               /* yCOLOR color identifier                  */
   char      cli;                 /* sends text to stdout                     */
   char      glx;                 /* uses open gl                             */
   char      show;                /* focus flag     -- 0=hide, 1=show         */
   char      r;                   /* recursive flag -- 0=no  , 1=yes          */
   float     lead;                /* position of lead side                    */
   float     tail;                /* position of tail side                    */
   float     arc;                 /* space on curve                           */
   tNODE    *prev;                /* pointer to parent                        */
   tNODE    *next;                /* pointer to next sibling                  */
};
extern int       nnode;
extern tNODE    *nhead;
extern tNODE    *ntail;



typedef struct cEDGE tEDGE;
struct cEDGE {
   tNODE    *beg;                 /* pointer to beginning of edge             */
   tNODE    *end;                 /* pointer to end of edge                   */
   char      lvl;                 /* level/depth of connection                */
   int       seq;                 /* sequential number at level               */
   tEDGE    *prev;                /* pointer to prev sibling                  */
   tEDGE    *next;                /* pointer to next sibling                  */
};
extern int      nedge;
extern tEDGE   *ehead;
extern tEDGE   *etail;




#define  DEG2RAD  (3.1415927 / 180.0)
#define  RAD2DEG  (180.0 / 3.1415927)
float  my_sin[3600];
float  my_cos[3600];


struct cGLOBALS {
   char     *file;
   char      format;
} my;


extern int       spaces;
extern float     single;
extern float     angle;
extern float     increment;
extern char      action;

#define   PLAY       -0.10
#define   STOP        0.0
#define   INC_SPEED   0.05
#define   MAX_SPEED   0.25

#define   MAXLINE    2000

extern char      debug_top;
extern char      debug_mem;
extern char      debug_input;
extern char      debug_graph;
#define   DEBUG_T    if (debug_top   == 'y')
#define   DEBUG_M    if (debug_mem   == 'y')
#define   DEBUG_I    if (debug_input == 'y')
#define   DEBUG_G    if (debug_graph == 'y')

extern char      file_format;
extern char      nfile;

extern float     xdist;
extern float     ydist;
extern float     zdist;

extern char      search;
extern char      focus [200];
extern uint      flen;
extern float     alpha;
extern char      edges;
extern char      mask;

/*---(fonts)-----------------------*/
/*> extern   FTGLfont *font;                                                          <*/
/*> extern   XFontStruct *xfont;                                                      <*/

int        main              (int argc, char *argv[]);
char       PROG_init         (void);
char       PROG_args         (int argc, char *argv[]);
char       PROG_begin        (void);
char       PROG_final        (void);
char       PROG_event        (void);
char       PROG_end          (void);

char       DRAW_init         (void);
char       DRAW_main         (void);
char       DRAW_back         (void);
char       DRAW_nodes        (void);
char       DRAW_edges        (void);
char       DRAW_texture      (void);
char       texture_create    (void);
char       texture_free      (void);

char       DRAW_resize       (uint, uint);
long       time_stamp        (void);

char       font_load         (void);
char       font_delete       (void);

void       str_trim          (char *a_cstring);
char       NODE_read         (void);

char       FIND_filemode        (char a_major, char a_minor);
char       FIND_hintmode        (char a_major, char a_minor);
char       FIND_searchmode      (char a_major, char a_minor);

#endif
