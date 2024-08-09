#pragma once

#include "Framework.h"

namespace Network
{
	void (*TickFlush)(UNetDriver*, float);
	void TickFlushHook(UNetDriver* NetDriver, float a2)
	{
		if (NetDriver->ClientConnections.Num() > 0)
		{
			if (NetDriver->ReplicationDriver)
				Server::ServerReplicateActors(NetDriver->ReplicationDriver);
		}
		return TickFlush(NetDriver, a2);
	}

	char ValidationFailure(__int64, __int64)
	{
		return 0;
	}

	__int64 __fastcall NoReservation(__int64 a1, __int64 a2, __int64 a3, char a4)
	{
		return 0;
	}
}