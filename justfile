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
    -b '{{justfile_directory()}}/build/host' \
    -b '{{justfile_directory()}}/build/stm32'


compile-commands-stm32:
    python -m tools.generators.merge_compile_commands \
    -b '{{justfile_directory()}}/build/stm32' \
    -b '{{justfile_directory()}}/build/host'


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
