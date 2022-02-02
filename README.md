# Huffman Coding  

This is a repo for a lab assignment that I had in ECE368 at Purdue my Sophomore year with Professor Koh. I remember not doing so hot on it my first time, so as I was cleaning out my notebooks after graduation, the assignment fell out and so I thought I should try it again with more knowledge and experience in C. The assignment can be found in Assignment.pdf.

Upon finishing this assignment, I am pleased at how it performs. It is able to compress and decompress ASCII files of varying sizes with losslessly with speed and low memory usage (although I am sure there are optimizations that can happen). It does not follow all of the functionality that is listed out in the assignment as I just wanted it to be able to compress a file and decompress a file. Given a mode (0 for compression, 1 for decompression), an input file, and an output file, the programm will attempt to compress or decompress the input file and place the modified file at the output file location.

## Overview

Huffman Coding is a type of text compression algorithm that uses the occurence of each character in the text file to generate codes that can represent each character in less bits than the normal 8 that ASCII characters use. A binary tree is created based upon the counts of each ASCII character and the traversal from the root to the leaf represents the code for each character. Then using those codes, the text is compressed by substituting each character with that code.

Decompression is done by recreating the tree from a pre-order traversal that is also stored in header information. Reading through the compressed text, when a 0 is read, go left in the tree and when a 1 is read, go right in the tree. When a leaf node is encountered, print the character in that leaf node and then restart from the root and do it again until all characters have been decompressed.

Read more about it [here on Wikipedia](https://en.wikipedia.org/wiki/Huffman_coding).

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

- Better bounds checking, EOF checking, and NULL value checking
  - I was having issues with fgetc and gave up of checking for EOF while decompressing
  - I think my attempt to do this with ENCODE_SUCCESS/FAILURE was pretty good though
- I think I could've had a better structure with different source files. I feel that I put too much in the encode.c file and could've instead put some of those functions in a different file
- A similar thing could maybe be said for the single header file, but overall it was a small project so no file was too long
- Perhaps in the future, I could refactor this code to also work for UTF-8 characters instead of just ASCII

## Final Remark

I enjoyed doing this assignment again as I always love working with C. It was a good challenge and I was able to a couple different data structures to solve this problem which made it more fun.
