#include "ThreadPool.h"

#include <thread>

ThreadPool & ThreadPool::getInstance()
{
	static ThreadPool pool;
	return pool;
}

ThreadPool::ThreadPool()
	: ThreadPool( std::thread::hardware_concurrency() )
{
}

ThreadPool::ThreadPool(size_t num_of_threads)
	: m_threadsCount(num_of_threads)
	, m_idleThreadsCount(num_of_threads)
	, m_isDestroying(false)
{
	for (auto i = 0u; i<m_threadsCount; ++i)
	{
		m_threads.emplace_back(std::thread(&ThreadPool::processJobQueue, this));
	}
}

ThreadPool::~ThreadPool()
{
	m_isDestroying = true;
	
	m_jobs.invalidate();

	for (auto & thread : m_threads) {
		thread.join();
	}
}

bool ThreadPool::isIdle() const noexcept
{
	return m_jobs.isEmpty() && m_idleThreadsCount == getThreadsCount();
}

void ThreadPool::processJobQueue()
{
	while (!m_isDestroying) {
		ThreadJob job;
		if (!m_jobs.waitTillPop(job)) {
			break; // queue is invalidated
		}
		
		--m_idleThreadsCount;
		
		// execute job
		job();

		++m_idleThreadsCount;
	}
}
