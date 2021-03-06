// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled((ECollisionEnabled::QueryOnly));
	OverlapComp->SetCollisionResponseToAllChannels((ECR_Ignore));
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f, 200.0f, 50.0f));
	OverlapComp->SetHiddenInGame(false);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 50.0f);
	DecalComp->SetupAttachment(RootComponent);
}
void AFPSLaunchPad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFx, GetActorLocation());
}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent * OverlappedComponent,
	AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogClass, Log, TEXT("Overlapping!! %s"), *OtherActor->GetClass()->GetName());

	AFPSCharacter * mainCharacter = Cast<AFPSCharacter>(OtherActor);
	if (mainCharacter != nullptr) {
		UGameplayStatics::PlaySound2D(this, LaunchSound);
		PlayEffects();

		UE_LOG(LogClass, Log, TEXT("Launching!!"));
		mainCharacter->LaunchCharacter(GetActorLocation() + FVector(1100.0f, -1100.0f, 1100.0f), true, true);
	}
	else {
		UE_LOG(LogClass, Log, TEXT("Failed Cast!!! %s"), *OtherActor->GetName());	
		if (OtherComp && OtherComp->IsSimulatingPhysics()) {
			OtherComp->AddImpulse(FVector(1100.0f, -1100.0f, 1100.0f), NAME_None, true);
		}
	}
		

}

