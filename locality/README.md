# README for Assignment 3: Locality
#### Partners: John O'Donnell and Luke Watkins

###### All ideas and code are our own.

#### UArray2 has been implemented
##### Design Checklist:
* Q1: What is the abstract thing you are trying to represent?
	* The UArray2b interface represents a set of i rows and j columns that make
	up any two dimensional array where the array is sectioned into n x n blocks
	of elements based on the specified blocksize or alternatively the size of
	the data structure representing an element.
* Q2: What functions will you offer, and what are the contracts of those functions?
	
	The UArray2 interface offers eight functions:
	
	* UArray2b_new, which returns a new UArray2b struct provided a given height,
		width, element size, and blocksize
	* UArray2b_new_64k_block, which returns a new UArray2b struct provided a 
		given height, width and unit size based on a maximum block memory size
		of 64 KB
	* UArray2b_free, which frees memory on the heap allocated for the 2d array 
		given a pointer to its location
	* UArray2b_height, which returns the height of a 2d array represented by a 
		given UArray2b
	* UArray2b_width, which returns the width of a 2d array represented by a 
		given UArray2b
	* UArray2b_size, which returns the unit size of the 2d array represented by a
		given UArray2b
	* UArray2b_blocksize, which returns the blocksize of the 2d array represented
		by a given UArray2b
	* UArray2b_at, which returns a pointer to the data stored at the given (i, j)
		index in a given UArray2b
	* UArray2b_map, which applies a given function to every entry in a given 
		UArray2b on a block by block basis in row major order
* Q4: What representation will you use, and what invariants will it satisfy?
	* The UArray2b_T struct will contain private members height, width, size, and
	blocksize, along with a pointers to a UArray2 containing a pointer for each
	block with each block being a Hanson Array. These data members are 
	initialized in the UArray2b_new function. Because of this, an element of 
	the array would be accessed by first getting the pointer to the block it is
	contained in using the formula and then the offset from the address of that
	block.
	
#### A2Methods has been correctly implemented using UArray2 and UArray2b.

#### ppmtrans has been implemented
##### Design Checklist:
* Q1: What is the abstract thing you are trying to represent?
	* This part of the assignment regards implementing ppmtrans, which performs 
	simple image transformations such as image rotations at 90 degree 
	increments, vertical and horizontal flips, and transpositions. All of 
	these transformations have to be capable of using row-major, column-major 
	and block-major mapping strategies.
* Q2: What functions will you offer, and what are the contracts of those functions?
	* This program will take as input a single ppm image from either standard 
	input or a file name provided through the command line.
* Q4: What representation will you use, and what invariants will it satisfy?
	
	Take data from a ppm file and store it in a new UArray2b_T or UArray2_T, 
	depending on the desired method of transformation.
	
	* This function will require an input file stream, and a new 2d array 
		abstraction.
	* The data will be passed from the ppm file to the 2d array iteratively.
	
	Each image transformation will be defined in its own function.
	
	* These will mostly take advantage of the mapping functions established
		in previous parts of this or prior assignments, those being row-major,
		column-major and block-major order.
	* Executing these image transformations in row or column-major order 
		will require using a UArray2_T, and block-major order requires using 
		a UArray2b_T.
	* Some transformations can be done in place with the help of a 
		temporary storage variable, such as a 180 degree rotation, but others 
		will require a seperate empty 2d array of swapped height and width values.
	* Transformations that require a separate 2d array for output storage will
		store the final output in a UArray2_T, as the optimal operation for 
		printing an image file is row-major mapping.
		
	A final function to output the final version of the 2d array abstraction 
	as a ppm file to standard output.
	
	* This function will require either version of the 2d array.
	* The data will be pulled from the array either iteratively or using a 
		row-major mapping function.

## Part E: Runtime Analysis

For this portion of the assignment we were tasked with comparing the runtime of 
the different mapping methods of the 2D arrays on large images to determine if
the use of the blocked 2d array along with the blocked mapping function could 
produce a significant reduction in number of cache misses and likewise a 
sizeable speedup for performing transpose operations on images whose size meant 
that the entire image could not fit into cache. The commands we ran to test this 
were as follows, with trrhe first command constituting a large image and the 
second being an extremely large image created by using `pnmscale`.

##### First Command Run:
```
/usr/bin/time ./ppmtrans -rotate [angle] -[mapping] /csc/411/images/large/oberon.ppm
 ```
 
 Image Transpose| row-major | col-major | block-major
------------ | ------------- | ------------- | -------------
180 degree | 0.68 s | 0.68 s | 1.52 s
90 degree | 0.90 s | 0.91 s | 1.54 s 

##### Second Command Run:
```
djpeg /csc/411/images/large/winter.jpg | pnmscale 3.5 | /usr/bin/time ./ppmtrans -rotate [angle] -[mapping] 
 ```
 
 Image Transpose| row-major | col-major | block-major
------------ | ------------- | ------------- | -------------
180 degree | 13.20 s | 13.24 s | 29.44 s
90 degree | 17.74 s | 17.89 s | 29.58 s 

##### Our Expectations Coming into the Assignment:
###### Obtained from our Estimates Document
> Ranking of Expected Performance

Image Transpose| row-major | col-major | block-major
------------ | ------------- | ------------- | -------------
180 degree | 2 | 3 | 1
90 degree | 2 | 3 | 1 

The results of the testing performed did not match with our predictions from our
estimates document, as the blocked 2d array and block-major mapping of our 2d 
transformation functions performed worse than the other two mappiong methods in
both large and extremely large images. It was expected that block-major mapping 
would be able to perform better than row or column major in both cases, and 
especially in the case of an extremely large image because it would result in 
less cache misses which would require retrieving data from either slower cache
or memory. We predicted that this would be the case regardless of what transpose
was being performed. However, the results say otherwise, which means that there 
are likely large slowdowns present in our code which prevents the block-major 
mapping from being as effecient as it can be.

###### 16 manhours spent on Assignment 3: locality
