#include <iostream>
#include "MainConsole.h"
#include "MemoryManager.h"

std::vector<int> MemoryManager::memoryBlocks;
long long MemoryManager::maxOverallMemory = 0;

MemoryManager::MemoryManager(long long maxOverallMem) {
	this->maxOverallMemory = maxOverallMem;
}

void MemoryManager::prepareMemoryBlocks() {
	long long numBlocks = (MemoryManager::maxOverallMemory / MainConsole::memPerProcess);

	for (int i = numBlocks; i >= 0; i--) {
		if (i > 0) {
			MemoryManager::memoryBlocks.push_back(MainConsole::memPerProcess * i);
			//std::cerr << MemoryManager::memoryBlocks.back() << std::endl;
		}
	}
}