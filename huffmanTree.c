#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LETTERS 200


typedef struct leaf {
	char character;
	int priority;
	void* left;
	void* right;
	int size;// maybe not needed
}node;

/// <summary>
/// Initialize char* str with space ' '
/// </summary>
/// <param name="str"></param>
void initString(char* str) {
	int i;
	for (i = 0; i < MAX_LETTERS; i++) {
		str[i] = ' ';
	}
}

/// <summary>
/// Appends string input into outputArray at index
/// </summary>
/// <param name="input"></param>
/// <param name="outputArray"></param>
/// <param name="index"></param>
void appendString(char* input, char* outputArray[], int index) {
	int i = 0;
	char* output = outputArray[index];
	if (output == NULL) {
		//Allocate memory if comes empty
		output = calloc(MAX_LETTERS, sizeof(char));
	}

	if (output == NULL) {
		return;
	}

	//find the end of the line 
	while (*(output + i)) {
		i++;
	}

	//appends space ' '
	//*(output + i++) = ' ';

	//insert input into output at index i
	int j = 0;
	while (*(input + j)) {
		*(output + i) = *(input + j);
		i++;
		j++;
	}
	//appends NULL to the end of the string
	*(output + i) = NULL;

	//copy the allocated memoty back to outputArray at index
	outputArray[index] = output;
}


/// <summary>
/// Replace outputArray at index with input string at posX
/// </summary>
/// <param name="input"></param>
/// <param name="outputArray"></param>
/// <param name="index"></param>
/// <param name="posX"></param>
/// <returns></returns>
int replaceString(char* input, char* outputArray[], int index, int posX) {
	int xOffset = posX;
	int inputOffset = 0;
	int z = posX;
	char* output = outputArray[index];

	if (output == NULL) {
		//Allocate memory if empty
		output = calloc(MAX_LETTERS, sizeof(char));
		initString(output);
	}

	//return if failed to create space in memory
	if (output == NULL) {
		return;
	}

	//Remove previous NULLs 
	while (z) {
		if (*(output + z) == NULL) {
			*(output + z) = ' ';
		}
		z--;
	}


	while (*(input + inputOffset)) {
		*(output + xOffset) = *(input + inputOffset);
		xOffset++;
		inputOffset++;
	}
	*(output + xOffset) = NULL;

	outputArray[index] = output;
	return xOffset;
}


/// <summary>
/// Simply builds Tree and generate output string
/// </summary>
/// <param name="tree"></param>
/// <param name="output"></param>
/// <param name="height"></param>
/// <returns></returns>
int buildTree(node* tree, char* output[], int height) {
	node* currentNode = tree;
	static int depth = 0;
	static int maxDepth = 0;
	char str[100];

	if (currentNode == NULL) {
		return depth;
	}
	//if (((currentNode->left) == NULL) && ((currentNode->right) == NULL)) {
	//	return 0;
	//}
	if (depth == 0) {
		sprintf_s(str, 100, "(%c,%d)", ((node*)(currentNode))->character, ((node*)(currentNode))->priority);
		appendString(str, output, depth);
		depth++;
	}
	if ((currentNode->left) != NULL) {
		sprintf_s(str, 100, "(%c,%d)", ((node*)(currentNode->left))->character, ((node*)(currentNode->left))->priority);
		appendString(str, output, depth);
	}
	else {
		sprintf_s(str, 100, "%s", "_____");
		appendString(str, output, depth);
		//return 0;
	}
	if ((currentNode->right) != NULL) {
		sprintf_s(str, 100, "(%c,%d)", ((node*)(currentNode->right))->character, ((node*)(currentNode->right))->priority);
		appendString(str, output, depth);
	}
	else {
		sprintf_s(str, 100, "%s", "_____");
		appendString(str, output, depth);
		//return 0;
	}

	depth++;
	buildTree(currentNode->left, output, height);
	buildTree(currentNode->right, output, height);
	depth--;

	if (depth > maxDepth) {
		maxDepth = depth;
	}
	return maxDepth;
}

/// <summary>
/// Initialize output with NULL
/// </summary>
/// <param name="output"></param>
void initVariable(char* output[]) {

	int i;
	for (i = 0; i < MAX_LETTERS; i++) {
		output[i] = NULL;
	}
}

/// <summary>
/// Using the currentNode calculates the spaces needed for the current depth
/// optionally use prefill=1 to prefill with Spaces ' '
/// </summary>
/// <param name="currentNode"></param>
/// <param name="str"></param>
/// <param name="depth"></param>
/// <param name="height"></param>
/// <param name="prefill"></param>
void fillSpaces(node* currentNode, char* str, int depth, int height, int prefill) {
	int base = (1 << height) * 6;  //6 is the number of characters to represent the printed text (x,y)_
	// the calculaton is as follow [((2^height)*6)/(depth+1)*2]-6/2
	int spaces = (base >> depth + 1) - 6;
	int i = 0;
	int j;

	if (prefill) {
		for (i = 0; i < spaces; i++) {
			*(str + i) = ' ';
		}
	}
	sprintf_s(str + i, MAX_LETTERS - i, "(%c,%d) ", ((node*)(currentNode))->character, ((node*)(currentNode))->priority);

	for (j = 0; j < spaces; j++) {
		*(str + i + j + 6) = ' ';
	}

	*(str + i + j + 0 + 6) = NULL;

}

/// <summary>
/// Calculates the offset requied based on the depth, col and height
/// </summary>
/// <param name="depth"></param>
/// <param name="col"></param>
/// <param name="height"></param>
/// <returns></returns>
int calculateOffset(int depth, int col, int height) {
	int offset;
	int base = (1 << height) * 6;
	offset = (base >> depth + 0) - 0;
	return offset * col;
}

/// <summary>
/// Formats tree in a print friendly way, outputs the result in output
/// </summary>
/// <param name="tree"></param>
/// <param name="output"></param>
/// <param name="height"></param>
/// <param name="col"></param>
/// <param name="nxtOffset"></param>
void formatTree(node* tree, char* output[], int height, int* col, int nxtOffset) {
	node* currentNode = tree;
	static int depth = 0;
	static int maxDepth = 0;
	int leftOffset = 0;
	int rightOffset = 0;
	//static int col[MAX_LETTERS];
	char str[MAX_LETTERS];
	initString(str);

	if (currentNode == NULL) {
		return depth;
	}

	if (depth == 0) {
		fillSpaces(currentNode, str, depth, height, 1);
		nxtOffset = replaceString(str, output, depth, 0);
		nxtOffset = nxtOffset / 2;
		depth++;
	}
	if ((currentNode->left) != NULL) {
		int offset = 0;
		int offsetIndex = 0;
		int tmp = (calculateOffset(depth, 1, height) / 2);

		fillSpaces(((node*)(currentNode->left)), str, depth, height, 0);
		offset = replaceString(str, output, depth, nxtOffset - tmp);
		leftOffset = nxtOffset - tmp;
		col[depth + 0] = col[depth + 0] + 1;
	}
	else {
		return 0;
	}
	if ((currentNode->right) != NULL) {
		int offset = 0;
		int offsetIndex = 0;
		int tmp = (calculateOffset(depth, 1, height) / 2);

		fillSpaces(((node*)(currentNode->right)), str, depth, height, 0);
		offset = replaceString(str, output, depth, nxtOffset + tmp);
		rightOffset = nxtOffset + tmp;
		col[depth + 0] = col[depth + 0] + 1;
	}
	else {
		return 0;
	}

	depth++;
	formatTree(currentNode->left, output, height, col, leftOffset);
	formatTree(currentNode->right, output, height, col, rightOffset);
	depth--;

	if (depth > maxDepth) {
		maxDepth = depth;
	}
	return maxDepth;
}

/// <summary>
/// Iterates through tree and finds the maximum height
/// </summary>
/// <param name="tree"></param>
/// <returns></returns>
int calculateHeight(node* tree) {
	node* currentNode = tree;
	static int depth = 0;
	static int height = 0;

	if (currentNode == NULL) {
		return depth;
	}

	depth++;
	calculateHeight(currentNode->left);
	calculateHeight(currentNode->right);
	depth--;

	if (depth > height) {
		height = depth;
	}
	return height;
}

/// <summary>
/// prints the tree in a friendly way
/// </summary>
/// <param name="tree"></param>
void printHuffmanTree(node* tree) {
	int height = 0;
	int i = 0;
	char* output[MAX_LETTERS];
	initVariable(output);
	int col[MAX_LETTERS] = { 0 };

	height = calculateHeight(tree);

	formatTree(tree, output, height, col, 0);
	while (output[i] != NULL) {
		printf("%s\r\n", output[i]);
		i++;
	}
	printf("depth = %d\r\n", height);
}

/// <summary>
/// Swaps nodes
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
void swap(node* a, node* b) {
	int temp = a->priority;
	char tempchar = a->character;
	node* tempLeft = a->left;
	node* tempRight = a->right;

	a->priority = b->priority;
	b->priority = temp;
	a->character = b->character;
	b->character = tempchar;
	a->left = b->left;
	b->left = tempLeft;
	a->right = b->right;
	b->right = tempRight;

	return;
}

/// <summary>
/// Sorts ht based from min to max priority
/// </summary>
/// <param name="ht"></param>
/// <param name="n"></param>
void leafOddEvenSort(node* ht[], int size)
{
	char isSorted = 0; // Initially array is unsorted

	while (!isSorted)
	{
		isSorted = 1;

		// Perform Bubble sort on odd indexed element
		for (int i = 1; i <= size - 2; i = i + 2)
		{
			if (ht[i]->priority > ht[i + 1]->priority)
			{
				swap(ht[i], ht[i + 1]);
				isSorted = 0;
			}
		}

		// Perform Bubble sort on even indexed element
		for (int i = 0; i <= size - 2; i = i + 2)
		{
			if (ht[i]->priority > ht[i + 1]->priority)
			{
				swap(ht[i], ht[i + 1]);
				isSorted = 0;
			}
		}
	}

	return;
}

/// <summary>
/// Finds value in array Data, returns 1 if found 0 otherwise
/// </summary>
/// <param name="value"></param>
/// <param name="arrayData"></param>
/// <returns></returns>
int findInArray(char value, char* arrayData) {
	int found = 0;

	while (*arrayData) {
		if (value == *arrayData) {
			found = 1;
			break;
		}
		arrayData++;
	}

	return found;
}

/// <summary>
/// Helper function to create array of unique letters and qty count
/// of message parameter
/// </summary>
/// <param name="message"></param>
/// <param name="letters"></param>
/// <param name="qty"></param>
/// <returns></returns>
int findUniqueLetters(char* message, char* letters, char* qty) {
	char* ptr = message;
	char* ptr2 = message;
	int i = 0;

	while (*ptr) {
		if (!findInArray(*ptr, letters)) {
			letters[i] = *ptr;
			qty[i] = 1;
			ptr2 = ptr;
			while (*ptr2) {
				if (letters[i] == *(ptr2 + 1))
					qty[i] = qty[i] + 1;

				ptr2++;
			}
			i++;
		}
		ptr++;
	}
	return i;
}


/// <summary>
/// Helper function to print the letters and their priorities
/// </summary>
/// <param name="letters"></param>
/// <param name="qty"></param>
void printUniqueLetters(char* letters, char* qty) {
	char* ptr = letters;
	int i = 0;
	//printf("%s\r\n", letters);
	while (*ptr) {
		printf("%c = %d\r\n", letters[i], qty[i]);
		i++;
		ptr++;
	}
}

/// <summary>
/// Creates a node based on the input params
/// </summary>
/// <param name="character"></param>
/// <param name="priority"></param>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns></returns>
node* newNode(char character, int priority, node* left, node* right) {
	node* ret = (node*)calloc(1, sizeof(node));

	if (ret == NULL) {
		return NULL;
	}

	ret->character = character;
	ret->priority = priority;
	ret->left = left;
	ret->right = right;


	return ret;
}

/// <summary>
/// Removes node from the node array and returns the deleted node
/// </summary>
/// <param name="index"></param>
/// <param name="leafArray"></param>
/// <param name="newSize"></param>
/// <returns></returns>
node* removeFromArray(int index, node* leafArray[], int* newSize) {

	int size = *newSize;
	node* removedItem = NULL;

	node* temp[MAX_LETTERS];

	int i;
	for (i = index; i < size; i++) {
		temp[index + i] = leafArray[index + i + 1];
	}

	//Makes room for the new value
	//memcpy(temp + index + 0, leafArray + index + 1, sizeof(node) * (size - index - 1));

	//Copies the data from index zero to the index needed
	if (index > 0) {
		//memcpy(temp, leafArray, (sizeof(node) * index) - 1);
		for (i = 0; i < index - 1; i++) {
			temp[i] = leafArray[i];
		}
	}

	//if (removedItem != NULL) {
		//pass back the item removed
	removedItem = leafArray[index];
	//memcpy(removedItem, leafArray[index], sizeof(node));

//}

//Copies all back to the pointer
//memcpy(leafArray, temp, sizeof(node) * (size));
	for (i = 0; i < size; i++) {
		leafArray[i] = temp[i];
	}

	//free(temp);

	(*newSize)--;
	return removedItem;
}

/// <summary>
/// Inserts value node into the nodearray and returns the new size array
/// </summary>
/// <param name="index"></param>
/// <param name="value"></param>
/// <param name="leafArray"></param>
/// <param name="arraySize"></param>
/// <returns></returns>
int insertIntoArray(int index, node* value, node* leafArray[], int arraySize) {
	int size = index > arraySize ? (index - arraySize) + arraySize : arraySize;
	int i;
	node* temp[MAX_LETTERS];// = calloc(MAX_LETTERS, sizeof(node));

	//if (temp == NULL) {
	//	return -1;
	//}

	//Makes room for the new value
	//memcpy(temp + index + 1, leafArray + index, sizeof(node) * (size - index));
	for (i = index; i < size - index; i++) {
		temp[index + i + 1] = leafArray[index + i];
	}

	//Copies the data from index zero to the index needed
	if (index > 0) {
		//memcpy(temp, leafArray, sizeof(node) * index);
		for (i = 0; i < index; i++) {
			temp[i] = leafArray[i];
		}
	}

	if (temp != NULL) {
		//Adds the value at the index
		//*(temp + index) = *value;
		temp[index] = value;



	}

	//Copies all back to the pointer
	memcpy(leafArray, temp, sizeof(node) * (size + 1));

	for (i = 0; i < size + 1; i++) {
		leafArray[i] = temp[i];
	}

	//free(temp);

	return size + 1;
}

/// <summary>
/// Builds the Huffma tree and returns the root
/// </summary>
/// <param name="ht"></param>
/// <param name="size"></param>
/// <returns></returns>
node* buildHuffmanTree(node* ht[], int size) {
	node* a = NULL;
	node* b = NULL;
	node* c = NULL;
	while (size != 1) {
		leafOddEvenSort(ht, size);

		a = removeFromArray(0, ht, &size);
		b = removeFromArray(0, ht, &size);

		c = newNode('@', a->priority + b->priority, a, b);
		size = insertIntoArray(0, c, ht, size);
	}

	return ht[0];

}

/// <summary>
/// Creates nodes based on the letters and the priorities
/// </summary>
/// <param name="letters"></param>
/// <param name="qty"></param>
/// <param name="size"></param>
/// <returns></returns>
node* createNodes(char* letters, char* qty, int size) {
	int i;
	//node** ht = (node**)malloc(size * sizeof(node*));
	node* tree = NULL;
	node* ht[MAX_LETTERS];

	if (ht == NULL) {
		return NULL;
	}
	for (i = 0; i < size; i++) {
		ht[i] = newNode(letters[i], qty[i], NULL, NULL);
	}

	buildHuffmanTree(ht,size);

	tree = ht[0];

	return tree;
}

int main() {
	node* tree = NULL;
	char  message[] = "this is an example of a huffman tree"; 
	char* letters   = calloc(MAX_LETTERS, sizeof(char));
	char* qty       = calloc(MAX_LETTERS, sizeof(char));
	int	  size;

	size = findUniqueLetters(message, letters, qty);
	printUniqueLetters(letters, qty);

	tree = createNodes(letters, qty, size);

	printHuffmanTree(tree);
	//todo: need to free all nodes created

	free(tree);

	return 0;
}