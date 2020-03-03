Partners: John O'Donnell and Luke Watkins

All ideas and code are our own.

UArray2 has been implemented
Design Checklist:
1. What is the abstract thing you are trying to represent?
	The UArray2b interface represents a set of i rows and j columns that make
	up any two dimensional array where the array is sectioned into n x n blocks
	of elements based on the specified blocksize or alternatively the size of
	the data structure representing an element.
2. What functions will you offer, and what are the contracts of those functions?
	The UArray2 interface offers eight functions:
	
	UArray2b_new, which returns a new UArray2b struct provided a given height,
		width, element size, and blocksize
	UArray2b_new_64k_block, which returns a new UArray2b struct provided a 
		given height, width and unit size based on a maximum block memory size
		of 64 KB
	UArray2b_free, which frees memory on the heap allocated for the 2d array 
		given a pointer to its location
	UArray2b_height, which returns the height of a 2d array represented by a 
		given UArray2b
	UArray2b_width, which returns the width of a 2d array represented by a 
		given UArray2b
	UArray2b_size, which returns the unit size of the 2d array represented by a
		given UArray2b
	UArray2b_blocksize, which returns the blocksize of the 2d array represented
		by a given UArray2b
	UArray2b_at, which returns a pointer to the data stored at the given (i, j)
		index in a given UArray2b
	UArray2b_map, which applies a given function to every entry in a given 
		UArray2b on a block by block basis in row major order
4. What representation will you use, and what invariants will it satisfy?
	The UArray2b_T struct will contain private members height, width, size, and
	blocksize, along with a pointers to a UArray2 containing a pointer for each
	block with each block being a Hanson Array. These data members are 
	initialized in the UArray2b_new function. Because of this, an element of 
	the array would be accessed by first getting the pointer to the block it is
	contained in using the formula and then the offset from the address of that
	block.
	
A2Methods has been correctly implemented using UArray2 and UArray2b.

ppmtrans has been implemented
Design Checklist:
1. What is the abstract thing you are trying to represent?
	This part of the assignment regards implementing ppmtrans, which performs 
	simple image transformations such as image rotations at 90 degree 
	increments, vertical and horizontal flips, and transpositions. All of 
	these transformations have to be capable of using row-major, column-major 
	and block-major mapping strategies.
2. What functions will you offer, and what are the contracts of those functions?
	This program will take as input a single ppm image from either standard 
	input or a file name provided through the command line.
4. What representation will you use, and what invariants will it satisfy?
	Take data from a ppm file and store it in a new UArray2b_T or UArray2_T, 
	depending on the desired method of transformation.
		-This function will require an input file stream, and a new 2d array 
		abstraction.
		-The data will be passed from the ppm file to the 2d array iteratively.
	Each image transformation will be defined in its own function.
		-These will mostly take advantage of the mapping functions established
		in previous parts of this or prior assignments, those being row-major,
		column-major and block-major order.
		-Executing these image transformations in row or column-major order 
		will require using a UArray2_T, and block-major order requires using 
		a UArray2b_T.
		-Some transformations can be done in place with the help of a 
		temporary storage variable, such as a 180 degree rotation, but others 
		will require a seperate empty 2d array of swapped height and width values.
		-Transformations that require a separate 2d array for output storage will
		store the final output in a UArray2_T, as the optimal operation for 
		printing an image file is row-major mapping.
	A final function to output the final version of the 2d array abstraction 
	as a ppm file to standard output.
		-This function will require either version of the 2d array.
		-The data will be pulled from the array either iteratively or using a 
		row-major mapping function.

16 manhours spent on Assignment 3: locality