ECHO OFF
CLS
C:
CD\
MD xblaster
CD xblaster
ECHO INSTALLING FILES..................
COPY A:xblaster.zip
COPY A:pkunzip.exe
pkunzip -o xblaster.zip
DEL xblaster.zip
CLS
ECHO TYPE XBLASTER TO PLAY



