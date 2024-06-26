DATE :- 6/22/24

1. HTTP FOTA code implmented refer from HTTP OTA code and assest tracker.
2. pending writing in flash and jumping. 

date :- 6/25/24 
http response fail problem :- in url we were taking the ip in parts and using %d 2-3 times. that creating the issue . so instead of this we use %s  and took the whole IP together. with this problem solve and fota start download again .
