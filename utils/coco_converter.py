import cv2
import os

CLASSES= {
    "Bear": 0,
    "Bull": 1,
    "Cattle": 2,
    "Cheetah": 3,
    "Deer": 4,
    "Elephant": 5,
    "Fox": 6,
    "Goat": 7,
    "Horse": 8,
    "Jaguar": 9,
    "Leopard": 10,
    "Lion": 11,
    "Monkey": 12,
    "Mouse": 13,
    "Mule": 14,
    "Raven": 15,
    "Rhinoceros": 16,
    "Sheep": 17,
    "Snake": 18,
    "Sparrow": 19,
    "Squirrel": 20,
    "Tiger": 21
}

folder_path = "../FOGriculture/animal_pics"
source_folder = folder_path + "/train"

for section in ["/test", "/train"]:
    source_folder = folder_path + section
    for folders in os.listdir(source_folder):
        label_dst = source_folder + '/labels/'
        if folders in CLASSES.keys():
            label_folder = source_folder + "/" + folders + "/Label"
            
            for label in os.listdir(label_folder):
                label_path = label_folder + "/" + label
                
                with open(label_path, "r") as f:
                    
                    lines = f.readlines()
                    img = cv2.imread(source_folder + "/" + folders + "/" + label.split('.')[0] + '.jpg')
                    image_height, image_width, _ = img.shape
                    
                    xmin = float(lines[0].strip().split(' ')[1])
                    ymin = float(lines[0].strip().split(' ')[2])
                    xmax = float(lines[0].strip().split(' ')[3])
                    ymax = float(lines[0].strip().split(' ')[4])
                    
                    cx   = ((xmin + xmax)/2.0)/image_width
                    cy   = ((ymin + ymax)/2.0)/image_height
                    bw   = (xmax - xmin)/image_width
                    bh   = (ymax - ymin)/image_height
                    
                    print(label_dst + label)
                    
                    with open(label_dst + label, "w") as wf:
                        wf.write(f'{CLASSES[folders]} {cx} {cy} {bw} {bh}')

