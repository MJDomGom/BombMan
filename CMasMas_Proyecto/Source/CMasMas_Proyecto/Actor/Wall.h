// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class CMASMAS_PROYECTO_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	AWall();
	void GetDamage(int Damage);
	int GetCurrentLife() const;
	int GetMaxLife() const;

protected:
	/*Atributos modificables en el editor para los muros*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MaxLife = 35;

	int CurrentLife = 35;

};
