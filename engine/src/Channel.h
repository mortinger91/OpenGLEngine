#pragma once

#include <list>
#include <thread>
#include <atomic>

#include "Mesh.h"

// TODO: change channel implementation so that the get() function is not blocking, but returns null if the list is empty
//template<class item>
class channel
{
private:
	std::list<std::shared_ptr<Mesh>> queue;
	std::mutex m;
	std::condition_variable cv;
public:
	void put(const std::shared_ptr<Mesh> &i)
	{
		std::unique_lock<std::mutex> lock(m);
		queue.push_back(i);
		cv.notify_one();
	}
	std::shared_ptr<Mesh> get()
	{
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, [&]() { return !queue.empty(); });
		std::shared_ptr<Mesh> result = queue.front();
		queue.pop_front();
		return result;
	}
};

class Channels
{
public:
	Channels() = delete;
	static channel meshChan;
	static std::atomic<int> toRead;
};