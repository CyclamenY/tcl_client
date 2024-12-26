#ifndef EVENT_H_H
#define EVENT_H_H

class Event{
public:
  Event(int num) : num_(num) {};
  int getNum();
private:
  int num_;
};

#endif