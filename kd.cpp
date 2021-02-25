// Original author: Morgan McKinney 3/2020

#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

const int k = 4; // Number of dimensions
const int b = 10; // Number of elements per bucket

// Element structure
typedef struct elements {
	int key;
	int x;
	int y;
	int z;
	int w;
} element;

// Bucket structure
typedef struct leafNodes {
	vector<element> bucket;
} leafNode;

// Internal node structure
typedef struct internalNodes {
	int dimension;
	int value;
	internalNodes *leftChild, *rightChild;
	vector<leafNode> leaves;
} internalNode;

// Find minimum value of a vector
int findMin(vector<vector<leafNodes>> vector, int key) {
	int min = vector[0].leaves.key;
	for (int i = 0; i < vector.size(); i ++) { // For every point
		if (vector[i].leaves.key < min) { // Update min as needed
			min = vector[i].leaves.key;
		}
	}
	return min;
} // findMin()

// Find maximum value of a vector
int findMax(vector<vector<leafNodes>> vector, int key) {
	int max = vector[0].leaves.key;
	for (int i = 0; i < vector.size(); i++) { // For every point
		if (vector[i].leaves.key > max) { // Update max as needed
			max = vector[i].leaves.key;
		}
	}
	return max;
} // findMax()

// Update key of an element
void updateKey(element pt, int key) {
	switch(key) {
		case 0: pt.key = pt.x;
			break;
		case 1: pt.key = pt.y;
			break;
		case 2: pt.key = pt.z;
			break;
		case 3: pt.key = pt.z;
			break;
		default: cout << "Error: Invalid key in updateKey()" << endl;
	}
} // updateKey()

// Determine best dimension to discriminate on
int bestDimension(vector<elements> pts) {
	int dimension = 0;
	int maxRange = 0;
	for (int i = 0; i < k; i++) { // For every dimension
		updateKey(i);
		int min = findMin(pts, i); 
		int max = findMax(pts, i);
		range = max - min;
		if (range > maxRange) { // Update dimension with highest range as needed
			maxRange = range;
			dimension = i;
		}
	}
	return dimension;
} // bestDimension()

// Conduct Lomuto's partitioning method
// (Referenced geeksforgeeks.org/hoares-vs-lomuto-partition-scheme-quicksort/)
int lomutosPartitioningMethod(vector<elements> &pts, int low, int high) {
	int pivot = pts[high].key;
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++) {
		if (pts[i].key <= pivot) {
			i++;
			pts.swap(pts[i].key, pts[j].key);
		}
	}
	pts.swap(pts[i + 1].key, pts[high].key);
	return (i + 1);
} // lomutosPartitioningMethod()

// Conduct quick select
// (Referenced geeksforgeeks.org/quickselect-algorithm/)
int quickSelect(vector<elements> pts, int left, int right, int mid) {
	mid = pts.size()/2; // Update median
	if (mid > 0 && mid <= right - left + 1) {
		// Partition array around last element
		int index = lomutosPartitioningMethod(pts, left, right, key);
		// Position is the same as the median
		if (index - left == mid - 1) {
			return index;
		}
		// Position is more than the median
		if (index - left > mid - 1) {
			return quickSelect(pts, left, index - 1, mid);
		}
		// Position is less than the median
		return quickSelect(pts, index + 1, right, mid - index + left - 1);
	}
} // quickSelect()

// Determine best value to discriminate on
int bestValue(vector<elements> pts) {
	// Use QuickSelect with Lomuto's part. method
	int discriminantIndex = quickSelect(pts, pts.begin(), pts.end(), 0);
	int discriminant = pts[discriminantIndex].key;
	return discriminant;
} // bestValue()

// Separate data based on a discriminant
// (Referenced thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/)
// (Referenced cplusplus.com/reference/vector/vector/erase/)
vector<elements> partition(vector<elements> &pts, int split) {
	auto i = find(pts.begin(), pts.end(), split);
	int indexSplit = distance(pts.begin(), it) - 1;
	vector<elements> secondSubVector;
	int runningIndex = 0;
	for (int i = indexSplit; i < pts.size(), i++) { // Populate second sub vector
		secondSubVector[runningIndex] = pts[i]; // Copy data over
		runningIndex++;
		pts.erase(i); // Erase point from first sub array
	}
	return secondSubVector;
} // partition()

// Initialize and assign members to a new node
// (Referenced CS315 lecture 5 notes)
internalNode *createNode(int dimension, int value) {
	// Allocate memory and initialize
	internalNode newNode = (internalNode *) malloc(sizeof(internalNode));
	// Assign members
	newNode->dimension = dimension;
	newNode->value = value;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	return newNode;
} // createNode()

// Find the closest bucket where a probe would be found if it were in the tree
leafNode probeTree(internalNode *root, element target, vector<int> dimensions) {
	leafNode closestLeaf;
	for (int i = 0; i < k; i++) { // For every dimension
		updateKey(target, dimensions[i]); // Determine how to compare first dimension
		if (target.key <= root.value) { // Traverse based on tree values; ties go left
			root->leftChild;
		}
		else {
			root->rightChild;
		}
	}
	closestLeaf = root->leaves;
	return closestLeaf;
} // probeTree()

// Display bucket contents
void dispBucket(leafNode bucket) {
	for (int i = 0; i < bucket.size(); i++) {
		cout << bucket[i].x << " "; 
		cout << bucket[i].y << " ";
		cout << bucket[i].z << " ";
		cout << bucket[i].w << endl;
	}
} // dispBucket()

int main(int argc, char* argv[]) {
	// Take two int parameters: number of data points and probes	
	if (argc != 3) { // Check for correct parameters
		cout << "Usage: ./randGen.pl seed modulus | ./kd n p" << endl;
	}
	// Read parameters
	int n = atoi(argv[1]); // Number of data points
	int p = atoi(argv[2]); // Number of probes
	// Accumulate all the data in a vector
	vector<element> points;
	for (int i = 0; i < n; i++) { // For every point
		element tempElement;
		// Read standard input
		cin >> tempElement.x;
		cin >> tempElement.y;
		cin >> tempElement.z;
		cin >> tempElement.w;
		points.insert(tempElement);
	}
	// Build a k-d tree
	// Starting with the root
	internalNode *root = createNode(bestDimension(points), bestValue(points));
	// Add rest of the k-d tree for arbitrary k
	for (int i = 1; i < k; i++) {
		// Find the best dimension to discriminate on
		int dimension = bestDimension(points);
		// Find the best value to discriminate on, the median value
		int value = bestValue(points);
		internalNode *node = createNode(dimension, value);
		if (i != k-1) {
			// Separate data into two subvectors based on discriminant
			vector<elements> subvector = partition(points, value);
		}
		else { // On final dimension, add leaves
			root->leftChild.leaves = points;
			root->rightChild.leaves = subvector;
		}
	}
	// Probe tree
	for (int i = 0; i < p; i++) {
		element probe;
		cin >> probe.x >> probe.y >> probe.z >> probe.w;
		leafNode foundProbe = probeTree(points, probe);
		// Display results to user
		cout << "probe ";
		cout << probe.x << " ";
		cout << probe.y << " ";
		cout << probe.z << " ";
		cout << probe.w << " reaches bucket: " << b << " elements" << endl;
		dispBucket(foundProbe);
	}
}
