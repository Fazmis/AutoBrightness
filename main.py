import numpy as np
import cv2
from native import set_brightness


def main() -> None:
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Не удалось захватить камеру")
        return

    while True:
        ret, frame = cap.read()
        cv2.imshow("Photo", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        frame_intensity_percent = round(frame.mean() / 255 * 100)
        print(frame_intensity_percent)
        brightness_level = frame_intensity_percent
        set_brightness(brightness_level)
    cap.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
