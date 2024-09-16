#include "queue.h"
#include "merc.h"

/// Processes all items on the queue. Any entry that has a timer of zero gets executed.
/// Once all items are processed, those functions that have executed are removed from the queue.
void CQueue::ProcessQueue()
{
	for (auto& item : newQueue)
	{
		if (--item.timer == 0)
		{
			Logger.Warn("Processing funcs ==> from {} func {} timer {}", item.callerFuncName, item.calleeFuncName, item.timer);

			auto func = item.function;
			func();
		}
	}

	newQueue.erase(
		std::remove_if(newQueue.begin(), newQueue.end(), [](const auto& item)
		{
			Logger.Warn("Processing delete ==> {}", item.timer);

			return item.timer < 0;
		}), newQueue.end());
}

/// Determines if a specific character has any remaining entries in the queue.
/// This function applies to both directions (eg. either character being affected or is affecting another pc or environment).
/// @param qChar: The character to lookup in the queue.
/// @return true if there are entries related to the character in the queue; otherwise false.
bool CQueue::HasQueuePending(CHAR_DATA *qChar)
{
	for (auto& item : newQueue)
	{
		auto delay = item.timer;
		auto v = item.charList;
		auto contains = std::find(v.begin(), v.end(), qChar) != v.end();

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
			auto delay = item.timer;
			auto v = item.charList;
			auto contains = std::find(v.begin(), v.end(), qChar) != v.end();

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
