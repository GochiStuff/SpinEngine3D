#include<iostream>

#include "Cube.h"


//For now the cube is center of the window . with a smooth acceleration and deaccelration . ( a fucking crazy effect )
// other than this the user can change the position of the light and also the speed of rotation 

//The Cube is default set to a prime rubics rube colour.



int main()
{
	RenderinngWindow rWindow;


	while (rWindow.getWindowIsOpen())
	{

		//Update
		rWindow.update();


		//Render
		rWindow.render();

	}

	return 0;
}


