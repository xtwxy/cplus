include ../common/Makefile.def

TARGET = event-store-api$(EXE)

SOURCES = \
    src/EventStore/EventStore.cpp \
    src/EventStore/BoostImpl/EventStoreBoostImpl.cpp \
    src/main.cpp

INCLUDE += -Iinclude -I/home/pi/binary/boost/include
LIB += #-L/home/pi/binary/boost/lib -lboost_system
CXX_FLAGS +=

include ../common/Makefile.in
