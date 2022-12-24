// The Last August. All Rights Reserved.


#include "Components/LA_HealthComponent.h"

ULA_HealthComponent::ULA_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void ULA_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, 0);
	if (AActor* ComponentOwner = GetOwner(); ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &ULA_HealthComponent::OnTakeAnyDamage);
	}
}

void ULA_HealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || IsDead() || !GetWorld()) return;
	
	SetHealth(CurrentHealth - Damage);

	if (IsDead()) OnDeath.Broadcast();
	
}

void ULA_HealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const float HealthDelta = NextHealth - CurrentHealth;
	CurrentHealth = NextHealth;
	OnHealthChanged.Broadcast(CurrentHealth, HealthDelta);
}