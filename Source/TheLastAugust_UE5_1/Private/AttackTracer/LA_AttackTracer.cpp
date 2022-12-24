// The Last August. All Rights Reserved.


#include "AttackTracer/LA_AttackTracer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../TheLastAugust_UE5_1Character.h"
#include "Components/SkeletalMeshComponent.h"

void ULA_AttackTracer::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                   UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Character = Cast<ATheLastAugust_UE5_1Character>(MeshComp->GetOwner());
	if (Character)
	{
		//Player is merged with weapon, so we should look for sockets in its mesh
		//In case of AI we should search them in WeaponComp
		Weapon = MeshComp;
			         
			         // Character->Controller->IsPlayerController() ? MeshComp : Cast<USkeletalMeshComponent>(
				        //  Character->GetWeaponComponent()->GetCurrentWeapon()->
				        //          GetRootComponent());
		ActorsToIgnore = {MeshComp->GetOwner()};
		LastLocation1 = Weapon->GetSocketLocation("Trace1");
		LastLocation2 = Weapon->GetSocketLocation("Trace2");
	}
}

void ULA_AttackTracer::NotifyEnd(USkeletalMeshComponent* MeshComp,
                              UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	HitActors.Empty();
}

void ULA_AttackTracer::NotifyTick(USkeletalMeshComponent* MeshComp,
                               UAnimSequenceBase* Animation, float FrameDeltaTime,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (Character)
	{
		UKismetSystemLibrary::BoxTraceMulti(Character->GetWorld(), LastLocation1,
		                                    Weapon->GetSocketLocation("Trace1"),
		                                    FVector(5, 30, 50),
		                                    Weapon->GetComponentRotation(),
		                                    ETraceTypeQuery::TraceTypeQuery1, false,
		                                    ActorsToIgnore, EDrawDebugTrace::None,
		                                    HitResults, true);
		for (int i = 0; i < HitResults.Num(); ++i)
		{
			AActor* HitActor = HitResults[i].GetActor();
			if (!HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);
				UGameplayStatics::ApplyDamage(HitActor, 10, EventInstigator, Character,
				                              DamageTypeClass);
			}
		}
		UKismetSystemLibrary::BoxTraceMulti(Character->GetWorld(), LastLocation2,
		                                    Weapon->GetSocketLocation("Trace2"),
		                                    FVector(5, 30, 50),
		                                    Weapon->GetComponentRotation(),
		                                    ETraceTypeQuery::TraceTypeQuery1, false,
		                                    ActorsToIgnore, EDrawDebugTrace::None,
		                                    HitResults, true);
		for (int i = 0; i < HitResults.Num(); ++i)
		{
			AActor* HitActor = HitResults[i].GetActor();
			if (!HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);
				UGameplayStatics::ApplyDamage(HitActor, 10, EventInstigator, Character,
				                              DamageTypeClass);
			}
		}
		LastLocation1 = Weapon->GetSocketLocation("Trace1");
		LastLocation2 = Weapon->GetSocketLocation("Trace2");
	}
}