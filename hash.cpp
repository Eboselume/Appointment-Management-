// CMSC 341 - Spring 2021 - Project 4
// hash.cpp
// The implementation of the class HashTable
#include "hash.h"
#include "math.h"
HashTable::HashTable(unsigned size, hash_fn hash) {

	m_size = 0;

	//checks if the size is smaller than MINPRIME
	if (size < MINPRIME)
	{
		m_capacity = MINPRIME;
	}

	//checks if the size is larger than MAXPRIME
	else if (size > MAXPRIME)
	{
		m_capacity = MAXPRIME;
	}

	
	//checks for if size is prime
	else
	{
		if (isPrime(size) == false)
		{
			m_capacity = findNextPrime(size);
		}

		//m_size is num of occupied buckets 
		//m_capacity is the number of buckets in total 
		else
		{
			m_capacity = size;
		}
		
	}
    
	        
	    m_numDeleted = 0;  
	    m_hash = hash;          
		m_table = new Person[m_capacity];
	    m_currentProb = LINEAR; 

}

HashTable::~HashTable() {

	delete[] m_table;
	m_table = nullptr;
}

bool HashTable::insert(Person person) {

	int index = m_hash(person.key()) % m_capacity;
	int counter = 1;


	while (!(m_table[index] == EMPTY))
	{
		

		//Linear probing
		//index = (Hash(key) + i) % TableSize
		if (m_currentProb == LINEAR)
		{
			 index = (m_hash(person.key()) + counter) % m_capacity;
		}

		//Quadractic Probing
		// index = ((Hash(key) % TableSize) + (i x i)) % TableSize
		else
		{

			index = ((m_hash(person.key()) % m_capacity) + (counter * counter)) % m_capacity;

		}

		counter++;

		//checks to see if were back at the starting postion
		//which means we could not find a place to put it so false
		if (index == m_hash(person.key()) % m_capacity)
		{

			return false;
		}
		
	}

	m_table[index] = person; // inserted person object
	m_size++;  //ASK if I need to add this

	//criteria for insertion to change probing
	if (lambda() > 0.5)
	{

		//Test condition if the probing is Linear switch to quadratic probing
		if (m_currentProb == LINEAR)
		{
			setProbingPolicy(QUADRATIC);
		}


		//vice-versa
		else
		{

			setProbingPolicy(LINEAR);

		}
	
	}

	return true;

}

bool HashTable::remove(Person person) 
{


	int index = m_hash(person.key()) % m_capacity;
	int counter = 1;


	while (!(m_table[index] == EMPTY))
	{


		if (m_table[index] == person)
		{
			break;
		}

		//Linear probing
		//index = (Hash(key) + i) % TableSize
		if (m_currentProb == LINEAR)
		{
			index = (m_hash(person.key()) + counter) % m_capacity;
		}

		//Quadractic Probing
		// index = ((Hash(key) % TableSize) + (i x i)) % TableSize
		else
		{

			index = ((m_hash(person.key()) % m_capacity) + (counter * counter)) % m_capacity;

		}

		counter++;
		
		//checks to see if were back at the starting postion
		//which means we could not find a place to put it so false
		//if (index == m_hash(person.key()) % m_capacity)
		if(counter >= m_capacity)
		{
			return false;
		}


	}


	m_table[index] = DELETED;
	m_numDeleted++;

	//criteria
	//if (m_numDeleted > (0.25 * m_size))
	if(deletedRatio() > 0.25 )
	{
		//Test condition if the probing is Linear switch to quadratic probing
		if (m_currentProb == LINEAR)
		{
			setProbingPolicy(QUADRATIC);
		}

		//vice-versa
		else
		{

			setProbingPolicy(LINEAR);

		}
	}

	return true;
}

void HashTable::setProbingPolicy(probing prob)
{

	//int index = m_hash(m_table[i].key()) % m_capacity;
	//int counter = 0;

	m_currentProb = prob;

	
	//make a rehash function
	// void rehash(new capcaity or old new capacity )

	//you could also do the if criteria 
	if ((lambda() > 0.5) || (deletedRatio() > 0.25))
	{
		int temp = m_capacity; // holds old capacity

		//new capacity
		// The current number of data points
		int newSIze = m_size - m_numDeleted;

		m_capacity = findNextPrime(newSIze * 4);

		//Temorary hash table
		Person* tempTable = new Person[m_capacity];

		//sets to zero since there are no occupied buckets
		m_size = 0;
		m_numDeleted = 0;

		//rehash 
		for (int i = 0; i < temp; i++)
		{
			if (!(m_table[i] == EMPTY) && !(m_table[i] == DELETED))
			{

				//index = m_hash(person.key()) % m_capacity;

				int index = m_hash(m_table[i].key()) % m_capacity;

				int counter = 1;

				while (!(tempTable[index] == EMPTY))
				{

					if (m_currentProb == LINEAR)
					{

						index = (m_hash(m_table[i].key()) + counter) % m_capacity;

					}

					else if (m_currentProb == QUADRATIC)
					{

						//index = ((Hash(key) % TableSize) + (i x i)) % TableSize
						index = ((m_hash(m_table[i].key()) % m_capacity) + (counter * counter)) % m_capacity;

					}

					counter++;
					
				}
				
				tempTable[index] = m_table[i];
				m_size++;
			}
			

		}
		

		delete[] m_table; //gets rid of the old table before rehsahing
		m_table = tempTable; // sets the old table to the new table(after rehashing)
	}
}

float HashTable::lambda() const {

	return ((float)m_size)/ m_capacity; //should I use float casting
}

float HashTable::deletedRatio() const {
	return((float)m_numDeleted) / m_size;

}

void HashTable::dump() const {
	for (int i = 0; i < m_capacity; i++) {
		cout << "[" << i << "] : " << m_table[i] << endl;
	}
}

bool HashTable::isPrime(int number) {
	// If number is prime this function returns true
	// otherwise it returns false
	bool result = true;
	for (int i = 2; i <= number / 2; ++i) {
		if (number % i == 0) {
			result = false;
			break;
		}
	}
	return result;
}

int HashTable::findNextPrime(int current) {
	//we won't go beyond MAXPRIME
	//the smallest prime would be next after MINPRIME
	if (current < MINPRIME) current = MINPRIME;
	for (int i = current; i < MAXPRIME; i++) {
		for (int j = 2; j * j <= i; j++) {
			if (i % j == 0)
				break;
			else if (j + 1 > sqrt(i) && i != current) {
				return i;
			}
		}
	}
	//if a user tries to go over MAXPRIME
	return MAXPRIME;
}