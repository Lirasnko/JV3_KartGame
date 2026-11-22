#include "Kart/KartData_Body.h"

#include "KartPawn.h"
#include "Kart/VisualKart.h"

void UKartData_Body::SetBody(AKartPawn* Kart) const
{
	Kart->SetKartBody(Mesh, LightMesh, LightPosition, LightRotator, SpotLightPosition);
	Kart->SetKartPartPosition(WheelFrontPosition, WheelRearPosition, SpoilerPosition);
}
void UKartData_Body::SetBodyVisual(AVisualKart* Kart) const
{
	Kart->SetKartBody(Mesh, LightMesh, LightPosition);
	Kart->SetKartPartPosition(WheelFrontPosition, WheelRearPosition, SpoilerPosition);
}

void UKartData_Body::SetStat(AKartPawn* Kart) const
{
	Kart->SetBodyStat(ID, StatSpeed, StatAcceleration, StatHeight, StatHandling, StatTraction);
}
