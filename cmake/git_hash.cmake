# Writes a header containing the Git hash of the source tree.
#
# Inputs (set by whoever runs this script, like function parameters):
#   GIT_HASH_SOURCE_DIR  Required. Folder to run git in (the repo root).
#   GIT_HASH_TEMPLATE    Required. Template header with a @GIT_HASH@ placeholder.
#   GIT_HASH_OUTPUT      Required. Where to write the generated header.
#   GIT_EXECUTABLE       Optional. Full path to git, set by find_package(Git). Empty if Git isn't
#                        installed, in which case the hash falls back to "c0ffee00" instead of
#                        failing the build.

# Hexspeak fallback, used when the hash can't be read (e.g. no Git, or a zip download with no .git)
set(GIT_HASH "c0ffee00")

# Only ask Git if it was found. Calling a missing git would fail the whole build.
if(GIT_EXECUTABLE)
    # Same as running `git rev-parse HEAD` in the terminal: prints the full 40-character hash
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
        WORKING_DIRECTORY "${GIT_HASH_SOURCE_DIR}"
        OUTPUT_VARIABLE git_full_hash
        RESULT_VARIABLE git_result
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    # git_result is 0 on success. It's non-zero if e.g. this isn't a git repository, in which case
    # GIT_HASH stays as the fallback.
    if(git_result EQUAL 0)
        # First 8 hex digits = 32 bits, fits in a uint32_t
        string(SUBSTRING "${git_full_hash}" 0 8 GIT_HASH)
    endif()
endif()

# Fill in @GIT_HASH@ in the template. Only rewrites the output if it changed, so an unchanged hash
# doesn't trigger a rebuild.
configure_file("${GIT_HASH_TEMPLATE}" "${GIT_HASH_OUTPUT}" @ONLY)