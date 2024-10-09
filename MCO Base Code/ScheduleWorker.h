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

private:
    std::vector<std::shared_ptr<Process>> processList;
};

