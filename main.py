import time
from native import set_brightness
from camera import Camera


def main() -> None:
    set_brightness(50)
    camera = Camera()
    while True:
        brightness_level = camera.get_brightness()
        if not 0 <= brightness_level <= 100:
            raise ValueError("Получено недопустимое значение яркости")
        set_brightness(brightness_level)
        time.sleep(3)


if __name__ == '__main__':
    main()
