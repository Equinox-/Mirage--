#ifndef _WEATHER_H
#define _WEATHER_H

#include <SGE.h>
using namespace SGE;

enum WeatherType
{
	noWeather = -1
	,rain
	,heavyRain
	,snow
	,heavySnow
	,fire
	,smoke
	,purple
	,ugly
};

class Weather
{
public:
	Weather();
	~Weather();

	void Init();
	void Update(float deltaTime);
	void Render(SVector2 offset);

	void SetWeather(WeatherType weather)					{ mCurrentWeather = weather; }

	WeatherType GetWeather()								{ return mCurrentWeather; }

private:
	SGE_Emitter mEmitter;
	WeatherType mCurrentWeather;

};

#endif