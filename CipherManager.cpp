#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<fstream>
#include<ctime>
#include<windows.h>

using namespace std;

/*
    需要实现的功能：
        ls:按照字典序输出当前所有账号密码备注，修改时间
        set:  -t terrace
              -a account
              -p password
              -n notes
        qr:   对应set，输出所有符合条件的
        cp:   -a copy last account
              -p copy last password
        del:  -t terrace
        exit: 退出
    实现逻辑：
        首先从文件读取数据，数据不以明文储存，故需要考虑加解密等，
        其次要正确处理用户输入,执行对应操作
        而后要将操作后的版本正确回存
*/

int veg[10]={33,20,45,100,7,2,45,88,11,96};

struct INFO
{
    string terrace="*";
    string account="*";
    string password="*";
    string notes="*";
};
vector<INFO> infomation;
int infoNum;
INFO lastQuery;

bool cmp(INFO a,INFO b)
{
    return a.terrace<b.terrace;
}

//LMS代表长度 SW代表强弱
string getRandCipher(string needs)
{
    srand(time(NULL));
    string ret;int length;
    if(needs[1]=='l') length=16;
    else if(needs[1]=='m') length=10;
    else if(needs[1]=='s') length=6;
    if(needs[2]=='w')
        for(int i=0;i<length;i++)
            ret.push_back(rand()%26+'a');
    else if(needs[2]=='s')
        for(int i=0;i<length;i++)
            ret.push_back(rand()%95+' ');
    return ret;
}

void inputInfo()
{
    ifstream infile;
    ofstream outfile;
    infile.open("cipher.txt",ios::in);
    outfile.open("tem.txt",ios::out);
    char ch;ch=infile.get();int cnt=0;
    while(ch!=EOF)
    {
        if(ch=='\n')    
        {
            outfile<<ch;
            ch=infile.get();
        }
        else
        {
            ch=(char)((ch-' '-veg[cnt++]+95)%95+' ');
            cnt%=10;
            outfile<<ch;
            ch=infile.get();
        }
    }
    infile.close();outfile.close();
    infile.open("tem.txt",ios::in);
    infile>>infoNum;
    INFO input;
    for(int i=0;i<infoNum;i++)
    {
        infile>>input.terrace;
        infile>>input.account;
        infile>>input.password;
        infile>>input.notes;
        infomation.push_back(input);
    }
    infile.close();
    remove("tem.txt");
}

void outputInfo()
{
    ifstream infile;
    ofstream outfile;
    outfile.open("tem.txt",ios::out);
    outfile<<infoNum<<endl;
    for(int i=0,sz=infomation.size();i<sz;i++)
    {
        outfile<<infomation[i].terrace<<' ';
        outfile<<infomation[i].account<<' ';
        outfile<<infomation[i].password<<' ';
        outfile<<infomation[i].notes<<endl;
    }
    outfile.close();
    infile.open("tem.txt",ios::in);
    outfile.open("cipher.txt",ios::out);
    char ch;ch=infile.get();int cnt=0;
    while(ch!=EOF)
    {
        if(ch=='\n')    
        {
            outfile<<ch;
            ch=infile.get();
        }
        else
        {
            ch=(char)((ch-' '+veg[cnt++]+95)%95+' ');
            cnt%=10;
            outfile<<ch;
            ch=infile.get();
        }
    }
    infile.close();outfile.close();
    remove("tem.txt");
    printf("SaveSuccess!\n");
}

void listInfo()
{
    sort(infomation.begin(),infomation.end(),cmp);
    printf("%10s%25s%25s%25s\n","TERRACE","ACCOUNT","PASSWORD","NOTES");
    for(int i=0,sz=infomation.size();i<sz;i++)
    {
        printf("%10s",infomation[i].terrace.c_str());
        printf("%25s",infomation[i].account.c_str());
        printf("%25s",infomation[i].password.c_str());
        printf("%25s\n",infomation[i].notes.c_str());
    }
    printf("ListInfoSuccess!\n");
}

void setInfo()
{
    INFO info;
    string opt,tem;
    while(cin>>opt>>tem)
    {
        if(opt=="-t") info.terrace=tem;
        else if(opt=="-a") info.account=tem;
        else if(opt=="-p") 
        {
            if(tem[0]=='*')
                info.password=getRandCipher(tem);
            else 
                info.password=tem;
        }
        else if(opt=="-n") info.notes=tem;
        if(cin.get()=='\n') break;
    }
    bool isFind=false;
    for(int i=0,sz=infomation.size();i<sz;i++)
        if(infomation[i].terrace==info.terrace)
        {  
            isFind=true;
            if(info.account!="*")
                infomation[i].account=info.account;
            if(info.password!="*")
                infomation[i].password=info.password;
            if(info.notes!="*")
                infomation[i].notes=info.notes;
            if(lastQuery.terrace==info.terrace)
                lastQuery=infomation[i];
            continue;
        }
    if(!isFind)
        infomation.push_back(info),infoNum++;
    printf("SetInfoSuccess!\n");
} 

void queryInfo()
{
    string opt,content;cin>>opt>>content;
    bool isFind=true;
    for(int i=0,sz=infomation.size();i<sz;i++)
    {
        string tem;
        if(opt=="-t") tem=infomation[i].terrace;
        else if(opt=="-a") tem=infomation[i].account;
        else if(opt=="-p") tem=infomation[i].password;
        else if(opt=="-n") tem=infomation[i].notes;
        if(tem==content)
        {
            isFind=true;
            lastQuery=infomation[i];
            printf("%10s",infomation[i].terrace.c_str());
            printf("%25s",infomation[i].account.c_str());
            printf("%25s",infomation[i].password.c_str());
            printf("%25s\n",infomation[i].notes.c_str());
        }
    }
    if(!isFind)
        printf("NotFind!\n");
}

void copyInfo()
{
    string opt;cin>>opt;
    HWND hWnd = NULL;
	OpenClipboard( hWnd );
	EmptyClipboard();
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 1000);
	char* pData = (char*) GlobalLock(hHandle);
    if(opt=="-a") 
        strcpy(pData, lastQuery.account.c_str());
    else if(opt=="-p")
        strcpy(pData, lastQuery.password.c_str());
    SetClipboardData(CF_TEXT, hHandle);
	GlobalUnlock(hHandle);
	CloseClipboard();
    printf("CopySuccess!\n");
}

void delInfo()
{
    string opt,content;cin>>opt>>content;
    bool isFind=true;
    for(int i=0,sz=infomation.size();i<sz;i++)
    {
        if(content==infomation[i].terrace)
        {
            isFind=true;
            infomation.erase(infomation.begin()+i);
            printf("DelSuccess!\n");
            break;
        }
    }
    if(!isFind)
        printf("NotFind!\n"); 
}

void help()
{
    printf("-t terrace\n-a account\n-p password\n-n notes\n");
    printf("ls    List all infomation.\n");
    printf("set   Change or insert infomation.\n");
    printf("qr    Query and print infomation.\n");
    printf("cp    Copy last query's infomation.\n");
    printf("del   Delete infomation.\n");
    printf("save  Save infomation.\n");
    printf("cls   Clean screen.\n");
    printf("exit  Exit the program.\n");
}

int main()
{
    inputInfo();
    while(1)
    {
        string opt;cin>>opt;
        if(opt=="ls")
            listInfo();
        else if(opt=="set")
            setInfo();
        else if(opt=="qr")
            queryInfo();
        else if(opt=="cp")
            copyInfo();
        else if(opt=="del")
            delInfo();
        else if(opt=="exit")
        {
            outputInfo();
            return 0;
        }
        else if(opt=="save")
            outputInfo();
        else if(opt=="cls")
            system("cls");
        else if(opt=="help")
            help();
    }
    outputInfo();
    return 0;
}

