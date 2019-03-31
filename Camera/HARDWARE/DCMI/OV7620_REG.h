#define GAIN      0x00
#define BLUE      0x01     
#define RED       0x02
#define SATC      0x03      //饱和度
#define GAVG      0x06      //亮度控制
#define ASC       0x07
#define WBBC      0x0C      //白平衡背景控制 (蓝)
#define COM4      0x0D      //白平衡背景控制 (红)
#define AEC       0x10      //自曝光控制
#define CLKRC     0x11      //分频 
#define COMA      0x12      //
#define COMB      0x13  
#define COMC      0x14      //0x24 QVGA   0x04 VGA
#define COMD      0x15      
#define FD        0x16      //丢帧
#define HSTART    0x17      //水平窗口开始   Min[05] Max[F6]   0x2f    VGA:(HSIZE-HSTARTHSIZE)*4   
#define HSIZE     0x18      //水平窗口结束   同上              0xCf    QVGA:(HSIZE-HSTART)*2            
#define VSTRT     0x19      //垂直窗口开始   同上              6       VGA:(VSIZE-VSTRT+1)
#define VSIZE     0x1A      //垂直窗口结束   同上              245     QVGA:(VSIZE-VSTRT+1)
#define PSHFT     0x1B      //像素偏移  
#define MIDH      0x1C      
#define MIDL      0x1D      
#define COME      0x20
#define YCOF      0x21      //Y输出偏移调整
#define UCOF      0x22      //U输出偏移调整
#define CCC       0x23      //晶体电流控制
#define AEW       0x24
#define AEB       0x25  
#define COMF      0x26
#define COMG      0x27
#define COMH      0x28     //0x20  连续扫描模式   0x00  隔行扫描
#define COMI      0x29
#define FRA1      0x2A
#define FRA2      0x2B
#define BE        0x2C
#define COMJ      0x2D     //隔行扫描：0xC1  只输出奇场   0x81  奇偶场输出帧率30帧/s
#define VCOF      0x2E     //V输出偏移调整
#define SPCA      0x60
#define SPCB      0x61
#define RGB_GC    0x62
#define Y_GC      0x64
#define SPCC      0x65
#define AWBPC     0x66
#define CPS       0x67
#define SPCD      0x68
#define ANS       0x69
#define VEEE      0x6A    //垂直边缘增强
#define EONC      0x6F    //奇偶场噪声补偿
#define COMK      0x70    
#define COMJJ     0x71  
#define HS1ES     0x72
#define HS2ES     0x73
#define COMM      0x74
#define COMN      0x75
#define COMO      0x76
#define FALS      0x7C




































