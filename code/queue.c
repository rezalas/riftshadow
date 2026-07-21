#include "queue.h"
#include "merc.h"
#include <algorithm>

/// Registers a new entry against every character it names.
void CQueue::RegisterEntry(const std::vector<CHAR_DATA*>& chars, entryRef ref)
{
	for (auto *ch : chars)
	{
		if (ch != nullptr)
			charIndex[ch].push_back(ref);
	}
}

/// Removes an entry's refs from every character it names.
/// Done for all of the entry's characters, not just the one being cancelled, so that
/// charIndex only ever holds refs to entries that can still run.
void CQueue::DropEntryRefs(const queueEntry_t& entry, entryRef ref)
{
	for (auto *ch : entry.charList)
	{
		auto it = charIndex.find(ch);

		if (it == charIndex.end())
			continue;

		auto& refs = it->second;
		refs.erase(std::remove(refs.begin(), refs.end(), ref), refs.end());

		if (refs.empty())
			charIndex.erase(it);
	}
}

/// @return the entry a ref points at, or nullptr if its bucket is already gone.
CQueue::queueEntry_t* CQueue::ResolveEntry(entryRef ref)
{
	auto it = buckets.find(ref.tick);

	if (it == buckets.end() || ref.index >= it->second.size())
		return nullptr;

	return &it->second[ref.index];
}

/// Processes everything due on this tick. Entries execute in the order they were queued.
///
/// The due bucket stays in the map while it drains, so a callee can still cancel entries
/// due in this same tick. Nothing can be appended to it — AddToQueue always schedules at
/// least one tick ahead — so the loop cannot be reallocated out from under itself.
void CQueue::ProcessQueue()
{
	currentTick++;

	while (!buckets.empty() && buckets.begin()->first <= currentTick)
	{
		auto tick = buckets.begin()->first;
		auto& due = buckets.begin()->second;

		for (size_t i = 0; i < due.size(); i++)
		{
			auto& item = due[i];

			// A callee earlier in this same bucket may have cancelled this entry.
			if (item.cancelled)
				continue;

			// Unregister before running: an entry that is executing is no longer
			// pending, and must not be reachable for cancellation.
			DropEntryRefs(item, {tick, i});

			// We need a copied handle to the function so that it
			// can't get lost during queue processing.
			auto func = item.function;
			func();
		}

		buckets.erase(tick);
	}
}

/// Determines if a specific character has any remaining entries in the queue.
/// This function applies to both directions (eg. either character being affected
/// or is affecting another pc or environment).
/// @param qChar: The character to lookup in the queue.
/// @return true if there are entries related to the character in the queue; otherwise false.
bool CQueue::HasQueuePending(CHAR_DATA *qChar)
{
	auto it = charIndex.find(qChar);

	return it != charIndex.end() && !it->second.empty();
}

/// Deletes all entries in the queue pertaining to the specified character.
/// Entries are tombstoned rather than erased so that we don't mutate the
/// queue while it's being processed. Tombstoning also lets an event due
/// in the tick currently draining actually be cancelled, which an erase
/// could not do.
/// @param qChar: The character to lookup in the queue.
void CQueue::DeleteQueuedEventsInvolving(CHAR_DATA *qChar)
{
	auto it = charIndex.find(qChar);

	if (it == charIndex.end())
		return;

	// Copied because cancelling mutates charIndex, including this character's own refs.
	auto refs = it->second;
	int cancelled = 0;

	for (auto ref : refs)
	{
		auto *item = ResolveEntry(ref);

		if (item == nullptr || item->cancelled)
			continue;

		item->cancelled = true;
		DropEntryRefs(*item, ref);
		cancelled++;
	}

	if (cancelled > 0)
		RS.Logger.Info("Cancelled {} queued event(s) involving character.", cancelled);
}
