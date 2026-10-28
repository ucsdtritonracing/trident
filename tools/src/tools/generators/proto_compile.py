import subprocess
from pathlib import Path
from tools.core import REPO_ROOT
proto_dir = REPO_ROOT / "shared/protocol/schema"
generated_dir = REPO_ROOT / "shared/generated"

for item in proto_dir.iterdir():
    if item.suffix == ".proto" and item.is_file():
        subprocess.run(
            [
                "protoc",
                f"--proto_path={proto_dir}",
                f"--cpp_out={generated_dir / 'cpp'}",
                f"--python_out={generated_dir / 'python/src/python_protocol'}",
                str(item),
            ],
            check=True,
        )
