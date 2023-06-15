#ifndef _OBSERVER_PATTERN_CPP_TEST_
#define _OBSERVER_PATTERN_CPP_TEST_ 1

#include <vector>

template <typename T>
class ObserverHandler;
template <typename T>
class Observer;

template <typename T>
class ObserverHandler
{
  std::vector<Observer<T> *> observers;

public:
  ObserverHandler();
  void notify(T notification);
  void addObserver(Observer<T> *observer);
  void removeObserver(Observer<T> *observer);
};

template <typename T>
class Observer
{
  using CallbackType = void(T);
  std::map<void*, std::function<CallbackType>> callbacks;
  ObserverHandler<T> *handler = NULL;

  void handleNotification(T notification);
  friend void ObserverHandler<T>::notify(T notification);

public:
  Observer();
  void subscribe(ObserverHandler<T> *handler);
  void unsubscribe();
  void addCallback(std::function<void(T)> cb, void* cptr);
  void removeCallback(void* cptr);
  ObserverHandler<T>* getHandler();
};

#include "ObserverPattern_impl.h"

#endif