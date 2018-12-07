#pragma once

template <class T>
class EventHandler
{
public:
	virtual void handle(T* e) = 0;
};

