
#include "TaskManager.h"

/**
 * @brief Class managing tasks assigned to multiple persons.
 */

TaskManager::TaskManager() : id(0) {}

void TaskManager::assignTask(const std::string &personName, const Task &task) {
    Task taskCopy = task;
    taskCopy.setId(id++);

    for (int i = 0; i < MAX_PERSONS; ++i) {
        if (person[i].getName() == personName) {
            person[i].assignTask(taskCopy);  // Assign the modified copy
            return;
        }
        if (person[i].getName() == "") {
            person[i] = Person(personName);
            person[i].getName() = personName;
            person[i].assignTask(taskCopy);  // Assign the modified copy
            return;
        }
    }
    throw std::runtime_error("Employer's List is Full!");
}


void TaskManager::completeTask(const string &personName) {
    for (int i = 0; i < MAX_PERSONS; ++i) {
        if (person[i].getName() == "") {
            break;
        }
        if (person[i].getName() == personName) {
            person->completeTask();
        }
    }
}


void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    auto changePriority = [type, priority](const Task &task) {
        int tempPriority = task.getPriority();
        if (task.getType() == type) {
            tempPriority += priority;
            if (tempPriority > 100) {
                tempPriority = 100;
            } else if (tempPriority < 0) {
                tempPriority = 0;
            }
        }
        Task finalTask (tempPriority, task.getType(), task.getDescription());
        finalTask.setId(task.getId());
        return finalTask;
    };
    for (int i = 0; i < MAX_PERSONS; ++i) {
        if (person[i].getName() == "") {
            break;
        }
        const SortedList<Task> &currentTask = person[i].getTasks();
        const SortedList<Task> tempTask = currentTask.apply(changePriority);
        person[i].setTasks(tempTask);
    }
}
/**
 * @brief Completes the highest priority task assigned to a person.
 *
 * @param personName The name of the person who will complete the task.
 */


/**
 * @brief Bumps the priority of all tasks of a specific type.
 *
 * @param type The type of tasks whose priority will be bumped.
 * @param priority The amount by which the priority will be increased.
 */


/**
 * @brief Prints all employees and their tasks.
 */
void TaskManager::printAllEmployees() const {
    for (int i = 0; i < MAX_PERSONS; ++i) {
        if (person[i].getName() == "") {
            break;
        }
        std::cout << person[i] << std::endl;
    }
}

/**
 * @brief Prints all tasks of a specific type.
 *
 * @param type The type of tasks to be printed.
 */
void TaskManager::printAllTasks() const {
    SortedList<Task> totalTasksList;
    for (int i = 0; i < MAX_PERSONS; i++) {
        if (person[i].getName() == "") {
            break;
        }
        const SortedList<Task> &tasksList = person[i].getTasks();
        for (const Task &task: tasksList) {
            totalTasksList.insert(task);
        }
    }

    for (const Task &task: totalTasksList) {
        std::cout << task << std::endl;
    }
}


void TaskManager::printTasksByType(TaskType type) const{
    SortedList<Task> filteredTasksList;
    auto typeFilter = [type](const Task &task) ->bool {
        return task.getType() == type;
    };
    for (int i = 0; i < MAX_PERSONS; ++i) {
        if(person[i].getName() == "") {
            break;
        }
        Person employee = person[i];
        const SortedList<Task> tasksList = employee.getTasks();
        SortedList<Task> filteredTasks = tasksList.filter(typeFilter);
        for (const Task& task : filteredTasks) {
            filteredTasksList.insert(task);
        }
    }

    for (const Task& task : filteredTasksList) {
        std::cout << task << std::endl;
    }
}