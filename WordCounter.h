// Author: Dale Berg, CPSC 2430 02
// Filename: WordCounter.h
// Assignment: p4, Hahsing the Hobbit
// Date: 5/30/2019

// Purpose: Creates hash table of given size determied by user and hashes words
// in .txt file given by user into the hash table. Users may determine how many
// total words and unique words appear in each text loaded into the system.
// Users may also search how many times a certain word appears in each file
// by inputing either a single word or a .txt file of words to be analyzed.



#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <string>
using namespace std;

class WordCounter {
public:
	WordCounter(int cap);
	// Constructor

	WordCounter( WordCounter& );
	// Copy Constructor

	~WordCounter();
	// Destructor

	WordCounter& operator=(const WordCounter&);
	// Overloaded assignment operator

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

private:
	struct Bucket {
		string word;
		int count;
		Bucket * next;

		Bucket(string w, int c);
		// robust constructor
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

WordCounter& WordCounter::operator=(const WordCounter& rhs ) {

	this->capacity = rhs.capacity;
	totalCount = rhs.totalCount;
	uniqueCount = rhs.uniqueCount;
	buckets = new Bucket*;

	for(int i = 0; i < capacity; ++i) {
		Bucket* other = rhs.buckets[i];
		Bucket* curr;

		if( rhs.buckets[i] == nullptr ) {
			buckets[i] = nullptr;
			continue; // nothing else to do
		}

		curr = new Bucket(other->word, other->count);
		buckets[i] = curr;

		other = other->next;

		while( other != nullptr ) {
			Bucket* b = new Bucket(other->word, other->count);
			curr->next = b;

			other = other->next;
		}
		curr->next = nullptr;
	}
	return *this;
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

WordCounter::WordCounter( WordCounter& other ) {
	this->capacity = other.capacity;
	totalCount = other.totalCount;
	uniqueCount = other.uniqueCount;
	buckets = new Bucket*[capacity];

	for(int i = 0; i < capacity; ++i) {

		Bucket* copy = other.buckets[i];
		Bucket* curr;


		if( other.buckets[i] == nullptr ) {
			buckets[i] = nullptr;
			continue;
		}

		curr = new Bucket(copy->word, copy->count);
		buckets[i] = curr;

		copy = copy->next;

		while(copy != nullptr) {
			Bucket* b = new Bucket(copy->word, copy->count);
			curr->next = b;
			curr = b;

			copy = copy->next;
		}
		curr->next = nullptr;
	}
}


WordCounter::~WordCounter() {
	for(int i = 0; i < capacity; ++i ) {
		Bucket* curr = buckets[i];
		while( curr != nullptr ) {
			Bucket* prev = curr;
			curr = curr->next;
			delete prev;
		}
	}
	delete[] buckets;
}

int WordCounter::addWord(string word) {
	int index = getIndex(word);
	Bucket* currBucket = buckets[index];
	Bucket* prev = nullptr;
	bool bucketFound = false;
	int currCount;
	while(currBucket != nullptr && !bucketFound) {
		if(currBucket->word == word) {
			currBucket->count = currBucket->count+1;
			currCount = currBucket->count;
			bucketFound = true;
		}
		prev = currBucket;
		currBucket = currBucket->next;
	}
	if(currBucket == nullptr && !bucketFound) {
		currCount = 1;
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
	return currCount;
}

int WordCounter::getWordCount(string word) {
	int index = getIndex(word);
	Bucket* curr = buckets[index];
	while( curr != nullptr ) {
		if( curr->word == word ) {
			return curr->count;
		}
		curr = curr->next;
	}
	return 0;
}

void WordCounter::removeWord(string word) {
	int index = getIndex(word);
	if( buckets[index] != nullptr ) {
		Bucket* curr = buckets[index];
		Bucket* prev = nullptr;
		while (curr->next != nullptr && curr->word != word ) {
			prev = curr;
			curr = curr->next;
		}
		if(curr->word == word) {
			if(prev == nullptr) {
				Bucket* connectNext = curr->next;
				totalCount -= curr->count;
				delete curr;
				buckets[index] = connectNext;
			}
			else {
				Bucket* nextBucket = curr->next;
				totalCount -= curr->count;
				delete curr;
				prev->next = nextBucket;
			}
			uniqueCount -= 1;
		}
	}
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

#endif
