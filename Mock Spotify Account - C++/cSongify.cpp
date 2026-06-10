/*! \file cSongify.cpp
	\author Peter Gortemaker
	\date 2025-11-10
	\version 1.0
	\brief Songify service functions

	Manages users, songs, and user-specific
	libraries with ratings and play counts

	--- If I had more time I would have worked on trying to make a template selection sort to
	reduce code ---
*/

#include "cSongify.hpp"


cSongify::cSongify() {}

/*! \brief Destructor - cleans up all dynamically allocated user libraries */
cSongify::~cSongify()
{
	// get all user IDs and their library pointers
	unsigned int count = 0;
	unsigned int* userIDs = nullptr;
	cHashTable<unsigned int, cUserSongData>** libraries = nullptr;
	m_userSongs.getAllPairs(userIDs, libraries, count);

	// delete each user's library hash table
	for (unsigned int i = 0; i < count; i++)
	{
		delete libraries[i];
	}

	// clean up temporary arrays
	delete[] userIDs;
	delete[] libraries;
}

// ------------ FUNCTIONS --------------

/*! \brief Adds a new user
	\param newPerson [in] person object containing all user data
	\param errorString [out] error message if failed
	\return true if user added successfully, false if user already exists
*/
bool cSongify::AddUser(cPerson newPerson, std::string& errorString)
{
	unsigned int userID = newPerson.getSongifyUniqueUserID();

	// check if user already exists
	if (m_users.exists(userID))
	{
		errorString = "User already exists.";
		return false;
	}

	return m_users.insert(userID, newPerson);
} 


/*! \brief Finds a user by songify ID and matches with unique user ID
	\param songifyID [in] user's songify ID
	\param foundPerson [out] user data if found
	\return true if user found and matched
*/
bool cSongify::FindUser(unsigned int songifyID, cPerson& foundPerson)
{
	// user not found
	if (!m_users.find(songifyID, foundPerson))
		return false;

	// verify the person we found actually exists
	return foundPerson.getSongifyUniqueUserID() == songifyID;
}


/*! \brief Finds user by SIN
	\param SIN [in] user's SIN
	\param foundPerson [out] user data if found
	\return true if user found with matching SIN
*/
bool cSongify::FindUserBySIN(unsigned int SIN, cPerson& foundPerson)
{
	// get all users
	unsigned int count = 0;
	cPerson* allUsers = m_users.getAllValues(count);

	if (allUsers == nullptr)
		return false;

	// search through all users for matching SIN
	bool found = false;
	for (unsigned int i = 0; i < count; i++)
	{
		if (allUsers[i].SIN == SIN)
		{
			foundPerson = allUsers[i];
			found = true;
			break;
		}
	}

	delete[] allUsers;
	return found;
}


/*! \brief Finds user by songify ID
	\param songifyID [in] user's songify ID
	\param foundPerson [out] user data if found
	\return true if user found
*/
bool cSongify::FindUserBySongifyID(unsigned int songifyID, cPerson& foundPerson)
{
	return m_users.find(songifyID, foundPerson);
}


/*! \brief Updates an existing user's data
	\param thePerson [in] person object containing updated data
	\param errorString [out] error message if failed
	\return true if updated successfully, false if user not found or SIN mismatch
*/
bool cSongify::UpdateUser(cPerson& thePerson, std::string& errorString)
{
	// get both IDs
	unsigned int userID = thePerson.getSongifyUniqueUserID();
	unsigned int userSIN = thePerson.SIN;

	// find the existing user
	cPerson existingUser;
	if (!m_users.find(userID, existingUser))
	{
		errorString = "User not found.";
		return false;
	}

	// verify songify ID and SIN match
	if (existingUser.SIN != userSIN)
	{
		errorString = "SIN does not match existing user.";
		return false;
	}

	// update the user
	return m_users.update(userID, thePerson);
}


/*! \brief Deletes a user
	\param songifyUserID [in] songify user ID
	\param errorString [out] error message if failed
	\return true if deleted successfully, false if user not found

	Also deletes the user's song library to prevent memory leaks.
*/
bool cSongify::DeleteUser(unsigned int songifyUserID, std::string& errorString)
{
	// check if user exists
	if (!m_users.exists(songifyUserID))
	{
		errorString = "User not found.";
		return false;
	}

	// if user has a library, delete it first
	cHashTable<unsigned int, cUserSongData>* userLibrary = nullptr;
	if (m_userSongs.find(songifyUserID, userLibrary))
	{
		delete userLibrary;  // Delete the hash table object
		m_userSongs.remove(songifyUserID);  // Remove the pointer from m_userSongs
	}

	// delete user
	return m_users.remove(songifyUserID);
}


/*! \brief Retrieves a copy (dynamic array) of a user's song library
	\param songifyUserID [in] songify user ID
	\param pLibraryArray [out] pointer to array of songs
	\param sizeOfLibary [out] number of songs in library
	\return true if successful, false if user not found
*/
bool cSongify::GetUsersSongLibrary(unsigned int songifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary)
{
	// check if user exists
	if (!m_users.exists(songifyUserID))
		return false;

	// get user's library
	cHashTable<unsigned int, cUserSongData>* userLibrary = nullptr;
	if (!m_userSongs.find(songifyUserID, userLibrary))
	{
		// user has no songs in library
		pLibraryArray = nullptr;
		sizeOfLibary = 0;
		return true;
	}

	// get all song IDs from user's library
	unsigned int songCount = 0;
	unsigned int* songIDs = nullptr;
	cUserSongData* songStats = nullptr;
	userLibrary->getAllPairs(songIDs, songStats, songCount);

	if (songCount == 0)
	{
		pLibraryArray = nullptr;
		sizeOfLibary = 0;
		return true;
	}

	// allocate array for songs
	pLibraryArray = new cSong[songCount];
	sizeOfLibary = songCount;

	// fill array with songs and their user-specific stats
	for (unsigned int i = 0; i < songCount; i++)
	{
		cSong song;
		if (m_songs.find(songIDs[i], song))
		{
			// copy songs
			pLibraryArray[i] = song;
			// override with user's stats
			pLibraryArray[i].rating = songStats[i].userRating;
			pLibraryArray[i].numberOfTimesPlayed = songStats[i].numberOfTimesPlayed;
		}
	}

	delete[] songIDs;
	delete[] songStats;

	return true;
}


/*! \brief Retrieves user's library sorted by rating
	\param songifyUserID [in] songify user ID
	\param pLibraryArray [out] pointer to sorted array of songs
	\param sizeOfLibary [out] the number of songs
	\return true if successful, false if user not found
*/
bool cSongify::GetUsersSongLibraryAscendingBySongRating(unsigned int songifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary)
{
	// get unsorted library
	if (!GetUsersSongLibrary(songifyUserID, pLibraryArray, sizeOfLibary))
		return false;

	if (pLibraryArray == nullptr || sizeOfLibary == 0)
		return true;  // empty library ie. "already sorted"


	// selection sort - find max, place at index
	for (unsigned int i = 0; i < sizeOfLibary - 1; i++)
	{
		unsigned int maxIndex = i;
		// find max in remainder
		for (unsigned int j = i + 1; j < sizeOfLibary; j++)
		{
			if (pLibraryArray[j].rating > pLibraryArray[maxIndex].rating)
				maxIndex = j;
		}

		// swap
		if (maxIndex != i)
		{
			cSong temp = pLibraryArray[i];
			pLibraryArray[i] = pLibraryArray[maxIndex];
			pLibraryArray[maxIndex] = temp;
		}
	}

	return true;
}


/*! \brief Retrieves user's library sorted by play count
	\param songifyUserID [in] songify user ID
	\param pLibraryArray [out] pointer to sorted array of songs
	\param sizeOfLibary [out] the number of songs
	\return true if successful, false if user not found
*/
bool cSongify::GetUsersSongLibraryAscendingByNumberOfPlays(unsigned int songifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary)
{
	// get unsorted library
	if (!GetUsersSongLibrary(songifyUserID, pLibraryArray, sizeOfLibary))
		return false;

	if (pLibraryArray == nullptr || sizeOfLibary == 0)
		return true;  // empty library ie. "already sorted"


	// selection sort
	for (unsigned int i = 0; i < sizeOfLibary - 1; i++)
	{
		unsigned int maxIndex = i;
		for (unsigned int j = i + 1; j < sizeOfLibary; j++)
		{
			if (pLibraryArray[j].numberOfTimesPlayed > pLibraryArray[maxIndex].numberOfTimesPlayed)
				maxIndex = j;
		}

		// swap
		if (maxIndex != i)
		{
			cSong temp = pLibraryArray[i];
			pLibraryArray[i] = pLibraryArray[maxIndex];
			pLibraryArray[maxIndex] = temp;
		}
	}

	return true;
}


/*! \brief Retrieves all users sorted by name
	\param pAllTheUsers [out] pointer to array of users
	\param sizeOfUserArray [out] the number of users
	\param sortAscending [in] true for ascending, false for descending
	\return true if successful (even if no users exist)
*/
bool cSongify::GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray, bool sortAscending)
{
	// get all users
	unsigned int count = 0;
	pAllTheUsers = m_users.getAllValues(count);
	sizeOfUserArray = count;

	if (pAllTheUsers == nullptr || count == 0)
	{
		return true;  // no users ie. "already sorted"
	}

	// selection sort by last name, then first name
	for (unsigned int i = 0; i < sizeOfUserArray - 1; i++)
	{
		// find target
		unsigned int targetIndex = i;
		for (unsigned int j = i + 1; j < sizeOfUserArray; j++)
		{
			bool shouldSwap = false;

			// compare last names
			if (pAllTheUsers[j].last != pAllTheUsers[targetIndex].last)
			{
				if (sortAscending)
					shouldSwap = (pAllTheUsers[j].last < pAllTheUsers[targetIndex].last);
				else
					shouldSwap = (pAllTheUsers[j].last > pAllTheUsers[targetIndex].last);
			}
			// compare first names if last names are the same
			else
			{
				if (sortAscending)
					shouldSwap = (pAllTheUsers[j].first < pAllTheUsers[targetIndex].first);
				else
					shouldSwap = (pAllTheUsers[j].first > pAllTheUsers[targetIndex].first);
			}

			if (shouldSwap)
				targetIndex = j;
		}

		// swap
		if (targetIndex != i)
		{
			cPerson temp = pAllTheUsers[i];
			pAllTheUsers[i] = pAllTheUsers[targetIndex];
			pAllTheUsers[targetIndex] = temp;
		}
	}

	return true;
}


/*! \brief Retrieves all users sorted by songify ID
	\param pAllTheUsers [out] pointer to array of users
	\param sizeOfUserArray [out] the number of users
	\return true if successful (even if no users exist)
*/
bool cSongify::GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray)
{
	// get all users and ID's
	unsigned int count = 0;
	unsigned int* userIDs = nullptr;
	m_users.getAllPairs(userIDs, pAllTheUsers, count);
	sizeOfUserArray = count;

	if (pAllTheUsers == nullptr || count == 0)
	{
		// if for some reason there are extraneous ID's
		if (userIDs != nullptr)
			delete[] userIDs;

		return true;
	}

	// selection sort both arrays together by ID
	for (unsigned int i = 0; i < sizeOfUserArray - 1; i++)
	{
		// find minimum ID in remainder
		unsigned int minIndex = i;
		for (unsigned int j = i + 1; j < sizeOfUserArray; j++)
		{
			if (userIDs[j] < userIDs[minIndex])
				minIndex = j;
		}

		// swap ID/Person pairs
		if (minIndex != i)
		{
			// swap IDs
			unsigned int tempID = userIDs[i];
			userIDs[i] = userIDs[minIndex];
			userIDs[minIndex] = tempID;

			// swap Persons
			cPerson tempPerson = pAllTheUsers[i];
			pAllTheUsers[i] = pAllTheUsers[minIndex];
			pAllTheUsers[minIndex] = tempPerson;
		}
	}

	delete[] userIDs;

	return true;
}


/*! \brief Adds a new song to the global library
	\param newSong [in] song object containing all song data
	\param errorString [out] error message if failed
	\return true if added successfully, false if song already exists
*/
bool cSongify::AddSong(cSong newSong, std::string& errorString)
{
	unsigned int songID = newSong.uniqueID;

	if (m_songs.exists(songID))
	{
		errorString = "Song already exists.";
		return false;
	}

	return m_songs.insert(songID, newSong);
}


/*! \brief Updates an existing song's data
	\param theSong [in] song object containing updated data
	\param errorString [out] error message if failed
	\return true if updated successfully, false if song not found
*/
bool cSongify::UpdateSong(cSong theSong, std::string& errorString)
{
	if (!m_songs.exists(theSong.uniqueID))
	{
		errorString = "Song not found.";
		return false;
	}

	return m_songs.update(theSong.uniqueID, theSong);
}


/*! \brief Deletes a song from the system (global and user library)
	\param uniqueSongID [in] unique song ID
	\param errorString [out] error message if failed
	\return true if deleted successfully, false if song not found
*/
bool cSongify::DeleteSong(unsigned int uniqueSongID, std::string& errorString)
{
	if (!m_songs.exists(uniqueSongID))
	{
		errorString = "Song not found.";
		return false;
	}

	// delete from global library
	m_songs.remove(uniqueSongID);

	// get all users' libraries
	unsigned int userCount = 0;
	unsigned int* userIDs = nullptr;
	cHashTable<unsigned int, cUserSongData>** libraries = nullptr;

	m_userSongs.getAllPairs(userIDs, libraries, userCount);

	// delete from each
	if (userIDs != nullptr)
	{
		for (unsigned int i = 0; i < userCount; i++)
		{
			libraries[i]->remove(uniqueSongID);
		}
		delete[] userIDs;
		delete[] libraries;
	}

	return true;
}


/*! \brief Adds a song to a user's library
	\param songifyUserID [in] songify user ID
	\param uniqueSongID [in] unique song ID
	\param errorString [out] error message if failed
	\return true if added successfully or already exists, false if user or song not found
*/
bool cSongify::AddSongToUserLibrary(unsigned int songifyUserID, unsigned int uniqueSongID, std::string& errorString)
{
	// check if song exists
	if (!m_songs.exists(uniqueSongID))
	{
		errorString = "Song does not exist.";
		return false;
	}

	// check if user exists
	if (!m_users.exists(songifyUserID))
	{
		errorString = "User does not exist.";
		return false;
	}

	// get or create user's library
	cHashTable<unsigned int, cUserSongData>* userLibrary;

	if (!m_userSongs.find(songifyUserID, userLibrary))
	{
		userLibrary = new cHashTable<unsigned int, cUserSongData>();
		m_userSongs.insert(songifyUserID, userLibrary);
	}

	// check if song already in library
	cUserSongData redundant;
	if (userLibrary->find(uniqueSongID, redundant))
	{
		return true;
	}

	// add song to user's library with default data
	cUserSongData songData;
	songData.userRating = 0;
	songData.numberOfTimesPlayed = 0;

	userLibrary->insert(uniqueSongID, songData);

	return true;
}


/*! \brief Removes a song from a user's library
	\param songifyUserID [in] songify user ID
	\param songifySongID [in] unique song ID
	\param errorString [out] error message if failed
	\return true if removed successfully, false if user not found,
			has no library, or song not in user library
*/
bool cSongify::RemoveSongFromUserLibrary(unsigned int songifyUserID, unsigned int songifySongID, std::string& errorString)
{
	// check if user exists
	if (!m_users.exists(songifyUserID))
	{
		errorString = "User not found.";
		return false;
	}

	// get user's library
	cHashTable<unsigned int, cUserSongData>* userLibrary;
	if (!m_userSongs.find(songifyUserID, userLibrary))
	{
		errorString = "User has no song library.";
		return false;
	}

	// check if song is in user's library
	if (!userLibrary->exists(songifySongID))
	{
		errorString = "Song not in user's library.";
		return false;
	}

	// Remove song from user's library
	userLibrary->remove(songifySongID);

	return true;
}


/*! \brief Updates a user's rating for a song
	\param songifyUserID [in] songify user ID
	\param songUniqueID [in] unique song ID
	\param newRating [in] new rating value (0-5)
	\return true if updated successfully, false if user has no library or song not in library
*/
bool cSongify::UpdateRatingOnSong(unsigned int songifyUserID, unsigned int songUniqueID, unsigned int newRating)
{
	// get pointer to user's library
	cHashTable<unsigned int, cUserSongData>* userLibrary;
	if (!m_userSongs.find(songifyUserID, userLibrary))
	{
		return false;
	}

	// get users current song data
	cUserSongData songData;
	if (!userLibrary->find(songUniqueID, songData))
	{
		return false;
	}

	// update rating
	songData.userRating = newRating;
	userLibrary->update(songUniqueID, songData);

	return true;
}


/*! \brief Retrieves a song for playback and increments play count
	\param songifyUserID [in] songify user ID
	\param songUniqueID [in] unique song ID
	\param foundSong [out] song data with user-specific stats
	\param errorString [out] error message if failed
	\return true if successful, false if song not found globally or not in user's library
*/
bool cSongify::GetSongToPlay(unsigned int songifyUserID, unsigned int songUniqueID, cSong& foundSong, std::string& errorString)
{
	// check that song exists globally
	if (!m_songs.find(songUniqueID, foundSong))
	{
		errorString = "Song not found in global library.";
		return false;
	}

	// get and check library exists
	cHashTable<unsigned int, cUserSongData>* userLibrary;
	if (!m_userSongs.find(songifyUserID, userLibrary))
	{
		errorString = "User library not found.";
		return false;
	}

	// get and check song exists in library
	cUserSongData songData;
	if (!userLibrary->find(songUniqueID, songData))
	{
		errorString = "Song not in user library.";
		return false;
	}

	// increment and update for user
	songData.numberOfTimesPlayed++;
	userLibrary->update(songUniqueID, songData);

	// update users stats
	foundSong.rating = songData.userRating;
	foundSong.numberOfTimesPlayed = songData.numberOfTimesPlayed;

	
	return true;
}


/*! \brief Retrieves a user's current rating for a song
	\param songifyUserID [in] songify user ID
	\param songUniqueID [in] unique song ID
	\param songRating [out] rating value if found
	\return true if found, false if user has no library or song not in library
*/
bool cSongify::GetCurrentSongRating(unsigned int songifyUserID, unsigned int songUniqueID, unsigned int& songRating)
{
	// get pointer to user's library
	cHashTable<unsigned int, cUserSongData>* userLibrary = nullptr;
	if (!m_userSongs.find(songifyUserID, userLibrary))
		return false;

	// get song data
	cUserSongData songData;
	if (!userLibrary->find(songUniqueID, songData))
		return false;

	songRating = songData.userRating;

	return true;
}


/*! \brief Retrieves the number of times a user has played a song
	\param songifyUserID [in] songify user ID
	\param songUniqueID [in] unique song ID
	\param numberOfPlays [out] play count if found
	\return true if found, false if user has no library or song not in library
*/
bool cSongify::GetCurrentSongNumberOfPlays(unsigned int songifyUserID, unsigned int songUniqueID, unsigned int& numberOfPlays)
{
	// get pointer to user's library
	cHashTable<unsigned int, cUserSongData>* userLibrary = nullptr;
	if (!m_userSongs.find(songifyUserID, userLibrary))
		return false;

	// get song data
	cUserSongData songData;
	if (!userLibrary->find(songUniqueID, songData))
		return false;

	numberOfPlays = songData.numberOfTimesPlayed;

	return true;
}


/*! \brief Finds a song by title and artist
	\param title [in] song title to search for
	\param artist [in] artist name to search for
	\param foundSong [out] song data if found
	\return true if found
*/
bool cSongify::FindSong(std::string title, std::string artist, cSong& foundSong)
{
	// get all songs
	unsigned int count = 0;
	cSong* allSongs = m_songs.getAllValues(count);

	if (allSongs == nullptr)
		return false;

	// search through all songs
	bool found = false;
	for (unsigned int i = 0; i < count; i++)
	{
		if (allSongs[i].name == title && allSongs[i].artist == artist)
		{
			foundSong = allSongs[i];
			found = true;
			break;
		}
	}

	delete[] allSongs;
	return found;
}


/*! \brief Finds a song by its unique ID
	\param uniqueID [in] unique song ID
	\param foundSong [out] song data if found
	\return true if found
*/
bool cSongify::FindSong(unsigned int uniqueID, cSong& foundSong)
{
	return m_songs.find(uniqueID, foundSong);
}



