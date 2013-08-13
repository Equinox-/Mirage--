#ifndef _GETNEWS_H
#define _GETNEWS_H

#include <stdio.h> 
#include <string.h>

//=================================================================================================
// Copyright 2013 Evan Davies																   	 //
//=================================================================================================

class News
{
public: 
	News();
	~News();
	void ReadNews();
	char* GetNews()						{ return mNews; }

private:
	char* mNews;
};

#endif