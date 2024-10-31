#pragma once
#include <vector>
#include <memory>
#include <deque>
#include "Process.h" 

class ScheduleWorker {
public:
    ScheduleWorker();
    ~ScheduleWorker();

    void initialize(int numCores);
    static void addProcess(std::shared_ptr<Process> process);
    static void addWaitProcess(std::shared_ptr<Process> process);
    void scheduleProcess(std::shared_ptr<Process> process);
    void displaySchedule() const;

    static int usedCores;                 
    static int availableCores;              
   // std::vector<std::shared_ptr<Process>> schedulerQueue;
    static std::vector<int> cores;
    int assignCore();

private:
    int coreAssigned;
    int schedulerCurCycle;
    void initializeCores(int numCores);
    void assignCoreToProcess(std::shared_ptr<Process> process, int coreIndex);

    static std::vector<std::shared_ptr<Process>> processList;
    static std::vector<std::shared_ptr<Process>> waitingQueue;
    std::vector<std::shared_ptr<Process>> schedulerQueue;
};
