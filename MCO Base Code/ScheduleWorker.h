#pragma once
#include <vector>
#include <memory>
#include <deque>
#include "Process.h" 
#include <mutex>  // Add this include to use std::mutex
#include <condition_variable>


class ScheduleWorker {
public:
    ScheduleWorker();
    ~ScheduleWorker();

    void initialize(int numCores);
    static void addProcess(std::shared_ptr<Process> process);
    static void addWaitProcess(std::shared_ptr<Process> process);
    void scheduleProcess();
    void roundRobin(int quantumCycles);
    void displaySchedule() const;

    static int usedCores;                 
    static int availableCores;              
    std::vector<std::shared_ptr<Process>> schedulerQueue;
    static std::vector<int> cores;
    int coreAssigned;

    static void testSchedule();
    static int schedulerCurCycle;

    // cycle counter for RR
    static int quantumCycleCounter;
    static int runningRRProcessCount;
    static std::vector<std::shared_ptr<Process>> runningRRProcessList;

    static bool stopTest;
    static std::mutex schedulerMutex;  // Add this line

    static std::vector<std::shared_ptr<Process>> processList;
    static std::vector<std::shared_ptr<Process>> waitingQueue;



private:

    void initializeCores(int numCores);
    std::condition_variable cv;
    std::mutex concurrentThread;
    int rrThreadsSize = 0;

};
