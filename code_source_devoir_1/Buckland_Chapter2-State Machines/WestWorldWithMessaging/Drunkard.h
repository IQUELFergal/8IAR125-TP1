#ifndef DRUNKARD_H
#define DRUNKARD_H

#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrunkardOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State;
struct Telegram;

//above this value a miner is drunk
const int DrunkLevel = 10;

const int MaxHealthDrunkard = 5;

class Drunkard : public BaseGameEntity {
private:

	//an instance of the state machine class
	StateMachine<Drunkard>* m_pStateMachine;

	location_type           m_Location;

    int                     m_iHealth;

    int                     m_iDrunkenness;

public:

    Drunkard(int id) :m_Location(shack),
        m_iHealth(MaxHealthDrunkard),
        m_iDrunkenness(0),
        BaseGameEntity(id)

    {
        //set up state machine
        m_pStateMachine = new StateMachine<Drunkard>(this);

        m_pStateMachine->SetCurrentState(GoToTheSaloonAndDrink::Instance());

        /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE DRUNKARD */
    }

    ~Drunkard() { delete m_pStateMachine; }

    //this must be implemented
    void Update();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);


    StateMachine<Drunkard>* GetFSM()const { return m_pStateMachine; }



    //-------------------------------------------------------------accessors
    location_type Location()const { return m_Location; }
    void          ChangeLocation(location_type loc) { m_Location = loc; }

    bool          Healthy()const { return m_iHealth > 0; };
    void          DecreaseHealth() { m_iHealth -= 1; }
    void          IncreaseHealth() { m_iHealth += 1; }

    bool          Drunk()const { return m_iDrunkenness >= DrunkLevel; };
    void          DecreaseDrunkenness() { m_iDrunkenness -= 1; }
    void          IncreaseDrunkenness() { m_iDrunkenness += 1; }
};


#endif