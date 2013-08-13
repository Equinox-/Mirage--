#include "News.h"

News::News()
:mNews(NULL)
{

}

News::~News()
{

}

void News::ReadNews()
{
	FILE* pFile = fopen ("Data/News.txt", "rt"); 
	perror("News.txt");

	if(pFile)
	{
		// Get File Size
		fseek(pFile, 0L, SEEK_END);
		const int fileSize = ftell(pFile) + 1;
		fseek(pFile, 0L, SEEK_SET);

		// New some memory
		mNews = new char[fileSize];

		memset(mNews, '\0', fileSize);

		for(int a = 0; a < fileSize; ++a)
		{
			fscanf(pFile, "%c", &mNews[a]);
		}

		fclose(pFile);
	}
}