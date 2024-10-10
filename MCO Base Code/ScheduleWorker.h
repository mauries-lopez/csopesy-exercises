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
    void scheduleProcess();
    void displaySchedule() const;

    std::vector<std::shared_ptr<Process>> schedulerQueue;
   
private:
    std::mutex mtx;
    std::vector<int> cores;
    void initializeCores(int numCores);
    static std::vector<std::shared_ptr<Process>> processList;
};
