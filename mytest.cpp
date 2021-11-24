// CMSC 341 - Spring 2021 - Project 4
// driver.cpp
// Simple driver program for use with HashTable class
#include "hash.h"
#include <random>

// Using a fixed seed value generates always the same sequence
// of pseudorandom numbers, e.g. reproducing scientific experiments
// here it helps us with testing since the same sequence repeats
std::mt19937 generator(10);// 10 is the fixed seed value

// We distribute the pseudorandom numbers uniformaly within
// the range [1000,99999]
std::uniform_int_distribution<> distribution(1000, 99999);

// The hash function used by HashTable class
unsigned int hashCode(const string str);

// the function to generate random serial numbers
unsigned int vaccineSerial() { return distribution(generator); }

class Tester { // Tester class to implement test functions
public:
	Tester() {}
	~Tester() {}
private:
};

int main() {

	Tester tester;

	// We use this temp array to store some data points for later use
	int tempSerials[50] = { 0 };
	int secondIndex = 0;
	// create a hash table with the size 101, and the hash function hashCode
	HashTable aTable(101, hashCode);
	int temp = 0;
	// we insert 50 Person objects with the same key, person's name is the key
	for (int i = 0; i < 50; i++) {
		temp = vaccineSerial();// generates a random number
		if (i % 3 == 0) {//this saves 17 numbers from the generated numbers for later use
			tempSerials[secondIndex] = temp;
			cout << temp << " was saved for later use." << endl;
			secondIndex++;
		}
		// print the random numbers which are inserted
		cout << "Insertion # " << i << " => " << temp << endl;
		aTable.insert(Person("John", temp));
	}
	cout << "Message: dump after 50 insertions in a table with 101 buckets:" << endl;
	aTable.dump();

	// We insert a data point to change the load factor, this should initiate a rehash
	temp = vaccineSerial();
	cout << "Insertion # 51" << " => " << temp << endl;
	aTable.insert(Person("John", temp));
	cout << "Message: dump after 51st insertion, the rehash should have happended to a table with 211 buckets:" << endl;
	aTable.dump();

	// Now we remove some data points which were saved in tempSerials array
	// Deleting 13 data points changes the deleteRatio, it should initiate a rehash 
	for (int i = 0; i < 14; i++)
		aTable.remove(Person("John", tempSerials[i]));
	cout << "Message: dump after 14 removes, a rehash should have happened to a table with 157 buckets," << endl;
	cout << "Message: after 13 removals the rehash happens, then the 14th removal will be tagged as DELETED in the new table." << endl;
	aTable.dump();
	cout << endl;

	//Test
	{
		//Insertion test
		cout << "\nTest case: Insertion Test: " << endl;
		HashTable bTable(MINPRIME, hashCode);
		bTable.insert(Person("John", 2));
		bTable.insert(Person("Louis", 3));
		bTable.insert(Person("Katy", 4));

		bTable.dump();

		//Removal test
		cout << "\nTest case: Removal Test: " << endl;
		bTable.remove(Person("John", 2));
		bTable.remove(Person("Louis", 3));
		bTable.remove(Person("Katy", 4));
		bTable.dump();

		//Quadratic Probing Test: Insertion
		cout << "\nTest case: Quadratic Probing Test: Insertion " << endl;
		bTable.insert(Person("Louis", 2));
		bTable.insert(Person("Louis", 3));
		bTable.insert(Person("Louis", 4));
		bTable.dump();

		//Quadratc Probing Test: Removal
		cout << "\nTest case: Quadratic Probing Test: Removal " << endl;
		bTable.setProbingPolicy(QUADRATIC);
		bTable.remove(Person("Louis", 2));
		bTable.remove(Person("Louis", 3));
		bTable.remove(Person("Louis", 4));
		bTable.dump();

		//Linear Probing Test: Insertion
		cout << "\nTest case: Linear Probing Test: Insertion" << endl;
		bTable.setProbingPolicy(LINEAR);
		bTable.insert(Person("Louis", 2));
		bTable.insert(Person("Louis", 3));
		bTable.insert(Person("Louis", 4));
		bTable.dump();

		//Linear Probing Test: Removal
		cout << "\nTest case: Linear Probing Test: Removal" << endl;
		bTable.setProbingPolicy(LINEAR);
		bTable.remove(Person("Louis", 2));
		bTable.remove(Person("Louis", 3));
		bTable.remove(Person("Louis", 4));
		bTable.dump();

	}
	return 0;
}

// The implementation of the hash function
// you can copy this function to your mytest.cpp file
unsigned int hashCode(const string str) {
	unsigned int val = 0;
	const unsigned int thirtyThree = 33;  // magic number from textbook
	for (int i = 0; i < str.length(); i++)
		val = val * thirtyThree + str[i];
	return val;
}

