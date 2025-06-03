PROJECT_NAME = VulkanPlayground

CMAKE_PROJECT_SUBDIR = .

BUILD_DIR_NAME = build

BUILD_DIR = $(CMAKE_PROJECT_SUBDIR)/$(BUILD_DIR_NAME)

EXECUTABLE = $(BUILD_DIR)/$(PROJECT_NAME)

BUILD_TYPE ?= Release

.PHONY: all configure build run clean debug release rebuild shader_clean

all: build

# Configure the project using CMake
configure:
	@echo "+++ Configuring $(PROJECT_NAME) for $(BUILD_TYPE) in $(BUILD_DIR) +++"
	@mkdir -p $(BUILD_DIR)
	@cmake -S $(CMAKE_PROJECT_SUBDIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build:
	@echo "+++ Building $(PROJECT_NAME) ($(BUILD_TYPE)) +++"
	@# Ensure configuration has run.
	@if [ ! -f "$(BUILD_DIR)/Makefile" ] && [ ! -f "$(BUILD_DIR)/build.ninja" ]; then \
		$(MAKE) configure BUILD_TYPE=$(BUILD_TYPE); \
	fi
	@cmake --build $(BUILD_DIR)

run: build
	@echo "+++ Running $(PROJECT_NAME) +++"
	@$(EXECUTABLE)

clean: shader_clean # explicitly clean shaders
	@echo "+++ Cleaning CMake build directory: $(BUILD_DIR) +++"
	@rm -rf $(BUILD_DIR)

shader_clean:
	@echo "+++ Cleaning shaders in $(CMAKE_PROJECT_SUBDIR)/shaders/ +++"
	@rm -f $(CMAKE_PROJECT_SUBDIR)/shaders/*.spv

rebuild: clean all

debug:
	$(MAKE) clean
	$(MAKE) configure BUILD_TYPE=Debug
	$(MAKE) build

release:
	$(MAKE) clean
	$(MAKE) configure BUILD_TYPE=Release
	$(MAKE) build