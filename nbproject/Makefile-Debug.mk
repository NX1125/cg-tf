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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/airplanemovement.o \
	${OBJECTDIR}/airstrip.o \
	${OBJECTDIR}/arena.o \
	${OBJECTDIR}/bomb.o \
	${OBJECTDIR}/cannon.o \
	${OBJECTDIR}/circle.o \
	${OBJECTDIR}/cube.o \
	${OBJECTDIR}/enemybase.o \
	${OBJECTDIR}/filepath.o \
	${OBJECTDIR}/flyingenemy.o \
	${OBJECTDIR}/game.o \
	${OBJECTDIR}/lines.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/player.o \
	${OBJECTDIR}/projectile.o \
	${OBJECTDIR}/settings.o \
	${OBJECTDIR}/shapes.o \
	${OBJECTDIR}/simplesvg.o \
	${OBJECTDIR}/stopwatch.o \
	${OBJECTDIR}/takeoff.o \
	${OBJECTDIR}/teleportable.o \
	${OBJECTDIR}/textrendering.o \
	${OBJECTDIR}/thirdpersonfollower.o \
	${OBJECTDIR}/tinyxml/tinystr.o \
	${OBJECTDIR}/tinyxml/tinyxml.o \
	${OBJECTDIR}/tinyxml/tinyxmlerror.o \
	${OBJECTDIR}/tinyxml/tinyxmlparser.o \
	${OBJECTDIR}/wfobj.o \
	${OBJECTDIR}/xmlutils.o


# C Compiler Flags
CFLAGS=-lGL -lGLU -lglut

# CC Compiler Flags
CCFLAGS=-lGL -lGLU -lglut
CXXFLAGS=-lGL -lGLU -lglut

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cg-tf

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cg-tf: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cg-tf ${OBJECTFILES} ${LDLIBSOPTIONS} -lGL -lGLU -lglut

${OBJECTDIR}/airplanemovement.o: airplanemovement.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/airplanemovement.o airplanemovement.cpp

${OBJECTDIR}/airstrip.o: airstrip.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/airstrip.o airstrip.cpp

${OBJECTDIR}/arena.o: arena.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/arena.o arena.cpp

${OBJECTDIR}/bomb.o: bomb.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bomb.o bomb.cpp

${OBJECTDIR}/cannon.o: cannon.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cannon.o cannon.cpp

${OBJECTDIR}/circle.o: circle.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/circle.o circle.cpp

${OBJECTDIR}/cube.o: cube.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cube.o cube.cpp

${OBJECTDIR}/enemybase.o: enemybase.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/enemybase.o enemybase.cpp

${OBJECTDIR}/filepath.o: filepath.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/filepath.o filepath.cpp

${OBJECTDIR}/flyingenemy.o: flyingenemy.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/flyingenemy.o flyingenemy.cpp

${OBJECTDIR}/game.o: game.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game.o game.cpp

${OBJECTDIR}/lines.o: lines.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lines.o lines.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/player.o: player.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/player.o player.cpp

${OBJECTDIR}/projectile.o: projectile.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/projectile.o projectile.cpp

${OBJECTDIR}/settings.o: settings.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/settings.o settings.cpp

${OBJECTDIR}/shapes.o: shapes.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/shapes.o shapes.cpp

${OBJECTDIR}/simplesvg.o: simplesvg.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/simplesvg.o simplesvg.cpp

${OBJECTDIR}/stopwatch.o: stopwatch.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stopwatch.o stopwatch.cpp

${OBJECTDIR}/takeoff.o: takeoff.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/takeoff.o takeoff.cpp

${OBJECTDIR}/teleportable.o: teleportable.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/teleportable.o teleportable.cpp

${OBJECTDIR}/textrendering.o: textrendering.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/textrendering.o textrendering.cpp

${OBJECTDIR}/thirdpersonfollower.o: thirdpersonfollower.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/thirdpersonfollower.o thirdpersonfollower.cpp

${OBJECTDIR}/tinyxml/tinystr.o: tinyxml/tinystr.cpp
	${MKDIR} -p ${OBJECTDIR}/tinyxml
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tinyxml/tinystr.o tinyxml/tinystr.cpp

${OBJECTDIR}/tinyxml/tinyxml.o: tinyxml/tinyxml.cpp
	${MKDIR} -p ${OBJECTDIR}/tinyxml
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tinyxml/tinyxml.o tinyxml/tinyxml.cpp

${OBJECTDIR}/tinyxml/tinyxmlerror.o: tinyxml/tinyxmlerror.cpp
	${MKDIR} -p ${OBJECTDIR}/tinyxml
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tinyxml/tinyxmlerror.o tinyxml/tinyxmlerror.cpp

${OBJECTDIR}/tinyxml/tinyxmlparser.o: tinyxml/tinyxmlparser.cpp
	${MKDIR} -p ${OBJECTDIR}/tinyxml
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tinyxml/tinyxmlparser.o tinyxml/tinyxmlparser.cpp

${OBJECTDIR}/wfobj.o: wfobj.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wfobj.o wfobj.cpp

${OBJECTDIR}/xmlutils.o: xmlutils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xmlutils.o xmlutils.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
