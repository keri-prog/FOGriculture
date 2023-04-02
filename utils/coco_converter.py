import cv2
import os

# CLASSES= {
#     "Bear": 0,
#     "Brown Bear": 0,
#     "Bull": 1,
#     "Cattle": 1,
#     "Tiger": 2,
#     "Deer": 3,
#     "Elephant": 4,
#     "Fox": 5,
#     "Goat": 6,
#     "Leopard": 8,
#     "Jaguar": 8,
#     "Lion": 9,
#     "Monkey": 10,
#     "Mouse": 11,
#     "Raven": 12,
#     "Rhinoceros": 13,
#     "Snake": 14,
#     "Sparrow": 15,
#     "Squirrel": 16,
#     "Rabbit": 17,
#     "Pig": 18
# }

CLASSES= {
    "Bear": 0,
    "Brown Bear": 0,
    "Bull": 1,
    "Cattle": 1,
    "Tiger": 6,
    "Deer": 2,
    "Elephant": 3,
    "Fox": 4,
    "Goat": 5,
    "Leopard": 6,
    "Jaguar": 6,
    "Lion": 6,
    "Monkey": 7,
    "Mouse": 8,
    "Raven": 9,
    "Rhinoceros": 10,
    "Snake": 11,
    "Sparrow": 9,
    "Squirrel": 12,
    "Rabbit": 13,
    "Pig": 14,
    "Canary": 9,
    "Magpie": 9
}

folder_path = "../../FOGriculture/animal_pics"
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
                    label_txt = ''
                    
                    for line in lines:
                        xmin = float(line.strip().split(' ')[1])
                        ymin = float(line.strip().split(' ')[2])
                        xmax = float(line.strip().split(' ')[3])
                        ymax = float(line.strip().split(' ')[4])
                        
                        cx   = ((xmin + xmax)/2.0)/image_width
                        cy   = ((ymin + ymax)/2.0)/image_height
                        bw   = (xmax - xmin)/image_width
                        bh   = (ymax - ymin)/image_height
                        
                        label_txt += f'{CLASSES[folders]} {cx} {cy} {bw} {bh}\n'
                    
                    with open(label_dst + label, "w") as wf:
                        wf.write(label_txt)

