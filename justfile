# print working directory variable
pwd := `pwd`

install:
    ./tools/scripts/install.sh


wipe:
    @rm -rf build
    @rm -rf .cache
    @rm -rf .venv


configure target="all":
    uv run python -m tools.cmake_target configure {{target}}


compile-commands-host:
    uv run python -m tools.generators.merge_compile_commands \
    -b {{pwd}}/build/host \
    -b {{pwd}}/build/stm32


compile-commands-stm32:
    uv run python -m tools.generators.merge_compile_commands \
    -b {{pwd}}/build/stm32 \
    -b {{pwd}}/build/host


build target="all": (configure target)
    uv run python -m tools.cmake_target build {{target}}


clean target="all":
    uv run python -m tools.cmake_target clean {{target}}


run app: (build "host")
    @printf "\n"
    ./build/host/embedded/app/{{app}}/{{app}}


format-check target = "all":
    uv run python -m tools.lint check {{target}}


format target = "all":
    uv run python -m tools.lint fix {{target}}


test: (build "host")
    tests/.venv/bin/pytest tests/integration -v
