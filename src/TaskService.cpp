#include <ctime>
#include <vector>
#include <string>
#include "TaskService.hpp"
#include "TaskRepository.hpp"
#include "TaskModel.hpp"

task::Service::Service(): repository {} {}
std::tm task::Service::getTmNow() const {
    auto t = std::time(nullptr);
    auto now = *std::localtime(&t);
    std::mktime(&now);
    return now;
}
long task::Service::add(const std::string& description) {
    auto tasks {find("all")};
    task::Model task;
    task.setId(repository.loadNextTaskId());
    task.setDescription(description);
    task.setCreatedAt(getTmNow());
    tasks.push_back(task);
    if(repository.saveData(tasks)) {
        repository.updateNextTaskId(task.getId() + 1);
        return task.getId();
    }
    return 0;
}
bool task::Service::update(const long& id, const std::string& description) {
    auto tasks {task::Service::find("all")};
    for(auto& task : tasks) {
        if(task.getId() == id) {
            task.setDescription(description);
            task.setUpdatedAt(getTmNow());
            return repository.saveData(tasks);
        }
    }
    return false;
}
std::vector<task::Model> task::Service::find(const std::string& filter) const {
    return repository.loadData(std::regex {(filter == "all") ? "\"id\":\\d+" : "\"status\":\"" + filter + "\""});
}