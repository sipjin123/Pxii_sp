#include "Data/PxiiHitFeedbackData.h"

const FHitFeedbackEntry* UPxiiHitFeedbackData::GetFeedback(EHitFeedbackType Type) const
{
	return FeedbackEntries.Find(Type);
}

const bool UPxiiHitFeedbackData::HasFeedback(EHitFeedbackType Type) const
{
	if(FeedbackEntries.IsEmpty())
	{
		return false;
	}
	
	return FeedbackEntries.Contains(Type);
}
