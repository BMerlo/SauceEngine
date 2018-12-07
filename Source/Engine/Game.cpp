/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/
#include "MainWindow.h"
#include "EventHandler.h"
#include "Game.h"

bool doOnce = false;

int x;
int y;
int redColor;
int greenColor;
int blueColor;


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();

	struct A : EventHandler<Mouse>
	{
		void handle(Mouse* e) override
		{
			OutputDebugStringA("Event Handled\n");
		}
	}_A;

	Mouse Event;
	if (wnd.mouse.RightIsPressed())  //otherwise it triggers constantly
		_A.handle(&Event);

	if (wnd.kbd.KeyIsPressed(VK_UP) || wnd.kbd.KeyIsPressed(VK_DOWN) || wnd.mouse.LeftIsPressed() || wnd.mouse.RightIsPressed())
	{
		presseddown = true;
	}
	else
	{
		presseddown = false;
		doOnce = false;
	}
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{


	if (presseddown)
	{
		if (doOnce == false)
		{
			x = rand() % 500 + 1;
			y = rand() % 500 + 1;

			redColor = rand() % 255 + 1;
			greenColor = rand() % 255 + 1;
			blueColor = rand() % 255 + 1;

			if (redColor == 0 && greenColor == 0 && blueColor == 0)
			{
				while (redColor == 0 && greenColor == 0 && blueColor == 0)
				{
					redColor = rand() % 255 + 1;
					greenColor = rand() % 255 + 1;
					blueColor = rand() % 255 + 1;
				}
			}
			doOnce = true;
		}
		print_Hi(x, y, redColor, greenColor, blueColor);
	}
}

void Game::print_Hi(int x, int y, int red, int green, int blue)
{
	//H character
	gfx.PutPixel(x + 10, y + 10, red, green, blue);//Horizontal line
	gfx.PutPixel(x + 9, y + 10, red, green, blue);
	gfx.PutPixel(x + 8, y + 10, red, green, blue);
	gfx.PutPixel(x + 7, y + 10, red, green, blue);
	gfx.PutPixel(x + 6, y + 10, red, green, blue);
	gfx.PutPixel(x + 5, y + 10, red, green, blue);
	gfx.PutPixel(x + 4, y + 10, red, green, blue);
	gfx.PutPixel(x + 3, y + 10, red, green, blue);
	gfx.PutPixel(x + 2, y + 10, red, green, blue);
	gfx.PutPixel(x + 10, y + 11, red, green, blue);//vertical line right
	gfx.PutPixel(x + 10, y + 12, red, green, blue);
	gfx.PutPixel(x + 10, y + 13, red, green, blue);
	gfx.PutPixel(x + 10, y + 14, red, green, blue);
	gfx.PutPixel(x + 10, y + 15, red, green, blue);
	gfx.PutPixel(x + 10, y + 16, red, green, blue);
	gfx.PutPixel(x + 10, y + 17, red, green, blue);
	gfx.PutPixel(x + 10, y + 18, red, green, blue);
	gfx.PutPixel(x + 10, y + 19, red, green, blue);
	gfx.PutPixel(x + 10, y + 10, red, green, blue);
	gfx.PutPixel(x + 10, y + 9, red, green, blue);
	gfx.PutPixel(x + 10, y + 8, red, green, blue);
	gfx.PutPixel(x + 10, y + 7, red, green, blue);
	gfx.PutPixel(x + 10, y + 6, red, green, blue);
	gfx.PutPixel(x + 10, y + 5, red, green, blue);
	gfx.PutPixel(x + 10, y + 4, red, green, blue);
	gfx.PutPixel(x + 10, y + 3, red, green, blue);
	gfx.PutPixel(x + 10, y + 2, red, green, blue);
	gfx.PutPixel(x + 2, y + 11, red, green, blue);//vertical line Left
	gfx.PutPixel(x + 2, y + 12, red, green, blue);
	gfx.PutPixel(x + 2, y + 13, red, green, blue);
	gfx.PutPixel(x + 2, y + 14, red, green, blue);
	gfx.PutPixel(x + 2, y + 15, red, green, blue);
	gfx.PutPixel(x + 2, y + 16, red, green, blue);
	gfx.PutPixel(x + 2, y + 17, red, green, blue);
	gfx.PutPixel(x + 2, y + 18, red, green, blue);
	gfx.PutPixel(x + 2, y + 19, red, green, blue);
	gfx.PutPixel(x + 2, y + 10, red, green, blue);
	gfx.PutPixel(x + 2, y + 9, red, green, blue);
	gfx.PutPixel(x + 2, y + 8, red, green, blue);
	gfx.PutPixel(x + 2, y + 7, red, green, blue);
	gfx.PutPixel(x + 2, y + 6, red, green, blue);
	gfx.PutPixel(x + 2, y + 5, red, green, blue);
	gfx.PutPixel(x + 2, y + 4, red, green, blue);
	gfx.PutPixel(x + 2, y + 3, red, green, blue);
	gfx.PutPixel(x + 2, y + 2, red, green, blue);

	// lower case I character
	gfx.PutPixel(x + 10 + 10, y + 4, red, green, blue);//Period
	gfx.PutPixel(x + 10 + 10, y + 12, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 13, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 14, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 15, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 16, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 17, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 18, red, green, blue);//i
	gfx.PutPixel(x + 10 + 10, y + 19, red, green, blue);//i
}
