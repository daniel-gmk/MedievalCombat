// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(config = Game)
class MEDIEVALCOMBAT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character (SpringArm)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera (FirstPersonCamera)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Capsule component for player collision (Player)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* PlayerCollision;

	/** Weapon Mesh object (BasicSword)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Weapon;

	/** Weapon hurtbox base (BasicSwordHurtbox followed by BasicSword1-BasicSword5)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WeaponHurtboxBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Hurtbox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Hurtbox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Hurtbox3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Hurtbox4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Hurtbox5;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/* **************************** Variables **************************** */

	/* ***** Component Variables ***** */

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* ***** Base Variables ("Vanilla" in blueprints) ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vanilla)
		bool Invincible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Vanilla)
		bool IsDead = false;

	/* ***** Hitbox Variables ***** */

	UPROPERTY(EditAnywhere, Category = Hitbox)
		UShapeComponent* HitboxComponentArray[6];

	UPROPERTY(EditAnywhere, Category = Hitbox)
		FVector HitboxArray[6];

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hitbox)
		bool InitialHitbox = true;

	/* ***** Health Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Health)
		float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float MaxHealth = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float KnockbackValue = 0;

	/* ***** Attack Handler Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackHandler)
		bool CanDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AttackHandler)
		bool CanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AttackHandler)
		bool CurrentAttackHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackHandler)
		float CurrentDamage = 0;

	/* ***** Block Handler Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = BlockHandler)
		bool IsBlocking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = BlockHandler)
		bool AttackWasBlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = BlockHandler)
		bool SuccessfullyDefended = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = BlockHandler)
		bool RecoilAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = BlockHandler)
		bool BlockingAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = BlockHandler)
		bool HoldingBlock = false;

	/* ***** Flinch Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flinch)
		float FlinchDuration = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flinch)
		bool FlinchTrigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flinch)
		bool Flinched = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flinch)
		bool CantFlinch = false;

	/* ***** Roll Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Roll)
		bool RollAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Roll)
		bool IsRolling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Roll)
		float RollSpeed = 1.0;

	/* ***** Movement Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool CanTurn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Movement)
		float CurrentFBLoc = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Movement)
		float CurrentLRLoc = 0;

	/* ***** Resilience Variables ***** */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Resilience)
		float Resilience = 100.0;

	/* ***** Other Variables ***** */

	/* **************************** Functions **************************** */

	/**Decrements cooldown by .1 every time called, if cd > 0 */
	UFUNCTION(BlueprintCallable)
		void CooldownDecrement(UPARAM(ref) float cd, UPARAM(ref) FTimerHandle& Handle);

	/** Hitbox Handler */
	UFUNCTION(BlueprintCallable)
		void HitboxHandler();

	/** Roll Direction Handler */
	UFUNCTION(BlueprintCallable)
		void RollDirectionHandler();

	/** Function for when an attack hits (SwordContactEvent) */
	UFUNCTION()
		void WeaponHitEvent(FHitResult HitResult);

	/** Timer Function to replicate DELAY in Blueprints */
	UFUNCTION()
		void onTimerEnd();

	/** Helper function for Hitbox Handler */
	UFUNCTION()
		void FillHitboxArray();

private:
	FTimerHandle delayTimerHandle;
};

