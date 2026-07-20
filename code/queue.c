#include "queue.h"
#include "merc.h"
#include <algorithm>
#include <iterator>

/// Processes all items on the queue. Any entry whose timer reaches zero gets executed.
/// Entries that ran, and entries cancelled while the loop was running, are swept afterwards.
///
/// The loop must not mutate newQueue while walking it. Additions are staged by AddToQueue
/// and merged below before iteration starts; removals are tombstones set on the entries
/// themselves. Between them, no reallocation and no erase can happen mid-iteration.
void CQueue::ProcessQueue()
{
	if (!stagedQueue.empty())
	{
		newQueue.insert(newQueue.end(),
			std::make_move_iterator(stagedQueue.begin()),
			std::make_move_iterator(stagedQueue.end()));

		stagedQueue.clear();
	}

	for (auto& item : newQueue)
	{
		// A callee earlier in this same loop may have cancelled this entry.
		if (item.cancelled)
			continue;

		// Changed to less-than-equals since we accept zero timers
		// and equals would ignore them.
		if (--item.timer <= 0)
		{
			item.executed = true;

			// We need a copied handle to the function so that it
			// can't get lost during queue processing.
			auto func = item.function;
			func();
		}
	}

	newQueue.erase(
		std::remove_if(newQueue.begin(), newQueue.end(), [](const auto& item)
		{
			return item.executed || item.cancelled;
		}), newQueue.end());
}

/// @return true if any live entry in the container names qChar.
bool CQueue::HasPendingIn(const std::vector<queueEntry_t>& entries, CHAR_DATA *qChar) const
{
	for (const auto& item : entries)
	{
		if (item.cancelled || item.executed)
			continue;

		const auto& v = item.charList;

		if (std::find(v.begin(), v.end(), qChar) != v.end())
			return true;
	}

	return false;
}

/// Determines if a specific character has any remaining entries in the queue.
/// This function applies to both directions (eg. either character being affected
/// or is affecting another pc or environment).
/// @param qChar: The character to lookup in the queue.
/// @return true if there are entries related to the character in the queue; otherwise false.
bool CQueue::HasQueuePending(CHAR_DATA *qChar)
{
	return HasPendingIn(newQueue, qChar) || HasPendingIn(stagedQueue, qChar);
}

/// Marks every live entry naming qChar as cancelled.
/// @return the number of entries tombstoned.
int CQueue::CancelEntriesInvolving(std::vector<queueEntry_t>& entries, CHAR_DATA *qChar)
{
	int cancelled = 0;

	for (auto& item : entries)
	{
		if (item.cancelled || item.executed)
			continue;

		const auto& v = item.charList;

		if (std::find(v.begin(), v.end(), qChar) != v.end())
		{
			item.cancelled = true;
			cancelled++;
		}
	}

	return cancelled;
}

/// Deletes all entries in the queue pertaining to the specified character.
/// Entries are tombstoned rather than erased so that we don't mutate the 
/// queue while it's being processed. Tombstoning also lets an event due
/// in the tick currently draining actually be cancelled, which an erase
/// could not do.
/// @param qChar: The character to lookup in the queue.
void CQueue::DeleteQueuedEventsInvolving(CHAR_DATA *qChar)
{
	auto cancelled = CancelEntriesInvolving(newQueue, qChar)
		+ CancelEntriesInvolving(stagedQueue, qChar);

	if (cancelled > 0)
		RS.Logger.Info("Cancelled {} queued event(s) involving character.", cancelled);
}
