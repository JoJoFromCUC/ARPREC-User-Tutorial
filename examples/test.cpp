#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <vector>
#include <mpi.h>
#include <arprec/mp_real.h>
#define OUT_PRECISION 30 //输出精度
#define WORDS 200 //总精度
using namespace std;

//！mp_real类型数据大小为16字节
char ** str2char(mp_real num[],int size){
    char** charstring = new char*[size*WORDS];
    string tmp;
    for(int i=0;i<size;i++){
        tmp = num[i].to_string();
        const char* m = tmp.c_str(); 
        char* change = const_cast<char*> (m);
        strcpy(charstring[i],change);
        //*charstring = change;
    }
    return charstring;
}
int main(int argc,char *argv[]){
    mp::mp_init(WORDS);
    cout.precision(OUT_PRECISION);
    //cout.setf(ios::fixed);
    static mp_real a = mp_real("1.34567867896746242634645636");//从字符串初始化
    static mp_real f = 6.315484;//直接定义
    string str_a = a.to_string();//转换为字符串
    const char *p = str_a.c_str();//不加const报错
    char* k =const_cast<char*> (p); //去掉const,不然mpi无法广播数据
    mp_real _pi;
    mp_real::mppi(_pi);
    //cout<<"a="<<a<<endl;
    // cout<<"混合数据类型运算："<<endl;
    // static mp_real b = 3.0,c,d,e;
    // c = a+b;
    // d = a*b;
    // e = a/b;
    // cout<<"c= "<<c<<endl;
    // cout<<"d= "<<d<<endl;
    // cout<<"e= "<<e<<endl;
    // cout<<"pi= "<<_pi<<endl;
    clock_t t1,t2;
    //cout<<"pow(pi)= "<<pow(_pi,2)<<endl;
    //cout<<"sqrt(pi)= "<<sqrt(_pi)<<endl;
    //cout<<"sin(pi)= "<<sin(M_PI)<<endl;
    //cout<<endl;
    //cout<<"MPI通信:"<<endl;
    int com_size = 0,myrank = 0;//MPI初始化
    MPI_Status status;
    MPI::Init(argc,argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&com_size);
    t1 = clock();
    //点到点通信
    // if(myrank%2 == 0){
    //     cout<<"process: "<<myrank<<endl;
    //    MPI_Send(p,strlen(p),MPI_CHAR,myrank+1,90,MPI_COMM_WORLD);
    //     cout<<"发送完毕."<<endl;
    // }
    // else{
    //     cout<<"process: "<<myrank<<endl;
    //     char t[OUT_PRECISION+5];//接受变量的空间应稍大于输出精度
    //     MPI_Recv(t,strlen(p),MPI_CHAR,myrank-1,90,MPI_COMM_WORLD,&status);
    //     string ans = t;
    //     cout<<"接收到数据: "<<ans<<endl;
    // }
    //广播通信
    mp_real arr[500];
    //char arr_str[500][WORDS];
    char** str_pass; 
    if(myrank == 0){
        fstream file_B;
        file_B.open("./B.txt",ios::out|ios::in);
        for(int i=0;i<500;i++){
            arr[i].read(file_B);//读进一维数组
        }
        str_pass = str2char(arr,500);
        cout<<"master process data ready."<<endl;  
    }
    //MPI_Bcast(arr_str,500*WORDS,MPI_CHAR,0,MPI_COMM_WORLD);
    cout<<"广播完毕."<<endl;
    // if(myrank == 1){
    //     cout<<"进程"<<myrank<<endl;
    //     //string ans = k;
    //     mp_real *ans = new mp_real[500];
    //     cout<<myrank<<"接收到数据: "<<endl;
    //     cout<< ans[0]<<endl;//<<ans[200]<<endl<<ans[499]<<endl;
    // }
    t2 = clock();
    MPI_Barrier(MPI_COMM_WORLD);
    cout<<fixed<<"mpi通信时间: "<<(double)(t2-t1)/CLOCKS_PER_SEC;
    cout<<endl;
    MPI::Finalize();
    // cout<<"高精度数据类型运算:"<<endl;
    // cout<<"a+f="<<a+f<<endl;
    // cout<<"a*f="<<a*f<<endl;
    // cout<<"a/f="<<a/f<<endl;
    // cout<<endl;

    // cout<<"数据文件读写测试："<<endl;
    // fstream file_in,file_out;
    // file_in.open("./data_in.txt",ios::in|ios::out);
    // file_out.open("./data_out.txt",ios::out|ios::in);
    // if(file_in.bad() || file_out.bad()){
    //     cout<<"files open failed!";
    //     exit(1);
    // }
    // mp_real arr[10][10];
    // for(int i=0;i<10;i++){
    //     for(int j=0;j<10;j++){
    //         arr[i][j].read(file_in);
    //         cout<<i*10+j+1<<": "<<arr[i][j]<<endl;
    //     }
    // }
    // fstream file_A,file_B,file_res;
    // file_A.open("./A.txt",ios::in|ios::out);
    // file_B.open("./B.txt",ios::out|ios::in);
    // file_res.open("./result.txt",ios::out);
    // if(file_A.bad() || file_B.bad()){
    //     cout<<"files open failed!";
    //     exit(1);
    // }
    //文件写入流测试
    // mp_real tmp = 0.0;
    // t1 = clock();
    // for(int i=0;i<100;i++){
    //     for(int j=0;j<10;j++){
    //         tmp = a+i+j;
    //         tmp.write(file_A,30,0,2);
    //         file_A << ",";
    //     }
    // }
    // for(int i=0;i<10;i++){
    //     for(int j=0;j<50;j++){
    //         tmp = a+i-j;
    //         tmp.write(file_B,30,0,2);
    //         file_B << ",";
    //     }
    // }
    // t2 = clock();
    // cout<<fixed<<"写入时间:"<<(double)(t2-t1)/CLOCKS_PER_SEC<<endl;
    //文件读取流测试
    // mp_real A[100][10],B[10][50],result[100][50];
    // for(int i=0;i<100;i++){
    //     for(int j=0;j<10;j++){
    //         A[i][j].read(file_A);
    //         //cout<<i*10+j+1<<": "<<A[i][j]<<endl;
    //     }
    // }
    // for(int i=0;i<10;i++){
    //     for(int j=0;j<50;j++){
    //         B[i][j].read(file_B);
    //         cout<<i*50+j+1<<": "<<B[i][j]<<endl;
    //     }
    // }
    // for(int i=0;i<100;i++){
    //     for(int j=0;j<50;j++){
    //         for(int k=0;k<10;k++){
    //             result[i][j] += A[i][k]*B[k][j];
    //         }
    //     }
    // }
    // for(int i=0;i<100;i++){
    //     for(int j=0;j<50;j++){
    //         //cout<<i*100+j<<": "<<result[i][j]<<endl;
    //         result[i][j].write(file_res,30,0,2);
    //         file_res << ",";
    //         //cout<<i*50+j+1<<": "<<result[i][j]<<endl;
    //     }
    // }
    //可以使用vector等模板
    // vector<mp_real> vec;
    // mp_real tmp;
    // for(int i=0;i<500;i++){
    //     tmp.read(file_B);
    //     vec.push_back(tmp);
    //     cout<<vec[i]<<endl;
    // }
    // file_A.close();
    // file_B.close();
    // file_res.close();
    // file_in.close();
    // file_out.close();
    mp::mp_finalize();//非必须
    return 0;
}

