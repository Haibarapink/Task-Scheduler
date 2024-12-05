#pragma once

#include <vector>
#include <string>
#include <map>
#include "task.hpp"

class Job {
    friend class JobTester;
protected:
    std::vector<std::shared_ptr<ITask>> tasks_;
    std::map<std::string , size_t> task_map_;
    std::map<std::string , std::vector<std::string>> contact_map_;

    std::vector<std::shared_ptr<ITask>> current_tasks_;

    size_t finished_tasks_num_;
private:
    inline std::shared_ptr<ITask> GetTask(const std::string& name) {
        return tasks_[task_map_[name]];
    }

    /// Update the status of the current tasks
    void UpdateStatus();
    /// Sort the tasks
    void SortTasks();
public:
    void AddTask(std::shared_ptr<ITask> task, const std::string& task_name);
    void ContactMap(const std::string& name, const std::vector<std::string>& prev, const std::vector<std::string>& next);

    inline bool IsFinished() {
        return finished_tasks_num_ == tasks_.size();
    }
};

inline void Job::UpdateStatus() {
    std::vector<size_t> remove_index;
    size_t p = 0;
    for (auto &task : current_tasks_) {
        p++;
        if (task->is_done) {
            /// 添加result到下一个task
            for (auto&& next : contact_map_[task->name]) {
                auto next_task = GetTask(next);
                next_task->AppendPrevResult(task->result_);
            }

            finished_tasks_num_++;
            contact_map_.erase(task->name);
            remove_index.push_back(p);
        }
    }

    for (size_t i = 0 ; i < remove_index.size(); ++i) {
        current_tasks_.erase(current_tasks_.begin() + remove_index[i]);
    }
}

inline void Job::SortTasks() {
    if (IsFinished()) {
        return;
    }

    std::vector<int> prev_task_count(tasks_.size(), 0);

    for (auto&& [name, nexts] : contact_map_) {
        for (auto && next : nexts) {
            prev_task_count[task_map_[next]]++;
        }
    }

    for (auto i = 0; i < prev_task_count.size(); ++i) {
        if (prev_task_count[i] == 0 && tasks_[i]->is_done == false) {
            current_tasks_.push_back(tasks_[i]);
        }
    }
}



inline void Job::AddTask(std::shared_ptr<ITask> task, const std::string& task_name) {
    task->name = task_name;
    tasks_.push_back(task);
    task_map_[task_name] = tasks_.size() - 1;
}

inline void Job::ContactMap(const std::string& name, const std::vector<std::string>& prev, const std::vector<std::string>& next) {
    contact_map_[name] = next;
    for (auto&& p : prev) {
        contact_map_[p].push_back(name);
    }
}