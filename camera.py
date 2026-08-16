import cv2
import numpy as np
from typing import Any


class Camera:
    def __init__(self, camera_index=0):
        self.camera_index = camera_index
        self.video_capture = cv2.VideoCapture(self.camera_index)
        if not self.video_capture.isOpened():
            raise RuntimeError("Ошибка захвата камеры")

    def _get_avg_color_intensity_percent(self) -> int:
        frame = self.get_frame()
        frame_intensity_percent = round(frame.mean() / 255 * 100)
        return frame_intensity_percent

    def get_frame(self) -> np.ndarray[Any, np.dtype]:
        bool_status, frame = self.video_capture.read()
        if not bool_status:
            raise RuntimeError("Ошибка захвата кадра")
        return frame

    def get_brightness(self) -> int:
        frame_intensity_percent = self._get_avg_color_intensity_percent()
        return frame_intensity_percent

    def release(self) -> None:
        self.video_capture.release()
