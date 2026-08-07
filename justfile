# print working directory variable
pwd := `pwd`


wipe:
    @rm -rf build
    @rm -rf .cache
    @rm -rf .venv


[script]
configure target="all":
    uv run python tools/cmake_target.py configure {{target}}


compile-commands:
    # combine compile_commands.json and write to root
    uv run python tools/generators/merge_compile_commands.py \
    -b {{pwd}}/build/stm32 \
    -b {{pwd}}/build/host


build target="all": (configure target)
    uv run python tools/cmake_target.py build {{target}}
    just compile-commands


clean target="all":
    uv run python tools/cmake_target.py clean {{target}}


run app: (build "host")
    @printf "\n"
    ./build/host/embedded/app/{{app}}/{{app}}

format-check target = "all":
    uv run python tools/lint.py check {{target}}

format target = "all":
    uv run python tools/lint.py fix {{target}}


test: (build "host")
    tests/.venv/bin/pytest tests/integration -v
