#ifndef QUEUE_H
#define QUEUE_H

#include "rift.h"
#include "./stdlibs/clogger.h"
#include <stdarg.h>
/*
* DO NOT TOUCH ANYTHING
* this is all in assembly and fucking around in queue.h or .c will break it
* then I WILL BREAK YOU
*/
#define MAX_QUEUE_ARGS		8	
typedef void (*QUEUE_FUNCTION)(...);

class	char_data;
typedef struct char_data CHAR_DATA;
class CQueue
{
public:
	CQueue();
	~CQueue();
	
	static void 	ProcessQueue(void);
	static void		AddToQueue(int nTimer, int nArgs, ...);
	static bool		HasQueuePending(void *qChar);	
	static void		DeleteQueuedEventsInvolving(void *qChar);

	/// Adds a function pointer to the queue with related arguments. The function will be executed at the specified time (in ticks).
	/// @tparam Func: Template type of the function to call. 
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param nTimer: The time (in ticks) which the function executes.
	/// @param func: The function to execute at the specified time.
	/// @param ...args: Variadic arguments used to call the function.
	template<class Func, class... Args>
	void AddToNewQueue(int nTimer, Func func, Args &&...args)
	{
		if(nTimer < 0)
			Logger.Warn("Negative Queue Timer - NumArgs: {}", sizeof...(Args));	

		// capture parameter pack
		auto tuple = std::tuple<Args...>(args...);
		auto chs = GetCharacterData(tuple);

		// place on queue
		auto qtip = std::make_tuple(nTimer, chs, [&]() { std::apply(func, tuple); });
		newQueue.push_back(qtip);
	}

	/// Processes all items on the queue. Any entry that has a timer of zero gets executed.
	/// Once all items are processed, those functions that have executed are removed from the queue.
	void ProcessNewQueue()
	{
		newQueue.erase(
			std::remove_if(newQueue.begin(), newQueue.end(), [](const auto& item)
			{
				auto delay = std::get<0>(item); 
				if (delay < 0)
					return true;

				if (--delay == 0)
				{
					auto func = std::get<2>(item);
					func();
					return true;
				}

				return false;
			}), newQueue.end());
	}

	/// Determines if a specific character has any remaining entries in the queue.
	/// This function applies to both directions (eg. either character being affected or is affecting another pc or environment).
	/// @param qChar: The character to lookup in the queue.
	/// @return true if there are entries related to the character in the queue; otherwise false.
	bool HasNewQueuePending(CHAR_DATA *qChar)
	{
		for (auto& q : newQueue)
		{
			auto delay = std::get<0>(q);
			auto v = std::get<1>(q);
			auto contains = std::find(v.begin(), v.end(), qChar) != v.end();
			if (contains && delay > 0)
				return true;
		}

		return false;
	}

	/// Deletes all entries in the queue pertaining to the specified character.
	/// @param qChar: The character to lookup in the queue.
	void DeleteNewQueuedEventsInvolving(CHAR_DATA *qChar)
	{
		int deleted = 0;
		newQueue.erase(
			std::remove_if(newQueue.begin(), newQueue.end(), [&](const auto& item)
			{
				auto delay = std::get<0>(item);
				auto v = std::get<1>(item);
				auto contains = std::find(v.begin(), v.end(), qChar) != v.end();
				if (contains && delay > 0)
				{
					deleted++;
					return true;
				}

				return false;
			}), newQueue.end());

		Logger.Warn("{} events deleted.", deleted);
	}
private:
	void			FreeQueue();

	inline static CLogger Logger = CLogger();
	static CQueue *	queue_first;
	CQueue *		queue_next;
	QUEUE_FUNCTION 	queue_function;
	int				queue_delay;				/* seconds _remaining_ */
	void *			queue_args[MAX_QUEUE_ARGS];	/* Queue function args */
	int				queue_numargs;
	std::vector<std::tuple<int, std::vector<CHAR_DATA*>, std::function<void()>>> newQueue;

	/// Helper function used to extract character data from the specfied tuple.
	/// @note Main use is for extracting character data sent to the AddToQueue method.
	/// @tparam ...Tp: Template parameter pack used to specify the variadic arguments. 
	/// @param t: Variadic arguments containing the values of the tuple.
	/// @return A list containing character data from the tuple.
	template<class... Tp>
	std::vector<CHAR_DATA*> GetCharacterData(std::tuple<Tp...>& t)
	{
		std::vector<CHAR_DATA*> accumulator;

		std::apply([&] (const auto&... tupleArgs) 
		{
			size_t index = 0;
			auto processTuple = [&](const auto& x)
			{
				if constexpr (std::is_same<decltype(x), CHAR_DATA*>::value)
					accumulator.push_back(x);
				index++;
			};
			(processTuple(tupleArgs), ...);
		}, t);

		return accumulator;
	}
};

#endif /* QUEUE_H */
