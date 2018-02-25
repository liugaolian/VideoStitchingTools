#include "XAudio.h"
#include <iostream>
#include <QMutex>
#include <fstream> 
#include <vector>
using namespace std;
struct XTime
{
	XTime(int tms)
	{   
		h = (tms / 1000) / 3600;    
		m = ((tms / 1000) % 3600) / 60;  
		s = ((tms / 1000) % 3600) % 60;
		ms = tms % 1000; 
	}
	std::string ToString()
	{
		char buf[16] = { 0 };
		sprintf(buf, "%d:%d:%d.%d", h, m, s, ms);
		return buf;
	}
	int h = 0;
	int m = 0;
	int s = 0;
	int ms = 0;
};

class CXAudio :public XAudio
{
public:
	
	QMutex mutex;
	/////////////////////////////////////////////////////////
	///导出音频文件
	///@para src string 源文件 (多媒体文件)
	///@para out string 输出的音频文件（MP3）
	bool ExportA(std::string src, std::string out, int beginMs, int outMs)
	{
		//ffmpeg -i test.mp4  -ss 0:1:10.112 -t 0:1:10.100 -vn -y test.mp3
		string cmd = "ffmpeg ";
		cmd += " -i ";
		cmd += src;
		cmd += " ";
		if (beginMs > 0)
		{
			cmd += " -ss ";
			XTime xt(beginMs);
			cmd += xt.ToString();
		}
		if (outMs > 0)
		{
			cmd += " -t ";
			XTime xt(outMs);
			cmd += xt.ToString();  //将xt转换成字符串
		}

		cmd += " -vn -y ";
		//out.erase(out.size() - 4); //Gordon
		cmd += out;
		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}

	/////////////////////////////////////////////////////////
	///合并音视频
	///@para v string 视频文件 (avi)
	///@para a string 音频文件（MP3）
	///@para out string 输出文件 （avi）
	bool Merge(std::string v, std::string a, std::string out)
	{
		//ffmpeg -i test.avi -i test.mp3 -c copy out.avi
		string cmd = "ffmpeg -i ";
		cmd += v;
		cmd += " -i ";
		cmd += a;
		cmd += " -c copy ";
		cmd += out;
		cout << cmd << endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}

	/////////////////////////////////////////////////////////
	///拼接多个视频
	///@para videoRootPath string 源视频存放目录
	///@para v1 char* 片头视频文件路径
	///@para spliceVec vector<char *> 后接片段文件路径 集合
	///@para outvideo char*  拼接后的视频文件输出路径
	bool SpliceVideo(string videoRootPath, char* v1, std::vector<char *> spliceVec, string outputPath)
	{
			//---------------1、 先创建中间文件list.txt，将多个待拼接.mp4文件转码为.MTS文件---------------
			string list_txt_Path = videoRootPath + "/list.txt";
			cout << "list_txt_Path = " << list_txt_Path << endl;
			ofstream fout(list_txt_Path);
			if (!fout)
			{
				cout << "文件 list.txt 不能打开" << endl;
			}
			else
			{
				char* temp1 = strrchr(v1, '\\') + 1;   //ffmpeg concat分离器不支持list.txt中有绝对路径，所以需要获取文件名
				string temp1_str = temp1;
				fout << "file 'clip-0.MTS'" << endl;// 输出到list.txt文件
				string cmd0 = "ffmpeg -i " + temp1_str + " -q 0 clip-0.MTS"; //ffmpeg -i 0.mp4 - q 0 clip-0.MTS
				cout << cmd0 << endl;
				mutex.lock();
				system(cmd0.data());  //执行ffmpeg生成 .MTS 中间文件！
				mutex.unlock();
				for (int i = 0; i < spliceNum - 1; ++i)//2018.1.7
				{
					char* temp2 = strrchr(spliceVec[i], '\\') + 1; //ffmpeg concat分离器不支持list.txt中有绝对路径，所以需要获取文件名
					string temp2_str = temp2;
					char outMtsFile[20];
					sprintf(outMtsFile, "clip-%d.MTS", i + 1);
					string outMtsFile_tmp = outMtsFile;
					fout << "file '" << outMtsFile_tmp << "'" << endl;// 输出到list.txt文件
					string cmd1 = "ffmpeg -i " + temp2_str + " -q 0 " + outMtsFile_tmp;
					cout << cmd1 << endl;
					mutex.lock();
					system(cmd1.data());  //执行ffmpeg生成 .MTS 中间文件！
					mutex.unlock();
				}
			}
			fout.close();//关闭文件输出流				

			/*---------2. 开始拼接所有.MTS中间文件为一个.MTS文件，然后将该拼接后的.MTS文件转码为.mp4格式-----------
																ffmpeg -i 0.mp4 -q 0 0.MTS
																ffmpeg -i 1.mp4 -q 0 1.MTS
																ffmpeg -i 2.mp4 -q 0 2.MTS
																ffmpeg -i 3.mp4 -q 0 3.MTS
																ffmpeg -f concat -i list.txt -c copy output.MTS
																ffmpeg -i output.MTS -c copy xxx/xxx/xxx.mp4
			---------------------------------------------------------------*/
				string cmd = "ffmpeg -f concat -i " + list_txt_Path + " -c copy output.MTS";
				cout << cmd << endl;
				mutex.lock();
				system(cmd.data());  //执行ffmpeg完成各个MTS视频文件拼接！
				mutex.unlock();
				cmd = "ffmpeg -i output.MTS -c copy " + outputPath;
				cout << cmd << endl;
				mutex.lock();
				system(cmd.data());  //执行ffmpeg将MTS视频文件无损转换为mp4文件（也可以最后手动用“格式工厂”转换，能压缩大小）
				mutex.unlock();

			//--------------- 3、 删除中间文件*.MTS和list.txt ---------------
			cmd = "del *.MTS";
			cout << cmd << endl;
			mutex.lock();
			system(cmd.c_str());
			mutex.unlock();
			cout << "本次拼接用到的中间文件 .MTS 都已删除！" << endl;

			cmd = "del list.txt";
			cout << cmd << endl;
			mutex.lock();
			system(cmd.c_str());
			mutex.unlock();
			cout << "本次拼接用到的中间文件list.txt已删除！" << endl;
			return true;
	}
};

XAudio * XAudio::Get()
{
	static CXAudio ca;
	return &ca;
}
XAudio::XAudio()
{
}


XAudio::~XAudio()
{
}
