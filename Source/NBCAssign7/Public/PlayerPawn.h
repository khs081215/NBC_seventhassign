// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class USpringArmComponent;
class UCapsuleComponent;
class UCameraComponent;
class InputActionValue;


UCLASS()
class NBCASSIGN7_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UCapsuleComponent* capsulecomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	USkeletalMeshComponent* skeletalmeshcomp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* springarm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* cameracomp;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void DroneRoll(const FInputActionValue& Value);

	UFUNCTION()
	void DroneUp(const FInputActionValue& Value);

	UFUNCTION()
	void DroneDown(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float movedelimeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float groundmovedelimeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float flyingmovedelimeter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float rotatedelimeter;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float camerapitchmin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float camerapitchmax;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	float playervelocity;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	bool isflying;
};
