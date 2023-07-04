// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CMasMas_ProyectoCharacter.generated.h"

class USceneComponent;
class ABomb;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRecievedDamage, int, Damage);
DECLARE_MULTICAST_DELEGATE(FOnPlayerDeadDelegate);

UCLASS(config=Game)
class ACMasMas_ProyectoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;	

	/** Bomb Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BombAction;
	
	/** Heavy Bomb Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HeavyBombAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters", meta = (AllowPrivateAccess = "true"))
		USceneComponent* BombSpawnPoint = nullptr;

public:
	UPROPERTY(BlueprintAssignable)
		FOnPlayerRecievedDamage OnPlayerRecivedDamage;

	FOnPlayerDeadDelegate OnPlayerDeadDelegate;

	ACMasMas_ProyectoCharacter();
	void GetDamage(int Damage);
	int GetCurrentLife() const;	
	int GetMaxLife() const;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MaxLife = 100;
	int CurrentLife = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ABomb> BombClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BombDelayTime_Light = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BombDelayTime_Heavy = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ABomb> HeavyBombClass;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	void HeavyBomb(const FInputActionValue& Value);

	void ThrowBomb(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

private:
	bool bCanPlayerThrowBomb = true;
	FTimerHandle BombTimerHandle;

	void SetCanThrowBomb();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

