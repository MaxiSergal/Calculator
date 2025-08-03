#ifndef THREADSAFEQQUEUE_H
#define THREADSAFEQQUEUE_H

#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

template<typename T>
class ThreadSafeQQueue
{
    QQueue<T> queue_;
    QMutex mutex_;

    // Добавить конструкторы

  public:
    void enqueue(const T &item)
    {
      QMutexLocker lock(&mutex_);
      queue_.enqueue(item);
    }

    bool tryDequeue(T &out)
    {
      QMutexLocker lock(&mutex_);
      if(queue_.isEmpty())
        return false;
      out = queue_.dequeue();
      return true;
    }

    inline qsizetype size() { return queue_.size(); }
};

#endif // THREADSAFEQQUEUE_H
