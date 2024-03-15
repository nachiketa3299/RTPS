#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("스프링 암"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("카메라"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;
    SpringArm->TargetArmLength = 600.f;


    GetCharacterMovement()->bUseSeparateBrakingFriction = true;
    SpeedOnNormal = 600.f;
    SpeedOnSprint = 800.f;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

    auto* PlayerController = Cast<APlayerController>(Controller);
    auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    Subsystem->AddMappingContext(DefaultMappingContext, 0);

    StopGazing(FInputActionValue {});
    StopSprinting(FInputActionValue {});
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    { // 입력과 관련 함수를 바인딩
        EnhancedInputComponent->BindAction(InputToMove, ETriggerEvent::Triggered, this, &ACharacterBase::Move);

        EnhancedInputComponent->BindAction(InputToLook, ETriggerEvent::Triggered, this, &ACharacterBase::Look);

        EnhancedInputComponent->BindAction(InputToGaze, ETriggerEvent::Triggered, this, &ACharacterBase::Gaze);
        EnhancedInputComponent->BindAction(InputToGaze, ETriggerEvent::Completed, this, &ACharacterBase::StopGazing);

        EnhancedInputComponent->BindAction(InputToSprint, ETriggerEvent::Triggered, this, &ACharacterBase::Sprint);
        EnhancedInputComponent->BindAction(InputToSprint, ETriggerEvent::Completed, this, &ACharacterBase::StopSprinting);

        EnhancedInputComponent->BindAction(InputToHop, ETriggerEvent::Triggered, this, &ACharacterBase::Hop);
        EnhancedInputComponent->BindAction(InputToHop, ETriggerEvent::Completed, this, &ACharacterBase::StopHopping);
    }
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
    // TODO: Controller가 유효하지 않은 경우 처리
    // TODO: Value 가 FVector2D가 아닌 경우 처리

    const auto ControlRotation = Controller->GetControlRotation();
    const auto YawControlRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
    const auto RotationMatrix = FRotationMatrix(YawControlRotation);

    const auto MovementVector = Value.Get<FVector2D>();
    const auto ForwardDirection = RotationMatrix.GetUnitAxis(EAxis::X);
    const auto RightDirection = RotationMatrix.GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementVector.Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
    const auto LookAxisVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(-1 * LookAxisVector.Y);
}

void ACharacterBase::Gaze(const FInputActionValue& Value)
{
    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationYaw = true;
}

void ACharacterBase::StopGazing(const FInputActionValue& Value)
{
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
}

void ACharacterBase::Sprint(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed = SpeedOnSprint;
}

void ACharacterBase::StopSprinting(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed = SpeedOnNormal;
}

void ACharacterBase::Hop(const FInputActionValue& Value)
{
    Super::Jump();
}

void ACharacterBase::StopHopping(const FInputActionValue& Value)
{
    Super::StopJumping();
}

