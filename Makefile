ifndef WASI_SDK_PATH
  echo No $WASI_SDK_PATH. Using /. Download the WASI SDK (https://github.com/WebAssembly/wasi-sdk) and set $$WASI_SDK_PATH
  WASI_SDK_PATH = "/"
endif

CC = "$(WASI_SDK_PATH)/bin/clang" --sysroot="$(WASI_SDK_PATH)/share/wasi-sysroot"

# Optional dependency from binaryen for smaller builds
WASM_OPT = "$(WASI_SDK_PATH)/bin/wasm-opt"
WASM_OPT_FLAGS = -Oz --zero-filled-memory --strip-producers

# Whether to build for debugging instead of release
DEBUG = 0

# Compilation flags
CFLAGS = -W -Wall -Wextra -Werror -Wno-unused -Wconversion -Wsign-conversion -MMD -MP -fno-exceptions -DNDEBUG -Oz -flto -Wdeprecated-non-prototype

# Linker flags
LDFLAGS = -Wl,-zstack-size=8192,--no-entry,--import-memory -mexec-model=reactor \
	-Wl,--initial-memory=262144,--max-memory=262144,--global-base=98304 \
	-Wl,--strip-debug,--gc-sections,--lto-O3 -Oz

OBJECTS = $(wildcard src/*.c)

ifeq ($(OS), Windows_NT)
	MKDIR = if not exist build md build
	RMDIR = rd /s /q
else
	MKDIR = mkdir -p 
	RMDIR = rm -rf
endif

all: build/cart.wasm

# Link cart.wasm from all object files and run wasm-opt
build/cart.wasm:
	$(MKDIR) build
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

ifeq (, $(shell command -v $(WASM_OPT)))
	@echo Tip: $(WASM_OPT) was not found. Install it from binaryen for smaller builds!
else
	$(WASM_OPT) $(WASM_OPT_FLAGS) $@ -o $@
endif

# Compile C sources
# build/%.o: src/%.c
# 	@$(MKDIR_BUILD)
# 	$(CC) -c $< -o $@ $(CFLAGS)


.PHONY: clean
clean:
	$(RMDIR) build

run: all
	tic80 --fs="." --cmd="load game.tic & import binary build/cart.wasm & run"

export_web: all
	tic80 --fs="." --cmd="load game.tic & import binary build/cart.wasm & export html out & save & exit" --cli
	$(MKDIR) out
	unzip out/out.zip -d out
