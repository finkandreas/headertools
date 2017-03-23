// g++ -std=c++11 -pthread -o queue_test queue_test.cpp

#include <iostream>
#include <unistd.h>
#include <thread>
#include <typeinfo>
#include "queue.h"

void read_from_queue(synchronized::queue<int>& q)
{
  while (1) {
    int i = q.get();
    q.task_done();
  }
}


void put_to_queue(synchronized::queue<int>& q)
{
  for (int i=0; i<1000; ++i) {
    q.put(i);
  }
}


int main(int argc, char** argv)
{
  synchronized::queue<int> q(300);

  // queue put threads
  std::vector<std::thread> put_threads;
  for (int i=0; i<10; ++i) {
    put_threads.push_back(std::thread(put_to_queue, std::ref(q)));
  }

  // queue get threads
  for (int i=0; i<10; ++i) {
    std::thread(read_from_queue, std::ref(q)).detach();
  }

  for (auto& t : put_threads) {
    t.join();
  }

  std::cout << "Joining the queue... " << std::endl;
  q.join();
  std::cout << "Queue joined..." << std::endl;

  return 0;
}
