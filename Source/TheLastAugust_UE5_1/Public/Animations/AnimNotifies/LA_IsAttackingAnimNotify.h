// The Last August. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LA_IsAttackingAnimNotify.generated.h"
DECLARE_MULTICAST_DELEGATE(FAttackingDelegate);

UCLASS()
class THELASTAUGUST_UE5_1_API ULA_IsAttackingAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
public:
	FAttackingDelegate AttackingDelegate;
};
