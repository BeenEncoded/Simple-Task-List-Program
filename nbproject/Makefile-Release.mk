#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/common.o \
	${OBJECTDIR}/date_class.o \
	${OBJECTDIR}/filesystem.o \
	${OBJECTDIR}/iofunctions.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/scroll_display.o \
	${OBJECTDIR}/task_class.o \
	${OBJECTDIR}/task_list_menu.o \
	${OBJECTDIR}/tasklist_menu_date_view.o \
	${OBJECTDIR}/tasklist_menu_globals.o \
	${OBJECTDIR}/tasklist_menu_list_view.o \
	${OBJECTDIR}/tasklist_menu_sorting.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lncurses -lboost_system -lboost_filesystem

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tasks

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tasks: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tasks ${OBJECTFILES} ${LDLIBSOPTIONS} -s

${OBJECTDIR}/common.o: common.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common.o common.cpp

${OBJECTDIR}/date_class.o: date_class.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/date_class.o date_class.cpp

${OBJECTDIR}/filesystem.o: filesystem.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/filesystem.o filesystem.cpp

${OBJECTDIR}/iofunctions.o: iofunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/iofunctions.o iofunctions.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/scroll_display.o: scroll_display.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/scroll_display.o scroll_display.cpp

${OBJECTDIR}/task_class.o: task_class.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/task_class.o task_class.cpp

${OBJECTDIR}/task_list_menu.o: task_list_menu.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/task_list_menu.o task_list_menu.cpp

${OBJECTDIR}/tasklist_menu_date_view.o: tasklist_menu_date_view.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tasklist_menu_date_view.o tasklist_menu_date_view.cpp

${OBJECTDIR}/tasklist_menu_globals.o: tasklist_menu_globals.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tasklist_menu_globals.o tasklist_menu_globals.cpp

${OBJECTDIR}/tasklist_menu_list_view.o: tasklist_menu_list_view.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tasklist_menu_list_view.o tasklist_menu_list_view.cpp

${OBJECTDIR}/tasklist_menu_sorting.o: tasklist_menu_sorting.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -s -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tasklist_menu_sorting.o tasklist_menu_sorting.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tasks

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
