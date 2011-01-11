#-*- mode: makefile; tab-width: 4; -*-
#
include(../../qmake.inc)
#
TEMPLATE = lib
CONFIG += staticlib

#
SOURCES  = 	BaseCompiler.cpp \
			Compiler.cpp \
			Compiler_helpers.cpp \
			Compiler_ops.cpp \
			Compiler_object_match.cpp \
			Preprocessor.cpp \
			NATCompiler.cpp \
			OSConfigurator.cpp \
			PolicyCompiler.cpp \
			PolicyCompiler_srvre_functions.cpp \
			RoutingCompiler.cpp

HEADERS  = 	BaseCompiler.h \
			Compiler.h \
			Preprocessor.h \
			NATCompiler.h \
			OSConfigurator.h \
			PolicyCompiler.h \
			RuleProcessor.h \
			RoutingCompiler.h \
			exceptions.h


TARGET = fwcompiler
# target.path = "$$prefix/lib"

# no need to install headers in fortress
#headers.files = $$HEADERS
#headers.path  = "$$prefix/include/fwb-4/fwcompiler"
#INSTALLS += headers

# and no need to install .a library
INSTALLS -= target
