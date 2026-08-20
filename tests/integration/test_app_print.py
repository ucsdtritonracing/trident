import subprocess
from pathlib import Path

import pytest

repo_root = Path(__file__).resolve().parents[2]
build_app_dir = repo_root / "build" / "host" / "embedded" / "app"


expected_output = {
    "front": ["Running on host!\nFront app running!"],
    "rear": ["Running on host!\nRear app running!"],
}


def app_executable(app_name: str):
    path_guess = build_app_dir / app_name / app_name
    if path_guess.exists():
        return path_guess
    return build_app_dir / app_name / f"{app_name}.exe"


@pytest.mark.parametrize("app_name", expected_output.keys())
def test_app_print(app_name):
    executable_path = app_executable(app_name)
    assert executable_path.exists(), (
        f"Executable not found at {executable_path}. "
        f"Did you run `just build' before running the tests?"
    )

    result = subprocess.run(
        [str(executable_path)],
        capture_output=True,
        text=True,
        timeout=10,
    )
    assert result.returncode == 0, (
        f"{app_name} exited with code {result.returncode}\n"
        f"stdout:\n{result.stdout}\n"
        f"stderr:\n{result.stderr}"
    )
    for output in expected_output[app_name]:
        assert output in result.stdout, (
            f"Expected line {output!r} not found in {app_name} stdout:\n{result.stdout!r}"
        )
