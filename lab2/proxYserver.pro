# Use GNU compilers - not clang
QMAKE_CXX	= g++
QMAKE_CC	= gcc

QMAKE_CXXFLAGS	+= -g

# Qt components required. Server does not use GUI.
QT		+= core
QT		+= network
QT		-= gui

TARGET		= proxYserver

# Use C++11 standard
CONFIG		+= console c++11
CONFIG		-= app_bundle

# Files to include while building
SOURCES		+= Communicator.cpp Handler.cpp	main.cpp RemoteCommunicator.cpp

HEADERS		+= Communicator.h Handler.h RemoteCommunicator.h
