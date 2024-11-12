#pragma once

#include <vector>

class MemoryManager {
public:
	MemoryManager(long long maxOverallMem);

	static std::vector<int> memoryBlocks;

	static void prepareMemoryBlocks();

private:
	static long long maxOverallMemory;
};