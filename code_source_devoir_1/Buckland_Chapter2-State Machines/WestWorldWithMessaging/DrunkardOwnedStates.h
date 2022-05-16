#ifndef DRUNKARD_OWNED_STATES_H
#define DRUNKARD_OWNED_STATES_H


#include "fsm/State.h"


class Drunkard;
struct Telegram;


//------------------------------------------------------------------------
//
//  Drunkard will go to the Saloon and drink until his fatigue is too high
//  
//------------------------------------------------------------------------
class GoToTheSaloonAndDrink : public State<Drunkard>
{
private:

	GoToTheSaloonAndDrink() {}

	//copy ctor and assignment should be private
	GoToTheSaloonAndDrink(const GoToTheSaloonAndDrink&);
	GoToTheSaloonAndDrink& operator=(const GoToTheSaloonAndDrink&);

public:

	//this is a singleton
	static GoToTheSaloonAndDrink* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Drunkard will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class GoHomeAndSleep : public State<Drunkard>
{
private:

	GoHomeAndSleep() {}

	//copy ctor and assignment should be private
	GoHomeAndSleep(const GoHomeAndSleep&);
	GoHomeAndSleep& operator=(const GoHomeAndSleep&);

public:

	//this is a singleton
	static GoHomeAndSleep* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);
};

//------------------------------------------------------------------------
//
//  Drunkard will fight
//  
//------------------------------------------------------------------------
class Fight : public State<Drunkard>
{
private:

	Fight() {}

	//copy ctor and assignment should be private
	Fight(const Fight&);
	Fight& operator=(const Fight&);

public:

	//this is a singleton
	static Fight* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* agent, const Telegram& msg);
};

#endif