#include "../job.hpp"
#include <cassert>

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

        /// Sort
        job.SortTasks();

        if (job.current_tasks_.size() != 2) {
            printf("TestSort failed\n");
            assert(false);
        }

        /// Execute



    }

};

int main() {
    JobTester tester;
    tester.TestSort();
    return 0;
}