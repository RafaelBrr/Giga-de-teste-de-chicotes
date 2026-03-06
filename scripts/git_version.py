Import("env")
import subprocess

GIT_PATH = r"C:/Program Files/Git/mingw64/bin/git.exe"  # Adjust this path if Git is installed elsewhere

def get_git_branch():
    try:
        branch = subprocess.check_output(
            [GIT_PATH, "rev-parse", "--abbrev-ref", "HEAD"]
        ).decode().strip()
        return branch
    except:
        return "unknown"

branch = get_git_branch()

print("Git branch:", branch)

env.Append(
    CPPDEFINES=[
        ("GIT_BRANCH", '\\"{}\\"'.format(branch))
    ]
)