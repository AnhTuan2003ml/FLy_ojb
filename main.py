import cv2 # type: ignore
import argparse

from ultralytics import YOLO # type: ignore
import supervision as sv # type: ignore
import numpy as np # type: ignore

import serial
import time
s = serial.Serial('COM5',9600)

ZONE_POLYGON = np.array([
    [0, 0],
    [0.5, 0],
    [0.5, 1],
    [0, 1]
])


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="YOLOv8 live")
    parser.add_argument(
        "--webcam-resolution", 
        default=[1280, 720], 
        nargs=2, 
        type=int
    )
    args = parser.parse_args()
    return args


def main():
    args = parse_arguments()
    frame_width, frame_height = args.webcam_resolution

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, frame_width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, frame_height)

    model = YOLO("best.pt")

    box_annotator = sv.BoxAnnotator(
        thickness=2,
        text_thickness=2,
        text_scale=1
    )

    zone_polygon = (ZONE_POLYGON * np.array(args.webcam_resolution)).astype(int)
    zone = sv.PolygonZone(polygon=zone_polygon, frame_resolution_wh=tuple(args.webcam_resolution))
    zone_annotator = sv.PolygonZoneAnnotator(
        zone=zone, 
        color=sv.Color.red(),
        thickness=2,
        text_thickness=4,
        text_scale=2
    )

    while True:
        ret, frame = cap.read()

        result = model(frame, agnostic_nms=True)[0]
        detections = sv.Detections.from_yolov8(result)
        if len(detections) == 0:
            s.write(b'd')
        else:
            for _, confidence, class_id, _ in detections:
                if model.model.names[class_id]:
                # Replace "your_flying_object_class_name" with the actual class name
                    if model.model.names[class_id] == 'bird':
                        s.write(b'a') 
                        # Add logic to send data to serial port (e.g., s.write(b'a'))
                        break
                        
                    elif model.model.names[class_id] == 'drone':
                        s.write(b'b') 
                        # Add logic to send data to serial port (e.g., s.write(b'a'))
                        break
                    else:
                        s.write(b'c') 
                        # Add logic to send data to serial port (e.g., s.write(b'a'))
                        break
               
         
        labels = [
            f"{model.model.names[class_id]} {confidence:0.2f}"
            for _, confidence, class_id, _
            in detections
        ]
        frame = box_annotator.annotate(
            scene=frame, 
            detections=detections, 
            labels=labels
        )

        # zone.trigger(detections=detections)
        # frame = zone_annotator.annotate(scene=frame)      
        
        cv2.imshow("yolov8", frame)

        if (cv2.waitKey(30) == 27):
            break


if __name__ == "__main__":
    main()