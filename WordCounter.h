// WordCounter.h
// TODO: Complete all the code that is marked as TODO in the file
// TODO: add class documentation

#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <string>
#include <iostream>	// TODO: may use for debugging; remove before submission
using namespace std;

class WordCounter {
public:
	WordCounter(int cap);
	// Constructor

	~WordCounter();
	// Destructor

	// TODO: complete Big 4

	int addWord(string word);
	// If the word does not exist, add word to hash table and set the word count
	// to 1. If the word does exist, increments the count of the given word in
	// the hash table. Returns the updated word count.

	int getWordCount(string word);
	// Returns the count of the specified word. If the word has not been
	// encountered before, returns 0.

	void removeWord(string word);
	// Removes the word record from the hash table.

	int getCapacity() const;
	// Returns the capacity of the hash table.

	int getUniqueWordCount() const;
	// Returns the number of unique words encountered (i.e. entries in the
	// hash table).

	int getTotalWordCount() const;
	// Returns the total number of words encountered, including duplicates.

   bool empty();
	// Returns true if no words have been added to the hash table; otherwise,
	// returns false.

	void display(); // display me before submission

private:
	struct Bucket {
		string word;
		int count;
		Bucket * next;

		Bucket(string w, int c);
	};
	Bucket ** buckets;
	int capacity;
	int totalCount;
	int uniqueCount;

	int getIndex(string word);
	// Returns the index based on hash code of the word.
};

WordCounter::Bucket::Bucket(string w, int c) {
   word = w;
   count = c;
	next = nullptr;
}

WordCounter::WordCounter(int cap) {
	capacity = cap;
	totalCount = 0;
	uniqueCount = 0;
	buckets = new Bucket*[capacity];

	for(int i = 0; i < capacity; ++i) {
		buckets[i] = nullptr;
	}
}

WordCounter::~WordCounter() {
	// TODO
}

int WordCounter::addWord(string word) {
	int index = getIndex(word);
	Bucket* currBucket = buckets[index];
	Bucket* prev = nullptr;
	bool bucketFound = false;
	while(currBucket != nullptr && !bucketFound) {
		if(currBucket->word == word) {
			currBucket->count = currBucket->count+1;
			bucketFound = true;
		}
		prev = currBucket;
		currBucket = currBucket->next;
	}
	if(currBucket == nullptr && !bucketFound) {
		++uniqueCount;
		currBucket = new Bucket(word, 1);
		if(prev == nullptr){
			buckets[index] = currBucket;
		}
		else{
			prev->next  = currBucket;
		}
	}
	++totalCount;
}

// int WordCounter::addWord(string word) {
// 	int index = getIndex(word);
// 	Bucket* currBucket = buckets[index];
// 	Bucket* prev = nullptr;
// 	while(currBucket != nullptr) {
// 		prev = currBucket;
// 		currBucket = currBucket->next;
// 	}
// 	if(currBucket == nullptr) {
// 		++uniqueCount;
// 		currBucket = new Bucket(word, 1);
// 		if(prev == nullptr){
// 			buckets[index] = currBucket;
// 		}
// 		else{
// 			prev->next  = currBucket;
// 		}
// 	}
// 	else {
// 		currBucket->word = word;
// 		currBucket->count = currBucket->count + 1;
// 	}
// 	++totalCount;
// }

int WordCounter::getWordCount(string word) {
	// TODO
	return -1;
}

void WordCounter::removeWord(string word) {
	// TODO
}

int WordCounter::getCapacity() const {
	return capacity;
}

int WordCounter::getUniqueWordCount() const {
	return uniqueCount;
}

int WordCounter::getTotalWordCount() const {
	return totalCount;
}

bool WordCounter::empty() {
	return uniqueCount == 0;
}

int WordCounter::getIndex(string word) {
	size_t hashCode = hash<string>{}(word);
	return hashCode % capacity;
}

void WordCounter::display() {
    for(int i=0 ; i<capacity ; i++)
    {
        if(buckets[i] != NULL)
            cout << "key = " << buckets[i]->word
                 <<"  value = "<< buckets[i]->count << endl;
    }
}

#endif
