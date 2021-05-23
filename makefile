# Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
#
# @authors
#  - Joao Filipe Guiomar Artur, 2019217853
#  - Sancho Amaral Simoes, 2019217590
#
# @date 02/04/2021
#

# VARIABLES

CC	= gcc
FLAGS 	= -Wall -Wextra -pthread
PROG 	= main.exe
CODE	= code
DIRS	= $(wildcard $(CODE)/*.c) $(wildcard $(CODE)/*/*.c) $(wildcard $(CODE)/*/*/*.c) $(wildcard $(CODE)/*/*/*/*.c)
OBJDIR	= obj
OBJS 	= $(patsubst $(CODE)/%.c, $(OBJDIR)/%.o, $(DIRS))

# GENERIC

all:		${PROG}

DEPS	 = 	${patsubst $(OBJDIR)/%.o,$(OBJDIR)/%.d,${OBJS}}
-include 	${DEPS}
DEPFLAGS = 	-MMD -F ${@:.o=.d}

${PROG}:	${OBJS}
			${CC} ${FLAGS} ${OBJS} -lm -o $@

$(OBJDIR)/%.o: $(CODE)/%.c
			rsync -a --include='*/' --exclude='*' $(CODE)/ $(OBJDIR)/
			${CC} ${FLAGS} -c $< -o $@ ${DEPFLAGS}

clean:
			rm -f ${OBJS} ${PROG} ${DEPS}
			rm -rf ${OBJDIR}

####################################

