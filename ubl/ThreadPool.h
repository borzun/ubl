#pragma once

#include <map>
#include <future>
#include <functional>
#include <vector>

#include "ThreadsafeQueue.h"

class ThreadPool
{
	using ThreadJob = std::function<void(void)>;

public:

	static ThreadPool& getInstance();

	ThreadPool();
	explicit ThreadPool(size_t num_of_threads);
	~ThreadPool();

	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool& operator=(const ThreadPool& other) = delete;

	template <class FunctionType>
	auto run(FunctionType && func) noexcept;

	template <class FunctionType, class ... Args>
	auto run(FunctionType && func, Args && ... args) noexcept;

	size_t getThreadsCount() const noexcept;
	
	// Indicates that threre are no job to be done and pool is in idle mode:
	bool isIdle() const noexcept;

private:

	template <class FunctionType, class ... Args>
	ThreadJob makeThreadJob(FunctionType && func, Args && ... args) noexcept;

	void processJobQueue();

private:

	const size_t m_threadsCount;
	std::atomic<size_t> m_idleThreadsCount;

	std::atomic<bool> m_isDestroying;
	ThreadsafeQueue<ThreadJob> m_jobs;
	std::vector<std::thread> m_threads;

	std::map<std::thread::id, std::string> m_threadNames;

};

template<class FunctionType>
inline auto ThreadPool::run(FunctionType && func) noexcept
{
	auto threadJob = std::bind(std::forward<FunctionType>(func));
	m_jobs.push(std::forward<ThreadJob>(threadJob));
	return true;
}

template<class FunctionType, class ...Args>
inline auto ThreadPool::run(FunctionType && func, Args && ...args) noexcept
{
	auto threadJob = makeThreadJob(std::forward<FunctionType>(func), 
								   std::forward<Args...>(args...));
	m_jobs.push(std::forward<ThreadJob>(threadJob));
	return true;
}

template<class FunctionType, class ...Args>
inline ThreadPool::ThreadJob ThreadPool::makeThreadJob(FunctionType && func, Args && ...args) noexcept
{
	return std::bind(std::forward<FunctionType>(func), std::forward<Args...>(args...));
}

inline size_t ThreadPool::getThreadsCount() const noexcept
{
	return m_threadsCount;
}
