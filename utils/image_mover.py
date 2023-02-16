# import shutils
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

folder_path = "/home/kushojha/FOGriculture/animal_pics"

for section in ["/test", "/train"]:
    source_folder = folder_path + section
    img_dst = source_folder + "/images"
    for folder in os.listdir(source_folder):
        if folder in CLASSES.keys():
            for image_name in os.listdir(source_folder+"/"+folder):
                if image_name[-3:] == 'jpg':
                    print(source_folder+"/"+folder+"/"+image_name, img_dst+"/"+image_name)
                    os.rename(source_folder+"/"+folder+"/"+image_name, img_dst+"/"+image_name)
                    