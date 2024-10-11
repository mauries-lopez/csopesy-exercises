#pragma once
#include <vector>
#include <memory>
#include "Process.h" 

class ScheduleWorker {
public:
    ScheduleWorker();
    ~ScheduleWorker();

    void initialize(int numCores);
    static void addProcess(std::shared_ptr<Process> process);
    static void addWaitProcess(std::shared_ptr<Process> process);
    void scheduleProcess();
    void displaySchedule() const;

    std::vector<std::shared_ptr<Process>> schedulerQueue;
    static std::vector<int> cores;
    int coreAssigned;

private:
    std::mutex mtx;
    void initializeCores(int numCores);
    static std::vector<std::shared_ptr<Process>> processList;
    static std::vector<std::shared_ptr<Process>> waitingQueue;
};

