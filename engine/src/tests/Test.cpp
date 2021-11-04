#include "Test.h"
#include <string>

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{

	}

	// rendering the test selection ui
	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if(ImGui::Button(test.first.c_str()))
			{
				std::cout << "Started Test: " + test.first + "\n\n";
				// clicking on the test name button calls its constructor
				m_CurrentTest = test.second();
			}
		}
	}
	
}
