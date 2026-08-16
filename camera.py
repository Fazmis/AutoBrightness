import time

import cv2
import numpy as np
from typing import Any


class Camera:
    def __init__(self, camera_index=0):
        self.camera_index = camera_index

    def _get_avg_color_intensity_percent(self) -> int:
        frame = self.get_frame()
        frame_intensity_percent = round(frame.mean() / 255 * 100)
        return frame_intensity_percent

    def get_frame(self) -> np.ndarray[Any, np.dtype]:
        video_capture = cv2.VideoCapture(self.camera_index, cv2.CAP_DSHOW)
        if not video_capture.isOpened():
            raise RuntimeError("Ошибка захвата камеры")
        try:
            time.sleep(0.25)
            bool_status, frame = video_capture.read()
            if not bool_status:
                raise RuntimeError("Ошибка захвата кадра")
            return frame
        finally:
            video_capture.release()

    def get_brightness(self) -> int:
        MAX_INTENSITY = 70
        MIN_INTENSITY = 0
        INTENSITY_OFFSET = 5

        MAX_BRIGHTNESS = 100
        MIN_BRIGHTNESS = 10
        # Получение интенсивности
        frame_intensity_percent = self._get_avg_color_intensity_percent()
        # Поправка интенсивности
        frame_intensity_percent += INTENSITY_OFFSET
        # Приведение интенсивности к диапазону
        frame_intensity_percent = min(MAX_INTENSITY, max(MIN_INTENSITY, frame_intensity_percent))
        # Пересчет интенсивности в уровень яркости
        brightness_level = round(frame_intensity_percent / MAX_INTENSITY * 100)
        # Приведение уровня яркости к диапазону
        brightness_level = min(MAX_BRIGHTNESS, max(MIN_BRIGHTNESS, brightness_level))
        return brightness_level
