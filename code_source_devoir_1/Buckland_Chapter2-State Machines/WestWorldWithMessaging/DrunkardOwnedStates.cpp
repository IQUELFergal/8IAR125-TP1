#include "DrunkardOwnedStates.h"
#include "fsm/State.h"
#include "Drunkard.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif
#include "../../Common/misc/utils.h"

//------------------------------------------------------------------------methods for GoToTheSaloonAndDrink
GoToTheSaloonAndDrink* GoToTheSaloonAndDrink::Instance()
{
    static GoToTheSaloonAndDrink instance;

    return &instance;
}


void GoToTheSaloonAndDrink::Enter(Drunkard* pDrunkard)
{
    //if the miner is not already located at the goldmine, he must
    //change location to the gold mine
    if (pDrunkard->Location() != saloon)
    {
        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Walkin' to the saloon";

        pDrunkard->ChangeLocation(saloon);
    }
}


void GoToTheSaloonAndDrink::Execute(Drunkard* pDrunkard)
{
    pDrunkard->IncreaseDrunkenness();

    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Sippin' some good ol' beer.";

    if (pDrunkard->Drunk())
    {
        pDrunkard->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
    }
}


void GoToTheSaloonAndDrink::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
        << "Ah'm leavin' the saloon";
}


bool GoToTheSaloonAndDrink::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}


//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested
GoHomeAndSleep* GoHomeAndSleep::Instance()
{
    static GoHomeAndSleep instance;

    return &instance;
}


void GoHomeAndSleep::Enter(Drunkard* pDrunkard)
{
    //if the miner is not already located at the goldmine, he must
    //change location to the gold mine
    if (pDrunkard->Location() != saloon)
    {
        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Goin' home to sleep";

        pDrunkard->ChangeLocation(shack);
    }
}


void GoHomeAndSleep::Execute(Drunkard* pDrunkard)
{
    pDrunkard->DecreaseDrunkenness();

    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Sleepin'";

    if (!pDrunkard->Drunk())
    {
        pDrunkard->GetFSM()->ChangeState(GoToTheSaloonAndDrink::Instance());
    }
}


void GoHomeAndSleep::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
        << "Ah'm leavin' mah shack";
}


bool GoHomeAndSleep::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}



//------------------------------------------------------------------------methods for Fight
Fight* Fight::Instance()
{
    static Fight instance;

    return &instance;
}


void Fight::Enter(Drunkard* pDrunkard)
{
    
}


void Fight::Execute(Drunkard* pDrunkard)
{
    if (!pDrunkard -> Healthy())
    {
        pDrunkard->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
    }

    if ((RandFloat() < 0.5))
    {
        pDrunkard->DecreaseHealth();
    }
}


void Fight::Exit(Drunkard* pDrunkard)
{
    
}


bool Fight::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    return false;
}