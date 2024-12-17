// Licensed for use with Unreal Engine products only

using UnrealBuildTool;
using System.Collections.Generic;

public class UndeadDomainEditorTarget : TargetRules
{
	public UndeadDomainEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "UndeadDomain" } );
	}
}
