import cv2
import torch
import serial 

BAUD_RATE = 115200
SERIAL_PORT = '/dev/ttyUSB0'
CLASSES = {
    0: "Bear",
    1: "Cattle",
    2: "Deer",
    3: "Elephant",
    4: "Fox",
    5: "Goat",
    6: "Big_Cat",
    7: "Monkey",
    8: "Mouse",
    9: "Bird",
    10: "Rhinoceros",
    11: "Snake",
    12: "Squirrel",
    13: "Rabbits",
    14: "Pig"
}

model = torch.hub.load('ultralytics/yolov5', 'custom', path="/home/kushojha/FOGriculture/model/weights/best.pt")
# vid = cv2.VideoCapture("../../../Downloads/mixkit-two-elephants-on-the-banks-of-a-river-3660-medium.mp4")
vid = cv2.VideoCapture(0)
ser = serial.Serial(port = SERIAL_PORT, baudrate=BAUD_RATE)

def bbox2rect(model, img_data):
        results = model(cv2.cvtColor(img_data, cv2.COLOR_BGR2RGB), size = 640)
        labels = results.xyxyn[0][:, -1].cpu().numpy()
        n = len(labels)
        cord = results.xyxyn[0][:, :-1].cpu().numpy()
        x_shape, y_shape = img_data.shape[1], img_data.shape[0]
        # print(cord, labels)
        rects = []
        for i in range(n):
            row = cord[i]
            conf = row[4]
            # If score is less than 0.4 we avoid making a prediction.
            if conf < 0.4: 
                continue
            x1 = int(row[0]*x_shape)
            y1 = int(row[1]*y_shape)
            x2 = int(row[2]*x_shape)
            y2 = int(row[3]*y_shape)
            rects.append((x1,y1,x2,y2, conf))
        return rects, labels

while(True):
    
    ret, frame = vid.read()
    
    rects, detections = bbox2rect(model, frame)
    for (x1, y1, x2, y2, conf), label in zip(rects, detections):
        conf_label = str(round(conf, 2))+" "+ CLASSES[int(label)]
        cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)
        cv2.putText(frame, conf_label,(x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)
        ser.write(bytes(CLASSES[int(label)], "utf-8")) 
    
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()
cv2.destroyAllWindows()
ser.close()