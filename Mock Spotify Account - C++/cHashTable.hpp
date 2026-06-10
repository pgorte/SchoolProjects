/*! \file cHashTable.hpp
	\author Peter Gortemaker
	\date 2025-11-10
	\version 1.0
	\brief Hash table using chaining for collision resolution

	Uses a table of linked lists of key-value pairs
	Provides methods for cSongify
*/

#ifndef _cHashTable_HG_
#define _cHashTable_HG_

// ---------- NODE CLASS -------------
/*! \class cHashNode
	\brief Node structure for storing key-value pairs in hash table chains

	Each node contains a key, value, and pointer to the next node in the chain
	These nodes form singly-linked lists at each bucket in the hash table
*/
template<typename TKey, typename TValue>
class cHashNode
{
public:
	TKey key_;
	TValue value_;
	cHashNode* next_;

	cHashNode(const TKey& key, const TValue& value)
		: key_(key), value_(value), next_(nullptr) {}
};

// -------------- HASH TABLE CLASS ------------------
/*! \class cHashTable
	\brief Hash table using chaining

	A table of linked lists to store data
	Collisions are resolved by chaining in linked lists
*/
template<typename TKey, typename TValue>
class cHashTable
{
private:
	cHashNode<TKey, TValue>** table_;	//!< Table "array" of linked list heads
	unsigned int size_;					//!< Number of lists
	unsigned int count_;				//!< Number of key-value pairs

	// division hash using djb2 prime
	unsigned int hash(const TKey& key) const { return (key * 5381) % size_; }

public:
	// constructor
	cHashTable(unsigned int size = 1000)
		: size_(size), count_(0)
	{

		// create table
		table_ = new cHashNode<TKey, TValue>* [size_];

		// initialize all heads to nullptr
		for (unsigned int i = 0; i < size_; i++)
		{
			table_[i] = nullptr;
		}
	}

	// Destructor
	~cHashTable()
	{
		// delete nodes
		for (unsigned int i = 0; i < size_; i++)
		{
			cHashNode<TKey, TValue>* curr = table_[i];
			while (curr != nullptr)
			{
				cHashNode<TKey, TValue>* temp = curr;
				curr = curr->next_;
				delete temp;
			}
		}
		// delete table
		delete[] table_;
	}

	// --- Disable copy/assignment since I'm using pointers ---
	cHashTable(const cHashTable&) = delete;
	cHashTable& operator=(const cHashTable&) = delete;



	/* --------------------- FUNCTIONS ----------------------*/


	/*! \brief Inserts a new key-value pair
		\param key [in] key to insert
		\param value [in] key's value
		\return true if inserted successfully, false if key already exists

		New nodes are inserted at the head of the chain
		Only inserted if unique to chain
	*/
	bool insert(const TKey& key, const TValue& value)
	{
		// table index
		unsigned int index = hash(key);

		// Check if key already exists in the current chain
		cHashNode<TKey, TValue>* curr = table_[index];
		while (curr != nullptr)
		{
			if (curr->key_ == key)
			{
				return false; // Already exists
			}
			curr = curr->next_; // Iterate through chain
		}

		// If not false, create node and add to chain head
		cHashNode<TKey, TValue>* newNode = new cHashNode<TKey, TValue>(key, value);
		newNode->next_ = table_[index];
		table_[index] = newNode;
		count_++;

		return true;
	}

	/*! \brief Finds a value by its key
		\param key [in] key to find
		\param value [out] receives found value if key exists
		\return true if key found and modifies value

		Searches the indexed chain for the key
		Value is only modified if key is found
	*/
	bool find(const TKey& key, TValue& value) const
	{
		// table index
		unsigned int index = hash(key);

		// look for key in chain
		cHashNode<TKey, TValue>* curr = table_[index];
		while (curr != nullptr)
		{
			if (curr->key_ == key)
			{
				value = curr->value_; // Modify value
				return true; 
			}
			curr = curr->next_; // Iterate through chain
		}
		
		return false; // Not found
	}

	/*! \brief Removes a key-value pair from chain, frees memory
		\param key [in] key to remove
		\return true if removed successfully, false if key not found
	*/
	bool remove(const TKey& key)
	{
		// table index
		unsigned int index = hash(key);

		cHashNode<TKey, TValue>* curr = table_[index];
		cHashNode<TKey, TValue>* prev = nullptr;

		// Look for key in chain
		while (curr != nullptr)
		{
			// found -> remove
			if (curr->key_ == key)
			{
				if (prev == nullptr)
					table_[index] = curr->next_;
				else
					prev->next_ = curr->next_;

				delete curr;
				count_--;
				return true;
			}
			prev = curr;
			curr = curr->next_;
		}

		return false; // key not found
	}

	/*! \brief Checks if a key exists
		\param key [in] The key to check for
		\return true if key exists, false otherwise

		More efficient than find() when you only need existence check.
	*/
	bool exists(const TKey& key) const
	{
		// table index
		unsigned int index = hash(key);

		cHashNode<TKey, TValue>* curr = table_[index];

		// look for key in chain
		while (curr != nullptr)
		{
			if (curr->key_ == key)
			{
				return true; // key found
			}
			curr = curr->next_;
		}

		return false; // key not found
	}

	/*! \brief Updates the value for an existing key
		\param key [in] key whose value to update
		\param value [in] new value to replace old
		\return true if updated successfully, false if key doesn't exist
	*/
	bool update(const TKey& key, const TValue& value)
	{
		// table index
		unsigned int index = hash(key);

		cHashNode<TKey, TValue>* curr = table_[index];
		
		// look for key in chain
		while (curr != nullptr)
		{
			if (curr->key_ == key)
			{
				curr->value_ = value; // update value
				return true;
			}
			curr = curr->next_;
		}
		
		return false; // key not found
	}

	/*! \brief Returns all values as a dynamic array
		\param numValues [out] number of values returned
		\return Pointer to array of values (nullptr if empty)
	*/
	TValue* getAllValues(unsigned int& numValues) const
	{
		numValues = count_;

		if (count_ == 0)
		{
			return nullptr;
		}

		// Allocate array to hold all values
		TValue* values = new TValue[count_];
		unsigned int index = 0;

		// Iterate through all chains
		for (unsigned int i = 0; i < size_; i++)
		{
			// Iterate through nodes in chain
			cHashNode<TKey, TValue>* curr = table_[i];
			while (curr != nullptr)
			{
				values[index] = curr->value_;
				index++;
				curr = curr->next_;
			}
		}

		return values;
	}

	/*! \brief Returns all keys as a dynamic array
		\param numKeys [out] number of keys returned
		\return Pointer to array of keys (nullptr if empty)
	*/
	TKey* getAllKeys(unsigned int& numKeys) const
	{
		numKeys = count_;

		if (count_ == 0)
		{
			return nullptr;
		}

		// Allocate array to hold all keys
		TKey* keys = new TKey[count_];
		unsigned int index = 0;

		// Iterate through all chains
		for (unsigned int i = 0; i < size_; i++)
		{
			// Iterate through nodes in chain
			cHashNode<TKey, TValue>* curr = table_[i];
			while (curr != nullptr)
			{
				keys[index] = curr->key_;
				index++;
				curr = curr->next_;
			}
		}

		return keys;
	}

	/*! \brief Returns all key-value pairs as two parallel dynamic arrays
		\param keys [out] pointer to array of keys
		\param values [out] pointer to array of values
		\param numPairs [out] the number of pairs returned
	*/
	void getAllPairs(TKey*& keys, TValue*& values, unsigned int& numPairs) const
	{
		numPairs = count_;

		if (count_ == 0)
		{
			keys = nullptr;
			values = nullptr;
			return;
		}

		// Allocate arrays
		keys = new TKey[count_];
		values = new TValue[count_];
		unsigned int index = 0;

		// Iterate through all chains
		for (unsigned int i = 0; i < size_; i++)
		{
			// Iterate through nodes in chain
			cHashNode<TKey, TValue>* curr = table_[i];
			while (curr != nullptr)
			{
				keys[index] = curr->key_;
				values[index] = curr->value_;
				index++;
				curr = curr->next_;
			}
		}
	}
};

#endif