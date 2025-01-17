#include "../job.hpp"
#include <cassert>

#define S_ASSERT(x) if (!(x)) { printf("Test failed %d\n", __LINE__); assert(false); }

class JobTester {
public:
    void TestSort() {
        Job job;
        std::shared_ptr<NumberGeneratorTask> number_gen_task1 = std::make_shared<NumberGeneratorTask>();
        std::shared_ptr<NumberGeneratorTask> number_gen_task2 = std::make_shared<NumberGeneratorTask>();
        job.AddTask(number_gen_task1, "gen1");
        job.AddTask(number_gen_task2, "gen2");

        std::shared_ptr<AddTask> add_task = std::make_shared<AddTask>();
        job.AddTask(add_task, "add");

        job.ContactMap("add", {"gen1", "gen2"}, {});

        std::shared_ptr<PrintNumberTask> print_task = std::make_shared<PrintNumberTask>();
        job.AddTask(print_task, "print");
        job.ContactMap("print", {"add"}, {});

        /// Sort
        job.SortTasks();
        S_ASSERT(job.current_tasks_.size() == 2);
        /// Execute
        for (auto& task : job.current_tasks_) {
            task->Execute();
            task->is_done = true;
        }
        job.UpdateStatus();
        S_ASSERT(job.finished_tasks_num_ == 2);
        /// Check
        S_ASSERT(job.current_tasks_.size() == 0);
        job.SortTasks();
        S_ASSERT(job.current_tasks_.size() == 1);
        job.UpdateStatus();

        auto cur_task = job.current_tasks_;
        for (auto i : cur_task) {
            i->Execute();
            i->is_done = true;
        }

        job.UpdateStatus();

        job.SortTasks();
        S_ASSERT(job.current_tasks_.size() == 1);
        for (auto i : job.current_tasks_) {
            i->Execute();
            i->is_done = true;
        }

        job.UpdateStatus();
        S_ASSERT(job.finished_tasks_num_ == 4);
        S_ASSERT(job.IsFinished());
    }

};

int main() {
    JobTester tester;
    tester.TestSort();
    return 0;
}