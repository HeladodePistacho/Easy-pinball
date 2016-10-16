#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "Engine/SDL/include/SDL.h"
#pragma comment( lib, "Engine/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "Engine/SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_AWAKE,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT,
	FAIL
};

int main(int argc, char ** args)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_AWAKE;
			break;

			// Awake all modules -----------------------------------------------
		case MAIN_AWAKE:
			LOG("AWAKE PHASE ===============================");
			if (App->Awake() == true)
				state = MAIN_START;
			else
			{
				LOG("ERROR: Awake failed");
				state = FAIL;
			}

			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

			// Exit with errors and shame ---------------------------------------
		case FAIL:
			LOG("Exiting with errors :(");
			main_return = EXIT_FAILURE;
			state = MAIN_EXIT;
			break;

		}
	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}