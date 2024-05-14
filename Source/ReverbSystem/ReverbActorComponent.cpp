#include "ReverbActorComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UReverbActorComponent::UReverbActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UReverbActorComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->IsA(ACharacter::StaticClass()))
	{
		Owner = GetOwner();
	}

	ActorHeightOffset = FVector(0,0,25);
	OrientationVectors = TArray({FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0)});
	Distances.Init(0.0f, 6);
}


void UReverbActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateVectorArray(Owner);
	UpdateWallDistances(Owner);
	UpdateAverageVolume();

	NormalizedAverage = UKismetMathLibrary::NormalizeToRange(AverageRoomVolume, 0, MaxRange);
}

float UReverbActorComponent::GetDistanceToWall(AActor* Character, FVector DrawDirection, float CheckDistance, FVector OriginPoint) const
{
	FHitResult OutHit(ForceInit);
	FCollisionQueryParams CollisionParams;
	CollisionParams.bReturnPhysicalMaterial = true;
	
	FVector End = Character->GetActorLocation() + DrawDirection * CheckDistance;

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, OriginPoint, End, ECC_Visibility, CollisionParams);
	
	if (ToggleDebugLines)
	{
		if (bHit)
		{
			DrawDebugLine(GetWorld(), OriginPoint, OutHit.Location, FColor::Green, false, -1.0f, 0, 1.0f);
			DrawDebugPoint(GetWorld(), OutHit.Location, 10.0f, FColor::Green, false, -1.0f, 0);
			DrawDebugLine(GetWorld(), OutHit.Location, End, FColor::Red, false, -1.0f, 0, 1.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), OriginPoint, End, FColor::Red, false, -1.0f, 0, 1.0f);
			DrawDebugPoint(GetWorld(), OutHit.Location, 10.0f, FColor::Red, false, -1.0f, 0);
		}
	}
	
	return OutHit.Distance;
}

// Update the array of our 6 vectors, the directions our raycasts will fire in
void UReverbActorComponent::UpdateVectorArray(const AActor* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	const FRotator CharacterRotation = Character->GetActorRotation();

	for (int32 Index = 0; Index < OrientationVectors.Num(); ++Index)
	{
		FVector NegativeMultiplierVector(-1.0f, -1.0f, -1.0f);

		switch (Index)
		{
		case 0:
			OrientationVectors[Index] = UKismetMathLibrary::GetRightVector(CharacterRotation);
			continue;
		case 1:
			OrientationVectors[Index] = NegativeMultiplierVector * UKismetMathLibrary::GetRightVector(CharacterRotation);
			continue;
		case 2:
			OrientationVectors[Index] = UKismetMathLibrary::GetForwardVector(CharacterRotation);
			continue;
		case 3:
			OrientationVectors[Index] = NegativeMultiplierVector * UKismetMathLibrary::GetForwardVector(CharacterRotation);
			continue;
		case 4:
			OrientationVectors[Index] = UKismetMathLibrary::GetUpVector(CharacterRotation);
			continue;
		case 5:
			OrientationVectors[Index] = NegativeMultiplierVector * UKismetMathLibrary::GetUpVector(CharacterRotation);
		default:
			break;
		}
	}
}

// super simple average volume calculation - replace this with a "real" cubic volume later
void UReverbActorComponent::UpdateAverageVolume()
{
	float DistanceSum = 0;
	
	for (const float Distance : Distances)
	{
		DistanceSum += Distance;
	}

	AverageRoomVolume = DistanceSum / Distances.Num();
}

void UReverbActorComponent::UpdateWallDistances(AActor* Character)
{
	const FVector OriginPoint = Character->GetActorLocation() + ActorHeightOffset;
	
	for (int32 Index = 0; Index < OrientationVectors.Num(); ++Index)
	{
		const float Distance = GetDistanceToWall(Character, OrientationVectors[Index], DrawDistance, OriginPoint);
		Distances[Index] = Distance;
	}
}
