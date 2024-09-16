#ifndef QUEUE_H
#define QUEUE_H

#include "rift.h"
#include "./stdlibs/clogger.h"
#include <stdarg.h>
#include <iostream>

/*
* DO NOT TOUCH ANYTHING
* this is all in assembly and fucking around in queue.h or .c will break it
* then I WILL BREAK YOU
*/

// forward declarations. Once char_data is separated to its own file, we can get rid of these and include the file.
// but since these are defined in merc.h, this will have to do. We can't include merc.h because that causes a circular 
// dependency.
class	char_data;
typedef struct char_data CHAR_DATA;
class CQueue
{
public:
	/// Adds a function pointer to the queue with related arguments. The function will be executed at the specified time (in ticks).
	/// @tparam Func: Template type of the function to call. 
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param nTimer: The time (in ticks) which the function executes.
	/// @param from: The name of the function that placed funcName on the queue.
	/// @param funcName: The name of the function being placed on the queue.
	/// @param func: The function to execute at the specified time.
	/// @param ...args: Variadic arguments used to call the function.
	template<class Func, class... Args>
	void AddToQueue(int nTimer, std::string from, std::string funcName, Func func, Args &&...args)
	{
		if(nTimer < 0)
			Logger.Warn("Negative Queue Timer - NumArgs: {}", sizeof...(Args));	

		// print parameters
		//((std::cout << ' ' << args << std::endl), ...);

		// capture parameter pack
		auto tuple = std::tuple<Args...>(args...);
		auto chs = GetCharacterData(tuple);

		// place on queue
		//newQueue.push_back({nTimer, from, funcName, chs, [&]() { std::apply(func, std::forward_as_tuple(std::forward<Args>(args)...)); }});

		newQueue.push_back({nTimer, from, funcName, chs, [=] () mutable
		{
			std::apply(func, std::forward_as_tuple(std::forward<Args>(args)...));
		}});

		Logger.Warn("Add => {} added {} with timer {}", from, funcName, nTimer);
	}

	/// Processes all items on the queue. Any entry that has a timer of zero gets executed.
	/// Once all items are processed, those functions that have executed are removed from the queue.
	void ProcessQueue();

	/// Determines if a specific character has any remaining entries in the queue.
	/// This function applies to both directions (eg. either character being affected or is affecting another pc or environment).
	/// @param qChar: The character to lookup in the queue.
	/// @return true if there are entries related to the character in the queue; otherwise false.
	bool HasQueuePending(CHAR_DATA *qChar);

	/// Deletes all entries in the queue pertaining to the specified character.
	/// @param qChar: The character to lookup in the queue.
	void DeleteQueuedEventsInvolving(CHAR_DATA *qChar);
private:
	inline static CLogger Logger = CLogger();

	struct queueEntry_t
	{
		int timer;
		std::string callerFuncName;
		std::string calleeFuncName;
		std::vector<CHAR_DATA*> charList;
		std::function<void()> function;
	};

	// std::vector<std::tuple<int, std::string, std::string, std::vector<CHAR_DATA*>, std::function<void()>>> newQueue;
	std::vector<queueEntry_t> newQueue;

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
			auto processTuple = [&](const auto& x)
			{
				// decay x to get the actual argument type to compare to our character data type 
		 		if constexpr (std::is_same<typename std::decay<decltype(x)>::type,CHAR_DATA*>::value)
					accumulator.push_back(x);
			};
			(processTuple(tupleArgs), ...);  // fold expression - calls processTuple for each argument in tupleArgs
		}, t);

		return accumulator;
	}
};

#endif /* QUEUE_H */
