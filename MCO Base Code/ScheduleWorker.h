#pragma once
#include <vector>
#include <memory>
#include "Process.h" 

class ScheduleWorker {
public:
    ScheduleWorker();
    ~ScheduleWorker();

    void addProcess(std::shared_ptr<Process> process);
    void executeProcesses();
    void displaySchedule() const;

private:
    std::vector<std::shared_ptr<Process>> processList;
};

