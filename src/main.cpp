#include"../Header/Framework.h"

#include"../Header/Constants.h"




int main()
{
	printf("Program Entry Point.\n");

	Framework fw = Framework();

	while (fw._bRunning)
	{
		fw.RunApp();
	}
	

	printf("Program Exit.\n");
	
	return 0;
}