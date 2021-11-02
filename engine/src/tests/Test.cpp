#include "Test.h"
#include <string>

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if(ImGui::Button(test.first.c_str()))
			{
				std::cout << "Started Test: " + test.first + "\n\n";
				// calling the selected test constructor
				m_CurrentTest = test.second();
			}
		}
	}
	
}
