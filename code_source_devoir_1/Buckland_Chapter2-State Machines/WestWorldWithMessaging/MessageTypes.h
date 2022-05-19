#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_EnterBar,
  Msg_ProvokeFight,
  Msg_WinFight
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case 0:
    
    return "HiHoneyImHome"; 

  case 1:
    
    return "StewReady";

  case 2:

      return "EnterBar";

  case 3:

      return "ProvokeFight";

  case 4:

      return "WinFight";

  default:

    return "Not recognized!";
  }
}

#endif