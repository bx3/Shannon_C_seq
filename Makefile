# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bx/Shannon_C_seq

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bx/Shannon_C_seq

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bx/Shannon_C_seq/CMakeFiles /home/bx/Shannon_C_seq/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bx/Shannon_C_seq/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Shannon_RNASeq_Cpp

# Build rule for target.
Shannon_RNASeq_Cpp: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Shannon_RNASeq_Cpp
.PHONY : Shannon_RNASeq_Cpp

# fast build rule for target.
Shannon_RNASeq_Cpp/fast:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/build
.PHONY : Shannon_RNASeq_Cpp/fast

Contig_graph_handler.o: Contig_graph_handler.cpp.o
.PHONY : Contig_graph_handler.o

# target to build an object file
Contig_graph_handler.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Contig_graph_handler.cpp.o
.PHONY : Contig_graph_handler.cpp.o

Contig_graph_handler.i: Contig_graph_handler.cpp.i
.PHONY : Contig_graph_handler.i

# target to preprocess a source file
Contig_graph_handler.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Contig_graph_handler.cpp.i
.PHONY : Contig_graph_handler.cpp.i

Contig_graph_handler.s: Contig_graph_handler.cpp.s
.PHONY : Contig_graph_handler.s

# target to generate assembly for a file
Contig_graph_handler.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Contig_graph_handler.cpp.s
.PHONY : Contig_graph_handler.cpp.s

Contig_handler.o: Contig_handler.cpp.o
.PHONY : Contig_handler.o

# target to build an object file
Contig_handler.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Contig_handler.cpp.o
.PHONY : Contig_handler.cpp.o

Contig_handler.i: Contig_handler.cpp.i
.PHONY : Contig_handler.i

# target to preprocess a source file
Contig_handler.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Contig_handler.cpp.i
.PHONY : Contig_handler.cpp.i

Contig_handler.s: Contig_handler.cpp.s
.PHONY : Contig_handler.s

# target to generate assembly for a file
Contig_handler.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Contig_handler.cpp.s
.PHONY : Contig_handler.cpp.s

Kmer_handler.o: Kmer_handler.cpp.o
.PHONY : Kmer_handler.o

# target to build an object file
Kmer_handler.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Kmer_handler.cpp.o
.PHONY : Kmer_handler.cpp.o

Kmer_handler.i: Kmer_handler.cpp.i
.PHONY : Kmer_handler.i

# target to preprocess a source file
Kmer_handler.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Kmer_handler.cpp.i
.PHONY : Kmer_handler.cpp.i

Kmer_handler.s: Kmer_handler.cpp.s
.PHONY : Kmer_handler.s

# target to generate assembly for a file
Kmer_handler.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/Kmer_handler.cpp.s
.PHONY : Kmer_handler.cpp.s

basic_log.o: basic_log.cpp.o
.PHONY : basic_log.o

# target to build an object file
basic_log.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/basic_log.cpp.o
.PHONY : basic_log.cpp.o

basic_log.i: basic_log.cpp.i
.PHONY : basic_log.i

# target to preprocess a source file
basic_log.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/basic_log.cpp.i
.PHONY : basic_log.cpp.i

basic_log.s: basic_log.cpp.s
.PHONY : basic_log.s

# target to generate assembly for a file
basic_log.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/basic_log.cpp.s
.PHONY : basic_log.cpp.s

encoding.o: encoding.cpp.o
.PHONY : encoding.o

# target to build an object file
encoding.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/encoding.cpp.o
.PHONY : encoding.cpp.o

encoding.i: encoding.cpp.i
.PHONY : encoding.i

# target to preprocess a source file
encoding.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/encoding.cpp.i
.PHONY : encoding.cpp.i

encoding.s: encoding.cpp.s
.PHONY : encoding.s

# target to generate assembly for a file
encoding.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/encoding.cpp.s
.PHONY : encoding.cpp.s

log.o: log.cpp.o
.PHONY : log.o

# target to build an object file
log.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/log.cpp.o
.PHONY : log.cpp.o

log.i: log.cpp.i
.PHONY : log.i

# target to preprocess a source file
log.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/log.cpp.i
.PHONY : log.cpp.i

log.s: log.cpp.s
.PHONY : log.s

# target to generate assembly for a file
log.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/log.cpp.s
.PHONY : log.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/Shannon_RNASeq_Cpp.dir/build.make CMakeFiles/Shannon_RNASeq_Cpp.dir/main.cpp.s
.PHONY : main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Shannon_RNASeq_Cpp"
	@echo "... Contig_graph_handler.o"
	@echo "... Contig_graph_handler.i"
	@echo "... Contig_graph_handler.s"
	@echo "... Contig_handler.o"
	@echo "... Contig_handler.i"
	@echo "... Contig_handler.s"
	@echo "... Kmer_handler.o"
	@echo "... Kmer_handler.i"
	@echo "... Kmer_handler.s"
	@echo "... basic_log.o"
	@echo "... basic_log.i"
	@echo "... basic_log.s"
	@echo "... encoding.o"
	@echo "... encoding.i"
	@echo "... encoding.s"
	@echo "... log.o"
	@echo "... log.i"
	@echo "... log.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

