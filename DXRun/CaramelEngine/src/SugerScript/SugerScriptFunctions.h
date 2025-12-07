#pragma once
#include "pch.h"

using namespace CaramelEngine;

namespace SugerScript
{
	/// <summary>
	/// Suger(©ìŒ¾Œê)‚ÅŒÄ‚Ño‚µ‚³‚ê‚éŠÖ”‚Ì’è‹`‚ğ‚·‚é–¼‘O‹óŠÔ
	/// </summary>
	namespace Functions
	{
		std::shared_ptr<CInt> CreateIntegerField(CInt init = 0, CString name = "")
		{
			return std::make_shared<CInt>(init);
		}

		std::shared_ptr<CFloat> CreateFloatField(CFloat init = 0, CString name = "")
		{
			return std::make_shared<CFloat>(init);
		}

		std::shared_ptr<double> CreateDoubleField(float init = 0, CString name = "")
		{
			return std::make_shared<double>(init);
		}

		std::shared_ptr<bool> CreateBooleanField(float init = 0, CString name = "")
		{
			return std::make_shared<bool>(init);
		}

		int CalcSum(int a, int b)
		{
			return a + b;
		}
	}
}