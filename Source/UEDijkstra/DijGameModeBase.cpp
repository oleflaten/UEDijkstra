// OEF 2026

#include "DijGameModeBase.h"
#include "DijPlayer.h"

ADijGameModeBase::ADijGameModeBase()
{
    DefaultPawnClass = ADijPlayer::StaticClass();
}
