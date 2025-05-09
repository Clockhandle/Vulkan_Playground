# Project name (must match the one in CMakeLists.txt)
PROJECT_NAME = VulkanPlayground

# Directory containing the CMakeLists.txt and project sources, relative to this Makefile
CMAKE_PROJECT_SUBDIR = .

# Name of the build directory (will be created inside CMAKE_PROJECT_SUBDIR)
BUILD_DIR_NAME = build

# Full path to the build directory
BUILD_DIR = $(CMAKE_PROJECT_SUBDIR)/$(BUILD_DIR_NAME)

# Path to the final executable
EXECUTABLE = $(BUILD_DIR)/$(PROJECT_NAME)

# Set the build type (default to Release if not specified via command line e.g., make BUILD_TYPE=Debug)
BUILD_TYPE ?= Release

.PHONY: all configure build run clean debug release rebuild shader_clean

# Default target
all: build

# Configure the project using CMake
# Creates the build directory and generates build files (e.g., Makefiles) within it.
configure:
	@echo "+++ Configuring $(PROJECT_NAME) for $(BUILD_TYPE) in $(BUILD_DIR) +++"
	@mkdir -p $(BUILD_DIR)
	@cmake -S $(CMAKE_PROJECT_SUBDIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

# Build the project using the generated build files
# This depends on the 'configure' step having been run.
build:
	@echo "+++ Building $(PROJECT_NAME) ($(BUILD_TYPE)) +++"
	@# Ensure configuration has run. A simple check, or make configure a prerequisite.
	@if [ ! -f "$(BUILD_DIR)/Makefile" ] && [ ! -f "$(BUILD_DIR)/build.ninja" ]; then \
		$(MAKE) configure BUILD_TYPE=$(BUILD_TYPE); \
	fi
	@cmake --build $(BUILD_DIR)

# Run the compiled executable
run: build
	@echo "+++ Running $(PROJECT_NAME) +++"
	@$(EXECUTABLE)

# Clean build files and shader outputs
clean: shader_clean
	@echo "+++ Cleaning build directory: $(BUILD_DIR) +++"
	@rm -rf $(BUILD_DIR)

# Clean only shader files
shader_clean:
	@echo "+++ Cleaning shaders in $(CMAKE_PROJECT_SUBDIR)/shaders/ +++"
	@rm -f $(CMAKE_PROJECT_SUBDIR)/shaders/*.spv

# Rebuild the project (clean then build all)
rebuild: clean all

# Debug mode: Clean, re-configure for Debug, then build
debug:
	$(MAKE) clean
	$(MAKE) configure BUILD_TYPE=Debug
	$(MAKE) build

# Release mode: Clean, re-configure for Release, then build
release:
	$(MAKE) clean
	$(MAKE) configure BUILD_TYPE=Release
	$(MAKE) build