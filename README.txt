DATE :- 6/22/24

1. HTTP FOTA code implmented refer from HTTP OTA code and assest tracker.
2. pending writing in flash and jumping. 

date :- 6/25/24 
http response fail problem :- in url we were taking the ip in parts and using %d 2-3 times. that creating the issue . so instead of this we use %s  and took the whole IP together. with this problem solve and fota start download again .

DATE :- 6/26/24
SPI Flash got  garbage data from somewhere. first we clear it using bulk erase function then store it download data again . with this
now able to store data in SPI flash memory .

1. file size more than 100 kb will cause problem the problem i face today.
while reading the file   the length i want to read and the length we receive in acknowledgement(connect <length>).
is difference this problem comes in 2g modem due file length is greater than 100kb.
SPI m garbage data write ho raha hai . dont know why .



