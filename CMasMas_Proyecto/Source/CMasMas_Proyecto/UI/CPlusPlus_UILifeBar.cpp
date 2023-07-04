

#include "CPlusPlus_UILifeBar.h"
#include <Kismet/GameplayStatics.h>
#include <CMasMas_Proyecto/CMasMas_ProyectoCharacter.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

/*Inicializacion de atributos para la UI*/
void UCPlusPlus_UILifeBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this,0);
	if (ACMasMas_ProyectoCharacter* CCharacter = Cast<ACMasMas_ProyectoCharacter>(Char)) {
		CCharacter->OnPlayerRecivedDamage.AddUniqueDynamic(this, &UCPlusPlus_UILifeBar::OnPlayerReciveDamageEvent);
	}
	SetLifeParams();
}

/*Metodo que modifica la UI dependiendo del daño que recibe el personaje*/
void UCPlusPlus_UILifeBar::OnPlayerReciveDamageEvent(int Damage)
{
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (ACMasMas_ProyectoCharacter* CCharacter = Cast<ACMasMas_ProyectoCharacter>(Char)) {
		const int CurrentLife = CCharacter->GetCurrentLife();
		const int MaxLife = CCharacter->GetMaxLife();

		if (LifeBar) {
			const float CurrentPercentLife = 0.0001f * CurrentLife * MaxLife;
			LifeBar->SetPercent(CurrentPercentLife);
		}

		if (DamageTxt) {
			DamageTxt->SetText(FText::AsNumber(Damage));
		}

		if (CurrentLifeTxt) {
			SetLifeParams();
		}
	}
}

/*Metodo que setea los atributos de vida del personaje con el formato:
	CurrentLife / MaxLife
*/
void UCPlusPlus_UILifeBar::SetLifeParams()
{
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (ACMasMas_ProyectoCharacter* CCharacter = Cast<ACMasMas_ProyectoCharacter>(Char)) {
		const int CurrentLife = CCharacter->GetCurrentLife();
		const int MaxLife = CCharacter->GetMaxLife();

		/*Formateo de strings para la UI*/
		FString CurrentLifeString = FString::FromInt(CurrentLife);
		FString MaxLifeString = FString::FromInt(MaxLife);
		FString Barra = FString(TEXT("/"));

		FString aux = CurrentLifeString.Append(Barra);
		FString finalString = aux.Append(MaxLifeString);
		
		CurrentLifeTxt->SetText(FText::FromString(finalString));
	}
}

