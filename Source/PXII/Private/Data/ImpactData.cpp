#include "Data/ImpactData.h"

const FImpactEffectData& UImpactData::GetEffectForSurface(EPhysicalSurface SurfaceType) const
{
	{
		if (const FImpactEffectData* Found = SurfaceEffects.Find(SurfaceType))
		{
			return *Found;
		}
		return DefaultEffect;
	}
}
