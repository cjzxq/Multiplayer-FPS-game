
#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlasterLearing/Weapon/Weapon.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter == nullptr) return;
	//以下完成后，创建父类为animinstance的animationBlueprint,里面的
	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();
	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();
	bIsCrouched = BlasterCharacter->bIsCrouched;
	bAiming = BlasterCharacter->IsAiming();
	TurningInPlace = BlasterCharacter->GetTurningInPlace();
	bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();
	bElimmed = BlasterCharacter->IsElimed();
	bHoldingTheFlag = BlasterCharacter->IsHoldingTheFlag();

	//启动偏航角偏移
	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BlasterCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
	//AO_Yaw虽然在.h中也有声明，但他是副本 把blastercharacter.h中的AO_Yaw值通过GetAO_Yaw()传递过来
	//这些写好后，要在ABP中连线设置
	AO_Yaw = BlasterCharacter->GetAO_Yaw();
	AO_Pitch = BlasterCharacter->GetAO_Pitch();
	//首先需要知道是否有装备武器还需要使用到武器，武器身上具有创建的插槽
	//动画实例应该具有一个装备的武器变量
	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
	{
		//设置武器上的插槽  
		//获取武器上左手socket在世界空间的位置
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		//声明两个临时变量，用来接收从“坐标变换”函数得到的输出
		FVector OutPosition;
		FRotator OutRotation;
		//// 把这个世界位置转换为以 right-hand bone 为参考的骨骼局部坐标
		BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		//LeftHandTransform 现在代表“左手相对于 right-hand 骨”的变换，可直接用于动画蓝图里作为手部的相对（骨骼）偏移
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (BlasterCharacter->IsLocallyControlled())
		{
			bLocallyController = true;
		FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("Hand_R"), ERelativeTransformSpace::RTS_World);
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BlasterCharacter->GetHitTarget()));
		RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 30.f);
		}

	}

	bUseFABRIK = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccuiped;
	bool bFABRIKOverride = BlasterCharacter->IsLocallyControlled() && 
		BlasterCharacter->GetCombatState() != ECombatState::ECS_ThrowingGrenade && 
		BlasterCharacter->bFinishedSwapping;
	if (bFABRIKOverride)
	{
		bUseFABRIK = !BlasterCharacter->IsLocallyReloading();
	}
	bUseAimOffsets = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccuiped && !BlasterCharacter->GetDisableGameplay();
	bTransformRightHand = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccuiped && !BlasterCharacter->GetDisableGameplay();
}
