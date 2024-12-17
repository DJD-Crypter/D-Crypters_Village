// Licensed for use with Unreal Engine products only

using UnrealBuildTool;
using System.Collections.Generic;

public class UndeadDomainTarget : TargetRules
{
	public UndeadDomainTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "UndeadDomain" } );
	}
}
