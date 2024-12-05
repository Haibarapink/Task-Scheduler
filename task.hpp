#pragma once

#include <atomic>
#include <memory>
#include <functional>
#include <utility>
#include <stdexcept>

class ITaskResult {};

class ITask {
protected:
    std::shared_ptr<ITaskResult> prev_result_;
    std::shared_ptr<ITaskResult> result_;
public:
    std::atomic_bool is_done {false};
    std::string name;

    virtual void Init(const std::vector<std::shared_ptr<ITaskResult>> &prev_result) = 0;
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
    void Init(const std::vector<std::shared_ptr<ITaskResult>> &prev_result) override {
        /// prev_result_ = prev_result;
    }
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
    void Init(const std::vector<std::shared_ptr<ITaskResult>> &prev_result) override {
        if (prev_result.size() != 2) {
            throw std::runtime_error("AddTask needs two previous results");
        }
        auto result1 = std::static_pointer_cast<NumberGeneratorResult>(prev_result[0]);
        auto result2 = std::static_pointer_cast<NumberGeneratorResult>(prev_result[1]);

        a_ = result1->number_;
        b_ = result2->number_;
    }

    void Execute() override {
        result_ = std::make_shared<NumberGeneratorResult>();
        auto result = std::static_pointer_cast<NumberGeneratorResult>(result_);
        result->number_ = a_ + b_;
    }
};

