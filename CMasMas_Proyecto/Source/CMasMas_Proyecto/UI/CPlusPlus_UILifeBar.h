#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlusPlus_UILifeBar.generated.h"

class UProgressBar;
class UTextBlock;
USTRUCT(BlueprintType)
struct FLifeBarParam {

	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	int Damage;
	UPROPERTY(BlueprintReadOnly)
	int CurrentLife;
	UPROPERTY(BlueprintReadOnly)
	int MaxLife;
};

UCLASS()
class CMASMAS_PROYECTO_API UCPlusPlus_UILifeBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeOnInitialized();

	/*Atributos de la UI*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* LifeBar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* DamageTxt = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* CurrentLifeTxt = nullptr;


protected:
	/*Metodo que se ejecuta cuando el personaje recibe daño*/
	UFUNCTION()
		void OnPlayerReciveDamageEvent(int Damage);
	void SetLifeParams();


};
