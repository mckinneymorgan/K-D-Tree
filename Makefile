# CS315 Makefile for Program 3: K-D Trees
# Original author: Raphael Finkel 3/2020

CFLAGS = -Wall -g
CPPFLAGS = -Wall -g
GOAL = kd

NUMPOINTS = 10
SEED = 43
MODULUS = 1000
NUMPROBES = 2

# Compile and run student program, placing result in tmpRun.out
run: $(GOAL) randGen.pl
	./randGen.pl $(SEED) $(MODULUS) | \
		./$(GOAL) $(NUMPOINTS) $(NUMPROBES) > tmpRun.out
	less tmpRun.out

# Compile and run a known working program, placing result in tmpWorking.out
runWorking: workingKD randGen.pl
	./randGen.pl $(SEED) $(MODULUS) | \
		./workingKD $(NUMPOINTS) $(NUMPROBES) > tmpWorking.out
	less tmpWorking.out

# Get the randGen.pl program
randGen.pl:
	wget http://www.cs.uky.edu/~raphael/courses/CS315/prog1/randGen.pl
	chmod +x randGen.pl

# Get the workingTrees program
workingKD:
	wget http://www.cs.uky.edu/~raphael/courses/CS315/prog3/workingKD
	chmod +x workingKD

# Modify the following to zip exactly what you want to include
zipAll:
	zip toSubmit.zip $(GOAL).* Makefile README.txt tmpRun.out

.PHONY: clean
clean:
	rm -f randGen.pl $(GOAL) workingTrees tmp* toSubmit.zip
