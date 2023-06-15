#ifndef _OBSERVER_PATTERN_IMPL_CPP_TEST_
#define _OBSERVER_PATTERN_IMPL_CPP_TEST_ 1

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "ObserverPattern.h"

template <typename T>
ObserverHandler<T>::ObserverHandler() {}

template <typename T>
void ObserverHandler<T>::notify(T notification)
{
  std::for_each(
    this->observers.begin(),
    this->observers.end(),
    [notification](Observer<T> *&observer)
  {
    observer->handleNotification(notification);
  });
}

template <typename T>
void ObserverHandler<T>::addObserver(Observer<T> *observer)
{
  auto found = std::find(this->observers.begin(), this->observers.end(), observer);
  if (found == this->observers.end())
  {
    this->observers.push_back(observer);
  }
}

template <typename T>
void ObserverHandler<T>::removeObserver(Observer<T> *observer)
{
  auto found = std::find(this->observers.begin(), this->observers.end(), observer);
  if (found != this->observers.end())
  {
    this->observers.erase(found);
  }
}

template <typename T>
Observer<T>::Observer() {}

template <typename T>
void Observer<T>::handleNotification(T notification)
{
  if (!this->callbacks.empty()) {
    std::for_each(
      this->callbacks.begin(),
      this->callbacks.end(),
      [notification](const std::pair<void*, std::function<CallbackType>>& callback)
      {
        callback.second(notification);
      }
    );
  }
}

template <typename T>
void Observer<T>::subscribe(ObserverHandler<T> *handler)
{
  if (this->handler != NULL)
  {
    this->unsubscribe();
  }
  this->handler = handler;
  this->handler->addObserver(this);
}

template <typename T>
void Observer<T>::unsubscribe()
{
  if (this->handler != NULL)
  {
    this->handler->removeObserver(this);
    this->handler = NULL;
  }
}

template <typename T>
void Observer<T>::addCallback(std::function<void(T)> cb, void* cptr)
{
  try
  {
      this->callbacks.at(cptr);
  }
  catch (std::out_of_range err)
  {
      this->callbacks[cptr] = cb;
  }
}

template <typename T>
void Observer<T>::removeCallback(void* cptr)
{
  try
  {
      this->callbacks.at(cptr);
      this->callbacks.erase(cptr);
  }
  catch(std::out_of_range err)
  {
      // nothing to remove
  }
}

template <typename T>
ObserverHandler<T>* Observer<T>::getHandler()
{
  return this->handler;
}

#endif