/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/
#include "MainWindow.h"
#include "Game.h"

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
	presseddown = wnd.kbd.KeyIsPressed(VK_UP);

}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	if (presseddown)
	{
		print_Hi();


	}
}

void Game::print_Hi()
{
	//Horizontal line
	gfx.PutPixel(100, 100, 255, 255, 255);
	gfx.PutPixel(99, 100, 255, 255, 255);
	gfx.PutPixel(98, 100, 255, 255, 255);
	gfx.PutPixel(97, 100, 255, 255, 255);
	gfx.PutPixel(96, 100, 255, 255, 255);
	gfx.PutPixel(95, 100, 255, 255, 255);
	gfx.PutPixel(94, 100, 255, 255, 255);
	gfx.PutPixel(93, 100, 255, 255, 255);
	gfx.PutPixel(92, 100, 255, 255, 255);

	//vertical line right
	gfx.PutPixel(100, 101, 255, 255, 255);
	gfx.PutPixel(100, 102, 255, 255, 255);
	gfx.PutPixel(100, 103, 255, 255, 255);
	gfx.PutPixel(100, 104, 255, 255, 255);
	gfx.PutPixel(100, 105, 255, 255, 255);
	gfx.PutPixel(100, 106, 255, 255, 255);
	gfx.PutPixel(100, 107, 255, 255, 255);
	gfx.PutPixel(100, 108, 255, 255, 255);
	gfx.PutPixel(100, 109, 255, 255, 255);
	gfx.PutPixel(100, 100, 255, 255, 255);
	gfx.PutPixel(100, 99, 255, 255, 255);
	gfx.PutPixel(100, 98, 255, 255, 255);
	gfx.PutPixel(100, 97, 255, 255, 255);
	gfx.PutPixel(100, 96, 255, 255, 255);
	gfx.PutPixel(100, 95, 255, 255, 255);
	gfx.PutPixel(100, 94, 255, 255, 255);
	gfx.PutPixel(100, 93, 255, 255, 255);
	gfx.PutPixel(100, 92, 255, 255, 255);

	//vertical line Left
	gfx.PutPixel(92, 101, 255, 255, 255);
	gfx.PutPixel(92, 102, 255, 255, 255);
	gfx.PutPixel(92, 103, 255, 255, 255);
	gfx.PutPixel(92, 104, 255, 255, 255);
	gfx.PutPixel(92, 105, 255, 255, 255);
	gfx.PutPixel(92, 106, 255, 255, 255);
	gfx.PutPixel(92, 107, 255, 255, 255);
	gfx.PutPixel(92, 108, 255, 255, 255);
	gfx.PutPixel(92, 109, 255, 255, 255);
	gfx.PutPixel(92, 100, 255, 255, 255);
	gfx.PutPixel(92, 99, 255, 255, 255);
	gfx.PutPixel(92, 98, 255, 255, 255);
	gfx.PutPixel(92, 97, 255, 255, 255);
	gfx.PutPixel(92, 96, 255, 255, 255);
	gfx.PutPixel(92, 95, 255, 255, 255);
	gfx.PutPixel(92, 94, 255, 255, 255);
	gfx.PutPixel(92, 93, 255, 255, 255);
	gfx.PutPixel(92, 92, 255, 255, 255);

	gfx.PutPixel(110, 94, 255, 255, 255);
	gfx.PutPixel(110, 95, 255, 255, 255);
	gfx.PutPixel(110, 101, 255, 255, 255);
	gfx.PutPixel(110, 102, 255, 255, 255);
	gfx.PutPixel(110, 103, 255, 255, 255);
	gfx.PutPixel(110, 104, 255, 255, 255);
	gfx.PutPixel(110, 105, 255, 255, 255);
	gfx.PutPixel(110, 106, 255, 255, 255);
	gfx.PutPixel(110, 107, 255, 255, 255);
	gfx.PutPixel(110, 108, 255, 255, 255);
	gfx.PutPixel(110, 109, 255, 255, 255);
	gfx.PutPixel(110, 100, 255, 255, 255);
}
