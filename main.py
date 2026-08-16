import time
from native import set_brightness
from camera import Camera


def main() -> None:
    set_brightness(50)
    camera = Camera()
    while True:
        t0 = time.time()
        brightness_level = camera.get_brightness()
        t0 = time.time()
        set_brightness(brightness_level)
        time.sleep(3)


if __name__ == '__main__':
    main()
