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

const int primes[] = {
    11, 13, 17, 19, 23, 29, 31, 37, 43, 53, 67, 79, 97, 107, 131, 157, 191,
    223, 269, 331, 389, 461, 557, 673, 797, 967, 1151, 1381, 1657, 1979,
    2377, 2851, 3433, 4111, 4931, 5923, 7103, 8513, 10211, 12251, 14699,
    17657, 21169, 25409, 30491, 36583, 43889, 52667, 63199, 75853, 91009,
    109211, 131059, 157259, 188707, 226451, 271753, 326087, 391331, 469583,
    563489, 676171, 811411, 973691, 1168451, 1402123, 1682531, 2019037,
    2422873, 2907419, 3488897, 4186673, 5024009, 6028807, 7234589, 8681483,
    10417769, 12501331, 15001603, 18001909, 21602311, 25922749, 31107317,
    37328761, 44794513, 53753431, 64504081, 77404907, 92885893, 111463049,
    133755659, 160506817, 192608173, 231129781, 277355759, 332826869,
    399392243, 479270713, 575124829, 690149821, 828179753, 993815743
};

#include <string>
#include <iostream>

using namespace std;

class WordCounter {
public:

   WordCounter();
   // Constructor. Initializes capacity of underlying array to 19


	WordCounter(int cap);
	// Constructor. Initializes capacity of underlying array of buckets from
   // class constant array of prime numbers. If the given capacity is larger
   // than the last element of primes, then capacity is initalized to the
   // value given by user. If the program were to be expanded, a method
   // could use the Sieve of Eratosthenes to find the next largest prime number.

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

   bool isPrime(int n);
   // Returns true if n is prime, otherwise returns false.

   int findNextPrime(int n);
   // Returns next prime in primes list

   void resize();
   // Resizes underlying array, doubling the capcity, then rounding up to
   // the nearest nearest prime number in prime list;
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

WordCounter::WordCounter() {

   capacity = 19;
	totalCount = 0;
	uniqueCount = 0;
	buckets = new Bucket*[capacity];

	for(int i = 0; i < capacity; ++i) {
		buckets[i] = nullptr;
	}

}

WordCounter::WordCounter(int cap) {

   int numPrimes = (sizeof(primes)/sizeof(*primes));

   if(!isPrime(cap)){
      if( cap > primes[numPrimes-1]) {
         capacity = cap; // if greater than greatest known prime
      }
      else {
            capacity = findNextPrime(cap); // find next prime from array
         }
      }
   else {
      capacity = cap; // if user entered prime number
   }

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

   double loadFactor = (uniqueCount+1) / capacity; // load factor

   if( loadFactor > .75) { // resize if the load factor greater than 75%
      resize();
   }

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

int WordCounter::getWordCount(string word) {
	int index = getIndex(word);
	Bucket* curr = buckets[index];
	while( curr != nullptr ) {
		if( curr->word == word ) { // found case
			return curr->count;
		}
		curr = curr->next;
	}
	return 0; // not found case
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
				buckets[index] = connectNext; // ensure chain stays connected
			}
			else {
				Bucket* nextBucket = curr->next;
				totalCount -= curr->count; // decrement total count
				delete curr; //  delete current w/o breaking chain
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

bool WordCounter::isPrime(int n) {
   bool isPri = true;
   for(int i = 2; (i <= n / 2) && isPri; ++i)
  {
      if(n % i == 0)
      {
          isPri = false;
      }
  }
  return isPri;
}

int WordCounter::findNextPrime(int n) {
   int numPrimes = (sizeof(primes)/sizeof(*primes));
   for(int i = 0; i < numPrimes; ++i) {
      if( primes[i] > n ) {
         return primes[i];
      }
   }
   return n;
}



/////
//// delete comments
////
void WordCounter::resize() {
   cout << "attempting resize, capacity" << capacity << endl;
   // this is a little more complicated, bbecause need to consider count already added

   int oldTableSize = capacity;
   capacity *= 2;
   capacity = findNextPrime(capacity);
   Bucket **oldTable = buckets;
   buckets = new Bucket*[capacity]; // make new array to populate
   for (int i = 0; i < capacity; i++) {
      buckets[i] = NULL; // set new array elements to null
   }
   uniqueCount = 0; // reset counts
   totalCount = 0;
   for (int hash = 0; hash < oldTableSize; hash++) { // for each in old
      if (oldTable[hash] != NULL) {
         Bucket *oldEntry; // entry in old bucket
         Bucket *entry = oldTable[hash]; // entry to be copied
         while (entry != NULL) {
            int counter = 0;
            while( counter < entry->count) {
               addWord(entry->word); // rehash 'count' times
               ++counter;
               }
            oldEntry = entry; // update old entry
            entry = entry->next; // entry moves to next in chain
            delete oldEntry; // delete entry
         }
      }
   }
   delete[] oldTable; // delete original array
}

#endif
