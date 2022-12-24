// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheLastAugust_UE5_1Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/TextRenderComponent.h"
#include "Animations/AnimNotifies/LA_IsAttackingAnimNotify.h"

//////////////////////////////////////////////////////////////////////////
// ATheLastAugust_UE5_1Character

void ATheLastAugust_UE5_1Character::OnHealthChanged(float CurrentHealth,
	float HealthDelta)
{
	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
}

ATheLastAugust_UE5_1Character::ATheLastAugust_UE5_1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	//Custom component-classes creation
	HealthComponent = CreateDefaultSubobject<ULA_HealthComponent>("HealthComponent");
	CombatComponent = CreateDefaultSubobject<ULA_CombatComponent>("CombatComponent");
	HealthText = CreateDefaultSubobject<UTextRenderComponent>("HealthText");
	HealthText->SetupAttachment(RootComponent);
	HealthText->SetOwnerNoSee(true);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATheLastAugust_UE5_1Character::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &ATheLastAugust_UE5_1Character::OnDeath);
	
}

void ATheLastAugust_UE5_1Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATheLastAugust_UE5_1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATheLastAugust_UE5_1Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATheLastAugust_UE5_1Character::Look);

		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATheLastAugust_UE5_1Character::Attack);
		
		//Using skill
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &ATheLastAugust_UE5_1Character::UseSkill);
	}

}

void ATheLastAugust_UE5_1Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATheLastAugust_UE5_1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATheLastAugust_UE5_1Character::Attack()
{
	CombatComponent->Attack();
}

void ATheLastAugust_UE5_1Character::UseSkill()
{
		CombatComponent->UseSkill();
}

void ATheLastAugust_UE5_1Character::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}


