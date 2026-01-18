ECHO OFF
CLS
C:
CD\
MD db
CD db
ECHO INSTALLING FILES..................
COPY B:db.zip
COPY B:pkunzip.exe
pkunzip -o db.zip
DEL db.zip
CLS
ECHO TYPE DB TO PLAY



