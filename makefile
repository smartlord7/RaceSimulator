# Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
#
# @author
#  - Joao Filipe Guiomar Artur, 2019217853
#  - Sancho Amaral Simoes, 2019217590
#
# @date 02/04/2021
#

# VARIABLES

CC		= gcc
FLAGS 	= -Wall -Wextra -pthread
PROG 	= main.exe
OBJS 	= race_simulator.o race_manager.o malfunction_manager.o team_manager.o race_config_reader.o log_generator.o ipc_manager.o process_manager.o exception_handler.o debug.o strings.o read_line.o to_float.o race_config_t.o race_team_t.o race_car_t.o

# GENERIC

all:		${PROG}

DEPS	 = 	${patsubst %.o,%.d,${OBJS}}
-include 	${DEPS}
DEPFLAGS = -MMD -F ${@:.o=.d}

${PROG}:	${OBJS}
			${CC} ${FLAGS} ${OBJS} -lm -o $@

%.o: %.c
			${CC} ${FLAGS} -c $< ${DEPFLAGS}

clean:
			rm -f ${OBJS} ${PROG} ${DEPS}

####################################

