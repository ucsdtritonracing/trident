import subprocess
from pathlib import Path
import pytest


repoRoot = Path(__file__).resolve().parents[2]
buildAppDir= repoRoot / "build"/"host"/"embedded"/"app"


expectedOutput = {"front": ["Running on host!\nFront app running!"],
                  "rear": ["Running on host!\nRear app running!"],}


def app_executable(app_name: str):
    return buildAppDir / app_name /app_name

@pytest.mark.parametrize("app_name",expectedOutput.keys())
def test_app_print(app_name):
    executablePath = app_executable(app_name)
    assert executablePath.exists(),(
        f"Executable not found at {executablePath}. "
        f"Did you run `just run {app_name}' before running the tests?"
    )

    result = subprocess.run(
        [str(executablePath)],
        capture_output=True,
        text=True,
        timeout=10,
    )
    assert result.returncode == 0, (
        f"{app_name} exited with code {result.returncode}\n"
        f"stdout:\n{result.stdout}\n"
        f"stderr:\n{result.stderr}"
    )
    for output in expectedOutput[app_name]:
        assert output in result.stdout, (
            f"Expected line {output!r} not found in {app_name} stdout:\n"
            f"{result.stdout!r}"
        )
