#pragma once
#include <vector>
#include <memory>
#include <deque>
#include "Process.h" 

class ScheduleWorker {
public:
    ScheduleWorker();
    ~ScheduleWorker();

    void addProcess(std::shared_ptr<Process> process);
    void executeProcess();
    void displaySchedule() const;
    void runProcess(int core);

    std::vector<std::shared_ptr<Process>> schedulerQueue;
    std::vector<std::shared_ptr<Process>> processList;

private:
    std::mutex mtx;

    
};

