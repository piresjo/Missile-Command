/**
 * mc.c
 */


#include "sl.h"
#include <stdlib.h>
#include <stdbool.h>
/************************************************
 *  DATA STRUCTURES FOR GAME STATE
 ***********************************************/

/** Global integer values for the textures.
    SIGIL recommends this */
 const char* cursorName = "Cursor.png";
 const char* cityName = "City.png";
 const char* eightMissilesName = "EightMissiles.png";
 const char* sevenMissilesName = "SevenMissiles.png";
 const char* sixMissilesName = "SixMissiles.png";
 const char* fiveMissilesName = "FiveMissiles.png";
 const char* fourMissilesName = "FourMissiles.png";
 const char* threeMissilesName = "ThreeMissiles.png";
 const char* twoMissilesName = "TwoMissiles.png";
 const char* oneMissileName = "OneMissile.png";
 const char* emptyName = "Template.png";
 const char* destroyedName = "DestroyedCity.png";
 const char* zeroName = "ZeroSprite.png";
 const char* oneName = "OneSprite.png";
 const char* twoName = "TwoSprite.png";
 const char* threeName = "ThreeSprite.png";
 const char* fourName = "FourSprite.png";
 const char* fiveName = "FiveSprite.png";
 const char* sixName = "SixSprite.png";
 const char* sevenName = "SevenSprite.png";
 const char* eightName = "EightSprite.png";
 const char* nineName = "NineSprite.png";
 int cursorTexture;
 int cityTexture;
 int eightMissilesTexture;
 int sevenMissilesTexture;
 int sixMissilesTexture;
 int fiveMissilesTexture;
 int fourMissilesTexture;
 int threeMissilesTexture;
 int twoMissilesTexture;
 int oneMissileTexture;
 int emptyTexture;
 int destroyedCityTexture;
 int zeroTexture;
 int oneTexture;
 int twoTexture;
 int threeTexture;
 int fourTexture;
 int fiveTexture;
 int sixTexture;
 int sevenTexture;
 int eightTexture;
 int nineTexture;

/** Round number. Will be used to help make game harder */
 int gameLevel = 1;

/** Score */
 int scoreCounter = 0;

/** Width and height of lc4 */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 124

/** Number of cities and launchers in the game */
#define NUM_CITIES 2

/** Number of targets incoming can target (cities + 1 launcher) */
#define NUM_TARGETS NUM_CITIES+1

/** Number of maximum outgoing and incoming allowed on the screen */
#define MAX_OUTGOING 1
#define MAX_INCOMING 3

/** Number of missiles allowed per round*/
#define MISSILES_PER_ROUND 8

/** The y position of where the cities and launcher are located */
#define GROUND_LEVEL 116

/** The x positions of where the cities and launcher are located */
#define LEFT_CITY_XPOS	20
#define RIGHT_CITY_XPOS 100
#define MISSILE_COMMAND_XPOS 60

/** The squared distance where missiles will contact
 * if it is in the radius */
#define CONTACT_RADIUS 250


/** Array of targets that the incoming will target */
int targets[NUM_TARGETS];

/** City struct that consists of the x position and 2D array */
/** representing the city's image */
typedef struct {
  bool isDestroyed;
  int x;
  int cityImage[8];
}City;

/** Array consisting of cities */
City cities[NUM_CITIES];

/** Missile launcher struct that consists of the number missiles */
/** that the launcher has left, its x position and 2D array */
/** representing the missile launcher's image */
typedef struct {
  bool isDestroyed;
  int missilesLeft;
  int x;
  int launcherImage[8];
}MissileLauncher;

/** Instance of the MissileLauncher Struct */
MissileLauncher missileLauncher;

/************************************************
 *  Projectile Struct
 *  lc4bool isActive - boolean representing
 *                     if the projectile
 *                     is active or not
 ***********************************************/
typedef struct {
  bool isActive;
  int startX;
  int startY;
  int currX;
  int currY;
  int endX;
  int endY;
  int deltX;
  int deltY;
  int stepX;
  int stepY;
  int error;
}Projectile;

/** Array consisting of missiles on the screen */
Projectile outgoing[MAX_OUTGOING];

/** Array consisting of incoming on the screen */
Projectile incoming[MAX_INCOMING];

/** Once player gets past level 2 or 3, add two more missiles
	for them to deal with */
Projectile moreIncoming[2];

/** Cursor struct consisting of the cursor's x and y position */
typedef struct{
  int x;
  int y;
}Cursor;

/** Instance of the cursor*/
Cursor cursor;

/***************************************************************
 * Debugging and utility functions
 * DO NOT EDIT
 ***************************************************************/

/************************************************
 *  abs - returns the absolute value
 ***********************************************/
int abs(int value)
{
  if(value < 0)
    return -value;
  return value;
}

/************************************************
 *  rand16 - This function returns a random
 *  number between 0 and 128
 ***********************************************/
int rand16 ()
{
  int randVal;
  srand(rand());
  randVal = rand() % 128;
  return randVal;

}

/***************************************************************
 * End of Debugging and utility functions
 ***************************************************************/

/************************************************
 * DrawCursor -
 * Draws the cursor sprite in white
 ***********************************************/
 void DrawCursor()
 {
    cursorTexture = slLoadTexture(cursorName);
    slSprite(cursorTexture, cursor.x, cursor.y, 1.0, 1.0);
 }

/************************************************
 *  DrawMissileLauncher -
 *  Draws the missile launcher in white based on
 *  the number of missiles left
 ***********************************************/
 void DrawMissileLauncher()
 {
 	/** First check if destroyed, then return */
 	if (missileLauncher.isDestroyed || missileLauncher.missilesLeft == 0) {
        emptyTexture = slLoadTexture(emptyName);
        slSprite(emptyTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}

 	/** Then check number, and draw sprites accordingly */
 	if (missileLauncher.missilesLeft == 8) {
        eightMissilesTexture = slLoadTexture(eightMissilesName);
 		slSprite(eightMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 7) {
 	    sevenMissilesTexture = slLoadTexture(sevenMissilesName);
 		slSprite(sevenMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 6) {
 	    sixMissilesTexture = slLoadTexture(sixMissilesName);
 		slSprite(sixMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 5) {
 	    fiveMissilesTexture = slLoadTexture(fiveMissilesName);
 		slSprite(fiveMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 4) {
 	    fourMissilesTexture = slLoadTexture(fourMissilesName);
 		slSprite(fourMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 3) {
 	    threeMissilesTexture = slLoadTexture(threeMissilesName);
 		slSprite(threeMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 2) {
 	    twoMissilesTexture = slLoadTexture(twoMissilesName);
 		slSprite(twoMissilesTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 	if (missileLauncher.missilesLeft == 1) {
 	    oneMissileTexture = slLoadTexture(oneMissileName);
 		slSprite(oneMissileTexture, missileLauncher.x, GROUND_LEVEL, 1.0, 1.0);
 		return;
 	}
 }

/************************************************
 *  DrawCities -
 *  Draws the cities in white
 ***********************************************/
 void DrawCities()
 {
 	/** Recall that there are only two cities */
 	/** Run a loop drawing the cities */
 	int i;
 	for (i = 0; i < 2; i++) {
 		City analizeCity = cities[i];
 		if (analizeCity.isDestroyed) {
            destroyedCityTexture = slLoadTexture(destroyedName);
 			slSprite(destroyedCityTexture, analizeCity.x, GROUND_LEVEL, 1.0, 1.0);
 		} else {
 		    cityTexture = slLoadTexture(cityName);
 			slSprite(cityTexture, analizeCity.x, GROUND_LEVEL, 1.0, 1.0);
 		}
 	}
 }

/************************************************
 *  DrawIncoming -
 *  Draws each incoming
 ***********************************************/
 void DrawIncoming()
 {
 	/** Note that there are at most three incoming missiles */
 	/** Loop through the array. If active, draw red line representing
 	missile */
 	slSetForeColor(0.75, 0.0, 0.0, 0.0);
 	int i;
 	for (i = 0; i < 3; i++) {
 		if (incoming[i].isActive) {
 			slLine(incoming[i].startX, incoming[i].startY,
 				incoming[i].currX, incoming[i].currY);
 		} else {
 			continue;
 		}
 	}
 	if (gameLevel > 1) {
 		for (i = 0; i < 2; i++) {
 			if (moreIncoming[i].isActive) {
 				slLine(moreIncoming[i].startX, moreIncoming[i].startY,
 					moreIncoming[i].currX, moreIncoming[i].currY);
 			} else {
 				continue;
 			}
 		}
 	}
 }

/************************************************
 *  DrawOutgoing
 *  Draws each outgoing missile
 ***********************************************/
 void DrawOutgoing()
 {
 	/** Since there is only at most one outgoing missile, just access
 	array directly */
 	slSetForeColor(0.0, 0.75, 0.0, 0.0);
 	if (outgoing[0].isActive) {
 		slLine(outgoing[0].startX, outgoing[0].startY,
 			outgoing[0].currX, outgoing[0].currY);
 	}
 }

 /* Draw score for the ones digit */
 void drawScoreOnes() {
 	int onesValue = scoreCounter % 10;
 	int firstDigitX = 111;
 	int secondDigitX = 118;
 	int digitsY = 5;
 	if (onesValue == 0) {
        zeroTexture = slLoadTexture(zeroName);
        slSprite(zeroTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 1) {
 	    oneTexture = slLoadTexture(oneName);
 		slSprite(oneTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 2) {
 	    twoTexture = slLoadTexture(twoName);
 		slSprite(twoTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 3) {
 	    threeTexture = slLoadTexture(threeName);
 		slSprite(threeTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 4) {
 	    fourTexture = slLoadTexture(fourName);
 		slSprite(fourTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 5) {
 	    fiveTexture = slLoadTexture(fiveName);
 		slSprite(fiveTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 6) {
 	    sixTexture = slLoadTexture(sixName);
 		slSprite(sixTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 7) {
 	    sevenTexture = slLoadTexture(sevenName);
 		slSprite(sevenTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 8) {
 	    eightTexture = slLoadTexture(eightName);
 		slSprite(eightTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (onesValue == 9) {
 	    nineTexture = slLoadTexture(nineName);
 		slSprite(nineTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 }

 /* Draw score for the tens digit */
 void drawScoreTens() {
 	int tensValue = scoreCounter / 10;
 	int firstDigitX = 111;
 	int secondDigitX = 118;
 	int digitsY = 5;
 	if (tensValue == 0) {
        zeroTexture = slLoadTexture(zeroName);
        slSprite(zeroTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 1) {
 	    oneTexture = slLoadTexture(oneName);
 		slSprite(oneTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 2) {
 	    twoTexture = slLoadTexture(twoName);
 		slSprite(twoTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 3) {
 	    threeTexture = slLoadTexture(threeName);
 		slSprite(threeTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 4) {
 	    fourTexture = slLoadTexture(fourName);
 		slSprite(fourTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 5) {
 	    fiveTexture = slLoadTexture(fiveName);
 		slSprite(fiveTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 6) {
 	    sixTexture = slLoadTexture(sixName);
 		slSprite(sixTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 7) {
 	    sevenTexture = slLoadTexture(sevenName);
 		slSprite(sevenTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 8) {
 	    eightTexture = slLoadTexture(eightName);
 		slSprite(eightTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 	if (tensValue == 9) {
 	    nineTexture = slLoadTexture(nineName);
 		slSprite(nineTexture, firstDigitX, digitsY, 1.0, 1.0);
 		return;
 	}
 }

 /************************************************
 *  DrawScore
 *  Draws the score
 ***********************************************/
 void DrawScore() {
 	int firstDigitX = 111;
 	int secondDigitX = 118;
 	int digitsY = 5;

 	/** Analyze the score */
 	if (scoreCounter >= 99) {
        nineTexture = slLoadTexture(nineName);
 		slSprite(nineTexture, firstDigitX, digitsY, 1.0, 1.0);
 		slSprite(nineTexture, secondDigitX, digitsY, 1.0, 1.0);
 		return;
 	} else {
 		drawScoreTens();
 		drawScoreOnes();
 	}
 }

/************************************************
 *  Redraw -
 *  Assuming that the PennSim is run with double
 *  buffered mode, (using the launcher ->
 *  (java -jar PennSim.jar -d))
 *  First, we should clear the video memory buffer
 *  using lc4_reset_vmem.  Then we draw the scene
 *  and then swap the video memory buffer using
 *  lc4_blt_vmem
 ***********************************************/
void Redraw()
{

  DrawCursor();
  DrawMissileLauncher();
  DrawCities();
  DrawOutgoing();
  DrawIncoming();
  DrawScore();
}

/************************************************
 *  Reset -
 *  Clears the screen
 ***********************************************/
void reset()
{
}

/************************************************
 *  ResetLevel -
 *  Resets the game screen.
 * 	But increase the level count and score
 ***********************************************/
 void ResetLevel()
 {
 	/** First, clear the screen */
 	reset();

 	/** Then reset the global vars */
 	gameLevel = gameLevel + 1;

 	/** Now, draw the cursor in the top-left corner */
 	cursor.x = 5;
 	cursor.y = 5;
 	DrawCursor();

 	/** Now, implement and draw the cities */
 	cities[0].isDestroyed = 0;
 	cities[0].x = LEFT_CITY_XPOS;
 	cities[0].cityImage[0] = 0x08;
  	cities[0].cityImage[1] = 0x68;
  	cities[0].cityImage[2] = 0x7D,
  	cities[0].cityImage[3] = 0x7D,
  	cities[0].cityImage[4] = 0x7D,
  	cities[0].cityImage[5] = 0x7D,
  	cities[0].cityImage[6] = 0xFF,
  	cities[0].cityImage[7] = 0xFF;
 	cities[1].isDestroyed = 0;
 	cities[1].x = RIGHT_CITY_XPOS;
 	cities[1].cityImage[0] = 0x08;
  	cities[1].cityImage[1] = 0x68;
  	cities[1].cityImage[2] = 0x7D,
  	cities[1].cityImage[3] = 0x7D,
  	cities[1].cityImage[4] = 0x7D,
  	cities[1].cityImage[5] = 0x7D,
  	cities[1].cityImage[6] = 0xFF,
  	cities[1].cityImage[7] = 0xFF;
 	DrawCities();

 	/** Now, implement and draw the launcher */
 	missileLauncher.missilesLeft = MISSILES_PER_ROUND;
 	missileLauncher.x = MISSILE_COMMAND_XPOS;
 	missileLauncher.launcherImage[0] = 0x66;
  	missileLauncher.launcherImage[1] = 0x99;
  	missileLauncher.launcherImage[2] = 0x66;
  	missileLauncher.launcherImage[3] = 0x99;
  	missileLauncher.launcherImage[4] = 0x66;
  	missileLauncher.launcherImage[5] = 0x99;
  	missileLauncher.launcherImage[6] = 0x66;
  	missileLauncher.launcherImage[7] = 0x99;
  	missileLauncher.isDestroyed = 0;
 	DrawMissileLauncher();

 	/** Define targets array */
 	targets[0] = LEFT_CITY_XPOS;
 	targets[1] = MISSILE_COMMAND_XPOS;
 	targets[2] = RIGHT_CITY_XPOS;

 	/** Define outgoing array */
 	outgoing[0].isActive = 0;
 	outgoing[1].isActive = 0;
 	outgoing[2].isActive = 0;

 	/** Define incoming array */
 	incoming[0].isActive = 0;

 	/** Draw score on top right */
 	DrawScore();

 }


/************************************************
 *  ResetGame -
 *  Resets the game.
 ***********************************************/

 void ResetGame()
 {
 	/** First, clear the screen */
 	reset();

 	/** Then reset the global vars */
 	gameLevel = 1;
 	scoreCounter = 0;

 	/** Now, draw the cursor in the top-left corner */
 	cursor.x = 5;
 	cursor.y = 5;
 	DrawCursor();

 	/** Now, implement and draw the cities */
 	cities[0].isDestroyed = 0;
 	cities[0].x = LEFT_CITY_XPOS;
 	cities[0].cityImage[0] = 0x08;
  	cities[0].cityImage[1] = 0x68;
  	cities[0].cityImage[2] = 0x7D,
  	cities[0].cityImage[3] = 0x7D,
  	cities[0].cityImage[4] = 0x7D,
  	cities[0].cityImage[5] = 0x7D,
  	cities[0].cityImage[6] = 0xFF,
  	cities[0].cityImage[7] = 0xFF;
 	cities[1].isDestroyed = 0;
 	cities[1].x = RIGHT_CITY_XPOS;
 	cities[1].cityImage[0] = 0x08;
  	cities[1].cityImage[1] = 0x68;
  	cities[1].cityImage[2] = 0x7D,
  	cities[1].cityImage[3] = 0x7D,
  	cities[1].cityImage[4] = 0x7D,
  	cities[1].cityImage[5] = 0x7D,
  	cities[1].cityImage[6] = 0xFF,
  	cities[1].cityImage[7] = 0xFF;
 	DrawCities();

 	/** Now, implement and draw the launcher */
 	missileLauncher.missilesLeft = MISSILES_PER_ROUND;
 	missileLauncher.x = MISSILE_COMMAND_XPOS;
 	missileLauncher.launcherImage[0] = 0x66;
  	missileLauncher.launcherImage[1] = 0x99;
  	missileLauncher.launcherImage[2] = 0x66;
  	missileLauncher.launcherImage[3] = 0x99;
  	missileLauncher.launcherImage[4] = 0x66;
  	missileLauncher.launcherImage[5] = 0x99;
  	missileLauncher.launcherImage[6] = 0x66;
  	missileLauncher.launcherImage[7] = 0x99;
  	missileLauncher.isDestroyed = 0;
 	DrawMissileLauncher();

 	/** Define targets array */
 	targets[0] = LEFT_CITY_XPOS;
 	targets[1] = MISSILE_COMMAND_XPOS;
 	targets[2] = RIGHT_CITY_XPOS;

 	/** Define outgoing array */
 	outgoing[0].isActive = 0;
 	outgoing[1].isActive = 0;
 	outgoing[2].isActive = 0;

 	/** Define incoming array */
 	incoming[0].isActive = 0;

 	/** Draw score on top right */
 	DrawScore();

 }

/* Move the cursor up */
void moveUp() {
	if (cursor.y - 4 < 0) {
		return;
	} else {
		cursor.y = cursor.y - 4;
		return;
	}
}
/* Move the cursor down */
void moveDown() {
	if (cursor.y + 4 > 123) {
		return;
	} else {
		cursor.y = cursor.y + 4;
		return;
	}
}
/* Move the cursor left */
void moveLeft() {
	if (cursor.x - 4 < 0) {
		return;
	} else {
		cursor.x = cursor.x - 4;
		return;
	}
}
/* Move the cursor right */
void moveRight() {
	if (cursor.x + 4 > 127) {
		return;
	} else {
		cursor.x = cursor.x + 4;
		return;
	}
}

/** Get the missile set up and ready for movement */
void fire() {
	if (outgoing[0].isActive == 0) {
		missileLauncher.missilesLeft = missileLauncher.missilesLeft - 1;
		outgoing[0].isActive = 1;
		outgoing[0].startX = MISSILE_COMMAND_XPOS;
		outgoing[0].currX = MISSILE_COMMAND_XPOS;
		outgoing[0].endX = cursor.x;
		outgoing[0].startY = GROUND_LEVEL;
		outgoing[0].currY = GROUND_LEVEL;
		outgoing[0].endY = cursor.y;

		outgoing[0].deltX = abs(outgoing[0].endX - outgoing[0].currX);
		outgoing[0].deltY = abs(outgoing[0].endY - outgoing[0].currY);

		if (outgoing[0].currX < outgoing[0].endX) {
			outgoing[0].stepX = 1;
		} else {
			outgoing[0].stepX = -1;
		}

		if (outgoing[0].currY < outgoing[0].endY) {
			outgoing[0].stepY = 1;
		} else {
			outgoing[0].stepY = -1;
		}

		if (outgoing[0].deltX > outgoing[0].deltY) {
			outgoing[0].error = outgoing[0].deltX;
		} else {
			outgoing[0].error = outgoing[0].deltY;
		}
		outgoing[0].error = outgoing[0].error / 2;

		return;
	}
}

void collisionLeft(int arrayPart) {
	incoming[arrayPart].isActive = 0;
	cities[0].isDestroyed = 1;
	return;
}

void collisionMid(int arrayPart) {
	incoming[arrayPart].isActive = 0;
	missileLauncher.isDestroyed = 1;
	missileLauncher.missilesLeft = 0;
	return;
}

void collisionRight(int arrayPart) {
	incoming[arrayPart].isActive = 0;
	cities[1].isDestroyed = 1;
	return;
}

void collisionLeftPastOne(int arrayPart) {
	moreIncoming[arrayPart].isActive = 0;
	cities[0].isDestroyed = 1;
	return;
}

void collisionMidPastOne(int arrayPart) {
	moreIncoming[arrayPart].isActive = 0;
	missileLauncher.isDestroyed = 1;
	missileLauncher.missilesLeft = 0;
	return;
}

void collisionRightPastOne(int arrayPart) {
	moreIncoming[arrayPart].isActive = 0;
	cities[1].isDestroyed = 1;
	return;
}

/** Check to see if the enemy missiles have collided with anything */
void moveMissileCheckCollisionPastOne() {
	int i;
	for (i = 0; i < 2; i++) {
		if (moreIncoming[i].isActive) {
			if (moreIncoming[i].currY >= moreIncoming[i].endY) {
				moreIncoming[i].isActive = 0;
				return;
			}
			if (moreIncoming[i].currY >= 109 && moreIncoming[i].currY <= 116 &&
				moreIncoming[i].currX >= 20 && moreIncoming[i].currX <= 27) {
				collisionLeftPastOne(i);
				return;
			}
			if (moreIncoming[i].currY >= 109 && moreIncoming[i].currY <= 116 &&
				moreIncoming[i].currX >= 60 && moreIncoming[i].currX <= 67) {
				collisionMidPastOne(i);
				return;
			}
			if (moreIncoming[i].currY >= 109 && moreIncoming[i].currY <= 116 &&
				moreIncoming[i].currX >= 100 && moreIncoming[i].currX <= 107) {
				collisionRightPastOne(i);
				return;
			}
		}
	}
}

void moveMissileCheckCollision() {
	int i;
	for (i = 0; i < 3; i++) {
		if (incoming[i].isActive) {
			if (incoming[i].currY >= incoming[i].endY) {
				incoming[i].isActive = 0;
				return;
			}
			if (incoming[i].currY >= 109 && incoming[i].currY <= 116 &&
				incoming[i].currX >= 20 && incoming[i].currX <= 27) {
				collisionLeft(i);
				return;
			}
			if (incoming[i].currY >= 109 && incoming[i].currY <= 116 &&
				incoming[i].currX >= 60 && incoming[i].currX <= 67) {
				collisionMid(i);
				return;
			}
			if (incoming[i].currY >= 109 && incoming[i].currY <= 116 &&
				incoming[i].currX >= 100 && incoming[i].currX <= 107) {
				collisionRight(i);
				return;
			}
		}
	}
	if (gameLevel > 1) {
		moveMissileCheckCollisionPastOne();
	}
}

/** Just like moveMissile(), except for the extra two missiles
	for after level 1 */
void moveMissilePastOne() {
	int i;
	for (i = 0; i < 2; i++) {
		if (moreIncoming[i].isActive) {
			if (moreIncoming[i].error > (moreIncoming[i].deltX * -1)) {
				moreIncoming[i].error = moreIncoming[i].error - moreIncoming[i].deltY;
				moreIncoming[i].currX = moreIncoming[i].currX + moreIncoming[i].stepX;
			}

			if (moreIncoming[i].error < moreIncoming[i].deltY) {
				moreIncoming[i].error = moreIncoming[i].error + moreIncoming[i].deltX;
				moreIncoming[i].currY = moreIncoming[i].currY + moreIncoming[i].stepY;
			}
		}
	}
}

/** Use the algorithm from HW6 (the line one) to move the missiles */
void moveMissile() {
	int i;
	moveMissileCheckCollision();
	for (i = 0; i < 3; i++) {
		if (incoming[i].isActive) {
			if (incoming[i].error > (incoming[i].deltX * -1)) {
				incoming[i].error = incoming[i].error - incoming[i].deltY;
				incoming[i].currX = incoming[i].currX + incoming[i].stepX;
			}

			if (incoming[i].error < incoming[i].deltY) {
				incoming[i].error = incoming[i].error + incoming[i].deltX;
				incoming[i].currY = incoming[i].currY + incoming[i].stepY;
			}
		}
	}
	if (gameLevel > 1) {
		moveMissilePastOne();
	}
}

/** Actually set up the specific missiles for movement */
void establishMissileHelper(int stateNumber, int counterNumber) {
	incoming[counterNumber].startX = rand16();
	incoming[counterNumber].startY = 0;
	incoming[counterNumber].currX = incoming[counterNumber].startX;
	incoming[counterNumber].currY = 0;
	if (stateNumber == 1) {
		incoming[counterNumber].endX = LEFT_CITY_XPOS;
		incoming[counterNumber].endY = GROUND_LEVEL;
		return;
	}
	if (stateNumber == 2) {
		incoming[counterNumber].endX = MISSILE_COMMAND_XPOS;
		incoming[counterNumber].endY = GROUND_LEVEL;
		return;
	}
	if (stateNumber == 3) {
		incoming[counterNumber].endX = RIGHT_CITY_XPOS;
		incoming[counterNumber].endY = GROUND_LEVEL;
		return;
	}
}

void establishMissileHelperPastOne(int stateNumber, int counterNumber) {
	moreIncoming[counterNumber].startX = rand16();
	moreIncoming[counterNumber].startY = 0;
	moreIncoming[counterNumber].currX = incoming[counterNumber].startX;
	moreIncoming[counterNumber].currY = 0;
	if (stateNumber == 1) {
		moreIncoming[counterNumber].endX = LEFT_CITY_XPOS;
		moreIncoming[counterNumber].endY = GROUND_LEVEL;
		return;
	}
	if (stateNumber == 2) {
		moreIncoming[counterNumber].endX = MISSILE_COMMAND_XPOS;
		moreIncoming[counterNumber].endY = GROUND_LEVEL;
		return;
	}
	if (stateNumber == 3) {
		moreIncoming[counterNumber].endX = RIGHT_CITY_XPOS;
		moreIncoming[counterNumber].endY = GROUND_LEVEL;
		return;
	}
}

/** Just like establishMissile(), except for the extra two missiles
	for after level 1 */
void establishMissilePastOne() {
	int i;
	int chooseRandNum;
	Projectile analyzeFireProjectile;
	for (i = 0; i < 2; i++) {
		analyzeFireProjectile = moreIncoming[i];
		if (analyzeFireProjectile.isActive) {
			continue;
		} else {
			moreIncoming[i].isActive = 1;
			chooseRandNum = rand16();
			if (chooseRandNum >= 0 && chooseRandNum < 42) {
				establishMissileHelperPastOne(1, i);

			}
			if (chooseRandNum >= 42 && chooseRandNum < 85) {
				establishMissileHelperPastOne(2, i);

			}
			if (chooseRandNum >= 85 && chooseRandNum < 128) {
				establishMissileHelperPastOne(3, i);
			}

			moreIncoming[i].deltX = abs(moreIncoming[i].endX - moreIncoming[i].currX);
			moreIncoming[i].deltY = abs(moreIncoming[i].endY - moreIncoming[i].currY);

			if (moreIncoming[i].currX < moreIncoming[i].endX) {
				moreIncoming[i].stepX = 1;
			} else {
				moreIncoming[i].stepX = -1;
			}

			if (moreIncoming[i].currY < moreIncoming[i].endY) {
				moreIncoming[i].stepY = 1;
			} else {
				moreIncoming[i].stepY = -1;
			}

			if (moreIncoming[i].deltX > moreIncoming[i].deltY) {
				moreIncoming[i].error = moreIncoming[i].deltX;
			} else {
				moreIncoming[i].error = moreIncoming[i].deltY;
			}
			moreIncoming[i].error = moreIncoming[i].error / 2;
		}
	}
}

/** Check if there's an empty spot */
/** If so, get a random number, if there's a match
	establish */
/** Once that happens, get a random number to determine what surviving
	structure will get hit */
void establishMissile() {
	int i;
	int chooseRandNum;
	Projectile analyzeFireProjectile;
	for (i = 0; i < 3; i++) {
		analyzeFireProjectile = incoming[i];
		if (analyzeFireProjectile.isActive) {
			continue;
		} else {
			incoming[i].isActive = 1;
			chooseRandNum = rand16();
			if (chooseRandNum >= 0 && chooseRandNum < 42) {
				establishMissileHelper(1, i);

			}
			if (chooseRandNum >= 42 && chooseRandNum < 85) {
				establishMissileHelper(2, i);

			}
			if (chooseRandNum >= 85 && chooseRandNum < 128) {
				establishMissileHelper(3, i);
			}

			incoming[i].deltX = abs(incoming[i].endX - incoming[i].currX);
			incoming[i].deltY = abs(incoming[i].endY - incoming[i].currY);

			if (incoming[i].currX < incoming[i].endX) {
				incoming[i].stepX = 1;
			} else {
				incoming[i].stepX = -1;
			}

			if (incoming[i].currY < incoming[i].endY) {
				incoming[i].stepY = 1;
			} else {
				incoming[i].stepY = -1;
			}

			if (incoming[i].deltX > incoming[i].deltY) {
				incoming[i].error = incoming[i].deltX;
			} else {
				incoming[i].error = incoming[i].deltY;
			}
			incoming[i].error = incoming[i].error / 2;
		}

	}
	if (gameLevel > 1) {
		establishMissilePastOne();
	}
}

/** If it does explode, see if it anything is within contact
	radius. If so, get rid of it */
void explosion() {
	int i;
	int deltX;
	int deltY;
	int squareX;
	int squareY;
	int sumSquare;
	for (i = 0; i < 3; i++) {
		deltX = outgoing[0].currX - incoming[i].currX;
		deltY = outgoing[0].currY - incoming[i].currY;
		squareX = deltX * deltX;
		squareY = deltY * deltY;
		sumSquare = squareX + squareY;
		if (sumSquare <= CONTACT_RADIUS) {
			scoreCounter = scoreCounter + 1;
			incoming[i].isActive = 0;
		}
	}
	outgoing[0].isActive = 0;
	return;
}

/** Check to see if outgoing missile is ready to explode, or if it already
	collided with an enemy missile */
void moveOutgoingCheckCollision() {
	int i;
	for (i = 0; i < 1; i++) {
		if (outgoing[i].isActive) {
			if (incoming[0].isActive && incoming[0].currX == outgoing[i].currX &&
				incoming[0].currY == outgoing[i].currY) {
				incoming[0].isActive = 0;
				outgoing[i].isActive = 0;
				scoreCounter = scoreCounter + 1;
				return;
			}
			if (incoming[1].isActive && incoming[1].currX == outgoing[i].currX &&
				incoming[1].currY == outgoing[i].currY) {
				incoming[1].isActive = 0;
				outgoing[i].isActive = 0;
				scoreCounter = scoreCounter + 1;
				return;
			}
			if (incoming[2].isActive && incoming[2].currX == outgoing[i].currX &&
				incoming[2].currY == outgoing[i].currY) {
				incoming[2].isActive = 0;
				outgoing[i].isActive = 0;
				scoreCounter = scoreCounter + 1;
				return;
			}
			if (outgoing[i].currY >= outgoing[i].endY - 2 &&
				outgoing[i].currY <= outgoing[i].endY + 2 &&
				outgoing[i].currX >= outgoing[i].endX - 2 &&
				outgoing[i].currX <= outgoing[i].endX + 2) {
				explosion();
				return;
			}
		}
	}
}

/** Utilize the algorithm from HW6 to move the outgoing missile */
void moveOutgoingMissile() {
	if (outgoing[0].isActive) {
		moveOutgoingCheckCollision();


		if (outgoing[0].error > (outgoing[0].deltX * -1)) {
			outgoing[0].error = outgoing[0].error - outgoing[0].deltY;
			outgoing[0].currX = outgoing[0].currX + outgoing[0].stepX;
		}

		if (outgoing[0].error < outgoing[0].deltY) {
			outgoing[0].error = outgoing[0].error + outgoing[0].deltX;
			outgoing[0].currY = outgoing[0].currY + outgoing[0].stepY;
		}
	}
}

/************************************************
 *  main -
 *  Initalize game state by reseting the game state
 *  Loops until the the user loses
 ***********************************************/
 int main ()
 {
  //** Print to screen and initalize game state. */
  slWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Missile Command", 0);
  int inputValLeft;
  int inputValRight;
  int inputValUp;
  int inputValDown;
  int inputValFire;

  ResetGame();

  while(1)
  {
  	/** Check if all three targets are destroyed
  		if so, game over */
  	if (cities[0].isDestroyed && missileLauncher.isDestroyed
  		&& cities[1].isDestroyed) {
  		break;
  	}

  	/** Check if all missiles have been expended
  		if so, reset the level and increase the level counter */
  	if (missileLauncher.missilesLeft == 0 &&
  		missileLauncher.isDestroyed == 0) {
  		ResetLevel();
  	}
  	/** Check for input */
  	inputValLeft = slGetKey('a');
  	inputValRight = slGetKey('d');
  	inputValUp = slGetKey('w');
  	inputValDown = slGetKey('s');
  	inputValFire = slGetKey('r');

  	/** If direction button pressed, move */
  	if (inputValUp != 0) {
  		moveUp();
  	}
  	if (inputValLeft != 0) {
  		moveLeft();
  	}
  	if (inputValDown != 0) {
  		moveDown();
  	}
  	if (inputValRight != 0) {
  		moveRight();
  	}

  	/** If fire button pressed */
  	if (inputValFire != 0) {
  		fire();
  	}

  	/** Go through the incoming array */
  	establishMissile();
  	moveMissile();

  	/** Check the outgoing missile */
  	moveOutgoingMissile();

  	Redraw();
  	slRender();
  }

  slClose();
  return 0;
}
