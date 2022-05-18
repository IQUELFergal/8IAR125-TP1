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
    //Go to saloon
    if (pDrunkard->Location() != saloon)
    {
        Dispatch->Msg(pDrunkard->ID(), "Walkin' to the saloon");
        pDrunkard->ChangeLocation(saloon);
    }
}


void GoToTheSaloonAndDrink::Execute(Drunkard* pDrunkard)
{
    // drinking
    pDrunkard->IncreaseDrunkenness();
    Dispatch->Msg(pDrunkard->ID(), "Sippin' some good ol' beer.");

    if (pDrunkard->Drunk())
    {
        pDrunkard->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
    }
}


void GoToTheSaloonAndDrink::Exit(Drunkard* pDrunkard)
{
    Dispatch->Msg(pDrunkard->ID(), "Ah'm leavin' the saloon");
}


bool GoToTheSaloonAndDrink::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    switch (msg.Msg)
    {
    case Msg_EnterBar:

        // Msg received that the Miner has enter so try to pick a fight
        Dispatch->MsgTelegramReceived(pDrunkard->ID());
        Dispatch->Msg(pDrunkard->ID(), "Hey you ! What are you lookin' at !?");


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
    // the drunkard go to sleep
    if (pDrunkard->Location() != saloon)
    {
        Dispatch->Msg(pDrunkard->ID(), "Goin' home to sleep");

        pDrunkard->ChangeLocation(shack);
    }
}


void GoHomeAndSleep::Execute(Drunkard* pDrunkard)
{
    // The drunkard sleep and lose drunkeness
    pDrunkard->DecreaseDrunkenness();
    pDrunkard->DecreaseDrunkenness();

    Dispatch->Msg(pDrunkard->ID(), "Sleepin");

    // Once not drunk anymore, go to drink
    if (!pDrunkard->Drunk())
    {
        pDrunkard->GetFSM()->ChangeState(GoToTheSaloonAndDrink::Instance());
    }
}


void GoHomeAndSleep::Exit(Drunkard* pDrunkard)
{
    Dispatch->Msg(pDrunkard->ID(), "Ah'm leavin' mah shack");
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
        // Drunkard defeated, send Msg to Miner that he won, and the drunkard go back home

        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
            pDrunkard->ID(),             //ID of sender
            ent_Miner_Bob,               //ID of recipient
            Msg_WinFight,            //the message
            NO_ADDITIONAL_INFO);

        Dispatch->Msg(pDrunkard->ID(), "Zblah");

        pDrunkard->GetFSM()->ChangeState(GoHomeAndSleep::Instance());

        return;
    }
    else 
    {
        Dispatch->Msg(pDrunkard->ID(), "I'm gonna beat ya !");
    }

    // The drunkard is fighting
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

    switch (msg.Msg)
    {
    case Msg_WinFight:

        // Received msg that the fight is won
        Dispatch->MsgTelegramReceived(pDrunkard->ID());
        Dispatch->Msg(pDrunkard->ID(), "I win ! hic !");

        pDrunkard->GetFSM()->RevertToPreviousState();

        return true;

    }
    return false;
}