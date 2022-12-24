// The Last August. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameFramework/DamageType.h"
#include "Templates/SubclassOf.h"
#include "LA_AttackTracer.generated.h"

struct FHitResult;
class AController;
/**
 * 
 */
UCLASS()
class THELASTAUGUST_UE5_1_API ULA_AttackTracer : public UAnimNotifyState
{
	GENERATED_BODY()
	TArray<FHitResult> HitResults;
	UPROPERTY()
	TArray<AActor*>HitActors;
	UPROPERTY()
	AController* EventInstigator;
	TSubclassOf<UDamageType>DamageTypeClass;
	FVector LastLocation1;
	FVector LastLocation2;
	UPROPERTY()
	class ATheLastAugust_UE5_1Character* Character;
	UPROPERTY()
	USkeletalMeshComponent* Weapon;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
