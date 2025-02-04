# Define the build directory
BUILD_DIR = build

# Set the build type (default to Release if not specified)
BUILD_TYPE ?= Release

# Configure the build system
configure:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..
	cmake --build $(BUILD_DIR)
	chmod +x compile.sh
	./compile.sh

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f shaders/*.spv
	chmod -x compile.sh

# Run the compiled executable
run: build
	$(BUILD_DIR)/HelloTriangle

# Debug mode
debug:
	$(MAKE) BUILD_TYPE=Debug

# Release mode
release:
	$(MAKE) BUILD_TYPE=Release



