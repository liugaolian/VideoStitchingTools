#pragma once
#include <string>
#include <vector>
class XAudio
{
public:
	static XAudio* Get();
	/////////////////////////////////////////////////////////
	///导出音频文件
	///@para src string 源文件 (多媒体文件)
	///@para out string 输出的音频文件（MP3）
	///@para beginMs int 开始音频位置毫秒
	///@para outMs int 音频时长
	virtual bool ExportA(std::string src, std::string out,int beginMs=0,int outMs=0) = 0;

	/////////////////////////////////////////////////////////
	///合并音视频
	///@para v string 视频文件 (avi)
	///@para a string 音频文件（MP3）
	///@para out string 输出文件 （avi）
	virtual bool Merge(std::string v, std::string a, std::string out) = 0;
	virtual bool SpliceVideo(std::string videoRootPath, char* v1, std::vector<char *> spliceVec, std::string outputPath) = 0;

	XAudio();
	virtual ~XAudio();

	//拼接个数变量，默认为5个
	int spliceNum = 5;//2018.1.7
};

