# import shutils
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
                    