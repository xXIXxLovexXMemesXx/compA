#include "memory.hh"

unsigned int clockX;
unsigned int numMisses;
int cache_org;

void resetClock()
{
	clockX = 0;
	numMisses = 0;

}

void printCacheOrg(int org)
{
	printf("Cache Organization: ");
	if (org == DIRECT)
		printf("DIRECT MAPPED\n");
	else if (org == TWOWAY)
		printf("TWO-WAY SET ASSOCIATIVE\n");
	else if (org == FULLY)
		printf("FULLY ASSOCIATIVE\n");
	else
		printf("ERROR: WRONG CACHE ORG\n");
}

int Cache::getData(int address)
{
	int data;

	if (cache_org == DIRECT)
		data = getDirect(address);
	else if (cache_org == TWOWAY)
		data = getTwoWay(address);
	else if (cache_org == FULLY)
		data = getFully(address);
	else
	{
		printf("ERROR: WRONG CACHE ORG in getData \n");
		return 0;
	}


	return data;
}

void Cache::putData(int address, int value)
{
	if (cache_org == DIRECT)
		putDirect(address, value);
	else if (cache_org == TWOWAY)
		putTwoWay(address, value);
	else if (cache_org == FULLY)
		putFully(address, value);
	else
	{
		printf("ERROR: WRONG CACHE ORG in putData \n");
		return;
	}

}
int Cache::getDirect(int address)
{
	//--- Start by applying bit manipulations to address ---
	int temp = address;
	temp = temp % 512;
	// word number (4 total) - 2 bits
	int wordIndx = address & 0x00000003;

	// block number (8 total) - 3 bits
	int blockIndx = (address & 0x0000001C) >> 2;

	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag = (address) >> 5;
	// --- end of bit manipulations for direct mapping ---

	// find the cached address
	int cacheAddr = ((cblocks[blockIndx].tag << 5) | (blockIndx << 2) | wordIndx);
	int value; // value in block line
	if (address == cacheAddr)
	{
		clockX += 2;// cache access for read
		value = cblocks[blockIndx].data[wordIndx];
	}
	else
	{
		numMisses++;
		clockX += 2; //cache accessed for read


		//move the correct address block from MainMemory to cache
		MainMemory.blocks[temp].tag = curTag;
		MainMemory.blocks[temp].valid = 1;

		cblocks[blockIndx].tag = MainMemory.blocks[temp].tag;
		cblocks[blockIndx].valid = MainMemory.blocks[temp].valid;

		for (int i = 0; i<WORDS_PER_BLOCK; i++)
		{
			cblocks[blockIndx].data[i] = MainMemory.blocks[temp].data[i];
		}
		// both cache and main memory were accessed to copy from MainMemory to cache
		clockX += 2;
		clockX += 100;


		//then read value
		value = cblocks[blockIndx].data[wordIndx];
		clockX += 2; //cache accessed for read
	}

	return value;
}
void Cache::putDirect(int address, int value)
{
	//--- Start by applying bit manipulations to address ---
	int temp = address;
	temp = temp % 512;
	// word number (4 total) - 2 bits
	int wordIndx = address & 0x00000003;

	// block number (8 total) - 3 bits
	int blockIndx = (address & 0x0000001C) >> 2;

	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag = (address) >> 5;
	// --- end of bit manipulations for direct mapping ---

	int cacheAddr = ((cblocks[blockIndx].tag << 5) | (blockIndx << 2) | wordIndx);
	if (address == cacheAddr)
	{
		clockX += 2;//access Cache

		cblocks[blockIndx].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else
	{
		numMisses++;
		clockX += 2;//Cache was accessed to check for it

		//write to the correct address from MainMemory to caches
		clockX += 100; //access MainMemory
		MainMemory.blocks[temp].data[wordIndx] = value;
		MainMemory.blocks[temp].tag = curTag;
		MainMemory.blocks[temp].valid = 1;


		// then move the block to the cached block
		cblocks[blockIndx].tag = MainMemory.blocks[temp].tag;
		cblocks[blockIndx].valid = MainMemory.blocks[temp].valid;
		for (int i = 0; i<WORDS_PER_BLOCK; i++)
		{
			cblocks[blockIndx].data[i] = MainMemory.blocks[temp].data[i];
		}
		// both cache and main memory were accessed to copy from MainMemory to cache
		clockX += 2;
		clockX += 100;
	}
}
int Cache::getTwoWay(int address)
{
	//--- Start by applying bit manipulations to address ---
	int tempAddr = address;
	tempAddr = tempAddr % 512;
	// word number (4 total) - 2 bits
	int wordIndx = address & 0x00000003;

	// block number (8 total) - 3 bits
	int blockIndx = (address & 0x0000001C) >> 2;
	//set index is block % 4-- 2 bits
	int setIndx = blockIndx % 4;

	// tag = remaining bits (32-2-3 = 28 bits)
	int curTag = (address) >> 4;
	// --- end of bit manipulations for 2 way mapping ---

	// find the potental current cached addresses
	int cacheAddr1 = ((cblocks[setIndx].tag << 4) | (setIndx << 2) | wordIndx);
	int cacheAddr2 = ((cblocks[setIndx + 4].tag << 4) | (setIndx << 2) | wordIndx);
	int value; // value in block line

	if (address == cacheAddr1) 	// check set 0 block
	{
		clockX += 2;// cache access for read
		value = cblocks[setIndx].data[wordIndx];
		cblocks[setIndx].last_used++;
	}
	else if (address == cacheAddr2) 	// check set 1 block
	{
		// is this a miss???
		clockX += 2;// cache access for read
		value = cblocks[setIndx + 4].data[wordIndx];
		cblocks[setIndx + 4].last_used++;
	}
	else
	{
		numMisses++;
		clockX += 2; //cache accessed for read

		//move the correct address block from MainMemory to cache
		MainMemory.blocks[tempAddr].tag = curTag;
		MainMemory.blocks[tempAddr].valid = 1;

		//decide which set to use
		if (cblocks[setIndx].last_used < cblocks[setIndx + 4].last_used)
		{
			cblocks[setIndx].tag = MainMemory.blocks[tempAddr].tag;
			cblocks[setIndx].valid = MainMemory.blocks[tempAddr].valid;

			for (int i = 0; i < WORDS_PER_BLOCK; i++)
			{
				cblocks[setIndx].data[i] = MainMemory.blocks[tempAddr].data[i];
			}
			// both cache and main memory were accessed to copy from MainMemory to cache
			clockX += 2;
			clockX += 100;


			//then read value
			value = cblocks[setIndx].data[wordIndx];
			clockX += 2; //cache accessed for read
		}
		else
		{
			cblocks[setIndx + 4].tag = MainMemory.blocks[tempAddr].tag;
			cblocks[setIndx + 4].valid = MainMemory.blocks[tempAddr].valid;

			for (int i = 0; i < WORDS_PER_BLOCK; i++)
			{
				cblocks[setIndx + 4].data[i] = MainMemory.blocks[tempAddr].data[i];
			}
			// both cache and main memory were accessed to copy from MainMemory to cache
			clockX += 2;
			clockX += 100;


			//then read value
			value = cblocks[setIndx + 4].data[wordIndx];
			clockX += 2; //cache accessed for read
		}
	}

	return value;
}
void Cache::putTwoWay(int address, int value)
{
	//--- Start by applying bit manipulations to address ---
	int tempAddr = address;
	tempAddr = tempAddr % 512;
	// word number (4 total) - 2 bits
	int wordIndx = address & 0x00000003;

	// block number (8 total) - 3 bits
	int blockIndx = (address & 0x0000001C) >> 2;
	//set is block % 4, setIndx = 0 - 3
	int setIndx = blockIndx % 4;

	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag = (address) >> 4;
	// --- end of bit manipulations for direct mapping ---

	// find potential cached addresses
	int cacheAddr1 = ((cblocks[setIndx].tag << 4) | (setIndx << 2) | wordIndx);
	int cacheAddr2 = ((cblocks[setIndx + 4].tag << 4) | (setIndx << 2) | wordIndx);

	if (address == cacheAddr1)
	{
		clockX += 2;//access Cache

		cblocks[setIndx].data[wordIndx] = value;
		cblocks[setIndx].last_used++;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[tempAddr].data[wordIndx] = cblocks[setIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr2)
	{
		clockX += 2;//access Cache
		// Is this a Miss?

		cblocks[setIndx + 4].data[wordIndx] = value;
		cblocks[setIndx].last_used++;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[tempAddr].data[wordIndx] = cblocks[setIndx + 4].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else
	{
		numMisses++;
		clockX += 2;//Cache was accessed to check for it

		//write to the correct address from MainMemory to caches
		clockX += 100; //access MainMemory
		MainMemory.blocks[tempAddr].data[wordIndx] = value;
		MainMemory.blocks[tempAddr].tag = curTag;
		MainMemory.blocks[tempAddr].valid = 1;


		// then move the block to the cached block

		//choose which set
		if (cblocks[setIndx].last_used < cblocks[setIndx + 4].last_used)
		{
			cblocks[setIndx].tag = MainMemory.blocks[tempAddr].tag;
			cblocks[setIndx].valid = MainMemory.blocks[tempAddr].valid;

			for (int i = 0; i < WORDS_PER_BLOCK; i++)
			{
				cblocks[setIndx].data[i] = MainMemory.blocks[tempAddr].data[i];
			}
			// both cache and main memory were accessed to copy from MainMemory to cache
			clockX += 2;
			clockX += 100;
		}
		else
		{
			cblocks[setIndx + 4].tag = MainMemory.blocks[tempAddr].tag;
			cblocks[setIndx + 4].valid = MainMemory.blocks[tempAddr].valid;
			for (int i = 0; i < WORDS_PER_BLOCK; i++)
			{
				cblocks[setIndx + 4].data[i] = MainMemory.blocks[tempAddr].data[i];
			}
			// both cache and main memory were accessed to copy from MainMemory to cache
			clockX += 2;
			clockX += 100;
		}
	}

}
int Cache::getFully(int address)
{
	//--- Start by applying bit manipulations to address ---
	int temp = address;
	temp = temp % 512;
	// word number (4 total) - 2 bits
	int wordIndx = address & 0x00000003;

	// block number (8 total) - 3 bits
	//int blockIndx = (address & 0x0000001C) >> 2;

	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag = (address) >> 2;
	// --- end of bit manipulations for direct mapping ---

	// find the cached address
	int cacheAddr0 = ((cblocks[0].tag << 2) |  wordIndx);
	int cacheAddr1 = ((cblocks[1].tag << 2) |  wordIndx);
	int cacheAddr2 = ((cblocks[2].tag << 2) |  wordIndx);
	int cacheAddr3 = ((cblocks[3].tag << 2) |  wordIndx);
	int cacheAddr4 = ((cblocks[4].tag << 2) |  wordIndx);
	int cacheAddr5 = ((cblocks[5].tag << 2) |  wordIndx);
	int cacheAddr6 = ((cblocks[6].tag << 2) |  wordIndx);
	int cacheAddr7 = ((cblocks[7].tag << 2) |  wordIndx);

	int value; // value in block line
	if (address == cacheAddr0)
	{
		clockX += 2;// cache access for read
		value = cblocks[0].data[wordIndx];
		cblocks[0].last_used++;
	}
	else if (address == cacheAddr1)
	{
		clockX += 2;// cache access for read
		value = cblocks[1].data[wordIndx];
		cblocks[1].last_used++;
	}
	else if (address == cacheAddr2)
	{
		clockX += 2;// cache access for read
		value = cblocks[2].data[wordIndx];
		cblocks[2].last_used++;
	}
	else if (address == cacheAddr3)
	{
		clockX += 2;// cache access for read
		value = cblocks[3].data[wordIndx];
		cblocks[3].last_used++;
	}
	else if (address == cacheAddr4)
	{
		clockX += 2;// cache access for read
		value = cblocks[4].data[wordIndx];
		cblocks[4].last_used++;
	}
	else if (address == cacheAddr5)
	{
		clockX += 2;// cache access for read
		value = cblocks[5].data[wordIndx];
		cblocks[5].last_used++;
	}
	else if (address == cacheAddr6)
	{
		clockX += 2;// cache access for read
		value = cblocks[6].data[wordIndx];
		cblocks[6].last_used++;
	}
	else if (address == cacheAddr7)
	{
		clockX += 2;// cache access for read
		value = cblocks[7].data[wordIndx];
		cblocks[7].last_used++;
	}
	else
	{
		numMisses++;
		clockX += 2; //cache accessed for read x8
		int tempLast = cblocks[0].last_used;
		int tempLastIndex = 0;
		for (int i = 0; i < 8; i++)
		{
			if (cblocks[i].last_used < tempLast)
				tempLastIndex = i;
		}

		//move the correct address block from MainMemory to cache
		MainMemory.blocks[temp].tag = curTag;
		MainMemory.blocks[temp].valid = 1;

		cblocks[tempLastIndex].tag = MainMemory.blocks[temp].tag;
		cblocks[tempLastIndex].valid = MainMemory.blocks[temp].valid;
		cblocks[tempLastIndex].last_used++;

		for (int i = 0; i<WORDS_PER_BLOCK; i++)
		{
			cblocks[tempLastIndex].data[i] = MainMemory.blocks[temp].data[i];
		}
		// both cache and main memory were accessed to copy from MainMemory to cache
		clockX += 2;
		clockX += 100;


		//then read value
		value = cblocks[tempLastIndex].data[wordIndx];
		clockX += 2; //cache accessed for read
	}

	return value;
}
void Cache::putFully(int address, int value)
{
	//--- Start by applying bit manipulations to address ---
	int temp = address;
	temp = temp % 512;
	// word number (4 total) - 2 bits
	int wordIndx = address & 0x00000003;

	// block number (8 total) - 3 bits
	//int blockIndx = (address & 0x0000001C) >> 2;

	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag = (address) >> 2;
	// --- end of bit manipulations for direct mapping ---

	int cacheAddr0 = ((cblocks[0].tag << 2) |  wordIndx);
	int cacheAddr1 = ((cblocks[1].tag << 2) |  wordIndx);
	int cacheAddr2 = ((cblocks[2].tag << 2) |  wordIndx);
	int cacheAddr3 = ((cblocks[3].tag << 2) |  wordIndx);
	int cacheAddr4 = ((cblocks[4].tag << 2) |  wordIndx);
	int cacheAddr5 = ((cblocks[5].tag << 2) |  wordIndx);
	int cacheAddr6 = ((cblocks[6].tag << 2) |  wordIndx);
	int cacheAddr7 = ((cblocks[7].tag << 2) |  wordIndx);

	int tempLast = cblocks[0].last_used;
	int tempLastIndex = 0;
	for (int i = 0; i < 8; i++)
	{
		if (cblocks[i].last_used < tempLast)
			tempLastIndex = i;
	}
	if (address == cacheAddr0)
	{
		clockX += 2;//access Cache

		cblocks[0].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[0].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr1)
	{
		clockX += 2;//access Cache

		cblocks[1].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[1].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr2)
	{
		clockX += 2;//access Cache

		cblocks[2].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[2].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr3)
	{
		clockX += 2;//access Cache

		cblocks[3].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[3].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr4)
	{
		clockX += 2;//access Cache

		cblocks[4].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[4].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr5)
	{
		clockX += 2;//access Cache

		cblocks[5].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[5].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr6)
	{
		clockX += 2;//access Cache

		cblocks[6].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[6].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr7)
	{
		clockX += 2;//access Cache

		cblocks[7].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[7].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}

	else
	{
		numMisses++;
		clockX += 2;//Cache was accessed to check for it

		//write to the correct address from MainMemory to caches
		clockX += 100; //access MainMemory
		MainMemory.blocks[temp].data[wordIndx] = value;
		MainMemory.blocks[temp].tag = curTag;
		MainMemory.blocks[temp].valid = 1;
		MainMemory.blocks[temp].last_used++;

		// then move the block to the cached block
		cblocks[tempLastIndex].tag = MainMemory.blocks[temp].tag;
		cblocks[tempLastIndex].valid = MainMemory.blocks[temp].valid;
		cblocks[tempLastIndex].last_used++;
		for (int i = 0; i<WORDS_PER_BLOCK; i++)
		{
			cblocks[tempLastIndex].data[i] = MainMemory.blocks[temp].data[i];
		}
		// both cache and main memory were accessed to copy from MainMemory to cache
		clockX += 2;
		clockX += 100;
	}
}
