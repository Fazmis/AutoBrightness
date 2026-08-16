import time
from native import set_brightness


def main() -> None:
    set_brightness(100)
    time.sleep(2)
    set_brightness(0)
    time.sleep(2)
    set_brightness(50)


if __name__ == '__main__':
    main()
