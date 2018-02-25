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
	///������Ƶ�ļ�
	///@para src string Դ�ļ� (��ý���ļ�)
	///@para out string �������Ƶ�ļ���MP3��
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
			cmd += xt.ToString();  //��xtת�����ַ���
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
	///�ϲ�����Ƶ
	///@para v string ��Ƶ�ļ� (avi)
	///@para a string ��Ƶ�ļ���MP3��
	///@para out string ����ļ� ��avi��
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
	///ƴ�Ӷ����Ƶ
	///@para videoRootPath string Դ��Ƶ���Ŀ¼
	///@para v1 char* Ƭͷ��Ƶ�ļ�·��
	///@para spliceVec vector<char *> ���Ƭ���ļ�·�� ����
	///@para outvideo char*  ƴ�Ӻ����Ƶ�ļ����·��
	bool SpliceVideo(string videoRootPath, char* v1, std::vector<char *> spliceVec, string outputPath)
	{
			//---------------1�� �ȴ����м��ļ�list.txt���������ƴ��.mp4�ļ�ת��Ϊ.MTS�ļ�---------------
			string list_txt_Path = videoRootPath + "/list.txt";
			cout << "list_txt_Path = " << list_txt_Path << endl;
			ofstream fout(list_txt_Path);
			if (!fout)
			{
				cout << "�ļ� list.txt ���ܴ�" << endl;
			}
			else
			{
				char* temp1 = strrchr(v1, '\\') + 1;   //ffmpeg concat��������֧��list.txt���о���·����������Ҫ��ȡ�ļ���
				string temp1_str = temp1;
				fout << "file 'clip-0.MTS'" << endl;// �����list.txt�ļ�
				string cmd0 = "ffmpeg -i " + temp1_str + " -q 0 clip-0.MTS"; //ffmpeg -i 0.mp4 - q 0 clip-0.MTS
				cout << cmd0 << endl;
				mutex.lock();
				system(cmd0.data());  //ִ��ffmpeg���� .MTS �м��ļ���
				mutex.unlock();
				for (int i = 0; i < spliceNum - 1; ++i)//2018.1.7
				{
					char* temp2 = strrchr(spliceVec[i], '\\') + 1; //ffmpeg concat��������֧��list.txt���о���·����������Ҫ��ȡ�ļ���
					string temp2_str = temp2;
					char outMtsFile[20];
					sprintf(outMtsFile, "clip-%d.MTS", i + 1);
					string outMtsFile_tmp = outMtsFile;
					fout << "file '" << outMtsFile_tmp << "'" << endl;// �����list.txt�ļ�
					string cmd1 = "ffmpeg -i " + temp2_str + " -q 0 " + outMtsFile_tmp;
					cout << cmd1 << endl;
					mutex.lock();
					system(cmd1.data());  //ִ��ffmpeg���� .MTS �м��ļ���
					mutex.unlock();
				}
			}
			fout.close();//�ر��ļ������				

			/*---------2. ��ʼƴ������.MTS�м��ļ�Ϊһ��.MTS�ļ���Ȼ�󽫸�ƴ�Ӻ��.MTS�ļ�ת��Ϊ.mp4��ʽ-----------
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
				system(cmd.data());  //ִ��ffmpeg��ɸ���MTS��Ƶ�ļ�ƴ�ӣ�
				mutex.unlock();
				cmd = "ffmpeg -i output.MTS -c copy " + outputPath;
				cout << cmd << endl;
				mutex.lock();
				system(cmd.data());  //ִ��ffmpeg��MTS��Ƶ�ļ�����ת��Ϊmp4�ļ���Ҳ��������ֶ��á���ʽ������ת������ѹ����С��
				mutex.unlock();

			//--------------- 3�� ɾ���м��ļ�*.MTS��list.txt ---------------
			cmd = "del *.MTS";
			cout << cmd << endl;
			mutex.lock();
			system(cmd.c_str());
			mutex.unlock();
			cout << "����ƴ���õ����м��ļ� .MTS ����ɾ����" << endl;

			cmd = "del list.txt";
			cout << cmd << endl;
			mutex.lock();
			system(cmd.c_str());
			mutex.unlock();
			cout << "����ƴ���õ����м��ļ�list.txt��ɾ����" << endl;
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
