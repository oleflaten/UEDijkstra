// OEF 2026

using UnrealBuildTool;
using System.Collections.Generic;

public class UEDijkstraTarget : TargetRules
{
	public UEDijkstraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;

		ExtraModuleNames.AddRange( new string[] { "UEDijkstra" } );
	}
}
