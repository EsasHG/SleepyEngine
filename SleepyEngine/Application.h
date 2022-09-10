#pragma once
class Application
{
public:
	Application() {};
	int Run();

private:

	unsigned int shaderId;
	double prevFrameTime;
};

