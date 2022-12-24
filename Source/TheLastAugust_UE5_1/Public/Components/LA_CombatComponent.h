// The Last August. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LA_CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELASTAUGUST_UE5_1_API ULA_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULA_CombatComponent();
	void Attack();
	void UseSkill();
	void EndAttack();
	void InvertIsAttacking();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	int32 DamageAmount = 10;
	void PlayAnimMontage(UAnimMontage* Animation);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* AttackAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* SkillAnimation;
private:
	bool bIsAttacking = false;
};
