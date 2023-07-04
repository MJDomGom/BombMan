// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Overlay.h>
#include "GameOverScreen.generated.h"


/**
 * 
 */
UCLASS()
class CMASMAS_PROYECTO_API UGameOverScreen : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void NativeOnInitialized();

private:
	/*Metodo y atributo para la escucha de la muerte del personaje*/
	void OnPlayerDead();
	FDelegateHandle OnPlayerDeadHandle;

};
