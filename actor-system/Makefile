include ../common/Makefile.def

TARGET = actor-system$(EXE)

SOURCES = \
    src/ActorSystem/ActorSystem.cpp \
    src/ActorSystem/BoostImpl/ActorSystemBoostImpl.cpp \
    src/main.cpp

INCLUDE += -Iinclude -I/home/pi/binary/boost/include
LIB += -L/home/pi/binary/boost/lib -lboost_system
CXX_FLAGS +=

include ../common/Makefile.in
