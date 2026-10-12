# print working directory variable
pwd := `pwd`

install:
    ./tools/scripts/install.sh


wipe:
    rm -rf build
    rm -rf .cache
    rm -rf .venv
    rm -rf */.pytest_cache


configure target="all":
    python -m tools.linter.cmake_target configure {{target}}


compile-commands-host:
    python -m tools.generators.merge_compile_commands \
    -b {{pwd}}/build/host \
    -b {{pwd}}/build/stm32


compile-commands-stm32:
    python -m tools.generators.merge_compile_commands \
    -b {{pwd}}/build/stm32 \
    -b {{pwd}}/build/host


build target="all": (configure target)
    python -m tools.linter.cmake_target build {{target}}


clean target="all":
    python -m tools.linter.cmake_target clean {{target}}


run app: (build "host")
    @printf "\n"
    ./build/host/embedded/app/{{app}}/{{app}}


format-check target = "all":
    python -m tools.linter.lint check {{target}}


format target = "all":
    python -m tools.linter.lint fix {{target}}


test: (build "host")
    pytest tests/integration -v
