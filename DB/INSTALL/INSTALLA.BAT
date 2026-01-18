ECHO OFF
CLS
C:
CD\
MD db
CD db
ECHO INSTALLING FILES..................
COPY A:db.zip
COPY A:pkunzip.exe
pkunzip -o db.zip
DEL db.zip
CLS
ECHO TYPE DB TO PLAY



