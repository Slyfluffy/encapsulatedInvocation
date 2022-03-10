//
//  command.cpp
//  Lab03
//
//  Created by Journey Curtis on 3/9/22.
//

#include "command.h"

/******************************************************************
 * RANDOM
 * This function generates a random number.
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/**************************************
 * CREATEBIRDSLEVEL1 :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to create birds
 * for level 1.
 *************************************/
void CreateBirdsLevel1::execute() {
   // spawns when there is nothing on the screen
   if (birds->size() == 0 && random(0, 15) == 1)
      birds->push_back(new Standard(size, 7.0));
   
   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Standard(size, 7.0));
}

/**************************************
 * CREATEBIRDSLEVEL2 :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to create birds
 * for level 2.
 *************************************/
void CreateBirdsLevel2::execute() {
   // spawns when there is nothing on the screen
   if (birds->size() == 0 && random(0, 15) == 1)
      birds->push_back(new Standard(size, 7.0, 12));

   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Standard(size, 5.0, 12));
   // spawn every 3 seconds
   if (random(0, 3 * 30) == 1)
      birds->push_back(new Sinker(size));
}

/**************************************
 * CREATEBIRDSLEVEL3 :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to create birds
 * for level 3.
 *************************************/
void CreateBirdsLevel3::execute() {
   // spawns when there is nothing on the screen
   if (birds->size() == 0 && random(0, 15) == 1)
      birds->push_back(new Standard(size, 5.0, 15));

   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Standard(size, 5.0, 15));
   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Sinker(size, 4.0, 22));
   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Floater(size));
}

/**************************************
 * CREATEBIRDSLEVEL4 :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to create birds
 * for level 4.
 *************************************/
void CreateBirdsLevel4::execute() {
   // spawns when there is nothing on the screen
   if (birds->size() == 0 && random(0, 15) == 1)
      birds->push_back(new Standard(size, 4.0, 18));

   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Standard(size, 4.0, 18));
   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Sinker(size, 3.5, 25));
   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Floater(size, 4.0, 25));
   // spawn every 4 seconds
   if (random(0, 4 * 30) == 1)
      birds->push_back(new Crazy(size));
}

/***************************************
 * ADVANCEBIRD :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to advance birds
 **************************************/
void AdvanceBird::execute() {
   for (auto bird : *birds) {
      bird->advance();
      hitRatio->adjust(bird->isDead() ? -1 : 0);
   }
}

/**********************************************
 * FIREBULLET :: FIREBULLET (CONSTRUCTOR)
 * INPUTS  :: bullets, ui, angle, time, score
 * OUTPUTS :: NONE
 * Assigns the variables in FireBullet for use.
 *********************************************/
FireBullet::FireBullet(std::list<Bullet*> * bullets, UserInput & ui, double angle, Time * time, Score * score) {
   this->bullets = bullets;
   this->ui = &ui;
   this->angle = angle;
   this->time = time;
   this->score = score;
}

/****************************************
 * FIREBULLET :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to fire a bullet.
 ***************************************/
void FireBullet::execute() {
   Bullet * p = nullptr;
   // a pellet can be shot at any time
   if (ui->isSpace())
      p = new Pellet(angle);
   // missiles can be shot at level 2 and higher
   else if (ui->isM() && time->level() > 1)
      p = new Missile(angle);
   // bombs can be shot at level 3 and higher
   else if (ui->isB() && time->level() > 2)
      p = new Bomb(angle);
   
   // add something if something has been added
   if (nullptr != p)
   {
      bullets->push_back(p);
      score->adjust(0 - p->getValue());
   }
}

/***************************************
 * MOVEMISSILE :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to move missiles
 **************************************/
void MoveMissile::execute() {
   // send movement information to all the bullets. Only the missile cares.
   for (auto bullet : *bullets)
      bullet->input(ui->isUp() + ui->isRight(), ui->isDown() + ui->isLeft(), ui->isB());
}

/*****************************************
 * ADVANCEBULLET :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to advance bullets
 ****************************************/
void AdvanceBullet::execute() {
   for (auto bullet : *bullets)
      bullet->move(*effects);
}

/*****************************************
 * ADVANCEEFFECTS :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to advance effects
 ****************************************/
void AdvanceEffects::execute() {
   for (auto effect : *effects)
      effect->fly();
}

/****************************************
 * DETECTHITS :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to detect hits on
 * birds.
 ***************************************/
void DetectHits::execute() {
   for (auto element : *birds)
      for (auto bullet : *bullets)
         if (!element->isDead() && !bullet->isDead() &&
             element->getRadius() + bullet->getRadius() >
             minimumDistance(element->getPosition(), element->getVelocity(),
                             bullet->getPosition(),  bullet->getVelocity()))
         {
            for (int i = 0; i < 25; i++)
               effects->push_back(new Fragment(bullet->getPosition(), bullet->getVelocity()));
            element->kill();
            bullet->kill();
            hitRatio->adjust(1);
         }
}

/*******************************************
 * REMOVEZOMBIES :: EXECUTE
 * INPUTS  :: NONE
 * OUTPUTS :: NONE
 * executes the command to remove dead items
 * from the lists.
 ******************************************/
void RemoveZombies::execute() {
   // remove the zombie birds
   for (auto it = birds->begin(); it != birds->end();)
      if ((*it)->isDead())
      {
         score->adjust((*it)->getPoints());
         it = birds->erase(it);
      }
      else
         ++it;
       
   // remove zombie bullets
   for (auto it = bullets->begin(); it != bullets->end(); )
      if ((*it)->isDead())
      {
         (*it)->death(*bullets);
         it = bullets->erase(it);
      }
      else
         ++it;
   
   // remove zombie fragments
   for (auto it = effects->begin(); it != effects->end();)
      if ((*it)->isDead())
         it = effects->erase(it);
      else
         ++it;
}
