// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "PlayerPawnController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    capsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
    SetRootComponent(capsulecomp);
    capsulecomp->SetCapsuleHalfHeight(95.0f);


    skeletalmeshcomp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
    skeletalmeshcomp->SetupAttachment(capsulecomp);
    skeletalmeshcomp->SetRelativeLocation(FVector(0, 0, -88.0f));
    skeletalmeshcomp->SetRelativeRotation(FRotator(0, -90.0f, 0));
    static ConstructorHelpers::FObjectFinder<USkeletalMesh>skmesh(TEXT(" / Script / Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
    if (skmesh.Succeeded())
    {
        skeletalmeshcomp->SetSkeletalMesh(skmesh.Object);
    }

    springarm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    springarm->SetupAttachment(capsulecomp);
    springarm->TargetArmLength = 300.0f;
    springarm->bUsePawnControlRotation = false;
    springarm->SetRelativeLocation(FVector(0, 0, 50.0f));

    cameracomp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    cameracomp->SetupAttachment(springarm, USpringArmComponent::SocketName);
    cameracomp->bUsePawnControlRotation = false;

    groundmovedelimeter = 500.0f;
    rotatedelimeter = 120.0f;
    camerapitchmin = -15.0f;
    camerapitchmax = 45.0f;
    playervelocity = 0.0f;
    isflying = false;
    flyingmovedelimeter = groundmovedelimeter * 0.3f;
    movedelimeter = groundmovedelimeter;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FHitResult Hit;
    FVector StartTrace = GetActorLocation();
    FVector EndTrace = GetActorLocation() + FVector(0, 0, -95.0f);
    if (!GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility))
    {
        playervelocity += (-980.0f * DeltaTime);
        AddActorWorldOffset(FVector(0, 0, playervelocity * DeltaTime));
        isflying = true;
        movedelimeter = flyingmovedelimeter;
    }
    else
    {
        isflying = false;
        playervelocity = 0.0f;
        movedelimeter = groundmovedelimeter;
    }
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APlayerPawnController* PlayerPawnController = Cast<APlayerPawnController>(GetController()))
        {
            if (PlayerPawnController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerPawnController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerPawn::Move
                );
            }
            if (PlayerPawnController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerPawnController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerPawn::Look
                );
            }

            if (PlayerPawnController->DroneRollAction)
            {
                EnhancedInput->BindAction(
                    PlayerPawnController->DroneRollAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerPawn::DroneRoll
                );
            }
            if (PlayerPawnController->DroneUpAction)
            {
                EnhancedInput->BindAction(
                    PlayerPawnController->DroneUpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerPawn::DroneUp
                );
            }
            if (PlayerPawnController->DroneDownAction)
            {
                EnhancedInput->BindAction(
                    PlayerPawnController->DroneDownAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerPawn::DroneDown
                );
            }

        }
    }
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
    if (!Controller) return;
    const FVector2D MoveInput = Value.Get<FVector2D>();
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        //AddMovementInput(GetActorForwardVector(), MoveInput.X);
        AddActorLocalOffset(FVector(MoveInput.X * 2.5f * DeltaTime * movedelimeter, 0, 0));
    }
    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        //AddMovementInput(GetActorForwardVector(), MoveInput.Y);
        AddActorLocalOffset(FVector(0, MoveInput.Y * 2.5f * DeltaTime * movedelimeter, 0));
    }
}

void APlayerPawn::Look(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D LookInput = Value.Get<FVector2D>();

    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

    //AddControllerYawInput(LookInput.X * DeltaTime * delimeter);
    //AddControllerPitchInput(LookInput.Y * DeltaTime * delimeter);
    float Yawvalue = springarm->GetRelativeRotation().Yaw + (LookInput.X * DeltaTime);
    float Pitchminmax = springarm->GetRelativeRotation().Pitch + (LookInput.Y * DeltaTime * rotatedelimeter);
    Pitchminmax = Pitchminmax > camerapitchmax ? camerapitchmax : Pitchminmax;
    Pitchminmax = Pitchminmax < camerapitchmin ? camerapitchmin : Pitchminmax;

    springarm->SetRelativeRotation(FRotator(Pitchminmax, Yawvalue, 0.0f));

    if (APlayerPawnController* PlayerPawnController = Cast<APlayerPawnController>(GetController()))
    {
        //PlayerPawnController->InputYawScale_DEPRECATED = 2.5f 이므로 DEPRECATED된 것을 사용하기 보다 2.5f를 사용
        //AddActorLocalRotation(FRotator(0, LookInput.X * PlayerPawnController->InputYawScale_DEPRECATED, 0));
        AddActorLocalRotation(FRotator(0, LookInput.X * 2.5f * DeltaTime * rotatedelimeter, 0));

        //드론 상태가 아니면 Pitch회전은 Pawn에 적용하지 않음. 드론 상태면 적용.
        if (PlayerPawnController->GetisPlayerPawnState() == false)
        {
            AddActorLocalRotation(FRotator(LookInput.Y * 2.5f * DeltaTime * rotatedelimeter, 0, 0));
        }
    }
}

void APlayerPawn::DroneRoll(const FInputActionValue& Value)
{
    if (APlayerPawnController* PlayerPawnController = Cast<APlayerPawnController>(GetController()))
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);


        //PlayerPawnController->InputYawScale_DEPRECATED = 2.5f 이므로 DEPRECATED된 것을 사용하기 보다 2.5f를 사용
        //AddActorLocalRotation(FRotator(0, LookInput.X * PlayerPawnController->InputYawScale_DEPRECATED, 0));
        AddActorLocalRotation(FRotator(0, 0, Value.Get<float>()*2.5f * DeltaTime * rotatedelimeter));
    }
}
void APlayerPawn::DroneUp(const FInputActionValue& Value)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(FVector(0, 0, DeltaTime * movedelimeter*10.0f));
}
void APlayerPawn::DroneDown(const FInputActionValue& Value)
{
    if (isflying)
    {
        float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
        AddActorLocalOffset(FVector(0, 0, -DeltaTime * movedelimeter * 10.0f));
    }
}