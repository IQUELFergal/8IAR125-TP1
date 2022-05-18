#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Miner_Bob,

  ent_Elsa,

  ent_Drunkard
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Miner_Bob:

    return "Miner Bob";

  case ent_Elsa:
    
    return "Elsa"; 

  case ent_Drunkard:

      return "Drunkard";

  default:

    return "UNKNOWN!";
  }
}

inline WORD GetColorOfEntity(int n)
{
    switch (n)
    {
    case ent_Miner_Bob:

        return FOREGROUND_RED | FOREGROUND_INTENSITY;

    case ent_Elsa:

        return FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    case ent_Drunkard:

        return FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    default:

        return FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

#endif