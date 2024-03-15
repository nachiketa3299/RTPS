#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS(Abstract)
class RTPS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()


public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> Camera;
protected:
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputAction> InputToMove;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputAction> InputToLook;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputAction> InputToGaze;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputAction> InputToSprint;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputAction> InputToHop;
protected:
    UFUNCTION()
    void Move(const FInputActionValue& Value);
    UFUNCTION()
    void Look(const FInputActionValue& Value);

    UFUNCTION()
    void Gaze(const FInputActionValue& Value);
    UFUNCTION()
    void StopGazing(const FInputActionValue& Value);
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    float SpeedOnNormal;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
    float SpeedOnSprint;
    UFUNCTION()
    void Sprint(const FInputActionValue& Value);
    UFUNCTION()
    void StopSprinting(const FInputActionValue& Value);
    
    UFUNCTION()
    void Hop(const FInputActionValue& Value);
    UFUNCTION()
    void StopHopping(const FInputActionValue& Value);

};
