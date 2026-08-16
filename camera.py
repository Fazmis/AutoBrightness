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
        video_capture = cv2.VideoCapture(self.camera_index)
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
        frame_intensity_percent = self._get_avg_color_intensity_percent()
        return frame_intensity_percent
