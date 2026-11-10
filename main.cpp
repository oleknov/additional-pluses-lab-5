#include <iostream>
#include <syncstream>
#include <future>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

void compute(const string& name, int seconds)
{
    this_thread::sleep_for(chrono::seconds(seconds));
    osyncstream(cout) << name << endl;
}

void slow(const string& name)
{
    compute(name, 7);
}

void quick(const string& name)
{
    compute(name, 1);
}

void sideBranch()
{
    slow("C2");
    quick("D2");
}

void mainBranch(future<void>& side)
{
    quick("A");
    slow("B");
    quick("C1");
    side.get();
    quick("D1");
    quick("F");
}

void work()
{
    auto begin = chrono::steady_clock::now();

    future<void> side = async(launch::async, sideBranch);
    mainBranch(side);

    auto end = chrono::steady_clock::now();
    double seconds = chrono::duration<double>(end - begin).count();

    osyncstream(cout) << "Time: " << seconds << " s" << endl;
    osyncstream(cout) << "Work is done!" << endl;
}

int main()
{
    work();
    return 0;
}