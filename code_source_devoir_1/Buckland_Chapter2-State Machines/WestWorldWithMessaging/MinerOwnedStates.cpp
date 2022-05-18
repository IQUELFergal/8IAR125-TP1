#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
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

//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
      Dispatch->Msg(pMiner->ID(), "Walkin' to the goldmine");

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

  Dispatch->Msg(pMiner->ID(), "Pickin' up a nugget");

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Ah'm leavin' the goldmine with mah pockets full o' sweet gold");
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
      Dispatch->Msg(pMiner->ID(), "Goin' to the bank. Yes siree");

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  Dispatch->Msg(pMiner->ID(), "Depositing gold");

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    Dispatch->Msg(pMiner->ID(), "WooHoo! Rich enough for now. Back home to mah li'lle lady");
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());      
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Leavin' the bank");
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
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

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
      Dispatch->Msg(pMiner->ID(), "Walkin' home");

    pMiner->ChangeLocation(shack); 

    //let the wife know I'm home
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_Elsa,            //ID of recipient
                              Msg_HiHoneyImHome,   //the message
                              NO_ADDITIONAL_INFO);    
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
      Dispatch->Msg(pMiner->ID(), "All mah fatigue has drained away. Time to find more gold!");
     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();
    Dispatch->Msg(pMiner->ID(), "ZZZZ... ");
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
   switch(msg.Msg)
   {
   case Msg_StewReady:

     Dispatch->MsgTelegramReceived(pMiner->ID());
     Dispatch->Msg(pMiner->ID(), "Okay Hun, ahm a comin'!");

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}










//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);

    Dispatch->Msg(pMiner->ID(), "Boy, ah sure is thusty! Walking to the saloon");
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
        pMiner->ID(),            //ID of sender
        ent_Drunkard,            //ID of recipient
        Msg_EnterBar,            //the message
        NO_ADDITIONAL_INFO);

    timeDrink = 1;
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
    if (timeDrink == 1) // Is drinking
    {
        pMiner->BuyAndDrinkAWhiskey();

        Dispatch->Msg(pMiner->ID(), "That's mighty fine sippin' liquer");
        pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
    }
    else if (timeDrink == 0) // Enough drinking
    {
        pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
    }

    timeDrink -= 1;
}


void QuenchThirst::Exit(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Leaving the saloon, feelin' good");
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{

    switch (msg.Msg)
    {
    case Msg_ProvokeFight:

        // The drunkard try to pick a fight
        Dispatch->MsgTelegramReceived(pMiner->ID());
        Dispatch->Msg(pMiner->ID(), "You want some !?");

        pMiner->GetFSM()->ChangeState(FightBar::Instance());

        return true;

    }
    return false;
}













//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Smells Reaaal goood Elsa!");
}

void EatStew::Execute(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Tastes real good too!");
  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Thankya li'lle lady. Ah better get back to whatever ah wuz doin");
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}











FightBar* FightBar::Instance()
{
    static FightBar instance;

    return &instance;
}


void FightBar::Enter(Miner* pMiner)
{
}

void FightBar::Execute(Miner* pMiner)
{
    Dispatch->Msg(pMiner->ID(), "Take that");

    if (!pMiner->Healthy())
    {
        // Miner defeated
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
            pMiner->ID(),                               //ID of sender
            ent_Drunkard,                               //ID of recipient
            Msg_WinFight,                               //the message
            NO_ADDITIONAL_INFO);

        Dispatch->Msg(pMiner->ID(), "Ouch");

        pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
        return;
    }

    // Fighting
    if ((RandFloat() < 0.5))
    {
        pMiner->DecreaseHealth();
    }
}

void FightBar::Exit(Miner* pMiner)
{
}


bool FightBar::OnMessage(Miner* pMiner, const Telegram& msg)
{
    //send msg to global message handler
    switch (msg.Msg)
    {
    case Msg_WinFight:

        // Drunkard has lost
        Dispatch->MsgTelegramReceived(pMiner->ID());
        Dispatch->Msg(pMiner->ID(), "And don't comme back");

        pMiner->GetFSM()->RevertToPreviousState();

        return true;

    }
    return false;
}


