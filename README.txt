Original author: Morgan McKinney 3/2020

kd.cpp: 
Creates, populates, and displays a k-d tree given the number of points and probes. Takes number of data points and probe through standard input. Features quick select and Lomuto's partitioning method.

May invoke using 'make run' which utilizes the Makefile.

Expected input: ./randGen.pl SEED MODULUS | ./kd POINTS PROBES

Expected output: Wall of text--per the inputted amount of probes--displaying 
all of the elements within the closest bucket to a probe within the tree.

tmpRun.out: 
Contains output of student designed k-d tree program. May invoke with
 'cat' command in the command prompt.

Makefile: 
Present for easy user compilation of trees.cpp.
