#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace synchronized {
template<typename T> class queue {
  public:
    queue(unsigned int nMaxSize=0);
    virtual ~queue();

    void put(T e);
    T get();
    void task_done();
    void join();

  private:
    unsigned int m_nMaxSize;
    unsigned int m_nQueuedJobs;
    std::mutex m_AccessMutex;
    std::condition_variable m_getCondVar;
    std::condition_variable m_putCondVar;
    std::condition_variable m_joinCondVar;
    std::queue<T> m_Queue;
};
}

template<typename T>
synchronized::queue<T>::queue(unsigned int nMaxSize)
: m_nMaxSize(nMaxSize)
, m_nQueuedJobs(0)
{
}


template<typename T>
synchronized::queue<T>::~queue()
{
  // Acquire the lock before deleting this object
  std::unique_lock<std::mutex> lock(m_AccessMutex);
}

template<typename T>
void synchronized::queue<T>::put(T e)
{
  std::unique_lock<std::mutex> lock(m_AccessMutex);
  if (0 == m_nMaxSize || m_Queue.size() < m_nMaxSize) {
    m_Queue.push(e);
  }
  else {
    m_putCondVar.wait(lock, [&](){ return m_Queue.size() < m_nMaxSize; });
    m_Queue.push(e);
  }
  ++m_nQueuedJobs;
  m_getCondVar.notify_one();
}


template<typename T>
T synchronized::queue<T>::get()
{
  std::unique_lock<std::mutex> lock(m_AccessMutex);
  if (0 == m_Queue.size()) {
    m_getCondVar.wait(lock, [&](){ return m_Queue.size() > 0; });
  }
  T ret = m_Queue.front();
  m_Queue.pop();
  m_putCondVar.notify_one();
  return ret;
}


template<typename T>
void synchronized::queue<T>::task_done()
{
  std::unique_lock<std::mutex> lock(m_AccessMutex);
  if (--m_nQueuedJobs == 0) {
    m_joinCondVar.notify_all();
  }
}


template<typename T>
void synchronized::queue<T>::join()
{
  std::unique_lock<std::mutex> lock(m_AccessMutex);
  m_joinCondVar.wait(lock, [&](){ return m_nQueuedJobs == 0; });
}
