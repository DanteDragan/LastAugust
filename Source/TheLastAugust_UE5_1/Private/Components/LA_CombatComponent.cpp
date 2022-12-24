// The Last August. All Rights Reserved.


#include "Components/LA_CombatComponent.h"
#include "GameFramework/Character.h"

ULA_CombatComponent::ULA_CombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void ULA_CombatComponent::Attack()
{
	if (!bIsAttacking)
	{
		PlayAnimMontage(AttackAnimation);
	}
}

void ULA_CombatComponent::UseSkill()
{
	PlayAnimMontage(SkillAnimation);
}

void ULA_CombatComponent::EndAttack()
{
}


void ULA_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULA_CombatComponent::InvertIsAttacking()
{
	bIsAttacking = !bIsAttacking;
}

void ULA_CombatComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::White, FString::SanitizeFloat(Character->PlayAnimMontage(Animation)));
}

