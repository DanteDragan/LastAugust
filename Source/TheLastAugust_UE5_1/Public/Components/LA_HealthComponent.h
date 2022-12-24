// The Last August. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LA_HealthComponent.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELASTAUGUST_UE5_1_API ULA_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULA_HealthComponent();
	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;
	float GetHealth() const { return CurrentHealth; }
	float GetHealthPercent() const { return CurrentHealth/MaxHealth; }
	bool IsDead() const { return CurrentHealth == 0; }
	bool IsHealthFull() const { return CurrentHealth == MaxHealth; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMax = 1000, ClampMin = 1))
	float MaxHealth = 100.0f;
private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage,
						 const class UDamageType* DamageType,
						 class AController* InstigatedBy, AActor* DamageCauser);

	void SetHealth(float NewHealth);
	float CurrentHealth;
};