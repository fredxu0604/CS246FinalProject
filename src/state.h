#ifndef __STATE_H__
#define __STATE_H__

enum class GameState {
  PreRoll,
  JustRolled,
  OnTheMove,
  SquareArrival,
  ChoiceBuyProperty,
  IdleTurn,
  Auction,
  Trade,
  MoneyCritical,
  EndTurn
  
};

#endif
