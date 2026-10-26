from pathlib import Path
import subprocess

here = here = Path(__file__).resolve()
proto_dir = here.parents[4]/"shared/protocol/schema"
generated_dir = proto_dir.parents[1]/"generated"

for item in proto_dir.iterdir():
    if item.suffix == ".proto" and item.is_file():
        subprocess.run(
        [
            "protoc",
            f"--proto_path={proto_dir}",
            f"--cpp_out={generated_dir/"cpp"}",
            f"--python_out={generated_dir/"python"}",
            str(item),
        ],
        check=True,
    )