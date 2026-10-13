// OEF 2026

using UnrealBuildTool;
using System.Collections.Generic;

public class UEDijkstraEditorTarget : TargetRules
{
	public UEDijkstraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;

        ExtraModuleNames.AddRange( new string[] { "UEDijkstra" } );
	}
}
