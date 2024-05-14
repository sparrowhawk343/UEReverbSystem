#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReverbActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REVERBSYSTEM_API UReverbActorComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	
public:	
	TArray<FVector> OrientationVectors;
	TArray<float> Distances;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float AverageRoomVolume;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float NormalizedAverage;

	UPROPERTY(BlueprintReadWrite)
	FVector ActorHeightOffset;

	UPROPERTY(BlueprintReadWrite)
	bool ToggleDebugLines = true;

	UReverbActorComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:
	UPROPERTY()
	AActor* Owner;
	
	float DrawDistance = 5000.f;
	float MaxRange = 400.f;

	float GetDistanceToWall(AActor* Character, FVector DrawDirection, float CheckDistance, FVector OriginPoint) const;
	void UpdateVectorArray(const AActor* Character);
	void UpdateAverageVolume();
	void UpdateWallDistances(AActor* Character);
	
	
		
};
