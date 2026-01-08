import re
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    print("Missing dependency: pyyaml (pip install pyyaml)")
    sys.exit(2)

REQ_PATTERN = re.compile(r"\bREQ:\s*(REQ_[A-Z0-9_]+)\b")
TEST_EXTS = {".c", ".h", ".py"}

def load_requirements(req_file):
    data = yaml.safe_load(req_file.read_text(encoding="utf-8"))
    return {r["id"] for r in data.get("requirements", [])}

def scan_tests(tests_dir):
    mapping = {}
    for f in tests_dir.rglob("*"):
        if f.is_file() and f.suffix in TEST_EXTS:
            content = f.read_text(encoding="utf-8", errors="ignore")
            mapping[f] = set(REQ_PATTERN.findall(content))
    return mapping

def main():
    root = Path(__file__).resolve().parents[2]
    req_file = root / "requirements" / "requirements.yml"
    tests_dir = root / "tests"
    report_dir = root / "reports"
    report_dir.mkdir(exist_ok=True)

    requirements = load_requirements(req_file)
    test_map = scan_tests(tests_dir)

    covered = set().union(*test_map.values())
    uncovered = requirements - covered
    tests_without_req = [f for f, r in test_map.items() if not r]

    report = report_dir / "traceability_report.md"
    with report.open("w", encoding="utf-8") as r:
        r.write("# Traceability Report\n\n")
        for req in sorted(requirements):
            linked = [str(f.relative_to(root)) for f, s in test_map.items() if req in s]
            r.write(f"- **{req}** → {', '.join(linked) if linked else '❌ NOT COVERED'}\n")

        r.write("\n## Tests without REQ tag\n")
        if tests_without_req:
            for f in tests_without_req:
                r.write(f"- {f.relative_to(root)}\n")
        else:
            r.write("- ✅ None\n")

        r.write("\n## Uncovered requirements\n")
        if uncovered:
            for req in uncovered:
                r.write(f"- {req}\n")
        else:
            r.write("- ✅ None\n")

    print(f"Traceability report generated: {report}")

    if uncovered or tests_without_req:
        print("TRACEABILITY CHECK FAILED")
        return 1

    print("TRACEABILITY CHECK PASSED")
    return 0

if __name__ == "__main__":
    sys.exit(main())
