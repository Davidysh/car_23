

#include "image_processing.h"

#include "dis_camera.h"
#include "message.h"
#include "HF_Double_DC_Motor.h"
#include <stdint.h>


uint8_t flag_show_status = 0;
#define  _avg_fre 12
static uint8_t _threshold_avg[_avg_fre] = {0};
static uint8_t _pos = 0;

uint8_t threshold_avg(uint8_t threshold){
    _threshold_avg[_pos] = threshold;
    _pos += 1;
    if (_pos >= _avg_fre)_pos = 0;
    uint8_t pos = _pos + _avg_fre;
    uint16_t ans = 0;
    for (uint8_t i = 1; i <= _avg_fre; ++i) {
        ans += _threshold_avg[(pos-i)%_avg_fre];
    }
    return ans/_avg_fre;
}

/**
args:

histogram：长度为256的整型数组，表示图像中0~255出现的像素值的个数。

pixel_total：整型变量，表示图像中像素的总数。

*/
uint8_t otsu_threshold( uint8_t* histogram, int pixel_total ){
    //用于计算均值
    unsigned int sumB = 0;
    unsigned int sum1 = 0;
    //用于计算类间方差
    float wB = 0.0f;
    float wF = 0.0f;
    float mF = 0.0f;
    //用于记录最大的类间方差
    float max_var = 0.0f;
    //用于计算类间方差
    float inter_var = 0.0f;
    //返回值：表示计算得到的阈值
    uint8_t threshold = 0;
    //索引buf
    uint16_t index_histo = 0;

    for ( index_histo = 1; index_histo < 256; ++index_histo ){
        sum1 += index_histo * histogram[ index_histo ];
    }

    for (index_histo = 1; index_histo < 256; ++index_histo){
        wB = wB + histogram[ index_histo ];
        wF = pixel_total - wB;
        if ( wB == 0 || wF == 0 ){
            continue;
        }
        sumB = sumB + index_histo * histogram[ index_histo ];
        mF = ( sum1 - sumB ) / wF;
        inter_var = wB * wF * ( ( sumB / wB ) - mF ) * ( ( sumB / wB ) - mF );
        if ( inter_var >= max_var ){
            threshold = index_histo;
            max_var = inter_var;
        }
    }

    threshold = threshold_avg(threshold);

    if(flag_show_status){
        sprintf(buf, " %d ", threshold);
        show_right_bottom_message(buf);
    }
    
    return threshold;
}



uint8_t OSTU(uint8_t* histogram, int pixel_total){
    //用于计算均值
    unsigned int sumB = 0;
    unsigned int sum1 = 0;
    //用于计算类间方差
    float wB = 0.0f;
    float wF = 0.0f;
    float mF = 0.0f;
    float oB = 0.0f;
    float oF = 0.0f;
    //用于记录最大的类间方差
    float max_var = 0.0f;
    //用于计算类间方差
    float inter_var = 0.0f;
    //返回值：表示计算得到的阈值
    uint8_t threshold = 0;
    //索引buf
    uint16_t index_histo = 0;

    //最大最小的灰度值
    uint16_t MaxValue,MinValue;

    for (MinValue = 0; MinValue < 256 && histogram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && histogram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    for ( index_histo = MinValue; index_histo <= MaxValue; ++index_histo ){  //计算灰度值总值
        sum1 += index_histo * histogram[ index_histo ];
    }

    for (index_histo = MinValue; index_histo < MaxValue; ++index_histo){
        wB = wB + histogram[ index_histo ];     //前景像素点个数
        wF = pixel_total - wB;                  //背景像素点个数
        oB = wB/pixel_total;
        oF = wF/pixel_total;
        sumB = sumB + index_histo * histogram[ index_histo ];
        mF = ( sum1 - sumB ) / wF;
        inter_var = oB * oF * ( ( sumB / wB ) - mF ) * ( ( sumB / wB ) - mF );
        if ( inter_var >= max_var ){
            threshold = index_histo;
            max_var = inter_var;
        }
    }

    // if(max_var > 80)
    // {
    //     if(threshold >= 50)
    //         threshold = 10;
    //     else if(threshold < 50)
    //          threshold = 100;
    // }

    if(flag_show_status){
        sprintf(buf, " %d ", threshold);
        show_right_bottom_message(buf);
    }
    
    return threshold;
}

// uint8 otsuThreshold(uint8 *GrayImage,uint32 ImageSize)
// {
//     uint32 pixelCount = c_w*c_h;
//     int T;                  //T为动态阈值
//     uint8 threshold = 0;    //最佳阈值
//     uint32 N0 = 0,N1 = ImageSize;      //图像中像素小于阈值T的像素个数记作N0,像素大于阈值T的像素个数记作N1
//     float w0,w1;    //w0为前景像素点占整幅图像的比例 w0 = N0/ImageSize (ImageSize为图像大小)
//                      //w1为前景像素点占整幅图像的比例 w1 = N1/ImageSize (ImageSize为图像大小)
//     uint32 u0den = 0,u1den = 0;  //图像中像素小于阈值T的像素加权和记作u0den
//                                   //图像中像素大于阈值T的像素加权和记作u1den
//     float u0,u1;    //u0为前景像素点平均灰度 u0 = u0den/N0
//                      //u1为前景像素点平均灰度 u1 = u1den/N1
//     float deltaTmp;         //类间方差 计算公式 w0*w1*(u0-u1)^2
//                              //类间方差最大的阈值T为所求最佳阈值

//     uint32 itemp,*pPixelCount = pixelCount;
//     float deltaMax = 0;

//     //获取灰度图像直方图
//     _Getting_GrayHistogram(pPixelCount,GrayImage,ImageSize);

//     //计算动态阈值
//     for (T = 0;T < 255;T ++)
//     {
//         itemp = *(pPixelCount + T);
//         u1den += T * itemp;
//     }
//     for (T = 0; T < 255; T++)
//     {
//         itemp = *(pPixelCount + T);
//         N0 += itemp;
//         N1 -= itemp;
//         if (N0 == 0 || N1 == 0)
//             continue ;
//         itemp = itemp * T;
//         u0den += itemp;
//         u1den -= itemp;

//         w0 = (float)N0 / (float)ImageSize;
//         w1 = (float)N1 / (float)ImageSize;
//         u0 = (float)u0den / (float)N0;
//         u1 = (float)u1den / (float)N1;

//         deltaTmp = w0*w1*(u0-u1)*(u0-u1);

//         if (deltaTmp > deltaMax)
//         {
//             deltaMax = deltaTmp;
//             threshold = T;
//         }
//     }
//     if(deltaMax < 80)
//     {
//         if(threshold >= 50)
//             threshold = 10;
//         else if(threshold < 50)
//             threshold = 100;
//     }
//     return threshold;
// }

uint8_t get_threshold() {//灰度值图像直方图
    uint8_t histogram[256];

    for (int i = 0; i < c_h; ++i) {
        uint8_t *one_h = mt9v03x_image_dvp[i];
        for (int j = 0; j < c_w; ++j) {
            histogram[one_h[j]]++;
        }
    }
    // return otsu_threshold(histogram, c_h * c_w);
    return OSTU(histogram, c_h * c_w);
}

// uint8_t _Getting_GrayHistogram(uint8_t pPixelCount,ImageSize){
//     uint8_t histogram[256];

//     for (int i = 0; i < c_h; ++i) {
//         uint8_t *one_h = mt9v03x_image_dvp[i];
//         for (int j = 0; j < c_w; ++j) {
//             histogram[one_h[j]]++;
//         }
//     }

    
//     // return otsu_threshold(histogram, c_h * c_w);
//     return OSTU(histogram, c_h * c_w);
// }

/*==================================================================
* 函数名: Get_Use_Image
* 功能: 获取比赛所用的图像大小即对图像进行分割选取
* 输入: void
* 输出: void
* 用法: Get_Use_Image();
* 修改记录:
===================================================================*/
void Get_Use_Image(void)
{
    short i = 0, j = 0, row = 0, line = 0;

    for (i = 0; i < c_h; i += 2)          //神眼高 120 / 2  = 60，
    // for (i = 0; i < IMAGEH; i += 3)       //OV7725高 240 / 3  = 80，
    {
        for (j = 0; j <= c_w; j += 2)     //神眼宽188 / 2  = 94，
        // for (j = 0; j <= IMAGEW; j += 3)  //OV7725宽320 / 3  = 106，
        {
            Image_Use[row][line] = Image_Data[i][j];
            line++;
        }
        line = 0;
        row++;
    }
}

/*==================================================================
* 函数名: Get_Use_Image
* 功能: 获取比赛所用的图像大小
* 输入: void
* 输出: void
* 用法: Get_Use_Image();
* 修改记录:
===================================================================*/
