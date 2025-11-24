import os

for home, dirs, files in os.walk('.'):
    if 'MDK-ARM' in home:
        for file in files:
            if os.path.splitext(file)[-1] != '.uvprojx' \
                and os.path.splitext(file)[-1] != '.sct' \
                and os.path.splitext(file)[-1] != '.bat':
                os.remove(home+'\\'+file)
                #print(home+file)
    elif 'GCC' in home:
        for file in files:
            if file == "Makefile":
                pass
            else:
                os.remove(home+'\\'+file)
    
