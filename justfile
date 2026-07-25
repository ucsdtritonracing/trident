# print working directory variable
pwd := `pwd`


[script]
configure target="all":
    if [ "{{target}}" = "all" ]; then
        cmake --preset stm32;
        cmake --preset host;
    else
        cmake --preset {{target}};
    fi


[script]
compile-commands:
    # combine compile_commands.json and write to root
    uv run python tools/generators/merge_compile_commands.py \
    -b {{pwd}}/build/stm32 \
    -b {{pwd}}/build/host;


[script]
build target="all": (configure target)
    if [ "{{target}}" = "all" ]; then
        cmake --build --preset stm32;
        cmake --build --preset host;
    else
        cmake --build --preset {{target}};
    fi
    
    just compile-commands;


[script]
clean target="all":
    if [ "{{target}}" = "all" ]; then
        cmake --build --preset stm32 --target clean;
        cmake --build --preset host --target clean;
    else
        cmake --preset {{target}};
    fi


run app: (build "host")
    @printf "\n"
    ./build/host/embedded/app/{{app}}/{{app}}


[script]
format-check target="all":
    set -eu; \
    case "{{target}}" in \
        all|cpp|python) ;; \
        *) echo "unsupported target: {{target}}"; exit 1 ;; \
    esac; \
    if [ "{{target}}" = "all" ] || [ "{{target}}" = "cpp" ]; then \
        echo "Checking C++ formatting and linting..."; \
        tidy_args=""; \
        if [ "$(uname -s)" = "Darwin" ]; then \
            sdkroot="$(xcrun --show-sdk-path 2>/dev/null || true)"; \
            if [ -n "$sdkroot" ]; then \
                tidy_args="--extra-arg=--sysroot=$sdkroot"; \
            fi; \
        fi; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' -o -path './embedded/boards/compute_module/cubemx/Core' -o -path './embedded/boards/compute_module/cubemx/Drivers' \) -prune -o \( -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.c' -o -name '*.h' -o -name '*.hpp' \) -print -exec clang-format --style=file --dry-run --Werror {} +; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' -o -path './embedded/boards/compute_module/cubemx/Core' -o -path './embedded/boards/compute_module/cubemx/Drivers' \) -prune -o \( -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.c' -o -name '*.h' -o -name '*.hpp' \) -print -exec clang-tidy -p build/host $tidy_args {} +; \
    fi; \
    if [ "{{target}}" = "all" ] || [ "{{target}}" = "python" ]; then \
        echo "Checking Python formatting and linting..."; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path '*/.venv' -o -path '*/venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' \) -prune -o -name '*.py' -print -exec python3 -m ruff format --check --diff --config pyproject.toml {} +; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path '*/.venv' -o -path '*/venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' \) -prune -o -name '*.py' -print -exec python3 -m ruff check --config pyproject.toml {} +; \
    fi


[script]
format target="all":
    set -eu; \
    case "{{target}}" in \
        all|cpp|python) ;; \
        *) echo "unsupported target: {{target}}"; exit 1 ;; \
    esac; \
    if [ "{{target}}" = "all" ] || [ "{{target}}" = "cpp" ]; then \
        echo "Formatting C++ files..."; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' -o -path './embedded/boards/compute_module/cubemx/Core' -o -path './embedded/boards/compute_module/cubemx/Drivers' \) -prune -o \( -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.c' -o -name '*.h' -o -name '*.hpp' \) -print -exec clang-format --style=file -i {} +; \
    fi; \
    if [ "{{target}}" = "all" ] || [ "{{target}}" = "python" ]; then \
        echo "Formatting Python files..."; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path '*/.venv' -o -path '*/venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' \) -prune -o -name '*.py' -print -exec python3 -m ruff format --config pyproject.toml {} +; \
        echo "Applying safe Python lint fixes..."; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path '*/.venv' -o -path '*/venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' \) -prune -o -name '*.py' -print -exec python3 -m ruff check --fix --config pyproject.toml {} +; \
    fi