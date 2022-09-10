#pragma once
class Window
{
public:
	Window(int width, int height, const char* title);
	~Window() {};
	bool ShouldClose();
	void SwapBuffers();
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
private:
	void WindowResizeCallback(int width, int height);
	struct GLFWwindow* m_Window;
	int m_Width, m_Height;

};

