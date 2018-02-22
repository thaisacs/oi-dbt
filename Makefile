# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/napoli/Documents/OpenISA/oi-dbt/oi-dbt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/napoli/Documents/OpenISA/oi-dbt/oi-dbt

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip

.PHONY : install/strip/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local

.PHONY : install/local/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/napoli/Documents/OpenISA/oi-dbt/oi-dbt/CMakeFiles /home/napoli/Documents/OpenISA/oi-dbt/oi-dbt/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/napoli/Documents/OpenISA/oi-dbt/oi-dbt/CMakeFiles 0
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
# Target rules for targets named dbt

# Build rule for target.
dbt: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 dbt
.PHONY : dbt

# fast build rule for target.
dbt/fast:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/build
.PHONY : dbt/fast

#=============================================================================
# Target rules for targets named oi-dbt

# Build rule for target.
oi-dbt: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 oi-dbt
.PHONY : oi-dbt

# fast build rule for target.
oi-dbt/fast:
	$(MAKE) -f CMakeFiles/oi-dbt.dir/build.make CMakeFiles/oi-dbt.dir/build
.PHONY : oi-dbt/fast

#=============================================================================
# Target rules for targets named RFT

# Build rule for target.
RFT: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 RFT
.PHONY : RFT

# fast build rule for target.
RFT/fast:
	$(MAKE) -f RFT/CMakeFiles/RFT.dir/build.make RFT/CMakeFiles/RFT.dir/build
.PHONY : RFT/fast

#=============================================================================
# Target rules for targets named arglib

# Build rule for target.
arglib: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 arglib
.PHONY : arglib

# fast build rule for target.
arglib/fast:
	$(MAKE) -f arglib/CMakeFiles/arglib.dir/build.make arglib/CMakeFiles/arglib.dir/build
.PHONY : arglib/fast

IREmitter.o: IREmitter.cpp.o

.PHONY : IREmitter.o

# target to build an object file
IREmitter.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IREmitter.cpp.o
.PHONY : IREmitter.cpp.o

IREmitter.i: IREmitter.cpp.i

.PHONY : IREmitter.i

# target to preprocess a source file
IREmitter.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IREmitter.cpp.i
.PHONY : IREmitter.cpp.i

IREmitter.s: IREmitter.cpp.s

.PHONY : IREmitter.s

# target to generate assembly for a file
IREmitter.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IREmitter.cpp.s
.PHONY : IREmitter.cpp.s

IROpt.o: IROpt.cpp.o

.PHONY : IROpt.o

# target to build an object file
IROpt.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IROpt.cpp.o
.PHONY : IROpt.cpp.o

IROpt.i: IROpt.cpp.i

.PHONY : IROpt.i

# target to preprocess a source file
IROpt.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IROpt.cpp.i
.PHONY : IROpt.cpp.i

IROpt.s: IROpt.cpp.s

.PHONY : IROpt.s

# target to generate assembly for a file
IROpt.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IROpt.cpp.s
.PHONY : IROpt.cpp.s

IRUtils.o: IRUtils.cpp.o

.PHONY : IRUtils.o

# target to build an object file
IRUtils.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IRUtils.cpp.o
.PHONY : IRUtils.cpp.o

IRUtils.i: IRUtils.cpp.i

.PHONY : IRUtils.i

# target to preprocess a source file
IRUtils.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IRUtils.cpp.i
.PHONY : IRUtils.cpp.i

IRUtils.s: IRUtils.cpp.s

.PHONY : IRUtils.s

# target to generate assembly for a file
IRUtils.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/IRUtils.cpp.s
.PHONY : IRUtils.cpp.s

interpreter.o: interpreter.cpp.o

.PHONY : interpreter.o

# target to build an object file
interpreter.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/interpreter.cpp.o
.PHONY : interpreter.cpp.o

interpreter.i: interpreter.cpp.i

.PHONY : interpreter.i

# target to preprocess a source file
interpreter.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/interpreter.cpp.i
.PHONY : interpreter.cpp.i

interpreter.s: interpreter.cpp.s

.PHONY : interpreter.s

# target to generate assembly for a file
interpreter.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/interpreter.cpp.s
.PHONY : interpreter.cpp.s

machine.o: machine.cpp.o

.PHONY : machine.o

# target to build an object file
machine.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/machine.cpp.o
.PHONY : machine.cpp.o

machine.i: machine.cpp.i

.PHONY : machine.i

# target to preprocess a source file
machine.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/machine.cpp.i
.PHONY : machine.cpp.i

machine.s: machine.cpp.s

.PHONY : machine.s

# target to generate assembly for a file
machine.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/machine.cpp.s
.PHONY : machine.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/oi-dbt.dir/build.make CMakeFiles/oi-dbt.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/oi-dbt.dir/build.make CMakeFiles/oi-dbt.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/oi-dbt.dir/build.make CMakeFiles/oi-dbt.dir/main.cpp.s
.PHONY : main.cpp.s

manager.o: manager.cpp.o

.PHONY : manager.o

# target to build an object file
manager.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/manager.cpp.o
.PHONY : manager.cpp.o

manager.i: manager.cpp.i

.PHONY : manager.i

# target to preprocess a source file
manager.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/manager.cpp.i
.PHONY : manager.cpp.i

manager.s: manager.cpp.s

.PHONY : manager.s

# target to generate assembly for a file
manager.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/manager.cpp.s
.PHONY : manager.cpp.s

syscall.o: syscall.cpp.o

.PHONY : syscall.o

# target to build an object file
syscall.cpp.o:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/syscall.cpp.o
.PHONY : syscall.cpp.o

syscall.i: syscall.cpp.i

.PHONY : syscall.i

# target to preprocess a source file
syscall.cpp.i:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/syscall.cpp.i
.PHONY : syscall.cpp.i

syscall.s: syscall.cpp.s

.PHONY : syscall.s

# target to generate assembly for a file
syscall.cpp.s:
	$(MAKE) -f CMakeFiles/dbt.dir/build.make CMakeFiles/dbt.dir/syscall.cpp.s
.PHONY : syscall.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... dbt"
	@echo "... install/strip"
	@echo "... install"
	@echo "... install/local"
	@echo "... oi-dbt"
	@echo "... list_install_components"
	@echo "... RFT"
	@echo "... arglib"
	@echo "... IREmitter.o"
	@echo "... IREmitter.i"
	@echo "... IREmitter.s"
	@echo "... IROpt.o"
	@echo "... IROpt.i"
	@echo "... IROpt.s"
	@echo "... IRUtils.o"
	@echo "... IRUtils.i"
	@echo "... IRUtils.s"
	@echo "... interpreter.o"
	@echo "... interpreter.i"
	@echo "... interpreter.s"
	@echo "... machine.o"
	@echo "... machine.i"
	@echo "... machine.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... manager.o"
	@echo "... manager.i"
	@echo "... manager.s"
	@echo "... syscall.o"
	@echo "... syscall.i"
	@echo "... syscall.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
