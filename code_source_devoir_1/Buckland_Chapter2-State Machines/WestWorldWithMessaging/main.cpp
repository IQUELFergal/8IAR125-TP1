#include <fstream>
#include <time.h>
#include <thread>

#include "Locations.h"
#include "BaseGameEntity.h"

#include "Miner.h"
#include "MinersWife.h"
#include "Drunkard.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;




void ThreadUpdate(BaseGameEntity* entity)
{
    for (int i = 0; i < 30; ++i)
    {
        entity->Update();

        //dispatch any delayed messages
        Dispatch->DispatchDelayedMessages(entity);

        Sleep(800);
    }
}







int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  // create drunkard
  Drunkard* John = new Drunkard(ent_Drunkard);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(John);



  // Version with Thread
  std::thread minerThread(ThreadUpdate, Bob);
  std::thread wifeThread(ThreadUpdate, Elsa);
  std::thread drunkardThread(ThreadUpdate, John);

  minerThread.join();
  wifeThread.join();
  drunkardThread.join();


  // Version without thread
  //run Bob and Elsa through a few Update calls
  /* for (int i = 0; i<30; ++i)
  { 
    Bob->Update();
    Elsa->Update(); 
    John->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }*/

  //tidy up
  delete Bob;
  delete Elsa;
  delete John;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}











