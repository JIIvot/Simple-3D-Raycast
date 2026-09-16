#include "MathUtil.h"

#include <numbers>

namespace MathUtil
{
float ToRadians( float angleDeg )
{
	return angleDeg / 180.0f * std::numbers::pi_v<float>;
}
} // namespace MathUtil
