# Contributing

## Table of Contents

- [Contributing](#contributing)
  - [Table of Contents](#table-of-contents)
  - [Your First Code Contribution](#your-first-code-contribution)
  - [Styleguides](#styleguides)
    - [C++](#c)
    - [Python](#python)
    - [All Languages](#all-languages)
  - [C++ Naming Conventions](#c-naming-conventions)
  - [Excluded Code](#excluded-code)

## Your First Code Contribution
1. Install the following tools:
    1. Visual Studio Code
    2. ...
2. Clone this repository to your working directory
3. Open this folder in Visual Studio Code
4. Install all 

## Styleguides

This repository uses `just` to run formatting and linting checks.

### C++

Format C++ code: `just format cpp`\
Check C++ formatting and linting: `just format-check cpp`

C++ formatting uses `clang-format`, and linting uses `clang-tidy`.

### Python

Format Python code: `just format python`\
Check Python formatting and linting: `just format-check python`

Python formatting and linting uses Ruff.

### All Languages

Format all supported languages: `just format`\
Check all supported languages: `just format-check`\
Run `just format` and `just format-check` before submitting a pull request.

## C++ Naming Conventions

New C++ code should follow these naming conventions:

* Struct/class member fields: `snake_case`
* Methods/member functions: `PascalCase`
* Free functions: `lower_case`
* Local variables: `lower_case`
* Function parameters: `lower_case`
* Constants and enum values: `UPPER_CASE`

These conventions are enforced by the repository's `clang-tidy` configuration.

## Excluded Code

The following directories are excluded from formatting and linting checks:

* `Core` — CubeMX-generated startup and HAL integration code.
* `Drivers` — STM32 HAL and CMSIS vendor/third-party code.

Project-owned source code should not be excluded from formatting or linting checks.
