#pragma once

#include <cmath>

//ref from http://qiita.com/hart_edsf/items/962ac03281b871dcc0df
namespace Easing
{
	const float Pi = 3.141592654f;

	inline float InQuad(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		return max*t*t + min;
	}

	inline float OutQuad(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		return -max*t*(t - 2) + min;
	}

	inline float InOutQuad(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t * t + min;
		--t;
		return -max * (t * (t - 2) - 1) + min;
	}

	inline float InCubic(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		return max * t*t*t + min;
	}

	inline float OutCubic(float t, float totaltime, float min, float max) {
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*t + 1) + min;
	}

	inline float InOutCubic(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t + min;
		t -= 2;
		return max / 2 * (t*t*t + 2) + min;
	}

	inline float InQuart(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		return max * t*t*t*t + min;
	}

	inline float OutQuart(float t, float totaltime, float min, float max) {
		max -= min;
		t = t / totaltime - 1;
		return -max*(t*t*t*t - 1) + min;
	}

	inline float InOutQuart(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t*t + min;
		t -= 2;
		return -max / 2 * (t*t*t*t - 2) + min;
	}

	inline float InQuint(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		return max*t*t*t*t*t + min;
	}

	inline float OutQuint(float t, float totaltime, float min, float max) {
		max -= min;
		t = t / totaltime - 1;
		return max*(t*t*t*t*t + 1) + min;
	}

	inline float InOutQuint(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t*t*t + min;
		t -= 2;
		return max / 2 * (t*t*t*t*t + 2) + min;
	}

	inline float InSine(float t, float totaltime, float min, float max) {
		max -= min;
		return -max*cosf(t*(Pi / 2) / totaltime) + max + min;
	}

	inline float OutSine(float t, float totaltime, float min, float max) {
		max -= min;
		return max * sinf(t*(Pi / 2) / totaltime) + min;
	}

	inline float InOutSine(float t, float totaltime, float min, float max) {
		max -= min;
		return -max / 2 * (cosf(t*Pi / totaltime) - 1) + min;
	}

	inline float InExp(float t, float totaltime, float min, float max) {
		max -= min;
		return t == 0.0f ? min : max*powf(2, 10 * (t / totaltime - 1)) + min;
	}

	inline float OutExp(float t, float totaltime, float min, float max) {
		max -= min;
		return t == totaltime ? max + min : max*(-powf(2, -10 * t / totaltime) + 1) + min;
	}

	inline float InOutExp(float t, float totaltime, float min, float max) {
		if (t == 0.0f)
			return min;
		if (t == totaltime)
			return max;
		max -= min;
		t /= totaltime;

		if (t / 2 < 1)
			return max / 2 * powf(2, 10 * (t - 1)) + min;
		--t;
		return max / 2 * (-powf(2, -10 * t) + 2) + min;

	}

	inline float InCirc(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		return -max*(sqrtf(1 - t*t) - 1) + min;
	}

	inline float OutCirc(float t, float totaltime, float min, float max) {
		max -= min;
		t = t / totaltime - 1;
		return max*sqrtf(1 - t*t) + min;
	}

	inline float InOutCirc(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return -max / 2 * (sqrtf(1 - t*t) - 1) + min;
		t -= 2;
		return max / 2 * (sqrtf(1 - t*t) + 1) + min;
	}

	inline float InBack(float t, float totaltime, float s, float min, float max) {
		max -= min;
		t /= totaltime;
		return max*t*t*((s + 1)*t - s) + min;
	}

	inline float OutBack(float t, float totaltime, float s, float min, float max) {
		max -= min;
		t = t / totaltime - 1;
		return max*(t*t*((s + 1)*t*s) + 1) + min;
	}

	inline float InOutBack(float t, float totaltime, float s, float min, float max) {
		max -= min;
		s *= 1.525f;
		if (t / 2 < 1) {
			return max*(t*t*((s + 1)*t - s)) + min;
		}
		t -= 2;
		return max / 2 * (t*t*((s + 1)*t + s) + 2) + min;
	}

	inline float OutBounce(float t, float totaltime, float min, float max) {
		max -= min;
		t /= totaltime;

		if (t < 1 / 2.75f)
			return max*(7.5625f*t*t) + min;
		else if (t < 2 / 2.75f) {
			t -= 1.5f / 2.75f;
			return max*(7.5625f*t*t + 0.75f) + min;
		} else if (t < 2.5f / 2.75f) {
			t -= 2.25f / 2.75f;
			return max*(7.5625f*t*t + 0.9375f) + min;
		} else {
			t -= 2.625f / 2.75f;
			return max*(7.5625f*t*t + 0.984375f) + min;
		}
	}

	inline float InBounce(float t, float totaltime, float min, float max) {
		return max - OutBounce(totaltime - t, totaltime, max - min, 0) + min;
	}

	inline float InOutBounce(float t, float totaltime, float min, float max) {
		if (t < totaltime / 2)
			return InBounce(t * 2, totaltime, max - min, max)*0.5f + min;
		else
			return OutBounce(t * 2 - totaltime, totaltime, max - min, 0)*0.5f + min + (max - min)*0.5f;
	}

	inline float Linear(float t, float totaltime, float min, float max) {
		return (max - min)*t / totaltime + min;
	}
}