import subprocess

from tools.core import REPO_ROOT

proto_dir = REPO_ROOT / "shared/protocol/schema"
cpp_dir = REPO_ROOT / "build/host/shared/generated/cpp"
python_dir = REPO_ROOT / "shared/generated/python/src/python_protocol"
for item in proto_dir.iterdir():
    if item.suffix == ".proto" and item.is_file():
        subprocess.run(
            [
                "protoc",
                f"--proto_path={proto_dir}",
                f"--cpp_out={cpp_dir}",
                f"--python_out={python_dir}",
                str(item),
            ],
            check=True,
        )
