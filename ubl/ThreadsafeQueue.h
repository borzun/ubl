#pragma once

#include <memory>
#include <deque>
#include <mutex>
#include <condition_variable>

template <class Type>
class ThreadsafeQueue
{
public:
	ThreadsafeQueue();
	~ThreadsafeQueue();

	bool push(Type&& value);
	Type pop();

	bool waitTillPop(Type & out);
	
	Type top() const;
	bool isEmpty() const;
	bool contains(const Type& value) const;

	void invalidate();

private:
	mutable std::mutex m_mutex;

	bool m_isValid = true;

	std::deque<Type> m_queue;
	std::condition_variable m_conditional;
};


template<class Type>
inline ThreadsafeQueue<Type>::ThreadsafeQueue()
{
}

template<class Type>
inline ThreadsafeQueue<Type>::~ThreadsafeQueue()
{
	if (!m_isValid) {
		invalidate();
	}
}

template<class Type>
inline bool ThreadsafeQueue<Type>::push(Type && value)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_queue.push_back(value);

	m_conditional.notify_one();

	return true;
}

template<class Type>
inline Type ThreadsafeQueue<Type>::pop()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	Type ret = std::move(m_queue.back());
	m_queue.pop_back();
	return ret;
}

template<class Type>
inline bool ThreadsafeQueue<Type>::waitTillPop(Type & out)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	
	// Wait till queue is empty - than process it:
	while (m_queue.empty() && m_isValid) {
		m_conditional.wait(lock);
	}

	// If the queue is destroying - return from func
	if (!m_isValid) {
		return false;
	}

	// Get actual element:
	out = std::move(m_queue.back());
	m_queue.pop_back();

	return true;
}

template<class Type>
inline Type ThreadsafeQueue<Type>::top() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_queue.back();
}

template<class Type>
inline bool ThreadsafeQueue<Type>::isEmpty() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_queue.empty();
}

template<class Type>
inline bool ThreadsafeQueue<Type>::contains(const Type & value) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto iter = std::find(m_queue.begin(), m_queue.end(), value);
	return iter != m_queue.end();
}

template<class Type>
inline void ThreadsafeQueue<Type>::invalidate()
{
	// Invalidate thread safe queue:
	m_isValid = false;
	m_conditional.notify_all();
}
