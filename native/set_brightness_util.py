from pathlib import Path
import subprocess

BRIGHTNESS_EXE = Path(__file__).parent / "bin" / "brightness.exe"


def set_brightness(brightness_level: int) -> int:
    result = subprocess.run([BRIGHTNESS_EXE, str(brightness_level)])
    return result.returncode
