#include "../../project2_test.h"
#include "../stubs/mgframeworkstub.h"

void Project2Test::test_MGSC_startServer()
{
	// Setup
	MGFrameworkStub server;
	server.setInstanceType(MGFSERVERINSTANCE);
	server.setPort(666);
//	MGFrameworkStub client;
//	client.setInstanceType(MGFCLIENTINSTANCE);
//	server.setPort(667);

	ASSERT_EQ(server.socketTerminalOpen(), false, "Server already running");
	server.runConsoleCommand("open terminalserver", &server, NULL);
	ASSERT_EQ(server.socketTerminalOpen(), true, "Failed to start server");
}