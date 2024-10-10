#pragma once
#include <vector>
#include <memory>
#include "Process.h" 

class ScheduleWorker {
public:
    ScheduleWorker();
    ~ScheduleWorker();

    void addProcess(std::shared_ptr<Process> process);
    void executeProcess();
    void displaySchedule() const;

    std::vector<std::shared_ptr<Process>> schedulerQueue;

    size_t cores = 4;
    // cores list
    std::shared_ptr<Process> coreList[4];

private:
    std::vector<std::shared_ptr<Process>> processList;
};

