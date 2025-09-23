// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERLEARING_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FVector& HitTarget) override;

private:
	//声明一个可在编辑器任意处编辑的、类型安全的变量，该变量用于引用一个 AProjectile 类或其任意子类的类型（UClass），以便在运行时生成相应类型的投射物对象
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ServerSideRewindProjectileClass;
};
