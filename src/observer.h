#ifndef __OBSERVER_H__
#define __OBSERVER_H__

template <typename InfoType, typename StateType> class Subject;

template <typename InfoType, typename StateType> class Observer {
public:
  virtual void notify(Subject<InfoType, StateType> &whoFrom) = 0; // pass the >
  virtual ~Observer() = default;
};

#endif
