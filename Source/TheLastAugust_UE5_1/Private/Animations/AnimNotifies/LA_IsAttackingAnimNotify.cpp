// The Last August. All Rights Reserved.


#include "Animations/AnimNotifies/LA_IsAttackingAnimNotify.h"

void ULA_IsAttackingAnimNotify::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AttackingDelegate.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
