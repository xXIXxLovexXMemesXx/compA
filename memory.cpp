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
    printf ("DIRECT MAPPED\n");
  else if (org == TWOWAY)
    printf ("TWO-WAY SET ASSOCIATIVE\n");
  else if (org == FULLY)
    printf ("FULLY ASSOCIATIVE\n");
  else
    printf ("ERROR: WRONG CACHE ORG\n");
}

int Cache::getData(int address)
{
	int data;
	
	if(cache_org == DIRECT)
		data=getDirect(address);
	else if(cache_org == TWOWAY)
		data = getTwoWay(address);
	else if (cache_org == FULLY)
		data = getFully(address);
	else
	{
		printf ("ERROR: WRONG CACHE ORG in getData \n");
		return 0;
	}
	
    
	return data;
}

void Cache::putData(int address, int value)
{
	if(cache_org == DIRECT)
		putDirect(address, value);
	else if(cache_org == TWOWAY)
		putTwoWay(address, value);
	else if (cache_org == FULLY)
		putFully(address, value);
	else
	{
		printf ("ERROR: WRONG CACHE ORG in putData \n");
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
	int blockIndx = (address & 0x0000001C)>>2;
  
	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag= (address)>>5;
	// --- end of bit manipulations for direct mapping ---
	
	// find the cached address
	int cacheAddr = ((cblocks[blockIndx].tag << 5) | (blockIndx << 2) | wordIndx);
	int value; // value in block line
	if (address == cacheAddr )
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
		
		for(int i = 0; i<WORDS_PER_BLOCK; i++)
		{
			cblocks[blockIndx].data[i]= MainMemory.blocks[temp].data[i];
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
	int blockIndx = (address & 0x0000001C)>>2;
  
	// tag = remaining bits (32-2-3 = 27 bits)
	int curTag= (address)>>5;
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
		MainMemory.blocks[temp].data[wordIndx]= value;
		MainMemory.blocks[temp].tag = curTag;
		MainMemory.blocks[temp].valid = 1;
				
		
		// then move the block to the cached block
		cblocks[blockIndx].tag = MainMemory.blocks[temp].tag;
		cblocks[blockIndx].valid = MainMemory.blocks[temp].valid;
		for(int i = 0; i<WORDS_PER_BLOCK; i++)
		{
			cblocks[blockIndx].data[i]= MainMemory.blocks[temp].data[i];
		}
		// both cache and main memory were accessed to copy from MainMemory to cache
		clockX += 2;
		clockX += 100;
	}
}
int Cache::getTwoWay(int address)
{
	return 0;
}
void Cache::putTwoWay(int address, int value)
{
	;
}
int Cache::getFully(int address)
{
	return 0;
}
void Cache::putFully(int address, int value)
{
	;
}
