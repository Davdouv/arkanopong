#ifdef __BONUS__FORWARD__DECLARATION
#undef __BONUS__FORWARD__DECLARATION

struct bonus;
typedef struct bonus Bonus, *BonusList;

#else

#ifndef __BONUS__H
#define __BONUS__H

#include "geometry.h"
#include "player.h"
#include "brick.h"

/** STRUCTURES **/

typedef struct bonus
{
  Point2D position;
  Vector2D direction;
  float radius;
  float speed;
  GLuint texture;
  GLuint textureTxt;
  PtBrick ptBrick;
  BrickType type;
  PtPlayer ptPlayer;
  bool actif;
  struct bonus* next;
  int animateTxt;
} Bonus, *BonusList;

/** FUNCTIONS **/

Bonus* createBonus(PtBrick ptBrick, GLuint* bonusTexture);
void addBonus(BonusList* bonusList, Bonus* bonus);
void bonusOrientation(Bonus* bonus, Player player);
void moveBonus (Bonus* bonus);
void drawBonus(Bonus* bonus);
void drawBonusText(Bonus* bonus);
void drawAllBonus(BonusList bonusList);

int selectBonus(BrickType type);

void barSizeUp (PtBar ptBar, GLuint* texture, int numPlayer);
void barSizeDown (PtBar ptBar, GLuint* texture, int numPlayer);
void barSpeedUp (PtBar ptBar);
void ballSpeedUp (PtBall ptBall);
void ballSpeedDown (PtBall ptBall);
void ballSizeUp (PtBall ptBall);
void moreBall (PtBall* ballList, Player* player);
void addLife (Player* player);
void slowPower(Player* player);
void fastPower(Player* player);

void getBonus(Bonus bonus, PtBall* ballList, GLuint* barTexture);

float bonusBottomPosition (Bonus bonus);
float bonusTopPosition (Bonus bonus);
float bonusLeftPosition (Bonus bonus);
float bonusRightPosition (Bonus bonus);

void deleteBonus(BonusList* bonusList, BonusList* bonus);
void deleteBonusList(BonusList* bonusList);

#endif
#endif