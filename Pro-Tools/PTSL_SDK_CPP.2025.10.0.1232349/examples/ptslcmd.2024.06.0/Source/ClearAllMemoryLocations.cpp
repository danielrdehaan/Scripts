// Copyright 2024 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief ptslcmd.cpp wrapper for the PTSLC_CPP::CppPTSLClient::ClearAllMemoryLocations command.
 */

#include "Common.h"

const string g_pszClearAllMemoryLocations = "ClearAllMemoryLocations";
const string g_pszClearAllMemoryLocationsHelp = g_pszClearAllMemoryLocations;

PtslCmdCommandResult ClearAllMemoryLocations(const vector<string>& params, CppPTSLClient& client)
{
	// Create a request:
	CommandRequest request;
	request.commandType = CommandType::ClearAllMemoryLocations;

	// Call the client's method with the created request:
	std::shared_ptr<ClearAllMemoryLocationsResponse> rsp = client.ClearAllMemoryLocations(request);

	// Output the response:
	if (!rsp)
	{
		cout << "No response received." << endl;
		return false;
	}

	if (rsp->status.type == PTSLC_CPP::CommandStatusType::Completed)
	{
		cout << "ClearAllMemoryLocations Response:" << endl;

		cout << "\t" << "Count of successfully cleared memory locations: " << "\t" << rsp->successCount << endl;
		
		cout << "\t" << "Count of failures during clearing memory locations: " << "\t" << rsp->failureCount << endl;
		
		if (rsp->failureList.size())
		{
			cout << "\t" << "Failure list:" << endl;
			for (auto &item : rsp->failureList)
			{
				cout << "\t\t" << item << endl;
			}
		}
	}
	else if (rsp->status.type == PTSLC_CPP::CommandStatusType::Failed)
	{
		cout << "ClearAllMemoryLocations Request Failed: " << endl;
		for (const auto& error : rsp->errors)
		{
			cout << "\t" << error->errorMessage << endl;
		}
	}
	else
	{
		cout << "ClearAllMemoryLocations returned an unexpected status" << endl;
	}

	return PtslCmdCommandResult(rsp);
}
