#include "Condition_Or.h"

bool UCondition_Or::CheckCondition(UObject* _Target) const
{
	return (Condition_Left_ && Condition_Left_->CheckCondition(_Target))
		|| (Condition_Right_ && Condition_Right_->CheckCondition(_Target));
}
