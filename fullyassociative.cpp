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
	int cacheAddr = ((cblocks[blockIndx % 2].tag << 5) | (blockIndx << 2) | wordIndx);
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
void Cache::putTwoWay(int address, int value)
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
		MainMemory.blocks[temp].last_used++;

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
int Cache::getFully(int address)
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
	int cacheAddr0 = ((cblocks[0].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr1 = ((cblocks[1].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr2 = ((cblocks[2].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr3 = ((cblocks[3].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr4 = ((cblocks[4].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr5 = ((cblocks[5].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr6 = ((cblocks[6].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr7 = ((cblocks[7].tag << 5) | (blockIndx << 2) | wordIndx);

	int value; // value in block line
	if (address == cacheAddr0)
	{
		clockX += 2;// cache access for read
		value = cblocks[0].data[wordIndx];
	}
	else if (address == cacheAddr1)
	{
		clockX += 4;// cache access for read
		value = cblocks[1].data[wordIndx];
	}
	else if (address == cacheAddr2)
	{
		clockX += 6;// cache access for read
		value = cblocks[2].data[wordIndx];
	}
	else if (address == cacheAddr3)
	{
		clockX += 8;// cache access for read
		value = cblocks[3].data[wordIndx];
	}
	else if (address == cacheAddr4)
	{
		clockX += 10;// cache access for read
		value = cblocks[4].data[wordIndx];
	}
	else if (address == cacheAddr5)
	{
		clockX += 12;// cache access for read
		value = cblocks[5].data[wordIndx];
	}
	else if (address == cacheAddr6)
	{
		clockX += 14;// cache access for read
		value = cblocks[6].data[wordIndx];
	}
	else if (address == cacheAddr7)
	{
		clockX += 16;// cache access for read
		value = cblocks[7].data[wordIndx];
	}
	else
	{
		numMisses++;
		clockX += 16; //cache accessed for read x8


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
void Cache::putFully(int address, int value)
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

	int cacheAddr0 = ((cblocks[0].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr1 = ((cblocks[1].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr2 = ((cblocks[2].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr3 = ((cblocks[3].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr4 = ((cblocks[4].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr5 = ((cblocks[5].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr6 = ((cblocks[6].tag << 5) | (blockIndx << 2) | wordIndx);
	int cacheAddr7 = ((cblocks[7].tag << 5) | (blockIndx << 2) | wordIndx);

	if (address == cacheAddr0)
	{
		clockX += 2;//access Cache

		cblocks[0].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr1)
	{
		clockX += 4;//access Cache

		cblocks[1].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr2)
	{
		clockX += 6;//access Cache

		cblocks[2].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr3)
	{
		clockX += 8;//access Cache

		cblocks[3].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr4)
	{
		clockX += 10;//access Cache

		cblocks[4].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr5)
	{
		clockX += 12;//access Cache

		cblocks[5].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr6)
	{
		clockX += 14;//access Cache

		cblocks[6].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}
	else if (address == cacheAddr7)
	{
		clockX += 16;//access Cache

		cblocks[7].data[wordIndx] = value;

		// Update MainMemory block with all the Cache Memory block info
		MainMemory.blocks[temp].data[wordIndx] = cblocks[blockIndx].data[wordIndx];
		// MainMemory accessed
		clockX += 100;
	}

	else
	{
		numMisses++;
		clockX += 16;//Cache was accessed to check for it

					 //write to the correct address from MainMemory to caches
		clockX += 100; //access MainMemory
		MainMemory.blocks[temp].data[wordIndx] = value;
		MainMemory.blocks[temp].tag = curTag;
		MainMemory.blocks[temp].valid = 1;
		MainMemory.blocks[temp].last_used++;

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
