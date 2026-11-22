#include "Kart/KartData_Wheels.h"

#include "KartPawn.h"
#include "Kart/VisualKart.h"

void UKartData_Wheels::SetWheels(AKartPawn* Kart) const
{
	Kart->SetKartWheels(Mesh, SecondMesh);
	Kart->SetBodyHeight(Raduis);
}
void UKartData_Wheels::SetWheelsVisual(AVisualKart* Kart) const
{
	Kart->SetKartWheels(Mesh, SecondMesh);
	Kart->SetBodyHeight(Raduis);
}

void UKartData_Wheels::SetStat(AKartPawn* Kart) const
{
	Kart->SetWheelsStat(ID, StatSpeed, StatAcceleration, StatHeight, StatHandling, StatTraction);
}