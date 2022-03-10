//
//  command.h
//  Skeet
//
//  Created by Journey Curtis on 3/9/22.
//

#ifndef command_h
#define command_h

#include "bird.h"
#include "bullet.h"
#include "score.h"
#include "uiInteract.h"
#include "time.h"

/************************************
 * SKEET :: ABSTRACTCOMMAND
 * Abstract class for commands.
 * Contains a single method, execute.
 ***********************************/
class AbstractCommand {
   
public:
   virtual void execute() = 0;
};

/**********************************
 * SKEET :: CREATEBIRDSLEVEL1
 * ATTRIBUTES :: size, birds
 * Command class that creates birds
 * for level one.
 *********************************/
class CreateBirdsLevel1 : public AbstractCommand {
private:
   double size;
   std::list<Bird*> * birds;
   
public:
   CreateBirdsLevel1(std::list<Bird*> * birds) : size(30.0), birds(birds) {}
   void execute();
};

/**********************************
 * SKEET :: CREATEBIRDSLEVEL2
 * ATTRIBUTES :: size, birds
 * Command class that creates birds
 * for level two.
 *********************************/
class CreateBirdsLevel2 : public AbstractCommand {
private:
   double size;
   std::list<Bird*> * birds;
   
public:
   CreateBirdsLevel2(std::list<Bird*> * birds) : size(25.0), birds(birds) {}
   void execute();
};

/**********************************
 * SKEET :: CREATEBIRDSLEVEL3
 * ATTRIBUTES :: size, birds
 * Command class that creates birds
 * for level three.
 *********************************/
class CreateBirdsLevel3 : public AbstractCommand {
private:
   double size;
   std::list<Bird*> * birds;
   
public:
   CreateBirdsLevel3(std::list<Bird*> * birds) : size(20.0), birds(birds) {}
   void execute();
};

/**********************************
 * SKEET :: CREATEBIRDSLEVEL4
 * ATTRIBUTES :: size, birds
 * Command class that creates birds
 * for level four.
 *********************************/
class CreateBirdsLevel4 : public AbstractCommand {
private:
   double size;
   std::list<Bird*> * birds;
   
public:
   CreateBirdsLevel4(std::list<Bird*> * birds) : size(15.0), birds(birds) {}
   void execute();
};

/************************************
 * SKEET :: ADVANCEBIRD
 * ATTRIBUTES :: birds, hitRatio
 * Command class that advances all
 * the birds
 ***********************************/
class AdvanceBird : public AbstractCommand {
private:
   std::list<Bird*> * birds;
   HitRatio * hitRatio;
   
public:
   AdvanceBird(std::list<Bird*> * birds, HitRatio * hitRatio) : birds(birds), hitRatio(hitRatio) {}
   void execute();
};

/***********************************************
 * SKEET :: FIREBULLET
 * ATTRIBUTES :: bullets, ui, angle, time, score
 * Command class that handles the creation
 * of a bullet
 **********************************************/
class FireBullet : public AbstractCommand {
private:
   std::list<Bullet*> * bullets;
   UserInput * ui;
   double angle;
   Time * time;
   Score * score;
   
public:
   FireBullet(std::list<Bullet*> * bullets, UserInput & ui, double angle, Time * time, Score * score);
   void execute();
};

/*************************************
 * SKEET :: MoveMissile
 * ATTRIBUTES :: bullets, ui
 * Command class that handles movement
 * of a missile
 ************************************/
class MoveMissile : public AbstractCommand {
private:
   std::list<Bullet*> * bullets;
   UserInput * ui;
   
public:
   MoveMissile(std::list<Bullet*> * bullets, UserInput & ui) : bullets(bullets), ui(&ui) {}
   void execute();
};

/*********************************
 * SKEET :: ADVANCEBULLET
 * ATTRIBUTES :: bullets, effects
 * Command class that advances all
 * the bullets
 ********************************/
class AdvanceBullet : public AbstractCommand {
private:
   std::list<Bullet*> * bullets;
   std::list<Effect*> * effects;
   
public:
   AdvanceBullet(std::list<Bullet*> * bullets, std::list<Effect*> * effects) : bullets(bullets), effects(effects) {}
   void execute();
};

/************************************
 * SKEET :: ADVANCEEFFECTS
 * ATTRIBUTES :: effects
 * Command class that advances all
 * the effects
 ***********************************/
class AdvanceEffects : public AbstractCommand {
private:
   std::list<Effect*> * effects;
   
public:
   AdvanceEffects(std::list<Effect*> * effects) : effects(effects) {}
   void execute();
};

/*******************************
 * SKEET :: DETECTHITS
 * ATTRIBUTES :: birds, hitRatio
 * Handles hit detection
 ******************************/
class DetectHits : public AbstractCommand {
private:
   std::list<Bird*> * birds;
   std::list<Bullet*> * bullets;
   std::list<Effect*> * effects;
   HitRatio * hitRatio;
   
public:
   DetectHits(std::list<Bird*> * birds, std::list<Bullet*> * bullets, std::list<Effect*> * effects, HitRatio * hitRatio) {
      this->birds = birds;
      this->bullets = bullets;
      this->effects = effects;
      this->hitRatio = hitRatio;
   }
   void execute();
};

/***************************************
 * SKEET :: REMOVEZOMBIES
 * ATTRIBUTES :: birds, bullets, effects
 * Removes all dead items from the lists
 **************************************/
class RemoveZombies : public AbstractCommand {
private:
   std::list<Bird*> * birds;
   std::list<Bullet*> * bullets;
   std::list<Effect*> * effects;
   Score * score;
   
public:
   RemoveZombies(std::list<Bird*> * birds, std::list<Bullet*> * bullets, std::list<Effect*> * effects, Score * score) {
      this->birds = birds;
      this->bullets = bullets;
      this->effects = effects;
      this->score = score;
   }
   void execute();
};

#endif /* command_h */
