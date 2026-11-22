#include "Kart/KartData_Spoiler.h"

#include "KartPawn.h"
#include "Kart/VisualKart.h"

void UKartData_Spoiler::SetSpoiler(AKartPawn* Kart) const
{
	Kart->SetKartSpoiler(Mesh);
}
void UKartData_Spoiler::SetSpoilerVisual(AVisualKart* Kart) const
{
	Kart->SetKartSpoiler(Mesh);
}

void UKartData_Spoiler::SetStat(AKartPawn* Kart) const
{
	Kart->SetSpoilerStat(ID, StatSpeed, StatAcceleration, StatHeight, StatHandling, StatTraction);
}