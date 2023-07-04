// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMasMas_ProyectoCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SceneComponent.h"
#include "Actor/Bomb/Bomb.h"



//////////////////////////////////////////////////////////////////////////
// ACMasMas_ProyectoCharacter

ACMasMas_ProyectoCharacter::ACMasMas_ProyectoCharacter()
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	BombSpawnPoint = CreateDefaultSubobject<USceneComponent>("Bomb SpawnLocation");
	BombSpawnPoint->SetupAttachment(RootComponent);

	CurrentLife = MaxLife;

}

/*Metodo que avisa de que el jugador a recibido daño*/
void ACMasMas_ProyectoCharacter::GetDamage(int Damage)
{
	CurrentLife -= Damage;
	OnPlayerRecivedDamage.Broadcast(Damage);
	if (CurrentLife <= 0) {
		OnPlayerDeadDelegate.Broadcast();
	}
}

int ACMasMas_ProyectoCharacter::GetCurrentLife() const
{
	return CurrentLife;
}

int ACMasMas_ProyectoCharacter::GetMaxLife() const
{
	return MaxLife;
}

void ACMasMas_ProyectoCharacter::BeginPlay()
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
}

void ACMasMas_ProyectoCharacter::SetCanThrowBomb()
{
	bCanPlayerThrowBomb = true;
	BombTimerHandle.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACMasMas_ProyectoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACMasMas_ProyectoCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACMasMas_ProyectoCharacter::Look);

		//Bomb
		EnhancedInputComponent->BindAction(BombAction, ETriggerEvent::Triggered, this, &ACMasMas_ProyectoCharacter::ThrowBomb);

		//Heavy Bomb
		EnhancedInputComponent->BindAction(HeavyBombAction, ETriggerEvent::Triggered, this, &ACMasMas_ProyectoCharacter::HeavyBomb);

	}
}

void ACMasMas_ProyectoCharacter::Move(const FInputActionValue& Value)
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

void ACMasMas_ProyectoCharacter::Look(const FInputActionValue& Value)
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

/*Metodo la lanzar una bomba fuerte*/
void ACMasMas_ProyectoCharacter::HeavyBomb(const FInputActionValue& Value)
{
	if (!bCanPlayerThrowBomb) {
		return;
	}

	const FTransform SpawnPoint = BombSpawnPoint->GetComponentTransform();
	AActor* Bomb = GetWorld()->SpawnActor<ABomb>(HeavyBombClass, SpawnPoint);
	bCanPlayerThrowBomb = false;
	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ACMasMas_ProyectoCharacter::SetCanThrowBomb, BombDelayTime_Heavy, false);
}

/*Metodo para lanzar una bomba normal*/
void ACMasMas_ProyectoCharacter::ThrowBomb(const FInputActionValue& Value)
{
	if (!bCanPlayerThrowBomb) {
		return;
	}
	const FTransform SpawnPoint = BombSpawnPoint->GetComponentTransform();
	AActor* Bomb = GetWorld()->SpawnActor<ABomb>(BombClass, SpawnPoint);
	bCanPlayerThrowBomb = false;
	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ACMasMas_ProyectoCharacter::SetCanThrowBomb, BombDelayTime_Light, false);
}




