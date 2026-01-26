#include "Gate.h"

#include "ThirdPersonGameMode.h"

AGate::AGate()
{
	GoalTrigger = CreateDefaultSubobject<UBoxComponent>("GoalTrigger");
	GoalTrigger->SetupAttachment(RootComponent);
}

void AGate::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GoalTrigger)
	{
		GoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGate::OnOverlapBegin);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GoalTrigger is null in BeginPlay"));
	}
}

void AGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlap したのがプレイヤーの時のみ処理する
	auto Player = Cast<APawn>(OtherActor);
	if (!Player)
		return;
	
	// 設定に応じて、ゲームを開始/終了する
	auto GameModeBase = GetWorld()->GetAuthGameMode();
	auto GameMode = Cast<AThirdPersonGameMode>(GameModeBase);
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Mode is null"));
		return;
	}
	
	switch (GateType)
	{
	case EGateType::Start:
		GameMode->StartGame();
		break;
	case EGateType::Goal:
		GameMode->StopGame();
		break;
	default:
		break;
	}

}
