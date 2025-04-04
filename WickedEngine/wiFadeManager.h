#pragma once
#include "CommonInclude.h"
#include "wiColor.h"
#include "wiTimer.h"
#include "wiMath.h"
#include "wiGraphics.h"

#include <functional>

namespace wi
{
	class FadeManager
	{
	public:
		float opacity = 0;
		float timer = 0;
		float targetFadeTimeInSeconds = 1.0f;
		enum class FadeType
		{
			FadeToColor,
			CrossFade
		} type = FadeType::FadeToColor;
		enum FADE_STATE
		{
			FADE_IN,	// no fade -> faded
			FADE_MID,	// completely faded
			FADE_OUT,	// faded -> no fade
			FADE_FINISHED,
		} state = FADE_FINISHED;
		wi::Color color = wi::Color(0, 0, 0, 255);
		std::function<void()> onFade = [] {};
		bool fadeEventTriggeredThisFrame = false;
		bool crossFadeTextureSaveRequired = false;
		wi::graphics::Texture crossFadeTexture;

		FadeManager()
		{
			Clear();
		}
		void Clear();
		void Start(float seconds, wi::Color color, std::function<void()> onFadeFunction, FadeType fadetype = FadeType::FadeToColor)
		{
			type = fadetype;
			targetFadeTimeInSeconds = seconds;
			this->color = color;
			timer = 0;
			if (IsFaded())
			{
				// If starting a new fade on mid-fadeout, it will start from faded and just transition out of mid
				state = FADE_MID;
			}
			else
			{
				state = FADE_IN;
			}
			onFade = onFadeFunction;
			if (type == FadeType::CrossFade)
			{
				// starts from full faded state and only blends out from it
				state = FADE_MID;
				crossFadeTextureSaveRequired = true;
				fadeEventTriggeredThisFrame = true;
				opacity = 1;
			}
		}
		void Update(float dt);
		bool IsFaded() const
		{
			return fadeEventTriggeredThisFrame;
		}
		bool IsActive() const
		{
			return state != FADE_FINISHED;
		}
	};
}
