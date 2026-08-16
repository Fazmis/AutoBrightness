import time
from native import set_brightness
from camera import Camera


def main() -> None:
    camera = Camera()
    try:
        while True:
            brightness_level = camera.get_brightness()
            set_brightness(brightness_level)
            time.sleep(3)
    finally:
        camera.release()


if __name__ == '__main__':
    main()
