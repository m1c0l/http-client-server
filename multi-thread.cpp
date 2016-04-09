#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void
countdown(int id, int count)
{
  while (count > 0) {
    cout << "thread " << id << " is still alive." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    count--;
  }
  cout << "thread " << id << " is about to terminate." << std::endl;
}

void
noJoin()
{
  for (int i = 0; i < 3; i++) {
    thread(countdown, i+1, (i+1)*5).detach();
    // thread t(countdown, i+1, (i+1)*5); // Wrong! thread is not safely destroyed.
  }
  std::this_thread::sleep_for(std::chrono::seconds(20));
  cout << "main thread is about to terminate." << std::endl;
}

void
join()
{
  thread t[3];
  for (int i = 0; i < 3; i++) {
    t[i] = thread(countdown, i+1, (i+1)*5);
  }

  for (int i = 0; i < 3; i++) { // must join, otherwise threads will not be released safely.
    t[i].join();
  }
  cout << "main thread is about to terminate." << std::endl;
}

int main(int argc, char *argv[])
{
  noJoin();
  // join();
  return 0;
}
