#define _CRT_SECURE_NO_WARNINGS


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#define VECTOR_INITIAL_CAPACITY 10
#define MAX_WORD_LENGTH 100


typedef char* String;

typedef struct{
	String word;
	int length;
} Word;

typedef struct{
	int size;		// slots used so far
	int capacity;	// total available slots
	Word *data;		// array of integers we're storing
} Vector;


//void mallocForString(String *pString, String value) {
//	*pString = malloc(strlen(value));
//	memcpy(*pString, value, strlen(value) + 1);
//}

void vectorInit(Vector *vector) {
	// initialize size and capacity
	vector->size = 0;
	vector->capacity = VECTOR_INITIAL_CAPACITY;

	// allocate memory for vector->data
	vector->data = malloc(sizeof(Word) * vector->capacity);
}

void vectorDoubleCapacityIfFull(Vector *vector) {
	if (vector->size >= vector->capacity) {
		// double vector->capacity and resize the allocated memory accordingly
		vector->capacity *= 2;
		vector->data = realloc(vector->data, sizeof(Word) * vector->capacity);
	}
}

void vectorHalfCapacityIfNotUsed(Vector *vector) {
	if (vector->capacity / vector->size >= 2) {
		// half vector->capacity and resize the allocated memory accordingly
		vector->capacity /= 2;
		vector->data = realloc(vector->data, sizeof(Word) * vector->capacity);
	}
}

void vectorAppend(Vector *vector, String value) {
	// make sure there's room to expand into
	vectorDoubleCapacityIfFull(vector);

	// append the value and increment vector->size
	vector->data[vector->size].length = (strlen(value) + 1);
	vector->data[vector->size].word = malloc(vector->data[vector->size].length);
	memcpy(vector->data[vector->size].word, value, vector->data[vector->size].length);
	//mallocForString(&vector->data[vector->size].word, value);
	vector->size++;
}

void vectorSet(Vector *vector, int index, String value) {
	// zero fill the vector up to the desired index
	while (index >= vector->size) {
		vectorAppend(vector, "");
	}

	free(vector->data[index].word);

	// set the value at the desired index
	vector->data[index].length = (strlen(value) + 1);
	vector->data[index].word = malloc(vector->data[index].length);
	memcpy(vector->data[index].word, value, vector->data[index].length);
}

Word vectorGet(Vector *vector, int index) {
	if (index >= vector->size || index < 0) {
		printf("Index %d is out of bounds for vector of size %d\n", index, vector->size);
		exit(1);
	}
	return vector->data[index];
}

void vectorInsert(Vector *vector, int index, String value) {
	// make sure there's room to expand into
	vectorDoubleCapacityIfFull(vector);

	for (int i = vector->size; i > index; i--) {
		vectorSet(vector, i, vector->data[i - 1].word);
	}

	vectorSet(vector, index, value);
}

void vectorRemove(Vector *vector, int index) {

	for (int i = index; i < (vector->size - 1); i++) {
		vectorSet(vector, i, vector->data[i + 1].word);
	}
	vector->size--;

	free(vector->data[vector->size].word);
	vectorHalfCapacityIfNotUsed(vector);
}



void storeWordsFromFile(String filename, Vector *pVector)
{
	FILE *file;
	String word;
	//int vectorSize;
	//pVector->size
	//pVector.size = 0;
	if (!(file = fopen(filename, "r")))						//Open file
	{
		printf("Error opening %s\n", filename);
		return;
	}

	//fscanf(file, "%d //Amount of words\n", &vectorSize);	//Read number of words and allocate memory
	//pVector->data = malloc((vectorSize)* sizeof(Word));
	//pVector->size = (vectorSize);							//First element contains size of array

	word = malloc(MAX_WORD_LENGTH);							//Temporary memory
	for (int i = 0; fscanf(file, "%s\n", word) != EOF; i++)	//Read word into temporary memory
	{
		vectorAppend(pVector, word);
		//pVector->data[i].length = (strlen(word) + 1);				//Save word length +1 to have room for '\0' character
		//pVector->data[i].word = malloc(pVector->data[i].length);
		//memcpy(pVector->data[i].word, word, pVector->data[i].length);		//Copy the word from temporary memory
	}

	free(word);
	fclose(file);
	//return pVector;
}


//Find word and return word position
int findWord(String word, Vector *pVector){
	for (int i = 1; i < pVector->size; i++){
		//Check if strings match with memcmp
		if (memcmp(word, pVector->data[i].word, pVector->data[i].length) == 0){
			return i; // i = position
		}
	}
	return 0;
}


int deleteWord(int index, Vector *vector){
	//Check if out of bounds
	if (index < 0 || index >= vector->size){
		printf("Index %d is out of bounds for vector of size %d\n", index, vector->size);
		return 0;
	}

	vectorRemove(vector, index);
	return 1;
}


int addWord(String word, int index, Vector *vector){
	/*int distance = 1,
		amountOfWords = (*words)[0].size - position,
		originalSize = (*words)[0].size;*/

	//Check if out of bounds
	if (index < 0 || index >= vector->size){
		printf("Index %d is out of bounds for vector of size %d\n", index, vector->size);
		return 0;
	}

	vectorInsert(vector, index, word);

	return 1;
}




int main()
{
	Vector vector;
	vectorInit(&vector);
	storeWordsFromFile("Ordlista.txt", &vector);

	vectorSet(&vector, 25, "-hej-");
	vectorSet(&vector, 89, "Coca-Cola");

	vectorRemove(&vector, 87);


	vectorInsert(&vector, 5, "--test--");
	vectorInsert(&vector, 10, "--test--");
	vectorInsert(&vector, 16, "--test--");

	deleteWord(86, &vector);
	deleteWord(findWord("Akrobat", &vector), &vector);
	deleteWord(findWord("Abstinens", &vector), &vector);

	printf("findWord returned: %d for word \"Brasa\".\n\n", findWord("Brasa", &vector));

	vectorRemove(&vector, 5);

	addWord("--test addWord--", 5, &vector);

	for (int i = 0; i < vector.size; i++)
	{
		printf("%s\n", vector.data[i].word);
	}

	Word a = vectorGet(&vector, 29);
	printf("\nvectorGet returned: %s", a.word);
}