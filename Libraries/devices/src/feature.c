#include "common.h"
#include "picture.h"

/*********************************************中线部分RR******************************************************/

//函数外部声明
int get_left();
int get_right();
void second_scan();
uint8_t feature2();
int feature3();
int get_black_length();
int ab();
uint8_t feature1();
void feature_init();
int black_count();
void first_scan();
int get_effective();
int get_single_effective(int, int);
int effective_row(int,int,int);
void featureline();
uint8_t big_turn();
void link();
void centerline_filter();
uint8_t cross_judge();
int cross_deal();
void lost_deal();
void end_judge();
uint8_t huanlu_judge();
int huanlu_deal();

//外部定义

//Center_line中定义
int variance = 0;
#define second_scan_line  (49)                //当首行黑点过多时从此行开始扫描：45

int effective_flag = 0;                       //有效行标志，用于计数
int effective_line_temp = 0;                  //有效行暂存
int effective_line = 0,last_effective = 0;    //该场有效行，上场有效行

int centerline_save[H];                       //存储上场每行中线
int centerline_last = W/2;                    //存储上场H-1行中线

int road_wide_temp[H];                            //记录赛道宽度
int right_line_temp[H] , left_line_temp[H];   //左右线暂存
int right_lost = 0 , left_lost = 0;               //丢线标志     
int right_lost_found = 0 , left_lost_found = 0;   //丢线找回
int right_lost_count = 0 , left_lost_count = 0;   //丢线计数
     
int wide_nature[H] =                          //直道0-59行赛道宽度，用于补线
{20,22,24,28,30,32,34,38,41,43,45,47,49,51,53,55,58,61,63,65,67,69,71,73,74,76,78,80,82,84,
86,87,89,91,92,94,95,97,98,99,101,102,104,105,107,108,110,111,112,114,114,116,118,119,
116,121,122,123,124,125};  
  
//int po_wide_nature[H] = [77,77,78,80,80,82,82,84,84,86,87,88,89,90,91]  //坡道赛道长度


//十字

#define cross_judge_count (3)
#define cross_record_line  (25)                //十字置标志位行数
#define cross_first_lost_add (8)              //十字首行丢线补点数
#define first_lost_add (10)                    //普通丢线补点数
#define featureline_add (1)                     //普通补线

int cross_flag = 0;                     //十字标志位
int cross_way_last = 0;                 //十字上一场图像的找线方向

//环路
int huanlu_flag = 0;   //环路标志位
int huanlu_way_last = 0;  //环路找线方向

//障碍

int block_flag = 0 , block_left_flag = 0 , block_right_flag = 0;  //障碍各标志位
int block_line = 0;                        //障碍的行数
int block_count = 0;                       //障碍计数

#define block_judge_count (4) 
#define block_deal_point (10)                //障碍中线左右加减值
#define block_deal_photo_count (3)

//大弯

#define lost_count_judge (20)                      //此行边界相隔大于时 7认为丢线
#define lost_found_judge_line (5)                 //当丢线数大于并重新找到时认为重新找到线
 
int last_error;
int big_right_turn_flag = 0,big_left_turn_flag = 0;

//坡道
//int podao_count = 0;
//int podao_flag_up = 0;                            //上坡标志
//int podao_flag_down = 0;                          //下坡标志

//终点线识别
//int end_flag = 0;                                //终点标志
//int end_count = 0;
//extern int speed_set;

/**************************中线算法****************************
* 函数名称：Center_line
* 功能说明：中线算法
* 备注：
利用上场图像：
1.初始化：
2.有效行：
先扫描一次，得到effective_line_temp；

3.保存上场中线

**************************************************************/
  //调试用
int bug_flag1 = 0;
int bug_flag2 = 0;

void Center_line()
{
  int count = 0;
  
  effective_flag++;     
  feature_init(); //图像初始化
	
  if(black_count(H-1)<(W-wide_nature[H-1]/3))     //第H-1行黑点少的情况 160 - 70
  {
    first_scan(H-1);                           
    effective_line_temp = get_effective(left_line_temp[H-1],right_line_temp[H-1],H-1);    
    featureline(H-2);
    
/*    //有效行之前的中线处理
    if(centerline[H-1] > centerline[effective_line_temp])
	{
		for(count=effective_line_temp; count>0; count--)
		{centerline[count] = 0;}
	}
	else if(centerline[H-1] < centerline[effective_line_temp])
	{
		for(count=effective_line_temp; count>0; count--)
		{centerline[count] = W-1;}
	}
	else 
	{
		for(count=effective_line_temp; count>0; count--)
			{centerline[count] = W/2;}
	}
*/		
    
/*    //小S处理一 注释
    if((effective_row(left_line_temp[H-1],right_line_temp[H-1],H-1) > 30) && effective_line_temp <= 10 
       && cross_flag != 1 && (left_lost_count < 5 && right_lost_count < 5))
   {
    link(centerline,H-1,effective_line_temp,centerline[effective_line_temp]);
    }
*/     
    
/*    //计算中线平均值 小S处理二
  for (count=H-1; count>effective_line_temp; count--)
  {
    variance += centerline[count];
  }
  variance = variance/(H-1-effective_line_temp);
    
  if((variance >= (W/2-10)) && (variance <= (W/2+10)) && effective_line_temp < 12)
  {
  for (count=H-1; count>effective_line_temp; count--)
  {centerline[count] = variance;}
  }
*/  
  }

  else                                           //如果首行黑点过多，则取second_scan_line行.
    if(black_count(second_scan_line) < (W-wide_nature[second_scan_line]/3))
    {
      first_scan(second_scan_line);                           
      effective_line_temp = get_effective(left_line_temp[second_scan_line],right_line_temp[second_scan_line],second_scan_line);
      featureline(second_scan_line-1);
/*      
      //有效行之前的中线处理
    if(centerline[second_scan_line] > centerline[effective_line_temp])
	{
		for(count=effective_line_temp; count>0; count--)
		{centerline[count] = 0;}
	}
	else if(centerline[H-1] < centerline[effective_line_temp])
	{
		for(count=effective_line_temp; count>0; count--)
		{centerline[count] = W-1;}
	}
	else 
	{
		for(count=effective_line_temp; count>0; count--)
			{centerline[count] = W/2;}
	}
*/      
/*//小S处理一 注释
      if((effective_row(left_line_temp[second_scan_line],right_line_temp[second_scan_line],second_scan_line) > 30) && effective_line_temp <= 10  
       && cross_flag != 1 && (left_lost_count < 5 && right_lost_count < 5))
    {
    link(centerline,second_scan_line,effective_line_temp,centerline[effective_line_temp]);
    }
*/
/*    //计算中线平均值 小S处理二
    for (count=second_scan_line; count>effective_line_temp; count--)
  {
    variance += centerline[count];
  }
  variance = variance/(second_scan_line-effective_line_temp);
    
  if((variance >= (W/2-10)) && (variance <= (W/2+10)) && effective_line_temp < 12)
  {
  for (count=second_scan_line; count>effective_line_temp; count--)
  {centerline[count] = variance;}
  }
*/  
      //将之前的行数补至扫描后的一行
      for(count = H-1;count > second_scan_line;count--)
      {
        left_line_temp[count] = left_line_temp[second_scan_line];
        right_line_temp[count] = right_line_temp[second_scan_line];
        centerline[count] = centerline[second_scan_line];
      }
    }
	
    else                                    //图像未判断出的情况
    {                                       
      centerline[H-1] = centerline_last;    //中线取上场中线
      effective_line_temp = H-1;
      return;
    }
	
  
  
/* 
  //障碍处理区
  if(block_count >= 0 && block_left_flag == 1)
  {
    block_count--;
    if(block_count == 0) 
      block_left_flag=0;
    
      for(count = H-1;count > effective_line_temp;count--)
      { 
        centerline[count] = centerline[count] + block_deal_point;
        //限幅
        if(centerline[count] > (W-1))  centerline[count]=W-1;
        if(centerline[count] < 0) centerline[count]=0;
      }
  }  
  if(block_count>=0 && block_right_flag==1)
  {
    block_count--;
    if(block_count == 0) 
      block_right_flag=0;
    
      for(count=H-1;count > effective_line_temp;count--)
      {
        centerline[count] = centerline[count]-block_deal_point;
        
        if(centerline[count] < 0)  centerline[count] = 0;
        if(centerline[count] > (W-1))  centerline[count] = W-1;
      }
  }
  
  if((block_count>=0 && block_right_flag==1)||(block_count >= 0 && block_left_flag == 1))  //蜂鸣器检测
    gpio_init(PTC11,GPO,0);
  else 
    gpio_init(PTC11,GPO,1);
*/
/*  
  //坡道识别
  if(effective_line_temp < 1 && cross_flag == 0 && variance < 5000)
  {
    for(count=0;count<10;count++)
  {
  if(((right_line_temp[count]-left_line_temp[count]) - wide_nature[count]) > 30)
    podao_count++;
  }
  if(podao_count > 9)
  podao_flag_up = 1;  
  else 
    podao_flag_up = 0;
  
  for(count=0;count<10;count++)
  {
//  if(((right_line_temp[count]-left_line_temp[count]) - wide_nature[count]) < -12 &&
//     ((right_line_temp[count]-left_line_temp[count]) - wide_nature[count]) > 20)
    
    if((right_line_temp[count]-left_line_temp[count]) < (29+count) &&
       (right_line_temp[count]-left_line_temp[count]) > (25+count))
    podao_count++;
  }
  if(podao_count > 8)
  podao_flag_down = 1;  
  else 
    podao_flag_down = 0;
  
  }  
  else;
*/  
  
  //存储有效行
  if(effective_flag >= 1)                       
  {
    last_effective = effective_line;
    effective_line = effective_line_temp;  //外部使用有效行
    effective_flag = 0;
  }
  
  //记录上场中线
  for(count = 0;count < H;count++)                    
    centerline_save[count] = centerline[count];
  
  //补线
  if(effective_line_temp > 55)
  {
    for(count = 0;count < H;count++)
      centerline[count] = centerline_save[count];
    effective_line_temp = effective_line;               //有效行为上场有效行
  }
    
}


/**************************中线初始化函数**************************
* 函数名称：feature_init
* 功能说明：上场图像初始化 
* 备注：记录上一场图像
        其他判断清零
********************************************************/

void feature_init()
{
  int init_count;
//记录上场H-1行中线
  centerline_last = centerline[H-1]; 
  
//上场图像初始化  各标志清零  
  for(init_count = 0;init_count < H;init_count++)
  {
    right_line_temp[init_count] = W-1;                     //左右两边暂存
    left_line_temp[init_count] = 0;
    centerline[init_count] = W/2;
  }

  block_line = 0;                                          //位置清零
//  if(block_flag > 0) block_flag--;
  if(cross_flag > 0) cross_flag--;  //十字
  if(huanlu_flag > 0) huanlu_flag--;//环路  
  
  left_lost_found = 0, right_lost_found = 0;                //丢线清零
  right_lost = 0, left_lost = 0;
  right_lost_count = 0, left_lost_count = 0;
  
  variance = 0;  //方差清零
}


/**************************中线扫描函数****************************
* 函数名称：first_scan
* 功能说明：第一行扫描
* 备注：确定车前最近一行的中线
*****************************************************************/

void first_scan(int aim)
{
  int first_lost_add_point = 10;                             //补点数
  
  if(block_flag == 0 && block_count == 0)                           //不是障碍，障碍另处理
  {
    if(img[aim*W + centerline_last] == white)
    {
      if(cross_flag == 1)                                           //补点
        first_lost_add_point = cross_first_lost_add;
      else 
        first_lost_add_point = first_lost_add;  
      
      left_line_temp[aim] = get_left(aim,centerline_last);
      right_line_temp[aim] = get_right(aim,centerline_last);
	  //第一行赛道宽度
      road_wide_temp[aim] = get_right(aim,centerline_last) - get_left(aim,centerline_last);    //记录赛道宽度

	  //未丢线
      if(left_line_temp[aim] > 0 && right_line_temp[aim] < (W-1)) 
      {
        centerline[aim] = (left_line_temp[aim] + right_line_temp[aim])/2;
      }
      //左丢线
      if(left_line_temp[aim] == 0 && right_line_temp[aim] < (W-1)) 
      {
        centerline[aim] = (right_line_temp[aim] - wide_nature[aim]/2 - first_lost_add_point) < 0 ? 0 : (right_line_temp[aim]-wide_nature[aim]/2-first_lost_add_point);
        left_lost = 1, left_lost_count++;
        return;
      }
      //右丢线
      if(left_line_temp[aim] > 0 && right_line_temp[aim] == (W-1))  
      {
        centerline[aim] = (left_line_temp[aim] + wide_nature[aim]/2 + first_lost_add_point) > (W-1) ? (W-1) : (left_line_temp[aim]+wide_nature[aim]/2+first_lost_add_point);
        right_lost = 1, right_lost_count++;
        return;
      }
	  //两边均丢线
      if(left_line_temp[aim] <= 0 && right_line_temp[aim] >= (W-1))  
      {
        centerline[aim] = centerline_last;
        if(centerline[aim] < 0) centerline[aim] = 0;
        if(centerline[aim] >= W ) centerline[aim] = W-1;
        
        right_lost = 1, right_lost_count++;
        left_lost = 1, left_lost_count++;
        return;
      }
    }
	
  }  
//  second_scan(aim-30);    //障碍情况处理,
}


/**************************中线扫描函数****************************
* 函数名称：second_scan
* 功能说明：第一行扫描
* 备注：障碍那有点问题，容易误判，要改。
**************************************************************/

#define head_block_judge_point (25)  //zhangai
#define block_length_max (45)
#define end_judge_point (20)

void second_scan(int aim)
{
  int black_point = 0, black_point1 = 0 , black_point2 = 0 , black_point3 = 0; 
  int block_left = 0, block_right = 0;                         //障碍左右
  int block_left_line_temp = 0, block_right_line_temp = 0;      //障碍左右线暂存
  int first_lost_add_point = 0;                       //补点数
    
  black_point = feature2(aim,0,W-1);
  
  if(cross_flag == 1) 
    first_lost_add_point = cross_first_lost_add;
  else 
    first_lost_add_point = first_lost_add;
  
  if(black_point != 0)                      //障碍情况
  {
    black_point1 = feature2(aim-1,0,W-1);                       //多判断几个点的黑白情况
    black_point2 = feature2(aim-2,0,W-1);
    black_point3 = feature2(aim-4,0,W-1);
    centerline[aim] = feature3(aim,0,W-1);                      //中点位于黑线、黑块中心
    
    if((black_point1 >= head_block_judge_point || black_point2 >= head_block_judge_point ||         //判断障碍
        black_point3 >= head_block_judge_point)&& black_point2 != 0 && black_point3 != 0 && 
       cross_flag == 0 && get_black_length(aim,centerline[aim]) < block_length_max)            //cross_flag == 0，十字情况可能影响障碍判断
    {   
      for(block_left = centerline[aim];block_left >= 0;block_left--)                         //判断障碍位置
      {
        if(img[aim*W + block_left] == white)
          break;
      }
      for(block_right = centerline[aim];block_right <= W-1;block_right++)
      {
        if(img[aim*W + block_right] == white)
          break;
      }
      
      block_left_line_temp = get_left(aim,block_left);
      block_right_line_temp = get_right(aim,block_right);
      
      if(ab(block_left_line_temp-centerline[aim]) >= ab(block_right_line_temp-centerline[aim])) //障碍在右侧
      {
        block_right_flag = 1;block_left_flag = 0;
        left_line_temp[aim] = block_left_line_temp;
        right_line_temp[aim] = get_right(aim,block_left);
      }
      else                                          //障碍在左侧
      {
        block_left_flag = 1;block_right_flag = 0;
        left_line_temp[aim] = get_left(aim,block_right);
        right_line_temp[aim] = block_right_line_temp;
      }
      
      if(left_line_temp[aim] == 0) 
      {
        left_lost = 1;
        left_lost_count++;
      }
      else left_lost = 0;
      
      if(right_line_temp[aim] == W-1) 
      {
        right_lost = 1;
        right_lost_count++;
      }
      else right_lost = 0;
      
      centerline[aim] = (left_line_temp[aim]+right_line_temp[aim])/2;
      
      if(img[(aim-1)*W+centerline[aim]] == white)
      {
        if((get_right(aim-1,centerline[aim]) - get_left(aim-1,centerline[aim])) > wide_nature[aim]*4/5)   //判定失败
        {
          block_right_flag = 0,block_left_flag = 0;
        }
        else
        {
          block_flag = block_judge_count;        //5
          block_line = aim; 
          block_count = block_deal_photo_count;  //10        //障碍标志
          return;
        }
      }
    } 
    else;
  }  
  else;
  
    if(left_line_temp[aim] == 0 && img[aim*W + left_line_temp[aim]] == white) 
      left_lost = 1, left_lost_count++;
    else left_lost = 0, left_lost_count = 0;
    
    if(right_line_temp[aim] == (W-1) && img[aim*W + right_line_temp[aim]] == white) 
      right_lost = 1, right_lost_count++;
    else right_lost = 0, right_lost_count = 0;
    
    
    if(left_lost == 0 && right_lost == 0) 
      centerline[aim] = (left_line_temp[aim]+right_line_temp[aim])/2;
    if(left_lost == 1 && right_lost == 0) 
      centerline[aim] = right_line_temp[aim]-wide_nature[aim]/2 - first_lost_add_point;
    if(left_lost == 0 && right_lost == 1) 
      centerline[aim] = left_line_temp[aim]+wide_nature[aim]/2 + first_lost_add_point;
    if(left_lost == 1 && right_lost == 1) 
      centerline[aim] = centerline_last;
    
    if(centerline[aim] < 0) 
      centerline[aim] = 0;
    if(centerline[aim] > W-1) 
      centerline[aim] = W-1;  
}

/**************************中线扫描函数********************
* 函数名称：end_judge
* 功能说明:终点线检测
* 备注：
       扫描第30行
***********************************************************/

/*
void end_judge(int aim)
{
  int black_point = 0, black_point1 = 0 , black_point2 = 0 , black_point3 = 0; 
  int first_lost_add_point = 0;                       //补点数
    
  black_point = feature2(aim,0,W-1);
    
  if(black_point != 0)                      //障碍情况
  {
    black_point1 = feature2(aim-1,0,W-1);                       //多判断几个点的黑白情况
    black_point2 = feature2(aim-2,0,W-1);
    black_point3 = feature2(aim-6,0,W-1);
    centerline[aim] = feature3(aim,0,W-1);                      //中点位于黑线、黑块中心
    
    if((black_point1 >= head_block_judge_point || black_point2 >= head_block_judge_point ||         //判断障碍
        black_point3 >= head_block_judge_point)&& black_point2 != 0 && black_point3 != 0 && 
       cross_flag == 0 && get_black_length(aim,centerline[aim]) < block_length_max)            //cross_flag == 0，十字情况可能影响障碍判断
    {   
      for(block_left = centerline[aim];block_left >= 0;block_left--)                         //判断障碍位置
      {
        if(img[aim*W + block_left] == white)
          break;
      }
      for(block_right = centerline[aim];block_right <= W-1;block_right++)
      {
        if(img[aim*W + block_right] == white)
          break;
      }
      
      block_left_line_temp = get_left(aim,block_left);
      block_right_line_temp = get_right(aim,block_right);
      
      if(ab(block_left_line_temp-centerline[aim]) >= ab(block_right_line_temp-centerline[aim])) //障碍在右侧
      {
        block_right_flag = 1;block_left_flag = 0;
        left_line_temp[aim] = block_left_line_temp;
        right_line_temp[aim] = get_right(aim,block_left);
      }
      else                                          //障碍在左侧
      {
        block_left_flag = 1;block_right_flag = 0;
        left_line_temp[aim] = get_left(aim,block_right);
        right_line_temp[aim] = block_right_line_temp;
      }
      
      if(left_line_temp[aim] == 0) 
      {
        left_lost = 1;
        left_lost_count++;
      }
      else left_lost = 0;
      
      if(right_line_temp[aim] == W-1) 
      {
        right_lost = 1;
        right_lost_count++;
      }
      else right_lost = 0;
      
      centerline[aim] = (left_line_temp[aim]+right_line_temp[aim])/2;
      
      if(img[(aim-1)*W+centerline[aim]] == white)
      {
        if((get_right(aim-1,centerline[aim]) - get_left(aim-1,centerline[aim])) > wide_nature[aim]*4/5)   //判定失败
        {
          block_right_flag = 0,block_left_flag = 0;
        }
        else
        {
          block_flag = block_judge_count;        //5
          block_line = aim; 
          block_count = block_deal_photo_count;  //10        //障碍标志
          return;
        }
      }
    } 
    else;
  }  
  else;
  
    if(left_line_temp[aim] == 0 && img[aim*W + left_line_temp[aim]] == white) 
      left_lost = 1, left_lost_count++;
    else left_lost = 0, left_lost_count = 0;
    
    if(right_line_temp[aim] == (W-1) && img[aim*W + right_line_temp[aim]] == white) 
      right_lost = 1, right_lost_count++;
    else right_lost = 0, right_lost_count = 0;
    
    
    if(left_lost == 0 && right_lost == 0) 
      centerline[aim] = (left_line_temp[aim]+right_line_temp[aim])/2;
    if(left_lost == 1 && right_lost == 0) 
      centerline[aim] = right_line_temp[aim]-wide_nature[aim]/2 - first_lost_add_point;
    if(left_lost == 0 && right_lost == 1) 
      centerline[aim] = left_line_temp[aim]+wide_nature[aim]/2 + first_lost_add_point;
    if(left_lost == 1 && right_lost == 1) 
      centerline[aim] = centerline_last;
    
    if(centerline[aim] < 0) 
      centerline[aim] = 0;
    if(centerline[aim] > W-1) 
      centerline[aim] = W-1;  }
*/
            
/**************************中线扫描函数********************
* 函数名称：featureline
* 功能说明：特征线提取
* 备注：
       从H-1行向前扫描，注意限制条件
***********************************************************/

void featureline(int start_line)
{
  int feature_count = 0;
  int cross_line = 0;  //找线行计数 十字完成计数
  int huanlu_line = 0;
  
  if(start_line < 0)
    return;
  
  for(feature_count = start_line;feature_count >= effective_line_temp;feature_count--) 
  {
    if(feature1(feature_count,centerline[feature_count+1]) == 0)
      continue;                                                  //未找到中点情况
    
    if(left_lost == 0)  //判断左右丢线情况  修改
    {
      if(left_line_temp[feature_count] < (left_line_temp[feature_count+1] - lost_count_judge) ||
         (left_line_temp[feature_count] == 0 && img[feature_count*W+left_line_temp[feature_count]] == white)) 
        left_lost = 1,left_lost_count++;
      else left_lost = 0;
    }
    else  
    {
      if(((left_line_temp[feature_count] >= left_line_temp[feature_count+1] - 2) || left_line_temp[H-1] == 0)
         && (left_line_temp[feature_count] != 0 || img[feature_count*W+left_line_temp[feature_count]] == black))
      {
        left_lost = 0,left_lost_count = 0;
        if(left_lost_count > lost_found_judge_line && left_line_temp[H-1] > 0)  //丢线多于5行
          left_lost_found = 1;  //又找到线          
      }
      else left_lost = 1,left_lost_count++;
    }
    
    if(right_lost == 0)
    {
      if(right_line_temp[feature_count] > (right_line_temp[feature_count+1] + lost_count_judge) ||
         (right_line_temp[feature_count] == (W-1) && img[feature_count*W+right_line_temp[feature_count]] == white)) 
        right_lost = 1,right_lost_count++;
      
      else right_lost = 0 ;
    }
    else
    {
      if(((right_line_temp[feature_count] <= right_line_temp[feature_count+1] + 2) || right_line_temp[H-1] == (W-1))
         && (right_line_temp[feature_count] != (W-1) || img[feature_count*W+right_line_temp[feature_count]] == black))
      {
        if(right_lost_count > lost_found_judge_line && right_line_temp[H-1] < (W-1))
          right_lost_found = 1;  //找到线
        right_lost = 0,right_lost_count = 0;
      }
      else right_lost = 1,right_lost_count++;
    }
	//记录该行宽度
	road_wide_temp[feature_count] = right_line_temp[feature_count] - left_line_temp[feature_count];
    
    //这一行左右均未丢线
    if(left_lost == 0 && right_lost == 0)   
    {
      centerline[feature_count] = (left_line_temp[feature_count] + right_line_temp[feature_count])/2;
	  
	  //判断环路情况
	  if( huanlu_judge(feature_count) == 1 && feature_count > effective_line_temp)
	  {
		 
            huanlu_line = huanlu_deal(feature_count);
		  if(huanlu_line < 0)
		  {
			  effective_line_temp = feature_count+1;
			  return;
		  }
		  
		  huanlu_flag = 1;  //环路标志置1
		  featureline(huanlu_line-1);
		  break;
	  }
	  left_lost_count = 0 , right_lost_count = 0;
	  
    }
    
    //出现丢线情况   	  
    if((left_lost == 1 || right_lost == 1 || right_lost_found == 1 || left_lost_found ==1) )  //十字
    {
    if( cross_judge(feature_count) == 1 && feature_count > effective_line_temp)   //大于有效行                            
    {
    left_lost_count = 0,right_lost_count = 0;        //左右丢线清零
    
    cross_line = cross_deal(feature_count);
    
    if(cross_line < 0)           //十字判断
    {
    effective_line_temp = feature_count+1;
    return;
    }
    
    if(feature_count > cross_record_line)           //25
    {
    cross_flag = cross_judge_count;                  //3
    }
    featureline(cross_line-1);
    break;
    }
    }
    
    	
    //补线  ，需要修改!!!
    if(left_lost == 1 && right_lost == 0)
      centerline[feature_count] = centerline[feature_count+1] + (right_line_temp[feature_count] - right_line_temp[feature_count+1] + featureline_add);  //featureline_add为补线点数
    if(left_lost == 0 && right_lost == 1)	 
      centerline[feature_count] = centerline[feature_count+1] + (left_line_temp[feature_count] - left_line_temp[feature_count+1] - featureline_add);
    if(left_lost == 1 && right_lost == 1)  
      centerline[feature_count] = centerline[feature_count+1];
    
    //限幅
    if(centerline[feature_count] < 0) centerline[feature_count] = 0;
    if(centerline[feature_count] > (W-1)) centerline[feature_count] = W-1;
  }
/*    //大弯处理
      if(big_turn() == 0)   
      {
        centerline_filter();  //滤波处理，防止反光丢线
        big_right_turn_flag = 0;
        big_left_turn_flag = 0;
      }
      else;    
*/  
}
  

/**************************大弯判断函数****************************
* 函数名称：big_turn
* 功能说明：大弯情况
* 备注：
**************************************************************************/

uint8_t big_turn()
{
#define big_turn_judge_effective_far (14)        //大弯有效行判定较远  经观察 这个是很合理的
#define big_turn_judge_far     (25)          //大弯有效行极限阈值  
  
#define big_turn_count_far    (6)           //大弯远处时判断行数
#define big_turn_count_near   (12)          //大弯近处时判断行数
#define big_turn_limit_judge   (effective_line_temp+3)      //大弯有效行前判断
#define big_turn_break_line   (6)             //大弯极限中断清零行数
  
  
  int count = 0,left_count = 0,right_count = 0,left_break = 0,right_break = 0;
  int left_start = 0,right_start = 0;       //左开始 左结束 右开始 右结束
  int big_turn_judge_line = 0;
  int big_turn_judge_count = 0;             //判定大弯的左右丢线的行数
  
  if(effective_line_temp < big_turn_judge_effective_far)           //有效行太小时不做判断处理14
    return 0;
  
  if(effective_line_temp < big_turn_judge_far)                 //如果有效行<25，则丢行数>4，判断为弯道
    big_turn_judge_count = big_turn_count_far;                  //若有效行靠前，则丢线行定为8
  else big_turn_judge_count = big_turn_count_near;
  
  for(count = H-1;count >= effective_line_temp;count--)
  {
    if(left_line_temp[count] == 0) 
      left_count++,left_break = 0;           //left_count计数
    else left_break++;
    
    if(left_count == 1)                         //记录丢线始
      left_start = count;
    if(left_break > big_turn_break_line) 
      left_count = 0,left_start = 0;        //去除杂点
    
    
    if(right_line_temp[count] == W-1) 
      right_count++,right_break = 0;      //右边判断
    else right_break++;
    
    if(right_count == 1) 
      right_start = count;
    if(right_break > big_turn_break_line)       //大弯极限清零
      right_count = 0,right_start = 0;
  }
  
  
  if(big_turn_limit_judge < H)                     //有效行+3、有效行，判断
    big_turn_judge_line = big_turn_limit_judge;
  else 
    big_turn_judge_line = effective_line_temp;
  
  
  if(left_count >= big_turn_judge_count && left_start > 0)  //左弯
  {
    if(big_left_turn_flag == 1)
    {
      for(count = H-1;count > effective_line_temp;count--)
        if(right_line_temp[count] == (W-1))                 
          break;
      if(count <= effective_line_temp)
      {
        link(centerline,left_start,left_start-left_count/2,0);
        effective_line_temp = left_start-left_count/2;
        return 1;
      }
//      end_count = 1;
     
    }
    
    if(left_line_temp[big_turn_judge_line] != 0)
      return 0;
    
    if((left_start-left_count) < 0)
      return 0;
    
    for(count = left_start-1;count > (left_start-left_count);count--)  //
      if(right_line_temp[count] < right_line_temp[count+1])
        break;
    if(count <= (left_start-left_count))
      return 0;
    
    for(count = count-1;count > (left_start-left_count);count--)
      if(right_line_temp[count] > right_line_temp[count+1])
        return 0;
    
    link(centerline,left_start,left_start-left_count/2,0);
    effective_line_temp = left_start-left_count/2;
    big_left_turn_flag = 1;
    big_right_turn_flag = 0;
    return 1;
  }
  
  else 
    if(right_count >= big_turn_judge_count && right_start > 0)     //右弯
    {
      if(big_right_turn_flag == 1)
      {
        for(count = H-1;count > effective_line_temp;count--)
          if(left_line_temp[count] == 0)
            break;
        if(count <= effective_line_temp)
        {
          link(centerline,right_start,right_start-right_count/2,W-1);
          effective_line_temp = right_start-right_count/2;
          return 1;
        }
//        end_count = 1;
      }
      
      if(right_line_temp[big_turn_judge_line] != (W-1))
        return 0;
      if((right_start-right_count) < 0)
        return 0;
      for(count = right_start-1;count > (right_start-right_count);count--)
        if(left_line_temp[count] > left_line_temp[count+1])
          break;
      if(count <= (right_start-right_count))
        return 0;
      for(count = count-1;count > (right_start-right_count);count--)
        if(left_line_temp[count] < left_line_temp[count+1])
          return 0;
      
      link(centerline,right_start,right_start-right_count/2,W-1);
      effective_line_temp = right_start-right_count/2;
      big_right_turn_flag = 1;
      big_left_turn_flag = 0;
      return 1;
    }
    else
    {
      big_left_turn_flag = 0;
      big_right_turn_flag = 0;
      return 0;
    }
}

/**************************前几行函数****************************
* 函数名称：lost_deal
* 功能说明：前几行丢线连线
* 备注：如果车前几行丢线，然后找到两个边界，则将H-1 行中线与
那行中线相连
*************************************************************/

void lost_deal(void)
{
  int count;
  int right_deal , left_deal;
  
  if(left_line_temp[H-1]== 0 && right_line_temp[H-1] != W-1)
  {
    for(count = H-1 ; count >= effective_line_temp+2; count--)
    {
      if(left_line_temp[count] > 0 && left_line_temp[count-2] > 0)
        break;
    }
    left_deal = count;
    if(right_line_temp[left_deal] < W-1)
      link(centerline,H-1,left_deal,centerline[left_deal]);
  }
  
  else if(right_line_temp[H-1]==W-1 && left_line_temp[H-1] != 0)
  {
    for(count = H-1 ; count >= effective_line_temp+2; count--)
    {
      if(right_line_temp[count] < W-1 && right_line_temp[count-2] < W-1)
        break;
    }
    right_deal = count;
    if(left_line_temp[right_deal] > 0)
      link(centerline,H-1,right_deal,centerline[right_deal]);
  }
  
  else;
}



/**************************找线函数****************************
* 函数名称：feature1
* 功能说明：得到双线特定行特定左右线间的中线
* 备注：先确定白色点，从白点向两边搜
**************************************************************************/
uint8_t feature1(int aim,int start_point)
{
  uint8_t result = 0;
  int w = 0;                         //计数必须用int
  int w_right = 0,w_left = 0,wide = 0;           //第一次找位置
  uint8_t *pic_fea1 = NULL;

  pic_fea1 = &img[aim*W];
  
  if(*(pic_fea1 + start_point) == black)
  {
    if(start_point < 5)
    {
      if(*(pic_fea1 + start_point + 10) == white) start_point = start_point + 10;
    }
    if(start_point >= W-5) 
    {
      if(*(pic_fea1 + start_point - 10) == white) start_point = start_point - 10;
    }
    if(*(pic_fea1 + start_point) == black)
    {
      result = 0,effective_line_temp = aim + 1;   //判断该行基本全黑
      if(effective_line_temp > (H-1)) effective_line_temp = H-1;
      return result;
    }
  }
  w = start_point;  //找到白点
  
  for(w = start_point;w < W-1;w++)
  {
    if(*(pic_fea1 + w) == black && *(pic_fea1 + w + 1) == black)
      break;
  }
  w_right = w;
  
  for(w = start_point;w > 0;w--)
  {
    
    if(*(pic_fea1 + w) == black && *(pic_fea1 + w - 1) == black)
      break;
  }
  w_left = w;
  
  wide = w_right - w_left;
  
  if(wide > (wide_nature[aim]/2) || (w_left == 0 && wide > 6) || (w_right == (W-1) && wide > 6) || block_flag >= 1)
  {	
    right_line_temp[aim] = w_right,left_line_temp[aim] = w_left;
    result = 1;
  }
  else 
  {
    result = 0 , effective_line_temp = aim+1;
    if(effective_line_temp >= H)
    effective_line_temp = H-1;
  }
  return result;
}



/**************************找线函数****************************
* 函数名称：feature2
* 功能说明：特定行特定点之间赛道上的黑点个数
* 备注：特征函数2
***********************************************************/

uint8_t feature2(int aim,int left,int right)         
{
    int i = 0;                         //计数必须用int
    int w_right = 0,w_left = 0;           //第一次找位置
    uint8_t black_point = 0,black_point_temp = 0;
    uint8_t *pic_fea2 = NULL;
    
    pic_fea2 = &img[aim*W];
    
    if(aim >= 58)
    {
      for(i = left;i < W-3;i++)
      {
        if(*(pic_fea2 + i+1) == white && *(pic_fea2 + i+3) == white)  //隔点扫描，去除杂点
          break;
      }
      w_left = i;
      
      for(i = right;i > 2;i--)
      {
        if(*(pic_fea2 + i-1) == white && *(pic_fea2 + i-3) == white)
          break;
      }
      w_right = i;
    }
    else
    {
      for(i = left;i < W-2;i++)
      {
        if(*(pic_fea2 + i+1) == white && *(pic_fea2 + i+2) == white)
          break;
      }
      w_left = i;
      
      for(i = right;i > 1;i--)
      {
        if(*(pic_fea2 + i-1) == white && *(pic_fea2 + i-2) == white)
          break;
      }
      w_right = i;
    }
    
    if(w_left >= w_right)  //防止误判（全黑情况）
    {
      left_line_temp[aim] = 0,right_line_temp[aim] = W-1;
      return 0;
    }
    
    for(i = w_left+1;i < w_right;i++)
    {
      if(*(pic_fea2 + i) == black)
      {
        black_point_temp++;
        if(*(pic_fea2 + i + 1) == white)  
        {
          if(black_point_temp > black_point)
            black_point = black_point_temp;
        }
      }
    }
    
    if(w_right - w_left < 10)   //限制？
    {
      black_point = 0;
      return black_point;
    }
    
    if(black_point == 0)
      right_line_temp[aim] = w_right,left_line_temp[aim] = w_left;
    return black_point;
}


/**************************特征函数****************************
* 函数名称：feature3
* 功能说明：求一段黑色点的中心
* 备注：用来处理障碍区
**************************************************************************/

int feature3(int aim,int left,int right)  
{
  int result = 0;
  int result_temp = 0;
  uint8_t black_point = 0,black_point_temp = 0;
  int w_left = 0,w_right = 0;
  int w = 0;
  uint8_t *pic_fea3 = NULL;
  
  pic_fea3 = &img[aim*W];
  
  if(left < 0) left = 0;
  if(right > W-1) right = W-1;
  
  if(aim>58)
  {
     for(w = left;w < W-3;w++)
     {
       if(*(pic_fea3 + w + 1) == white && *(pic_fea3 + w + 3) == white)
         break;
     }
     w_left = w;
     
     for(w = right;w > 2;w--)
     {
       if(*(pic_fea3 + w - 1) == white && *(pic_fea3 + w - 3) == white)
         break;
     }
     w_right = w;
     
  }
  else
  {
    for(w = left;w < W-2;w++)
    {
      if(*(pic_fea3 + w + 1) == white && *(pic_fea3 + w + 2) == white)
        break;
    }
    w_left = w;
    
    for(w = right;w > 1;w--)
    {
      if(*(pic_fea3 + w - 1) == white && *(pic_fea3 + w - 2) == white)
        break;
    }
    w_right = w;
  }
  
  for(w = w_left+1;w < w_right;w++)      //w_left+1到w_right-1为白色  
  {
    if(*(pic_fea3 + w) == black)                           //算法可以简便些
    {
      black_point_temp++;
      result_temp = result_temp + w;     //黑点位置累加
      
      if(*(pic_fea3 + w+1) == white)
      {
        if(black_point_temp > black_point)
        {
          black_point = black_point_temp;
          result = result_temp;
        }
      }
    }
    else black_point_temp = 0,result_temp = 0;  //中间没有黑点的情况
  }
  if(black_point > 0) 
    result = result/black_point;
  else result = W/2;                                
  return result;
}


/********************************************部分函数*******************************************************/


/*********************绝对值函数***********************
* 函数名称：ab
* 功能说明：求取整型的绝对值
* 备注：
*********************************************************/
int ab(int a)                      
{
  if(a > 0) return a;
  else return ( -a );
}


/**************************绝对值函数****************************
* 函数名称：ab_double
* 功能说明：求取double型的绝对值
* 备注：
*****************************************************************/
double ab_double(double a)                      
{
  if(a >= 0) return a;
  else return ( -a );
}


/**************************斜率函数****************************
* 函数名称：xielv
* 功能说明：得到图像起始点到目标点的斜率
* 备注：
*************************************************************/

double xielv(int aim)
{
  double result = 0;
  int count = 0,subs = 0;
  
  if(aim >= (H-1) || aim < 0)//限制
    return 0.0;
  
  for(count = H-2;count >= aim;count--)
  subs += centerline[count] - centerline[count+1];
  result = (double)(subs) / (H - (aim+1));
  return result;
    
}


/****************左丢线函数(边界)RR*************
* 函数名称：Left_lost
* 功能说明：判断某特定行是否左边丢线
* 备注：aim 目标行 start_point 特征点
从某行某特征点出开始向左丢线
*************************************************/
int Left_lost(int aim,int start_point)
{
int i;
int a=0;
if(img[W*aim+start_point]==black)
return 0;
if(img[W*aim+start_point]==white)
{
for(i=start_point;i>=0;i--)
{
if(img[W*aim+i]==white)
a=a+1;
}
}
if(a==(start_point+1))
return 1;
else
return 0;

}

/*********************右丢线函数(边界)RR*******************
* 函数名称：Right_lost
* 功能说明：判断某特定行是否右边丢线
* 备注：aim 目标行 start_point 特征点
从某行某特征点出开始向右丢线
*********************************************************/
int Right_lost(int aim,int start_point)
{
int i;
int a=0;
if(img[W*aim+start_point]==black)
return 0;
if(img[W*aim+start_point]==white)
{
for(i=start_point;i<W;i++)
{
if(img[W*aim+i]==white)
a=a+1;
}
}
if(a==(W-start_point))
return 1;
else
return 0;
}

/*******************左右两边丢线函数(边界)RR****************
* 函数名称：Both_lost
* 功能说明：判断某特定行是否左右两边都丢线
* 备注：aim 目标行 start_point 特征点
从某行某特征点出开始向右丢线
*********************************************************/
int Both_lost(int aim,int start_point)
{
int i;
int a=0,b=0;
if(img[W*aim+start_point]==black)
return 0;
for(i=start_point;i>=0;i--)
{
if(img[W*aim+i]==white)
a=a+1;
}
for(i=start_point;i<W;i++)
{
if(img[W*aim+i]==white)
b=b+1;
}
if((a+b)==(W+1))
return 1;
else
return 0;
}


/**************************寻找左边界函数RR*********************
* 函数名称：get_left
* 功能说明：得到特定行(aim)特定起始位置(start_point)的左边界
* 备注：与上述两个丢线函数可以互用
****************************************************************/

int get_left(int aim,int start_point)
{
  uint8_t *pic_left = NULL;
  int left_count = 0;
  int result = 0;
  
  pic_left = &img[aim*W];
  left_count = start_point;
  
  if(*(pic_left + left_count) == white)
  {
    for(left_count = start_point;left_count > 0;left_count--)  //注意是>0
    {
      if(*(pic_left + left_count) == black)
        break;
    }
    result = left_count;
  }
  return result;
}


/*********************寻找右边界函数RR**********************
* 函数名称：get_right
* 功能说明：得到特定行(aim)特定起始位置的右边界
* 备注：与上述两个丢线函数可以互用
**********************************************************/

int get_right(int aim,int start_point)
{
  uint8_t *pic_right = NULL;
  int right_count = 0;
  int result = W-1;
  
  pic_right = &img[aim*W];
  right_count = start_point;
  if(*(pic_right + right_count) == white)
  {
    for(right_count = start_point;right_count < W-1;right_count++)
    {
      if(*(pic_right + right_count) == black)
        break;
    }
    result = right_count;
  }
  return result;
}




/**************************函数****************************
* 函数名称：centerline_filter
* 功能说明：中线滤波
* 备注：
**************************************************************************/
void centerline_filter()
{ 
#define centerline_error_limit   (5)
#define center_deal_point     (5)
#define o_error_limit        (8)

  int count = 0,deal_count = 0;
  int error = 0;
   
  for(count = H-1;count > effective_line_temp;count--)
  {
    if(ab(centerline[count] - centerline[count - 1]) > centerline_error_limit)
    {		      
      error = centerline[count] - centerline[count - 1];	  
	  	  
      //近处中线偏移
      if(count > H - center_deal_point)
      {
        for(deal_count = count;deal_count < H;deal_count++)
          centerline[deal_count] -= error/3;
      }
      else
      {
        for(deal_count = count;deal_count < count+center_deal_point;deal_count++)
          centerline[deal_count] -= error/3;
      }
      //远处中线偏移
      if(count-1 < effective_line_temp + center_deal_point)
      {
        for(deal_count = count-1;deal_count >= effective_line_temp;deal_count--)
          centerline[deal_count] += error/3;
      }
      else
      {
        for(deal_count = count-1;deal_count > count-1-center_deal_point;deal_count--)
          centerline[deal_count] += error/3;
      }

    }
  }
}


/*********************数黑点函数RR***********************
* 函数名称：black_count
* 功能说明：得到特定行(aim)黑点个数
* 备注：
*******************************************************/

int black_count(int aim)
{
  uint8_t *pic_count = NULL;
  int count = 0,black_point = 0;
  
  pic_count = &img[aim*W];
  for(count = 0;count < W;count++)
  {
    if(*(pic_count + count) == black)
      black_point++;
  }
  return black_point;
}


/********************函数RR***********************
* 函数名称：get_black_length
* 功能说明：得到黑块长度
* 备注：
*************************************************/

int get_black_length(int aim,int point)
{
  int count = 0,mean = 0;
  int left_end = 0,right_end = W-1;
  int result = 0;
  if(aim <= 0 || img[aim*W+point] == white)
	return 0;
  for(count = point;count >= 0;count--)
  {
	if(img[aim*W+count] == black)
	  mean += count;
	else 
	{
	  left_end = count;
	  break;
	}
  }
  for(count = point+1;count < W;count++)
  {
	if(img[aim*W+count] == black)
	  mean += count;
	else
	{
	  right_end = count;
	  break;
	}
  }
  if(right_end - left_end <= 1)
	return 0;
  mean = mean/(right_end - left_end - 1);
  return  (result + get_black_length(aim-1,mean) + 1);
}


/**************************函数***********************
* 函数名称：get_black_wild
* 功能说明：得到黑块长度
* 备注：
*****************************************************/

int get_black_wild(int aim,int point)
{
  int count = 0;
  int left_end = 0,right_end = 0,black_center = 0;
  int wild = 0,wild_next = 0;
  if(aim < 0 || point < 0)
    return 0;
  if(img[aim*W+point] == white)
	return 0;
  for(count = point;count < W;count++)
	if(img[aim*W+count] == white)
	  break;
  right_end = count > (W-1) ? (W-1) : count;
  for(count = point;count >= 0;count--)
	if(img[aim*W+count] == white)
	  break;
  left_end = count < 0 ? 0 : count;
  wild = right_end - left_end;
  black_center = (right_end + left_end)/2;
  wild_next = get_black_wild(aim-1,black_center);
  if(wild > wild_next)
	return wild;
  else return wild_next;
}


/**************************两点相连函数***********************
* 函数名称：link
* 功能说明：数组两点直线连接
* 备注： //a[b]为确定值，c为目标行数，d为目标值

***********************************************************/
void link(int *aim,int start_point,int end_point,int value)                                                     
{
  int count = 0;
  int line_subs = 0,point_subs = 0;
  int point_mean = 0,point_left = 0,point_left_left = 0;
  int way = 0;

  if(start_point < 0 || end_point < 0)
    return;
  if(start_point == 0)
  {
    aim[start_point] = value;
    return;
  }
  if(start_point <= end_point)
  {
    aim[end_point] = value;
    return;
  }

  line_subs = start_point - end_point;
  point_subs = value - aim[start_point];
  point_mean = point_subs/line_subs;
  point_left = point_subs%line_subs;

  if(point_subs >= 0) way = 1;
  else way = -1;
  if(point_left != 0)
	point_left_left = ab(line_subs/point_left);

  for(count = start_point -1;count >= end_point;count--)
  {
    if(point_left_left > 0)
    {
      if(((start_point - count) % point_left_left) == 0 && point_left != 0)
      {
        aim[count] = aim[count+1] + point_mean + way;
        point_left -= way;
      }
      else aim[count] = aim[count+1] + point_mean;
    }
    else aim[count] = aim[count+1] + point_mean;
  }
}


/**************************有效行函数****************************
* 函数名称：get_effective
* 功能说明：得到最远有效行
* 备注：从left—right每一列从特定行（start_line）向前(0)扫，如果出现黑色结束
,得到这些列最前的行数。

                **注意：有效行、目标行、控制行的应用。
**************************************************************************/

#define effective_max 59

int get_effective(int left,int right,int start_line)                
{
  int h_count = 0 , effective_temp = H , effective_min = H;
  int w = 0;
  uint8_t *pic_eff = NULL;
  
  pic_eff = &img[0];
  for(w = left;w <= right;w++)
  {
    for(h_count = start_line;h_count >= 0;h_count--)
    {
      if(*(pic_eff+h_count*W+w) == black || h_count == 0)
      {
        effective_temp = h_count;
        break;
      }               
    }
    if(effective_min > effective_temp) effective_min = effective_temp;
  }
  
  if(effective_min > 59) effective_min = 59;
  if(effective_min < 0) effective_min = 0;
  
  return effective_min;
}


/***********************有效行扫描************************
* 函数名称：effective_row
* 功能说明：得到最远有效行区域
* 备注：用来判断直道和小S
*************************************************************/
int effective_row(int left,int right,int start_line)
{
	int w = 0;
	int left_line = 0, right_line = 0, wide_row = 0, wide_effective_row = 0;

    //得到边界
	for(w=left; w<=right; w++)
	{
		if(get_single_effective(w,start_line) == 1 || w==right)
			break;
	}
	left_line = w;
        
	for(w=right; w>=left; w--)
	{
		if(get_single_effective(w,start_line) == 1 || w==left)
			break;
	}	
	right_line = w;
        
	if(left_line > right_line) 
                  return -1;
        
	else 
          wide_row = right_line - left_line;
	//得到有效列宽度
	for(w=left_line; w<=right_line; w++)
	{
		if(get_single_effective(w,start_line) == 0 || w==right_line)
			break;
	}
	wide_effective_row = w - left_line;
	
	if(wide_effective_row <= wide_row)
        {
        return wide_effective_row;
        }
	else 
        {
        return -1;
        }
}

/**************************单列有效行函数RR****************************
* 函数名称：get_single_effective
* 功能说明：得到最远有效行
* 备注：得到具体某一列的有效行
**************************************************************************/

int get_single_effective(int row, int start_line)
{
	int h_count;
	uint8_t *pic_eff = NULL;
	
	pic_eff = &img[0];
	
	for(h_count = start_line;h_count >= 0;h_count--)
	{
		if(*(pic_eff+h_count*W+row) == black || h_count == 0)
			break;
	}
	if(h_count == 0)
        {
        return 1;
        }
	else return 0;
}



/***************************************************************************************************/



/*************************************************************************************************/


/********************************************普通弯道部分********************************************/

/*****************普通左弯道判断函数RR***********
* 函数名称：left_wan_judge
* 功能说明：普通左弯判断
* 备注：
*************************************************/

#define left_wan_judge_aim 30 
#define left_length 10
#define left_wan_judge_point 10  
int left_wan_judge(void)
{
if(Left_lost(left_wan_judge_aim,left_wan_judge_point)==1&&
   Both_lost(left_wan_judge_aim,left_wan_judge_point)==0&&
   Left_lost((left_wan_judge_aim+left_length),left_wan_judge_point)==1&&
     Both_lost((left_wan_judge_aim+left_length),left_wan_judge_point)==0&&
   Left_lost((left_wan_judge_aim-left_length),left_wan_judge_point)==0)
return 1;
else 
return 0;
}



/****************普通右弯道判断函数RR*************
* 函数名称：right_wan_judge
* 功能说明：普通右弯判断
* 备注：
****************************************************/

#define right_wan_judge_aim 30 
#define right_length 10
#define right_wan_judge_point 150  
int right_wan_judge(void)
{
if(Right_lost(right_wan_judge_aim,right_wan_judge_point)==1&&
   Both_lost(right_wan_judge_aim,right_wan_judge_point)==0&&
   Right_lost((right_wan_judge_aim+right_length),right_wan_judge_point)==1&&
     Both_lost((right_wan_judge_aim+right_length),right_wan_judge_point)==0&&
   Right_lost((right_wan_judge_aim-right_length),right_wan_judge_point)==0  )
return 1;
else 
return 0;
}



/**************************函数****************************
* 函数名称：cross_judge
* 功能说明：十字判断
* 备注：
**************************************************************************/

uint8_t cross_judge(int aim)
{
#define cross_judge_effective (10)                        // 十字判断的场图像有效行应当较小
#define cross_judge_aim (10)                             //十字判断的场图像中目标行应当较大
#define cross_judge_length (45)                           //近处行与远处行的判断方法

#define cross_judge_double_near (aim-12)                      //近处双边丢线时判断的行数
#define cross_judge_double_far (aim-1)                       //远处双边丢线时判断的行数

#define cross_judge_single_far1 (aim-1)                      //远处单边丢先时判断的行数
#define cross_judge_single_far2 (aim-2)
#define cross_judge_single_far3 (aim-4)
#define cross_judge_single_near1 (aim-4)
#define cross_judge_single_near2 (aim-5)
#define cross_judge_single_near3 (aim-6)

#define cross_single_lost_error  (10)
  
  if( aim < effective_line_temp + 6 || effective_line_temp > cross_judge_effective)  //十字情况有效行较远
    return 0;                                    
	                                                                                          
  if(aim < cross_judge_aim || block_flag != 0)      //行数小于10或者判断出障碍，不处理
    return 0;
  
  if(feature2(aim-5,left_line_temp[aim],right_line_temp[aim]) != 0)
    return 0;

  //十字处理  双边丢线 
  if(left_lost == 1 && right_lost == 1)             
  {
    if(cross_flag == 1 && (img[(aim-1)*W] == white || img[(aim-1)*W+W-1] == white))  //远处十字判断
      return 1;
    if(img[cross_judge_double_far*W+centerline[aim+1]] == black) //前一行中线为黑
      return 0;
    
    left_line_temp[cross_judge_double_far] = get_left(cross_judge_double_far,centerline[aim+1]);
    right_line_temp[cross_judge_double_far] = get_right(cross_judge_double_far,centerline[aim+1]);
    
    if(aim >= cross_judge_length && cross_flag == 0)         //近处十字判断
    {
      if(img[cross_judge_double_near*W+centerline[aim+1]] == white)     //判断该行前14行的情况  
      {
        left_line_temp[cross_judge_double_near] = get_left(cross_judge_double_near,centerline[aim+1]);
        right_line_temp[cross_judge_double_near] = get_right(cross_judge_double_near,centerline[aim+1]);
		
        if(left_line_temp[cross_judge_double_near] > left_line_temp[aim] ||
           right_line_temp[cross_judge_double_near] < right_line_temp[aim])
          return 0;
		  
        if((left_line_temp[cross_judge_double_near] == 0 && right_line_temp[cross_judge_double_near] != (W-1)) 
           || (right_line_temp[cross_judge_double_near] == (W-1) && left_line_temp[cross_judge_double_near] != 0))
          return 0;   //应当是两边都丢线
      }
    }
    
    if(left_line_temp[cross_judge_double_far] <= left_line_temp[aim] && right_line_temp[cross_judge_double_far] >= right_line_temp[aim])
      return 1;
  }
  
  //左线已丢 另一边距离减少
  if(left_lost == 1 && right_line_temp[aim] > right_line_temp[aim+1])      
  {
    if(cross_flag == 1 && (img[(aim-1)*W] == white || img[(aim-1)*W+W-1] == white))
      return 1;
    if(img[cross_judge_single_far3*W+centerline[aim+1]] == black)
      return 0; 
    
    right_line_temp[cross_judge_single_far1] = get_right(cross_judge_single_far1,centerline[aim+1]);  //前几行判断
    right_line_temp[cross_judge_single_far2] = get_right(cross_judge_single_far2,centerline[aim+1]);
    right_line_temp[cross_judge_single_far3] = get_right(cross_judge_single_far3,centerline[aim+1]);
    
    if(right_line_temp[cross_judge_single_far3] - right_line_temp[cross_judge_single_far1] < cross_single_lost_error
       && right_line_temp[cross_judge_single_far3] != (W-1))
      return 0;
    
    left_line_temp[cross_judge_single_far3] = get_left(cross_judge_single_far3,centerline[aim+1]);
    
    if(aim >= cross_judge_length && cross_flag == 0)                            //距离过近时可能再弯道处产生误判
    {
      if(img[W*cross_judge_single_near3+centerline[aim+1]] == black)
        return 0;
      left_line_temp[cross_judge_single_near1] = get_left(cross_judge_single_near1,centerline[aim+1]);
      left_line_temp[cross_judge_single_near2] = get_left(cross_judge_single_near2,centerline[aim+1]);
      left_line_temp[cross_judge_single_near3] = get_left(cross_judge_single_near3,centerline[aim+1]);
	  
      if(left_line_temp[cross_judge_single_near1] != 0 || left_line_temp[cross_judge_single_near2] != 0 ||
         left_line_temp[cross_judge_single_near3] != 0)
        return 0;
    }
    
    if((left_line_temp[cross_judge_single_far3] == 0 || left_lost_count > 1) &&
       right_line_temp[cross_judge_single_far1] >= right_line_temp[aim] &&
         right_line_temp[cross_judge_single_far2] >= right_line_temp[cross_judge_single_far1] &&
           right_line_temp[cross_judge_single_far3] >= right_line_temp[cross_judge_single_far2] &&
             (right_line_temp[cross_judge_single_far3] >= (right_line_temp[aim] + 5) || right_line_temp[cross_judge_single_far3]>=(W-1)))
      return 1;
  }
  
  //右线已丢 另一边距离减少
  if(right_lost == 1 && left_line_temp[aim] < left_line_temp[aim+1])       
  {
    if(cross_flag == 1 && (img[(aim-1)*W] == white || img[(aim-1)*W+W-1] == white))
      return 1;
    if(img[cross_judge_single_far3*W+centerline[aim+1]]==black)
      return 0;
    
    left_line_temp[cross_judge_single_far1]=get_left(cross_judge_single_far1,centerline[aim+1]);
    left_line_temp[cross_judge_single_far2]=get_left(cross_judge_single_far2,centerline[aim+1]);
    left_line_temp[cross_judge_single_far3]=get_left(cross_judge_single_far3,centerline[aim+1]);
    
    if(left_line_temp[cross_judge_single_far1] - left_line_temp[cross_judge_single_far3] < cross_single_lost_error
       && left_line_temp[cross_judge_single_far3] != 0)
      return 0;
    
    right_line_temp[cross_judge_single_far3]=get_right(cross_judge_single_far3,centerline[aim+1]);
    
    if(aim>=cross_judge_length&&cross_flag==0)
    {
      if(img[W*cross_judge_single_near3+centerline[aim+1]]==black)
        return 0;
      right_line_temp[cross_judge_single_near1]=get_right(cross_judge_single_near1,centerline[aim+1]);
      right_line_temp[cross_judge_single_near2]=get_right(cross_judge_single_near2,centerline[aim+1]);
      right_line_temp[cross_judge_single_near3]=get_right(cross_judge_single_near3,centerline[aim+1]);
      if(right_line_temp[cross_judge_single_near1]!=(W-1)||right_line_temp[cross_judge_single_near2]!=(W-1)||
         right_line_temp[cross_judge_single_near3]!=(W-1))
        return 0;
    }
    
    if((right_line_temp[cross_judge_single_far3] == (W-1) || right_lost_count > 1) &&
       left_line_temp[cross_judge_single_far1] <= left_line_temp[aim] &&
         left_line_temp[cross_judge_single_far2] <= left_line_temp[cross_judge_single_far1] &&
           left_line_temp[cross_judge_single_far3] <= left_line_temp[cross_judge_single_far2] &&
             (left_line_temp[cross_judge_single_far3] <= (left_line_temp[aim]-5) || left_line_temp[cross_judge_single_far3] <= 0))
      return 1;
  }

  if(right_line_temp[aim] > right_line_temp[aim+1] && left_lost_found == 1)  //
  {
    if(aim > H-6)
      return 0;
    if(left_line_temp[aim +5] == 0)
      return 1;
    else
      return 0;
  }
  if(left_line_temp[aim] < left_line_temp[aim+1] && right_lost_found == 1)
	{
    if(aim > H-6)
      return 0;
    if(right_line_temp[aim+5] == (W-1))
      return 1;
    else
      return 0;
  }
  return 0;
}


/**************************函数****************************
* 函数名称：cross_deal
* 功能说明：十字处理
* 备注：
**************************************************************************/

int cross_deal(int aim)            //先判断之前赛道斜率（偏左，偏右）再根据趋势判断寻找方向
{
#define cross_judge_error_piont (10) //当找到的赛道宽度小于原本赛道宽度+cross_point时认为找到赛道
//#define cross_way_judge_point (-10)
    
    int cross_count = 0,cross_count2 = 0;      //十字行记数
    int subs = 0,way = 0;     // 方向判断 
    int start_point = 0;       //十字开始点
    int wide_judge = 0,wide_last = 0;         //宽度判断
    int right_temp = 0,left_temp = 0;
    int cross_error_point = 0;
    uint8_t *pic_cross = NULL;
    pic_cross=&img[0];

    if(aim >= H-5)               //当行数靠前时
    {
      if(left_line_temp[aim+1] <= 0 && right_line_temp[aim+1] < (W-1)) 
        subs = -1;
      if(left_line_temp[aim+1] > 0 && right_line_temp[aim+1] >= (W-1)) 
        subs = 1;
      if(left_line_temp[aim+1] <= 0 && right_line_temp[aim+1] >= (W-1)) 
        subs = cross_way_last;       
    }
	
    if(subs == 0)                    //当之前的判断失效时
    {
      for(cross_count = H-2;cross_count > aim;cross_count--)
        subs += centerline[cross_count] - centerline[cross_count+1];
    }
    
    if(subs >= 0) way = 1;
    if(subs < 0) way = -1;
    
    cross_way_last = way;
    start_point =  centerline[aim+1];     
    wide_last = right_line_temp[aim+1] - left_line_temp[aim+1];
	
    if(wide_last < 20 && right_line_temp[aim+1] != (W-1) && left_line_temp[aim+1] != 0) 
      return -1;                     //宽度太小时大多是图像差
    
    
    for(cross_count = aim-3;cross_count > 0;cross_count--)              //在判断函数中有条件aim大于5
    {
      while(*(pic_cross + cross_count*W+start_point) == black)         //找到白色区域
      {
        start_point += way;
        if(start_point <= 0 || start_point >= (W-1))
          return -1;
      }
      
      right_temp = get_right(cross_count,start_point),left_temp = get_left(cross_count,start_point);
      wide_judge = right_temp - left_temp;
      
     
      if(wide_judge <= (wide_nature[cross_count] + cross_judge_error_piont) && wide_judge <= wide_last)
      {
        if(get_effective(left_temp,right_temp,cross_count) > (effective_line_temp+1))
          continue;
        
        if(right_temp != (W-1) && left_temp != 0)
          centerline[cross_count] = (right_temp+left_temp)/2;
        if(right_temp == (W-1) && left_temp != 0)
          centerline[cross_count] = (left_temp+wide_nature[cross_count]/2) > (W-1) ? (W-1) : (left_temp+wide_nature[cross_count]/2);
        if(right_temp != (W-1) && left_temp == 0)
          centerline[cross_count] = (right_temp-wide_nature[cross_count]/2) < 0 ? 0 : (right_temp-wide_nature[cross_count]/2);
        if(right_temp == (W-1) && left_temp == 0)
          continue;
        
        right_line_temp[cross_count] = right_temp,left_line_temp[cross_count] = left_temp;
        //首行双边丢线时可能出现 中点判定不准
        if(left_line_temp[H-1] <= 0 && right_line_temp[H-1] >= (W-1))
        {
          cross_error_point = centerline[H-1] - (centerline[H-1] + centerline[cross_count])/2;
          for(cross_count2 = H-1;cross_count2 >= aim+1;cross_count2--)
          {
            centerline[cross_count2] -= cross_error_point;
            if(centerline[cross_count2] < 0) centerline[cross_count2] = 0;
            if(centerline[cross_count2] >= W) centerline[cross_count2] = W-1;
          }
        }
        link(centerline,aim+1,cross_count,centerline[cross_count]);
        link(right_line_temp,aim+1,cross_count,right_line_temp[cross_count]);
        link(left_line_temp,aim+1,cross_count,left_line_temp[cross_count]);
        return cross_count;
        
      }
    }
    return -1;
}


/**************************环路判断****************************
* 函数名称：huanlu_judge
* 功能说明：识别环路
* 备注：
**************************************************************/

uint8_t huanlu_judge(int aim)
{
	#define huanlu_judge_effective (20)                        //十字判断的场图像有效行应当较小
	#define huanlu_judge_aim (10)                             //十字判断的场图像中目标行应当较大
    #define huanlu_judge_length (30)                           //近处行与远处行的判断方法
    #define huanlu_judge_double_near (aim-10)                      //近处丢线时判断的行数
    #define huanlu_judge_double_far (aim-3)                       //远处丢线时判断的行数
	#define black_start_line (5)
	#define black_length (3)
	
	int huanlu_start;  //环路起始点
	int black_start, black_end;  //定义黑块起始 结束点
	int huanlu_count = 0;  
	
  //有效行判断 先注释
  if( /*effective_line_temp > huanlu_judge_effective ||*/ aim < effective_line_temp+2)  
    return 0;                                    
	                                                                                          
  if(aim < huanlu_judge_aim)      //行数小于12不处理
    return 0;
  
//  if(feature2(aim-5,left_line_temp[aim],right_line_temp[aim]) != 0)
//    return 0;

  //环路处理
  if(((left_line_temp[aim] < left_line_temp[aim+1]+1 && right_line_temp[aim] > right_line_temp[aim+1]+2)
  || (left_line_temp[aim] < left_line_temp[aim+1]-2 && right_line_temp[aim] > right_line_temp[aim+1]-1))
  && (road_wide_temp[aim] >= road_wide_temp[aim+1]+4))             
  {
     
    if(img[huanlu_judge_double_far*W+centerline[aim+1]] == black) //前一行中线为黑
      return 0;
    
    left_line_temp[huanlu_judge_double_far] = get_left(huanlu_judge_double_far,centerline[aim+1]);
    right_line_temp[huanlu_judge_double_far] = get_right(huanlu_judge_double_far,centerline[aim+1]);
    road_wide_temp[huanlu_judge_double_far] = right_line_temp[huanlu_judge_double_far]-left_line_temp[huanlu_judge_double_far];
/*  
    if(aim > huanlu_judge_length)         //近处环路判断
    {
      if(img[huanlu_judge_double_near*W+centerline[aim+1]] == white)     //判断该行前14行的情况  
      {
        left_line_temp[huanlu_judge_double_near] = get_left(huanlu_judge_double_near,centerline[aim+1]);
        right_line_temp[huanlu_judge_double_near] = get_right(huanlu_judge_double_near,centerline[aim+1]);
		
        if(left_line_temp[huanlu_judge_double_near] > left_line_temp[aim] ||
           right_line_temp[huanlu_judge_double_near] < right_line_temp[aim])
          return 0;
		  
        if(left_line_temp[huanlu_judge_double_near] == 0 && right_line_temp[huanlu_judge_double_near] == W-1)
          return 0;   //应当是两边都丢线
      }
    }
*/	
	if((left_line_temp[huanlu_judge_double_far] < left_line_temp[aim] && right_line_temp[huanlu_judge_double_far] > right_line_temp[aim]) || road_wide_temp[huanlu_judge_double_far] > road_wide_temp[aim])
		{
            //bug_flag1 = 1;
			if(left_line_temp[huanlu_judge_double_far] <= 0 && right_line_temp[huanlu_judge_double_far] >= W-1)
				return 0;
			huanlu_start = aim+1;  //起始点为该行上行  effective_line_temp条件可能要改
            //bug_flag2 = huanlu_start;
			for(huanlu_count = huanlu_start; huanlu_count > effective_line_temp+1; huanlu_count--)
			{
				if(img[huanlu_count*W+centerline[aim+1]] == black && img[(huanlu_count-1)*W+centerline[aim+1]] == black)
					break;
			}
			black_start = huanlu_count-1;  //黑块起始点
			if(black_start < black_start_line) 
				return 0;
                        
			//huanlu_flag = 1;
/*			
			for(huanlu_count = black_start; huanlu_count > effective_line_temp; huanlu_count--)
			{
				if(img[huanlu_count*W+centerline[aim+1]] == white)
					break;
			}
			black_end = huanlu_count;  //黑块终点
*/			
            if(get_black_length(black_start,centerline[aim+1]) > black_length || get_black_length(black_start-1,centerline[aim+1]) > black_length)
			{
				if(get_black_wild(black_start,centerline[aim+1]) > 20 && (get_black_wild(black_start-1,centerline[aim+1]) >= get_black_wild(black_start,centerline[aim+1])) && 
				(get_black_wild(black_start-2,centerline[aim+1]) >= get_black_wild(black_start,centerline[aim+1])) )
				//huanlu_flag = 1;  //环路标志置1
                                  bug_flag1 = 1;
				return 1;
				
				//return 0;
			}
		}
    
  }
  return 0;
}


/**************************环路判断****************************
* 函数名称：huanlu_deal
* 功能说明：环路处理
* 备注：
**************************************************************/

int huanlu_deal(int aim)
{
	#define huanlu_judge_error_piont (20) //当找到的赛道宽度小于原本赛道宽度+cross_point时认为找到赛道
    
    int huanlu_count = 0,huanlu_count2 = 0;      //十字行记数
    int way = 0 , subs = 0;     // 方向判断 
    int start_point = 0;       //十字开始点
    int wide_judge = 0,wide_last = 0;         //宽度判断
    int right_temp = 0,left_temp = 0;
//    int huanlu_error_point = 0;
    uint8_t *pic_huanlu = NULL;
    pic_huanlu=&img[0];
	
	//wai代表找线方向
    for(huanlu_count = H-2;huanlu_count > aim;huanlu_count--)
        subs += centerline[huanlu_count] - centerline[huanlu_count+1];
    
//    if(subs >= 15) way = -1;
//	else way = 1;
	
    way = 1;
    cross_way_last = way;
	
    start_point =  centerline[aim+1];     
    wide_last = right_line_temp[aim+1] - left_line_temp[aim+1];
    
    for(huanlu_count = aim-2; huanlu_count > 0; huanlu_count--)              //在判断函数中有条件aim大于5
    {
      while(*(pic_huanlu + huanlu_count*W+start_point) == black)         //找到白色区域
      {
        start_point += way;
        if(start_point <= 0 || start_point >= (W-1))
          return -1;
      }
      
      right_temp = get_right(huanlu_count,start_point),left_temp = get_left(huanlu_count,start_point);
      wide_judge = right_temp - left_temp;
           
      if( wide_judge <= (wide_nature[huanlu_count] + huanlu_judge_error_piont) &&  wide_judge <= wide_last)
      {
        //if(get_effective(left_temp,right_temp,huanlu_count) > (effective_line_temp+1))
          //continue;
/*        
        if(right_temp != (W-1) && left_temp != 0)
          centerline[huanlu_count] = (right_temp+left_temp)/2;
        if(right_temp == (W-1) && left_temp != 0)
          centerline[huanlu_count] = (left_temp+wide_nature[huanlu_count]/2) > (W-1) ? (W-1) : (left_temp+wide_nature[huanlu_count]/2);
        if(right_temp != (W-1) && left_temp == 0)
          centerline[huanlu_count] = (right_temp-wide_nature[huanlu_count]/2) < 0 ? 0 : (right_temp-wide_nature[huanlu_count]/2);
        if(right_temp == (W-1) && left_temp == 0)
          continue;
*/        
        if(right_temp == (W-1) && left_temp == 0)
			continue;
		centerline[huanlu_count] = (right_temp+left_temp)/2;
		
        right_line_temp[huanlu_count] = right_temp, left_line_temp[huanlu_count] = left_temp;
		
/*        //首行双边丢线时可能出现 中点判定不准
        if(left_line_temp[H-1] <= 0 && right_line_temp[H-1] >= (W-1))
        {
          huanlu_error_point = centerline[H-1] - (centerline[H-1] + centerline[huanlu_count])/2;
          for(huanlu_count2 = H-1;huanlu_count2 >= aim+1;huanlu_count2--)
          {
            centerline[huanlu_count2] -= cross_error_point;
            if(centerline[huanlu_count2] < 0) centerline[huanlu_count2] = 0;
            if(centerline[huanlu_count2] >= W) centerline[huanlu_count2] = W-1;
          }
        }
*/		
        link(centerline,aim+1,huanlu_count,centerline[huanlu_count]);
        link(right_line_temp,aim+1,huanlu_count,right_line_temp[huanlu_count]);
        link(left_line_temp,aim+1,huanlu_count,left_line_temp[huanlu_count]);
        return huanlu_count;
        
      }
    }
    return -1;
}

/**************************起跑线检测****************************
* 函数名称：starting_line
* 功能说明：十字处理
* 备注：
**************************************************************************/
/*
int end_count_1=0; 

void starting_line(void)
{
int i=0;
int j=0;
int row_temp=0;	//记录一行黑点数，左边
int starline_row=0;
int starline_stop=0;

for (i=59;i>30;i--)	//计算左边黑点数
{
	row_temp=0;	//计数器及时清零
	for (j=left_line_temp[i]+1;j<right_line_temp[i];j++) 	//左线计算到右线，不包括左右边界
	{
		if (img[W*i+j]==black)
		{
			row_temp++;
		}

	}
	if (row_temp>18*2 && row_temp<30*2)
	{
		starline_row++;
	}
}
if (starline_row>=2&&starline_row<=5)	//说明是起跑线
{
	starline_stop=1;
	
}
else 
{
	starline_stop=0;
}


if (starline_stop==1 && cross_flag == 0 && block_left_flag == 0 && block_left_flag == 0)
{
  if(end_count == 1)
  {
//  gpio_init(led_1,GPO,0);          //加个延时
//  gpio_init(BUZZER,GPO,0);
  
  speed_set=51;
  speed_set=52;
  speed_set=53;
  speed_set=54;
  speed_set=55;
//  speed_set=0;
//  end_count_1++;

//  if (end_count_1>10)
//    {

          while(1)
      {
        SD5_init();
        DELAY_MS(100);
        speed_set=0; 
      }
      
//    }

//  gpio_init(MOTOR1_IO,GPO,0);
  } 
  else;
  
  end_count = 0;
  
}
else; 
//{
//	gpio_init(led_1,GPO,1);
//        gpio_init(BUZZER,GPO,1);
//}

}
*/
