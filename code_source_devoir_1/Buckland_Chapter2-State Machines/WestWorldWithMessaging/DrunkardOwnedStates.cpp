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

    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_EnterBar:

        cout << "\nMessage handled by " << GetNameOfEntity(pDrunkard->ID())
            << " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Hey you ! What are you lookin' at !? ";

        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
            pDrunkard->ID(),             //ID of sender
            ent_Miner_Bob,               //ID of recipient
            Msg_ProvokeFight,            //the message
            NO_ADDITIONAL_INFO);

        // We directly go to fight, Miner should always accept the fight if we have received the EnterBar msg
        pDrunkard->GetFSM()->ChangeState(Fight::Instance());

        return true;

    }
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
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
            pDrunkard->ID(),             //ID of sender
            ent_Miner_Bob,               //ID of recipient
            Msg_WinFight,            //the message
            NO_ADDITIONAL_INFO);
        pDrunkard->GetFSM()->ChangeState(GoHomeAndSleep::Instance());

        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
            << "Zblah";
    }
    else 
    {

        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
            << "I'm gonna beat ya !";
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
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_WinFight:

        cout << "\nMessage handled by " << GetNameOfEntity(pDrunkard->ID())
            << " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

        cout << "\n" << GetNameOfEntity(pDrunkard->ID())
            << ": I win ! hic ! ";

        pDrunkard->GetFSM()->RevertToPreviousState();

        return true;

    }
    return false;
}