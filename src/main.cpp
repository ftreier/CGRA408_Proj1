//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>
#include <string>

#include "renderer.hpp"

using namespace std;

//Main program
int main(int argc, char **argv)
{
	if (argc != 2) {
		cout << "Scene filename expected, eg:" << endl;
		abort(); // Unrecoverable error
	}

	Renderer().Render(argv[1]);
}