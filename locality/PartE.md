# PartE for Assignment 3: Locality
#### Partners: John O'Donnell and Luke Watkins
###### First Command Run:
```
djpeg /csc/411/images/large/winter.jpg | pnmscale 3.5 | /usr/bin/time ./ppmtrans -rotate [angle] -[mapping] 
 ```
 
 Image Transpose| row-major | col-major | block-major
------------ | ------------- | ------------- | -------------
180 degree | 13.20 s | 13.24 s | 29.44 s
90 degree | 17.74 s | 17.89 s | 29.58 s 

###### Second Command Run:
```
/usr/bin/time ./ppmtrans -rotate [angle] -[mapping] /csc/411/images/large/oberon.ppm
 ```
 
 Image Transpose| row-major | col-major | block-major
------------ | ------------- | ------------- | -------------
180 degree | 0.68 s | 0.68 s | 1.52 s
90 degree | 0.90 s | 0.91 s | 1.54 s 
