
#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = asterion
# additional standard and outside libraries
LIB_STD    = -lX11 -lGL -lGLU -lm -lXext
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lyX11_debug -lyFONT_debug -lySTR_debug
# directory for production code, no trailing slash
INST_DIR   = /usr/local/bin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#



#*============================----beg-of-source---============================*#

#BASE    = converge

# libraries
#    shape          -lXext            <X11/extentions/shape.h>

#*---(standard variables)-------------*#
#COMP    = gcc -c -std=gnu89 -g -pg -Wall -Wextra
#INCS    = 
#LINK    = gcc 
#LIBS    = -lyX11 -lX11 -lGL -lGLU -lm -lXext
#COPY    = cp -f
#CLEAN   = rm -f
#ECHO    = @echo


#${BASE}            : ${BASE}.o
#	${LINK}  -o ${BASE}   ${BASE}.o -lyFONT ${LIBS}

#${BASE}.o          : ${BASE}.h ${BASE}.c
#	${COMP}  ${INCS} ${BASE}.c

#bigclean           :
#	${CLEAN} .*.swp

#clean              :
#	${ECHO}  cleaning out object, backup, and temp files
#	${CLEAN} *.o
#	${CLEAN} *~
#	${CLEAN} temp*
#	${CLEAN} ${BASE}

#install            : ${BASE}
#	${ECHO}  installing in b_nvdo
#	${COPY}  ${BASE}  /usr/local/bin
#	chmod    0755     /usr/local/bin/${BASE}
#	_inst --bin -v call_graph
