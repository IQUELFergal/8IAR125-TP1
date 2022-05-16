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
        pDrunkard->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
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
GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
    static GoHomeAndSleepTilRested instance;

    return &instance;
}


void GoHomeAndSleepTilRested::Enter(Drunkard* pDrunkard)
{
    //if the miner is not already located at the goldmine, he must
    //change location to the gold mine
    if (pDrunkard->Location() != saloon)
    {
        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Goin' home to sleep";

        pDrunkard->ChangeLocation(shack);
    }
}


void GoHomeAndSleepTilRested::Execute(Drunkard* pDrunkard)
{
    pDrunkard->DecreaseDrunkenness();

    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Sleepin'";

    if (!pDrunkard->Drunk())
    {
        pDrunkard->GetFSM()->ChangeState(GoToTheSaloonAndDrink::Instance());
    }
}


void GoHomeAndSleepTilRested::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
        << "Ah'm leavin' mah shack";
}


bool GoHomeAndSleepTilRested::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
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
    if (pDrunkard->Location() != saloon)
    {
        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Goin' home to sleep";

        pDrunkard->ChangeLocation(shack);
    }
}


void Fight::Execute(Drunkard* pDrunkard)
{
    pDrunkard->DecreaseDrunkenness();

    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Sleepin'";

    if (!pDrunkard->Drunk())
    {
        pDrunkard->GetFSM()->ChangeState(Fight::Instance());
    }
}


void Fight::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
        << "Ah'm leavin' mah shack";
}


bool Fight::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_StewReady:

        cout << "\nMessage handled by " << GetNameOfEntity(pDrunkard->ID())
            << " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

        cout << "\n" << GetNameOfEntity(pDrunkard->ID())
            << ": Ouch !";

        pDrunkard->

        return true;

    }//end switch
    return false;
}