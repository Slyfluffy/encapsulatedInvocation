/**********************************************************************
 * Skeet
 * No actual birds were killed in the making of this game.
 **********************************************************************/

#include <string>
#include <sstream>
#include "skeet.h"
using namespace std;


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_18
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied 
#define _USE_MATH_DEFINES
#include <math.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // _WIN32

/************************
 * SKEET ANIMATE
 * move the gameplay by one unit of time
 ************************/
void Skeet::animate()
{
   time++;
   
   // if status, then do not move the game
   if (time.isStatus())
   {
      // get rid of the bullets and the birds without changing the score
      birds.clear();
      bullets.clear();
      effects.clear();
      return;
   }
   
   // spawn
   spawn();
   
   AbstractCommand * moveBirds = new AdvanceBird(&birds, &hitRatio);
   AbstractCommand * moveBullets = new AdvanceBullet(&bullets, &effects);
   AbstractCommand * moveEffects = new AdvanceEffects(&effects);
   AbstractCommand * detect = new DetectHits(&birds, &bullets, &effects, &hitRatio);
   AbstractCommand * remove = new RemoveZombies(&birds, &bullets, &effects, &score);
   
   invoker.setCommand(moveBirds);
   invoker.executeCommand();
   
   invoker.setCommand(moveBullets);
   invoker.executeCommand();
   
   invoker.setCommand(moveEffects);
   invoker.executeCommand();
   
   invoker.setCommand(detect);
   invoker.executeCommand();
   
   invoker.setCommand(remove);
   invoker.executeCommand();
}

/************************************************************************
 * DRAW Background
 * Fill in the background
 *  INPUT color   Background color
 *************************************************************************/
void Skeet::drawBackground(double redBack, double greenBack, double blueBack) const
{
   glBegin(GL_TRIANGLE_FAN);

   // two rectangles is the fastest way to fill the screen.
   glColor3f((GLfloat)redBack /* red % */, (GLfloat)greenBack /* green % */, (GLfloat)blueBack /* blue % */);
   glVertex2f((GLfloat)0.0, (GLfloat)0.0);
   glVertex2f((GLfloat)dimensions.getX(), (GLfloat)0.0);
   glVertex2f((GLfloat)dimensions.getX(), (GLfloat)dimensions.getY());
   glVertex2f((GLfloat)0.0, (GLfloat)dimensions.getY());

   glEnd();
}

/************************************************************************
 * DRAW Timer
 * Draw a large timer on the screen
 *  INPUT percent     Amount of time left
 *        Foreground  Foreground color
 *        Background  Background color
 *************************************************************************/
void Skeet::drawTimer(double percent,
                     double redFore, double greenFore, double blueFore,
                     double redBack, double greenBack, double blueBack) const
{
   double radians;

   GLfloat length = (GLfloat)dimensions.getX();
   GLfloat half = length / (GLfloat)2.0;

   // do the background stuff
   drawBackground(redBack, greenBack, blueBack);

   // foreground stuff
   radians = percent * M_PI * 2.0;
   GLfloat x_extent = half + length * (GLfloat)sin(radians);
   GLfloat y_extent = half + length * (GLfloat)cos(radians);

   // get read to draw the triangles
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)redFore /* red % */, (GLfloat)greenFore /* green % */, (GLfloat)blueFore /* blue % */);
   glVertex2f(half, half);

   // fill in the triangles, one eight at a time
   switch ((int)(percent * 8.0))
   {
   case 7: // 315 - 360
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, length);
      break;
   case 6: // 270 - 315
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, half);
      break;
   case 5: // 225 - 270
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      break;
   case 4: // 180 - 225
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(half, 0.0);
      break;
   case 3: // 135 - 180
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      glVertex2f(length, 0.0);
      break;
   case 2: // 90 - 135 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      break;
   case 1: // 45 - 90 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      break;
   case 0: // 0 - 45 degrees
      glVertex2f(half, length);
      break;
   }
   glVertex2f(x_extent, y_extent);

   // complete drawing
   glEnd();

   // draw the red line now
   glBegin(GL_LINES);
   glColor3f((GLfloat)0.6, (GLfloat)0.0, (GLfloat)0.0);
   glVertex2f(half, half);
   glVertex2f(x_extent, y_extent);
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void drawText(const Point& topLeft, const char* text) 
{
   void* pFont = GLUT_TEXT;
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);

   // prepare to output the text from the top-left corner
   glRasterPos2f((GLfloat)topLeft.getX(), (GLfloat)topLeft.getY());

   // loop through the text
   for (const char* p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}
void drawText(const Point & topLeft, const string & text)
{
   drawText(topLeft, text.c_str());
}

/************************
 * SKEET DRAW LEVEL
 * output everything that will be on the screen
 ************************/
void Skeet::drawLevel() const
{
   // output the background
   drawBackground(time.level() * .1, 0.0, 0.0);
   
   // output the gun
   gun.display();
         
   // output the birds, bullets, and fragments
   for (auto effect : effects)
      effect->render();
   for (auto bullet : bullets)
      bullet->output();
   for (auto element : birds)
      element->draw();
   
   // status
   drawText(Point(10,                         dimensions.getY() - 30), score.getText()  );
   drawText(Point(dimensions.getX() / 2 - 30, dimensions.getY() - 30), time.getText()   );
   drawText(Point(dimensions.getX() - 110,    dimensions.getY() - 30), hitRatio.getText());
}

/************************
 * SKEET DRAW STATUS
 * place the status message on the center of the screen
 ************************/
void Skeet::drawStatus() const
{
   // output the text information
   ostringstream sout;
   if (time.isGameOver())
   {
      // draw the end of game message
      drawText(Point(dimensions.getX() / 2 - 30, dimensions.getY() / 2 + 10),
               "Game Over");

      // draw end of game status
      drawText(Point(dimensions.getX() / 2 - 30, dimensions.getY() / 2 - 10),
               score.getText());
   }
   else
   {
      // output the status timer
      drawTimer(1.0 - time.percentLeft(),
                     (time.level() - 0.0) * .1, 0.0, 0.0,
                     (time.level() - 1.0) * .1, 0.0, 0.0);

      // draw the message giving a countdown
      sout << "Level " << time.level()
           << " begins in " << time.secondsLeft() << " seconds";
      drawText(Point(dimensions.getX() / 2 - 110, dimensions.getY() / 2 - 10),
         sout.str());
   }
}

/************************
 * SKEET INTERACT
 * handle all user input
 ************************/
void Skeet::interact(UserInput & ui)
{
   // reset the game
   if (time.isGameOver() && ui.isSpace())
   { 
      time.reset();
      score.reset();
      hitRatio.reset();
      return;
   }

   // gather input from the interface
   gun.interact(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft());

   // a pellet can be shot at any time
   AbstractCommand * command = new FireBullet(&bullets, ui, gun.getAngle(), &time, &score);
   invoker.setCommand(command);
   invoker.executeCommand();
   
   // send movement information to all the bullets. Only the missile cares.
   AbstractCommand * move = new MoveMissile(&bullets, ui);
   invoker.setCommand(move);
   invoker.executeCommand();
}

/************************
 * SKEET SPAWN
 * lanuch new birds
 ************************/
void Skeet::spawn()
{
   AbstractCommand * command;
   switch (time.level())
   {
      // in level 1 spawn big birds occasionally
      case 1:
         command = new CreateBirdsLevel1(&birds);
         invoker.setCommand(command);
         invoker.executeCommand();
         break;
      // two kinds of birds in level 2
      case 2:
         command = new CreateBirdsLevel2(&birds);
         invoker.setCommand(command);
         invoker.executeCommand();
         break;
      // three kinds of birds in level 3
      case 3:
         command = new CreateBirdsLevel3(&birds);
         invoker.setCommand(command);
         invoker.executeCommand();
         break;
      // three kinds of birds in level 4
      case 4:
         command = new CreateBirdsLevel4(&birds);
         invoker.setCommand(command);
         invoker.executeCommand();
         break;
      default:
         break;
   }
}
