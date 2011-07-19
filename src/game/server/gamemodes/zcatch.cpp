/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "zcatch.h"
#include <game/server/entities/character.h>
#include <game/server/player.h>


CGameControllerZCATCH::CGameControllerZCATCH(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "zCatch";
}

void CGameControllerZCATCH::Tick()
{
	IGameController::Tick();
}

int CGameControllerZCATCH::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	char buf[256];
	int m_ClientID = pVictim->GetPlayer()->GetCID();
	int i;

	if(pKiller != pVictim->GetPlayer())
	{
		pKiller->m_Score++;
		GameServer()->m_apPlayers[m_ClientID]->m_CaughtBy = pKiller->GetCID();
		GameServer()->m_apPlayers[m_ClientID]->SetTeamDirect(-1);
		
		GameServer()->m_apPlayers[m_ClientID]->m_SpectatorID = pKiller->GetCID();
	
		str_format(buf, 255, "Caught by \"%s\". You will join the game automatically when \"%s\" dies.", Server()->ClientName(pKiller->GetCID()), Server()->ClientName(pKiller->GetCID()));
		GameServer()->SendChatTarget(m_ClientID, buf);
	}
	
	for(i=0;i<MAX_CLIENTS;i++)
	{
		if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->m_CaughtBy == m_ClientID)
		{
			GameServer()->m_apPlayers[i]->m_CaughtBy = -1;
			//GameServer()->m_apPlayers[i]->SetTeamDirect(GameServer()->m_pController->ClampTeam(1));
			GameServer()->m_apPlayers[i]->SetTeamDirect(1);
			GameServer()->m_pController->OnPlayerInfoChange(GameServer()->m_apPlayers[i]);
			
			if(pKiller != pVictim->GetPlayer())
				pKiller->m_Score++;
		}
	}
	
	return 0;
}