import os

# FILE USED TO FIND 'main.jang' FILE!
# THIS IS THE ONLY PYTHON AUTOMATION FOR THE Jang PROGRAMMING LANGUAGE
# gather_main.py AUTOMATES THE STRUGGLE OF NEEDING TO WRITE
# CONTINUOUS LINES OF C TO A. FIND A DIRECTORY WITH .jang FILES IN IT
# AND B. FIND THE 'main.jang' FILE
# THIS WILL NOT, WHATSOEVER, INTERVIEN WITH THE RUNTIME/SPEED
# OF THE Jang PROGRAMMING LANGUAGE
# USAGE IN file_reader.c TO OPEN 'main.txt' WITH INFORMATION OF EITHER:
# A. directory_path/main.jang, MEANING FILE 'main.jang' WAS FOUND IN 'directory_path', OR
# B. err, MEANING THERE WAS NO DIRECTORY THAT STORED A FILE 'main.jang'
paths = [i for i in os.listdir() if os.path.isdir(i)]
amount_of_mains = 0

for i in paths:
    for dir, _, files in os.walk(i, topdown = True):
        files = [dir+'/'+d for d in files if '.jang' in d and 'main' in d]
        if len(files) > 0:
            amount_of_mains += 1
            with open('main.txt','w') as file:
                file.write(files[0])
                file.close()

if amount_of_mains < 1:
    with open('main.txt','w') as file:
        file.write('err')
        file.close()