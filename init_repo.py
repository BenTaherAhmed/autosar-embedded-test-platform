from pathlib import Path

dirs = [
    "docs/diagrams",
    "requirements",
    "test_specs/unit",
    "test_specs/integration",
    "platform/target",
    "platform/host",
    "platform/config",
    "src/mcal_mock",
    "src/utils",
    "tests/unit_c",
    "tests/integration_py",
    "tests/assets",
    "tools/traceability",
    "tools/reporting",
    "tools/lint",
    "reports",
    "ci/github/workflows"
]

files = {
    "README.md": """# AUTOSAR Embedded Test Platform

Embedded AUTOSAR-oriented test platform with traceability and CI.
""",
    "requirements/requirements.yml": """requirements:
  - id: REQ_SPI_001
    title: SPI initialization shall configure bus parameters
    asil: B
    type: functional
""",
    ".gitignore": """build/
*.o
*.exe
__pycache__/
""",
    "CMakeLists.txt": "cmake_minimum_required(VERSION 3.20)\nproject(autosar_test_platform C)\n",
}

for d in dirs:
    Path(d).mkdir(parents=True, exist_ok=True)
    Path(d, ".gitkeep").touch()

for f, content in files.items():
    path = Path(f)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content)

print("Repository structure created successfully.")
