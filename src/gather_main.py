import os

# FILE USED TO FIND ALL .jang FILES!

paths = []
amount_of_mains = 0

for i in os.listdir():
    if os.path.isdir(i):
        paths.append(i)

for i in paths:
    for dir, _, files in os.walk(i, topdown = True):
        for f in files:
            if '.jang' in f:
                if 'main' in f:
                    amount_of_mains+=1
                    if amount_of_mains > 1:
                        raise Exception('\nPython exception: More than 1 main.jang file found. Must only be one\n\n')
                    with open('main.txt','w') as file:
                        file.write(dir+'/'+f)