// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MenuInterface.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(MainMenuButton != nullptr)) return false;
	MainMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinGameButton != nullptr)) return false;
	JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);


	
	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface_)
{
	this->MenuInterface = MenuInterface_;

}

void UMainMenu::Setup()
{
	// Add widget to viewport
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	// gain player control
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// enable UI interaction
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	// show cursor on the screen
	PlayerController->bShowMouseCursor = true;
}

// OnDestroy
void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	// gain player control
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);

	// cancel mouse cursor control
	PlayerController->bShowMouseCursor = false;



}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
	UE_LOG(LogTemp, Warning, TEXT("Hosting..."));

}
void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAdressField != nullptr)) return;
		FString IPAddress = IPAdressField->GetText().ToString();
		MenuInterface->Join(IPAddress);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

}



void UMainMenu::OpenMainMenu()
{
	if (!ensure(JoinMenu != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}
