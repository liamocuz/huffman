# Huffman Coding  

This is a repo for a lab assignment that I had in ECE368 at Purdue my Sophomore year with Professor Koh. I remember not doing so hot on it my first time, so as I was cleaning out my notebooks after graduation, the assignment fell out and so I thought I should try it again with more knowledge and experience in C. The assignment can be found in Assignment.pdf.

Upon finishing this assignment, I am pleased at how it performs. It is able to compress and decompress ASCII files of varying sizes with losslessly with speed and low memory usage (although I am sure there are optimizations that can happen). It does not follow all of the functionality that is listed out in the assignment as I just wanted it to be able to compress a file and decompress a file.

## Overview

Huffman Coding is a type of text compression algorithm that uses the occurence of each character in the text file to generate codes that can represent each character in less bits than the normal 8 that ASCII characters use. A binary tree is created based upon the counts of each ASCII character and the traversal from the root to the leaf represents the code for each character. Then using those codes, the text is compressed by substituting each character with that code.

Decompression is done by recreating the tree from a pre-order traversal that is also stored in header information. Reading through the compressed text, when a 0 is read, go left in the tree and when a 1 is read, go right in the tree. When a leaf node is encountered, print the character in that leaf node and then restart from the root and do it again until all characters have been decompressed.

Read more about it [here on Wikipedia](https://en.wikipedia.org/wiki/Huffman_coding).

## How to Use

A Makefile has been provided to compile the executable that should work for Unix-based systems. Run "make" and the "huffman" exectuable will be generated. This program will only work with text files where every character in it is an ASCII character. If not, the program will exit. Given a mode (0 for compression, 1 for decompression), an input file, and an output file, the program will compress or decompress the input file and place the modified file at the output file location.

A small python script "ascii.py" has been provided that can be used to check if a text file is pure ASCII and if it is not, it will show which lines and which characters are not ASCII.

## Performance

Huffman coding is not the most efficient nor optimal compression algorithm as much better algorithms have been created since its inception in the early 1950s. However, it still does lossless compression nicely.

These are the sizes of some files before and after compression that I have tested:

- 13 B -> 39 B
- 40 B -> 65 B
- 286 B -> 259 B
- 729 B -> 479 B
- 6.1 KB -> 3.6 KB
- 597 KB -> 329 KB
- 4.2 MB -> 2.4 MB

As you can see, for small files the compression actually produces a larger file as the header information along with the compressed text is larger than the original text. However for larger texts, the compression cuts the original size by nearly 40-50 percent.

## Reflection

Some things that I think I did well:

- I think the main compress and decompress functions are easy to follow and have plenty of comments. I like the flow of the functions overall.
- I do not read in the entire text to memory and instead compress or decompress directly from the input file to the output file
  - This keeps the memory used much lower especially for large files
  - This could cause an issue if the input file is changed during execution
- I think my memory management was very good in this project

Some things that I think I could've done better:

- Better error checking and handling
  - Bounds checking, EOF checking, NULL value checking, etc.
  - I was having issues with fgetc and gave up on checking for EOF while decompressing
  - I think my attempt to do this with ENCODE_SUCCESS/FAILURE was pretty good though
  - Maybe I should've written to stderr instead out stdout with simply saying error
- I think I could've had a better project structure with different source files. I feel that I put too much in the encode.c file and could've instead put some of those functions in a different file
- A similar thing could maybe be said for the single header file, but overall it was a small project so no file was too long
- Perhaps in the future, I could refactor this code to also work for any type of binary file instead of just ASCII

## Final Remarks

I enjoyed doing this assignment again as I always love working with C and it was cool coming back to redo a project that I did poorly on the first time. It was a good challenge and I am glad I was able to utilize different data structures to solve this problem which made it more fun.
