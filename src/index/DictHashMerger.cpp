//
// Copyright(C) 2005--2006 Institute of Computing Tech, Chinese Academy of Sciences. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: �����(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2005/12/3
//
#include "index/DictHashMerger.h"
#include "store/IndexOutput.h"

namespace firtex
{
	namespace index
	{
		CDictHashMerger::CDictHashMerger(CFieldFactory* pFactory)
			:CFieldMerger(pFactory)
			,m_termCount(0)
			,m_lastTerm(0)
			,m_lastPOffset(0)
			,m_beginOfVoc(0)
		{

		}
		CDictHashMerger::~CDictHashMerger()
		{

		}	
		void CDictHashMerger::flushTermInfo(COutputDescriptor* pOutputDescriptor,CMergeTermInfo** ppTermInfo,int32_t numTermInfos)
		{
			CIndexOutput* pVocOutput = pOutputDescriptor->getVocOutput();
			
			if(m_termCount == 0)
				m_beginOfVoc = pVocOutput->getFilePointer();
			CTermX<termid_t>* pTermX = NULL;
			termid_t tid;
			fileoffset_t poffset;
			for (int32_t i = 0;i<numTermInfos;i++)
			{
				pTermX = (CTermX<termid_t>*)ppTermInfo[i]->getTerm()->asTermX();
				tid = pTermX->getValue();
				pVocOutput->writeVInt(tid - m_lastTerm);				
				pVocOutput->writeVInt(ppTermInfo[i]->getTermInfo()->docFreq());
				poffset = ppTermInfo[i]->getTermInfo()->docPointer();
				pVocOutput->writeVLong(poffset - m_lastPOffset);
				m_lastTerm = tid;
				m_lastPOffset = poffset;

				m_termCount++;
			}
			
		}		
		fileoffset_t CDictHashMerger::endMerge(COutputDescriptor* pOutputDescriptor)
		{
			CIndexOutput* pVocOutput = pOutputDescriptor->getVocOutput();
			fileoffset_t voffset = pVocOutput->getFilePointer();
			///begin write vocabulary descriptor
			pVocOutput->writeVLong(voffset - m_beginOfVoc);
			pVocOutput->writeVLong(m_termCount);
			///end write vocabulary descriptor
			return voffset;
		}
	}
}

