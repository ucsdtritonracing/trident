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
    ./build/host/embedded/app/{{app}}/{{app}}.exe