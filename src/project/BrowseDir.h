#pragma once
#include "direct.h"
#include "cstring"
#include "io.h"
#include "cstdio" 
#include <vector>
#include <iostream>

class CBrowseDir
{
protected:
	char m_szInitDir[_MAX_PATH];

public:
	CBrowseDir();
	bool SetInitDir(const char *dir);
	bool BeginBrowse(const char *filespec);
	std::vector<char*> BeginBrowseFilenames(const char *filespec);

protected:
	bool BrowseDir(const char *dir,const char *filespec);
	std::vector<char*> GetDirFilenames(const char *dir, const char *filespec);
	virtual bool ProcessFile(const char *filename);
	virtual void ProcessDir(const char *currentdir,const char *parentdir);
};


class CStatDir : public CBrowseDir
{
protected:
	int m_nFileCount;   //保存文件个数
	int m_nSubdirCount; //保存子目录个数

public:
	CStatDir()
	{
		m_nFileCount = m_nSubdirCount = 0;
	}

	int GetFileCount()
	{
		return m_nFileCount;
	}

	int GetSubdirCount()
	{
		return m_nSubdirCount - 1;
	}

protected:
	virtual bool ProcessFile(const char *filename)
	{
		m_nFileCount++;
		return CBrowseDir::ProcessFile(filename);
	}

	virtual void ProcessDir
		(const char *currentdir, const char *parentdir)
	{
		m_nSubdirCount++;
		CBrowseDir::ProcessDir(currentdir, parentdir);
	}
};