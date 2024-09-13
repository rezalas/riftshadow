#include "queue.h"
#include "merc.h"

/// Processes all items on the queue. Any entry that has a timer of zero gets executed.
/// Once all items are processed, those functions that have executed are removed from the queue.
void CQueue::ProcessQueue()
{
	// newQueue.erase(
	// 	std::remove_if(newQueue.begin(), newQueue.end(), [](const auto& item)
	// 	{
	// 		auto delay = std::get<0>(item); 
	// 		if (delay < 0)
	// 			return true;

	// 		if (--delay == 0)
	// 		{
	// 			auto func = std::get<2>(item);
	// 			func();
	// 			return true;
	// 		}

	// 		return false;
	// 	}), newQueue.end());

	for (auto& item : newQueue)
	{
		// if (--std::get<0>(item) == 0)
		if (--item.timer == 0)
		{
			// Logger.Warn("Processing funcs ==> from {} func {} timer {}", std::get<1>(item), std::get<2>(item), std::get<0>(item));
			Logger.Warn("Processing funcs ==> from {} func {} timer {}", item.callerFuncName, item.calleeFuncName, item.timer);

			// auto func = std::get<4>(item);
			auto func = item.function;
			func();
		}
	}

	newQueue.erase(
		std::remove_if(newQueue.begin(), newQueue.end(), [](const auto& item)
		{
			// Logger.Warn("Processing delete ==> {}", std::get<0>(item));
			Logger.Warn("Processing delete ==> {}", item.timer);

			// return std::get<0>(item) < 0;
			return item.timer < 0;
		}), newQueue.end());


	// auto it = newQueue.begin();
	// while (it != newQueue.end())
	// {
	// 	Logger.Warn("Process size ==> {} / delay ==> {}", newQueue.size(), std::get<0>(*it));
	// 	//int ith = it - newQueue.begin();
	// 	//Logger.Warn("==> delay {}", std::get<0>(*it));
	// 	if (std::get<0>(*it) < 0) // remove rogue timer
	// 	{
	// 		it = newQueue.erase(it);
	// 		continue;
	// 	}

	// 	if (--std::get<0>(*it) == 0) // showtime
	// 	{
	// 		auto func = std::get<2>(*it);
	// 		func();

	// 		it = newQueue.erase(it);
	// 	}
	// 	else
	// 	{
	// 		it++;
	// 	}
	// }
}

/// Determines if a specific character has any remaining entries in the queue.
/// This function applies to both directions (eg. either character being affected or is affecting another pc or environment).
/// @param qChar: The character to lookup in the queue.
/// @return true if there are entries related to the character in the queue; otherwise false.
bool CQueue::HasQueuePending(CHAR_DATA *qChar)
{
	for (auto& item : newQueue)
	{
		// auto delay = std::get<0>(item);
		// auto v = std::get<3>(item);
		auto delay = item.timer;
		auto v = item.charList;
		auto contains = std::find(v.begin(), v.end(), qChar) != v.end();
		// auto contains = std::find_if(v.begin(), v.end(), [&](auto& ch)
		// {
		// 	Logger.Warn("==> {}", ch->name);
		// 	Logger.Warn("==> {}", qChar->name);
		// 	return strcmp(ch->name, qChar->name) == 0;
		// }) != v.end();

		Logger.Warn("==> Has queue {} {}", contains, delay);
		if (contains && delay > 0)
			return true;
	}

	return false;
}

/// Deletes all entries in the queue pertaining to the specified character.
/// @param qChar: The character to lookup in the queue.
void CQueue::DeleteQueuedEventsInvolving(CHAR_DATA *qChar)
{
	int deleted = 0;
	newQueue.erase(
		std::remove_if(newQueue.begin(), newQueue.end(), [&](const auto& item)
		{
			// auto delay = std::get<0>(item);
			// auto v = std::get<3>(item);
			auto delay = item.timer;
			auto v = item.charList;
			auto contains = std::find(v.begin(), v.end(), qChar) != v.end();
			// auto contains = std::find_if(v.begin(), v.end(), [&](auto& ch)
			// {
			// 	Logger.Warn("==> {}", ch->name);
			// 	Logger.Warn("==> {}", qChar->name);
			// 	 return strcmp(ch->name, qChar->name) == 0;
			// }) != v.end();

			Logger.Warn("==> Delete {} {}", contains, delay);
			if (contains && delay > 0)
			{
				deleted++;
				return true;
			}

			return false;
		}), newQueue.end());

	Logger.Warn("{} events deleted.", deleted);
}
