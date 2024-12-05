#pragma once

#include <atomic>
#include <memory>
#include <functional>
#include <utility>
#include <stdexcept>

class ITaskResult {};

class ITask {
protected:
    friend class Job;
    std::vector<std::shared_ptr<ITaskResult>> prev_result_;
    std::shared_ptr<ITaskResult> result_;
public:
    std::atomic_bool is_done {false};
    std::string name;

    virtual void AppendPrevResult(std::shared_ptr<ITaskResult> result) {
        prev_result_.push_back(result);
    }
    virtual ~ITask() = default;
    virtual void Execute() = 0;
};

/// @brief Number Generator Task
class NumberGeneratorResult : public ITaskResult {
public:
    int number_;
};

class NumberGeneratorTask : public ITask {
    int number_;
public:

    void Execute() override {
        this->number_ = rand();
        result_ = std::make_shared<NumberGeneratorResult>();
        auto result = std::static_pointer_cast<NumberGeneratorResult>(result_);
        result->number_ = this->number_;
    }
};

/// @brief Task that adds two numbers
class AddTask : public ITask {
    int a_{};
    int b_{};
public:
    AddTask() = default;

    void Execute() override {
        if (prev_result_.size() != 2) {
            throw std::runtime_error("AddTask needs two previous results");
        }
        auto a = std::static_pointer_cast<NumberGeneratorResult>(prev_result_[0])->number_;
        auto b = std::static_pointer_cast<NumberGeneratorResult>(prev_result_[1])->number_;
        result_ = std::make_shared<NumberGeneratorResult>();
        auto result = std::static_pointer_cast<NumberGeneratorResult>(result_);
        result->number_ = a_ + b_;
    }
};

