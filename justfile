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
    lint_status=0; \
    if [ "{{target}}" = "all" ] || [ "{{target}}" = "cpp" ]; then \
        echo "Checking C++ formatting and linting..."; \
        cmake --preset host; \
        cmake --preset stm32; \
        host_tidy_args=""; \
        if [ "$(uname -s)" = "Darwin" ]; then \
            sdkroot="$(xcrun --show-sdk-path 2>/dev/null || true)"; \
            if [ -n "$sdkroot" ]; then \
                host_tidy_args="--extra-arg=--sysroot=$sdkroot"; \
            fi; \
        fi; \
        stm32_cc=$(python3 -c "import json; db=json.load(open('build/stm32/compile_commands.json')); print(db[0]['command'].split()[0])"); \
        stm32_tidy_args="--extra-arg=--target=arm-none-eabi"; \
        tmp_includes=$(mktemp); \
        echo | "$stm32_cc" -std=gnu++20 -xc++ -E -Wp,-v - 2>&1 | awk '/search starts here/{flag=1; next} /End of search list/{flag=0} flag' | sed 's/^ *//' | sort -u > "$tmp_includes"; \
        while IFS= read -r inc_dir; do \
            [ -n "$inc_dir" ] && stm32_tidy_args="$stm32_tidy_args --extra-arg=-isystem$inc_dir"; \
        done < "$tmp_includes"; \
        rm -f "$tmp_includes"; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' -o -path './embedded/boards/compute_module/cubemx/Core' -o -path './embedded/boards/compute_module/cubemx/Drivers' \) -prune -o \( -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.c' -o -name '*.h' -o -name '*.hpp' \) -print -exec clang-format --style=file --dry-run --Werror {} + || lint_status=1; \
        echo "Linting host code with build/host compile database..."; \
        tmp_host=$(mktemp); \
        python3 tools/generators/list_compile_db_files.py "$PWD" "build/host/compile_commands.json" > "$tmp_host"; \
        while IFS= read -r file; do \
            clang-tidy -p build/host $host_tidy_args "$file" || lint_status=1; \
        done < "$tmp_host"; \
        rm -f "$tmp_host"; \
        echo "Linting STM32 code with build/stm32 compile database..."; \
        tmp_stm32=$(mktemp); \
        python3 tools/generators/list_compile_db_files.py "$PWD" "build/stm32/compile_commands.json" > "$tmp_stm32"; \
        while IFS= read -r file; do \
            clang-tidy -p build/stm32 $stm32_tidy_args "$file" || lint_status=1; \
        done < "$tmp_stm32"; \
        rm -f "$tmp_stm32"; \
    fi; \
    if [ "{{target}}" = "all" ] || [ "{{target}}" = "python" ]; then \
        echo "Checking Python formatting and linting..."; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path '*/.venv' -o -path '*/venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' \) -prune -o -name '*.py' -print -exec python3 -m ruff format --check --diff --config pyproject.toml {} + || lint_status=1; \
        find . \( -path './build' -o -path './.git' -o -path './.venv' -o -path './venv' -o -path '*/.venv' -o -path '*/venv' -o -path './node_modules' -o -path './dist' -o -path './site-packages' \) -prune -o -name '*.py' -print -exec python3 -m ruff check --config pyproject.toml {} + || lint_status=1; \
    fi; \
    exit $lint_status


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