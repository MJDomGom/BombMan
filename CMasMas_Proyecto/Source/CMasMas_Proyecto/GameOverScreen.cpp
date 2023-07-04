// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreen.h"
#include <Kismet/GameplayStatics.h>
#include "CMasMas_ProyectoCharacter.h"

/*Metodo que inicializa la UI y la escucha*/
void UGameOverScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (ACMasMas_ProyectoCharacter* CCharacter = Cast<ACMasMas_ProyectoCharacter>(Char)) {
		OnPlayerDeadHandle = CCharacter->OnPlayerDeadDelegate.AddUObject(this, &UGameOverScreen::OnPlayerDead);
	}
}

/*Metodo que activa la UI una vez que el personaje muera*/
void UGameOverScreen::OnPlayerDead()
{
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (ACMasMas_ProyectoCharacter* CCharacter = Cast<ACMasMas_ProyectoCharacter>(Char)) {
		CCharacter->OnPlayerDeadDelegate.Remove(OnPlayerDeadHandle);
	}
	this->SetVisibility(ESlateVisibility::Visible);
}
