#include "common.h"
#include "picture.h"

/*********************************************���߲���RR******************************************************/

//�����ⲿ����
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

//�ⲿ����

//Center_line�ж���
int variance = 0;
#define second_scan_line  (49)                //�����кڵ����ʱ�Ӵ��п�ʼɨ�裺45

int effective_flag = 0;                       //��Ч�б�־�����ڼ���
int effective_line_temp = 0;                  //��Ч���ݴ�
int effective_line = 0,last_effective = 0;    //�ó���Ч�У��ϳ���Ч��

int centerline_save[H];                       //�洢�ϳ�ÿ������
int centerline_last = W/2;                    //�洢�ϳ�H-1������

int road_wide_temp[H];                            //��¼�������
int right_line_temp[H] , left_line_temp[H];   //�������ݴ�
int right_lost = 0 , left_lost = 0;               //���߱�־     
int right_lost_found = 0 , left_lost_found = 0;   //�����һ�
int right_lost_count = 0 , left_lost_count = 0;   //���߼���
     
int wide_nature[H] =                          //ֱ��0-59��������ȣ����ڲ���
{20,22,24,28,30,32,34,38,41,43,45,47,49,51,53,55,58,61,63,65,67,69,71,73,74,76,78,80,82,84,
86,87,89,91,92,94,95,97,98,99,101,102,104,105,107,108,110,111,112,114,114,116,118,119,
116,121,122,123,124,125};  
  
//int po_wide_nature[H] = [77,77,78,80,80,82,82,84,84,86,87,88,89,90,91]  //�µ���������


//ʮ��

#define cross_judge_count (3)
#define cross_record_line  (25)                //ʮ���ñ�־λ����
#define cross_first_lost_add (8)              //ʮ�����ж��߲�����
#define first_lost_add (10)                    //��ͨ���߲�����
#define featureline_add (1)                     //��ͨ����

int cross_flag = 0;                     //ʮ�ֱ�־λ
int cross_way_last = 0;                 //ʮ����һ��ͼ������߷���

//��·
int huanlu_flag = 0;   //��·��־λ
int huanlu_way_last = 0;  //��·���߷���

//�ϰ�

int block_flag = 0 , block_left_flag = 0 , block_right_flag = 0;  //�ϰ�����־λ
int block_line = 0;                        //�ϰ�������
int block_count = 0;                       //�ϰ�����

#define block_judge_count (4) 
#define block_deal_point (10)                //�ϰ��������ҼӼ�ֵ
#define block_deal_photo_count (3)

//����

#define lost_count_judge (20)                      //���б߽��������ʱ 7��Ϊ����
#define lost_found_judge_line (5)                 //�����������ڲ������ҵ�ʱ��Ϊ�����ҵ���
 
int last_error;
int big_right_turn_flag = 0,big_left_turn_flag = 0;

//�µ�
//int podao_count = 0;
//int podao_flag_up = 0;                            //���±�־
//int podao_flag_down = 0;                          //���±�־

//�յ���ʶ��
//int end_flag = 0;                                //�յ��־
//int end_count = 0;
//extern int speed_set;

/**************************�����㷨****************************
* �������ƣ�Center_line
* ����˵���������㷨
* ��ע��
�����ϳ�ͼ��
1.��ʼ����
2.��Ч�У�
��ɨ��һ�Σ��õ�effective_line_temp��

3.�����ϳ�����

**************************************************************/
  //������
int bug_flag1 = 0;
int bug_flag2 = 0;

void Center_line()
{
  int count = 0;
  
  effective_flag++;     
  feature_init(); //ͼ���ʼ��
	
  if(black_count(H-1)<(W-wide_nature[H-1]/3))     //��H-1�кڵ��ٵ���� 160 - 70
  {
    first_scan(H-1);                           
    effective_line_temp = get_effective(left_line_temp[H-1],right_line_temp[H-1],H-1);    
    featureline(H-2);
    
/*    //��Ч��֮ǰ�����ߴ���
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
    
/*    //СS����һ ע��
    if((effective_row(left_line_temp[H-1],right_line_temp[H-1],H-1) > 30) && effective_line_temp <= 10 
       && cross_flag != 1 && (left_lost_count < 5 && right_lost_count < 5))
   {
    link(centerline,H-1,effective_line_temp,centerline[effective_line_temp]);
    }
*/     
    
/*    //��������ƽ��ֵ СS�����
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

  else                                           //������кڵ���࣬��ȡsecond_scan_line��.
    if(black_count(second_scan_line) < (W-wide_nature[second_scan_line]/3))
    {
      first_scan(second_scan_line);                           
      effective_line_temp = get_effective(left_line_temp[second_scan_line],right_line_temp[second_scan_line],second_scan_line);
      featureline(second_scan_line-1);
/*      
      //��Ч��֮ǰ�����ߴ���
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
/*//СS����һ ע��
      if((effective_row(left_line_temp[second_scan_line],right_line_temp[second_scan_line],second_scan_line) > 30) && effective_line_temp <= 10  
       && cross_flag != 1 && (left_lost_count < 5 && right_lost_count < 5))
    {
    link(centerline,second_scan_line,effective_line_temp,centerline[effective_line_temp]);
    }
*/
/*    //��������ƽ��ֵ СS�����
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
      //��֮ǰ����������ɨ����һ��
      for(count = H-1;count > second_scan_line;count--)
      {
        left_line_temp[count] = left_line_temp[second_scan_line];
        right_line_temp[count] = right_line_temp[second_scan_line];
        centerline[count] = centerline[second_scan_line];
      }
    }
	
    else                                    //ͼ��δ�жϳ������
    {                                       
      centerline[H-1] = centerline_last;    //����ȡ�ϳ�����
      effective_line_temp = H-1;
      return;
    }
	
  
  
/* 
  //�ϰ�������
  if(block_count >= 0 && block_left_flag == 1)
  {
    block_count--;
    if(block_count == 0) 
      block_left_flag=0;
    
      for(count = H-1;count > effective_line_temp;count--)
      { 
        centerline[count] = centerline[count] + block_deal_point;
        //�޷�
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
  
  if((block_count>=0 && block_right_flag==1)||(block_count >= 0 && block_left_flag == 1))  //���������
    gpio_init(PTC11,GPO,0);
  else 
    gpio_init(PTC11,GPO,1);
*/
/*  
  //�µ�ʶ��
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
  
  //�洢��Ч��
  if(effective_flag >= 1)                       
  {
    last_effective = effective_line;
    effective_line = effective_line_temp;  //�ⲿʹ����Ч��
    effective_flag = 0;
  }
  
  //��¼�ϳ�����
  for(count = 0;count < H;count++)                    
    centerline_save[count] = centerline[count];
  
  //����
  if(effective_line_temp > 55)
  {
    for(count = 0;count < H;count++)
      centerline[count] = centerline_save[count];
    effective_line_temp = effective_line;               //��Ч��Ϊ�ϳ���Ч��
  }
    
}


/**************************���߳�ʼ������**************************
* �������ƣ�feature_init
* ����˵�����ϳ�ͼ���ʼ�� 
* ��ע����¼��һ��ͼ��
        �����ж�����
********************************************************/

void feature_init()
{
  int init_count;
//��¼�ϳ�H-1������
  centerline_last = centerline[H-1]; 
  
//�ϳ�ͼ���ʼ��  ����־����  
  for(init_count = 0;init_count < H;init_count++)
  {
    right_line_temp[init_count] = W-1;                     //���������ݴ�
    left_line_temp[init_count] = 0;
    centerline[init_count] = W/2;
  }

  block_line = 0;                                          //λ������
//  if(block_flag > 0) block_flag--;
  if(cross_flag > 0) cross_flag--;  //ʮ��
  if(huanlu_flag > 0) huanlu_flag--;//��·  
  
  left_lost_found = 0, right_lost_found = 0;                //��������
  right_lost = 0, left_lost = 0;
  right_lost_count = 0, left_lost_count = 0;
  
  variance = 0;  //��������
}


/**************************����ɨ�躯��****************************
* �������ƣ�first_scan
* ����˵������һ��ɨ��
* ��ע��ȷ����ǰ���һ�е�����
*****************************************************************/

void first_scan(int aim)
{
  int first_lost_add_point = 10;                             //������
  
  if(block_flag == 0 && block_count == 0)                           //�����ϰ����ϰ�����
  {
    if(img[aim*W + centerline_last] == white)
    {
      if(cross_flag == 1)                                           //����
        first_lost_add_point = cross_first_lost_add;
      else 
        first_lost_add_point = first_lost_add;  
      
      left_line_temp[aim] = get_left(aim,centerline_last);
      right_line_temp[aim] = get_right(aim,centerline_last);
	  //��һ���������
      road_wide_temp[aim] = get_right(aim,centerline_last) - get_left(aim,centerline_last);    //��¼�������

	  //δ����
      if(left_line_temp[aim] > 0 && right_line_temp[aim] < (W-1)) 
      {
        centerline[aim] = (left_line_temp[aim] + right_line_temp[aim])/2;
      }
      //����
      if(left_line_temp[aim] == 0 && right_line_temp[aim] < (W-1)) 
      {
        centerline[aim] = (right_line_temp[aim] - wide_nature[aim]/2 - first_lost_add_point) < 0 ? 0 : (right_line_temp[aim]-wide_nature[aim]/2-first_lost_add_point);
        left_lost = 1, left_lost_count++;
        return;
      }
      //�Ҷ���
      if(left_line_temp[aim] > 0 && right_line_temp[aim] == (W-1))  
      {
        centerline[aim] = (left_line_temp[aim] + wide_nature[aim]/2 + first_lost_add_point) > (W-1) ? (W-1) : (left_line_temp[aim]+wide_nature[aim]/2+first_lost_add_point);
        right_lost = 1, right_lost_count++;
        return;
      }
	  //���߾�����
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
//  second_scan(aim-30);    //�ϰ��������,
}


/**************************����ɨ�躯��****************************
* �������ƣ�second_scan
* ����˵������һ��ɨ��
* ��ע���ϰ����е����⣬�������У�Ҫ�ġ�
**************************************************************/

#define head_block_judge_point (25)  //zhangai
#define block_length_max (45)
#define end_judge_point (20)

void second_scan(int aim)
{
  int black_point = 0, black_point1 = 0 , black_point2 = 0 , black_point3 = 0; 
  int block_left = 0, block_right = 0;                         //�ϰ�����
  int block_left_line_temp = 0, block_right_line_temp = 0;      //�ϰ��������ݴ�
  int first_lost_add_point = 0;                       //������
    
  black_point = feature2(aim,0,W-1);
  
  if(cross_flag == 1) 
    first_lost_add_point = cross_first_lost_add;
  else 
    first_lost_add_point = first_lost_add;
  
  if(black_point != 0)                      //�ϰ����
  {
    black_point1 = feature2(aim-1,0,W-1);                       //���жϼ�����ĺڰ����
    black_point2 = feature2(aim-2,0,W-1);
    black_point3 = feature2(aim-4,0,W-1);
    centerline[aim] = feature3(aim,0,W-1);                      //�е�λ�ں��ߡ��ڿ�����
    
    if((black_point1 >= head_block_judge_point || black_point2 >= head_block_judge_point ||         //�ж��ϰ�
        black_point3 >= head_block_judge_point)&& black_point2 != 0 && black_point3 != 0 && 
       cross_flag == 0 && get_black_length(aim,centerline[aim]) < block_length_max)            //cross_flag == 0��ʮ���������Ӱ���ϰ��ж�
    {   
      for(block_left = centerline[aim];block_left >= 0;block_left--)                         //�ж��ϰ�λ��
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
      
      if(ab(block_left_line_temp-centerline[aim]) >= ab(block_right_line_temp-centerline[aim])) //�ϰ����Ҳ�
      {
        block_right_flag = 1;block_left_flag = 0;
        left_line_temp[aim] = block_left_line_temp;
        right_line_temp[aim] = get_right(aim,block_left);
      }
      else                                          //�ϰ������
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
        if((get_right(aim-1,centerline[aim]) - get_left(aim-1,centerline[aim])) > wide_nature[aim]*4/5)   //�ж�ʧ��
        {
          block_right_flag = 0,block_left_flag = 0;
        }
        else
        {
          block_flag = block_judge_count;        //5
          block_line = aim; 
          block_count = block_deal_photo_count;  //10        //�ϰ���־
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

/**************************����ɨ�躯��********************
* �������ƣ�end_judge
* ����˵��:�յ��߼��
* ��ע��
       ɨ���30��
***********************************************************/

/*
void end_judge(int aim)
{
  int black_point = 0, black_point1 = 0 , black_point2 = 0 , black_point3 = 0; 
  int first_lost_add_point = 0;                       //������
    
  black_point = feature2(aim,0,W-1);
    
  if(black_point != 0)                      //�ϰ����
  {
    black_point1 = feature2(aim-1,0,W-1);                       //���жϼ�����ĺڰ����
    black_point2 = feature2(aim-2,0,W-1);
    black_point3 = feature2(aim-6,0,W-1);
    centerline[aim] = feature3(aim,0,W-1);                      //�е�λ�ں��ߡ��ڿ�����
    
    if((black_point1 >= head_block_judge_point || black_point2 >= head_block_judge_point ||         //�ж��ϰ�
        black_point3 >= head_block_judge_point)&& black_point2 != 0 && black_point3 != 0 && 
       cross_flag == 0 && get_black_length(aim,centerline[aim]) < block_length_max)            //cross_flag == 0��ʮ���������Ӱ���ϰ��ж�
    {   
      for(block_left = centerline[aim];block_left >= 0;block_left--)                         //�ж��ϰ�λ��
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
      
      if(ab(block_left_line_temp-centerline[aim]) >= ab(block_right_line_temp-centerline[aim])) //�ϰ����Ҳ�
      {
        block_right_flag = 1;block_left_flag = 0;
        left_line_temp[aim] = block_left_line_temp;
        right_line_temp[aim] = get_right(aim,block_left);
      }
      else                                          //�ϰ������
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
        if((get_right(aim-1,centerline[aim]) - get_left(aim-1,centerline[aim])) > wide_nature[aim]*4/5)   //�ж�ʧ��
        {
          block_right_flag = 0,block_left_flag = 0;
        }
        else
        {
          block_flag = block_judge_count;        //5
          block_line = aim; 
          block_count = block_deal_photo_count;  //10        //�ϰ���־
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
            
/**************************����ɨ�躯��********************
* �������ƣ�featureline
* ����˵������������ȡ
* ��ע��
       ��H-1����ǰɨ�裬ע����������
***********************************************************/

void featureline(int start_line)
{
  int feature_count = 0;
  int cross_line = 0;  //�����м��� ʮ����ɼ���
  int huanlu_line = 0;
  
  if(start_line < 0)
    return;
  
  for(feature_count = start_line;feature_count >= effective_line_temp;feature_count--) 
  {
    if(feature1(feature_count,centerline[feature_count+1]) == 0)
      continue;                                                  //δ�ҵ��е����
    
    if(left_lost == 0)  //�ж����Ҷ������  �޸�
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
        if(left_lost_count > lost_found_judge_line && left_line_temp[H-1] > 0)  //���߶���5��
          left_lost_found = 1;  //���ҵ���          
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
          right_lost_found = 1;  //�ҵ���
        right_lost = 0,right_lost_count = 0;
      }
      else right_lost = 1,right_lost_count++;
    }
	//��¼���п��
	road_wide_temp[feature_count] = right_line_temp[feature_count] - left_line_temp[feature_count];
    
    //��һ�����Ҿ�δ����
    if(left_lost == 0 && right_lost == 0)   
    {
      centerline[feature_count] = (left_line_temp[feature_count] + right_line_temp[feature_count])/2;
	  
	  //�жϻ�·���
	  if( huanlu_judge(feature_count) == 1 && feature_count > effective_line_temp)
	  {
		 
            huanlu_line = huanlu_deal(feature_count);
		  if(huanlu_line < 0)
		  {
			  effective_line_temp = feature_count+1;
			  return;
		  }
		  
		  huanlu_flag = 1;  //��·��־��1
		  featureline(huanlu_line-1);
		  break;
	  }
	  left_lost_count = 0 , right_lost_count = 0;
	  
    }
    
    //���ֶ������   	  
    if((left_lost == 1 || right_lost == 1 || right_lost_found == 1 || left_lost_found ==1) )  //ʮ��
    {
    if( cross_judge(feature_count) == 1 && feature_count > effective_line_temp)   //������Ч��                            
    {
    left_lost_count = 0,right_lost_count = 0;        //���Ҷ�������
    
    cross_line = cross_deal(feature_count);
    
    if(cross_line < 0)           //ʮ���ж�
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
    
    	
    //����  ����Ҫ�޸�!!!
    if(left_lost == 1 && right_lost == 0)
      centerline[feature_count] = centerline[feature_count+1] + (right_line_temp[feature_count] - right_line_temp[feature_count+1] + featureline_add);  //featureline_addΪ���ߵ���
    if(left_lost == 0 && right_lost == 1)	 
      centerline[feature_count] = centerline[feature_count+1] + (left_line_temp[feature_count] - left_line_temp[feature_count+1] - featureline_add);
    if(left_lost == 1 && right_lost == 1)  
      centerline[feature_count] = centerline[feature_count+1];
    
    //�޷�
    if(centerline[feature_count] < 0) centerline[feature_count] = 0;
    if(centerline[feature_count] > (W-1)) centerline[feature_count] = W-1;
  }
/*    //���䴦��
      if(big_turn() == 0)   
      {
        centerline_filter();  //�˲�������ֹ���ⶪ��
        big_right_turn_flag = 0;
        big_left_turn_flag = 0;
      }
      else;    
*/  
}
  

/**************************�����жϺ���****************************
* �������ƣ�big_turn
* ����˵�����������
* ��ע��
**************************************************************************/

uint8_t big_turn()
{
#define big_turn_judge_effective_far (14)        //������Ч���ж���Զ  ���۲� ����Ǻܺ����
#define big_turn_judge_far     (25)          //������Ч�м�����ֵ  
  
#define big_turn_count_far    (6)           //����Զ��ʱ�ж�����
#define big_turn_count_near   (12)          //�������ʱ�ж�����
#define big_turn_limit_judge   (effective_line_temp+3)      //������Ч��ǰ�ж�
#define big_turn_break_line   (6)             //���伫���ж���������
  
  
  int count = 0,left_count = 0,right_count = 0,left_break = 0,right_break = 0;
  int left_start = 0,right_start = 0;       //��ʼ ����� �ҿ�ʼ �ҽ���
  int big_turn_judge_line = 0;
  int big_turn_judge_count = 0;             //�ж���������Ҷ��ߵ�����
  
  if(effective_line_temp < big_turn_judge_effective_far)           //��Ч��̫Сʱ�����жϴ���14
    return 0;
  
  if(effective_line_temp < big_turn_judge_far)                 //�����Ч��<25��������>4���ж�Ϊ���
    big_turn_judge_count = big_turn_count_far;                  //����Ч�п�ǰ�������ж�Ϊ8
  else big_turn_judge_count = big_turn_count_near;
  
  for(count = H-1;count >= effective_line_temp;count--)
  {
    if(left_line_temp[count] == 0) 
      left_count++,left_break = 0;           //left_count����
    else left_break++;
    
    if(left_count == 1)                         //��¼����ʼ
      left_start = count;
    if(left_break > big_turn_break_line) 
      left_count = 0,left_start = 0;        //ȥ���ӵ�
    
    
    if(right_line_temp[count] == W-1) 
      right_count++,right_break = 0;      //�ұ��ж�
    else right_break++;
    
    if(right_count == 1) 
      right_start = count;
    if(right_break > big_turn_break_line)       //���伫������
      right_count = 0,right_start = 0;
  }
  
  
  if(big_turn_limit_judge < H)                     //��Ч��+3����Ч�У��ж�
    big_turn_judge_line = big_turn_limit_judge;
  else 
    big_turn_judge_line = effective_line_temp;
  
  
  if(left_count >= big_turn_judge_count && left_start > 0)  //����
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
    if(right_count >= big_turn_judge_count && right_start > 0)     //����
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

/**************************ǰ���к���****************************
* �������ƣ�lost_deal
* ����˵����ǰ���ж�������
* ��ע�������ǰ���ж��ߣ�Ȼ���ҵ������߽磬��H-1 ��������
������������
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



/**************************���ߺ���****************************
* �������ƣ�feature1
* ����˵�����õ�˫���ض����ض������߼������
* ��ע����ȷ����ɫ�㣬�Ӱ׵���������
**************************************************************************/
uint8_t feature1(int aim,int start_point)
{
  uint8_t result = 0;
  int w = 0;                         //����������int
  int w_right = 0,w_left = 0,wide = 0;           //��һ����λ��
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
      result = 0,effective_line_temp = aim + 1;   //�жϸ��л���ȫ��
      if(effective_line_temp > (H-1)) effective_line_temp = H-1;
      return result;
    }
  }
  w = start_point;  //�ҵ��׵�
  
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



/**************************���ߺ���****************************
* �������ƣ�feature2
* ����˵�����ض����ض���֮�������ϵĺڵ����
* ��ע����������2
***********************************************************/

uint8_t feature2(int aim,int left,int right)         
{
    int i = 0;                         //����������int
    int w_right = 0,w_left = 0;           //��һ����λ��
    uint8_t black_point = 0,black_point_temp = 0;
    uint8_t *pic_fea2 = NULL;
    
    pic_fea2 = &img[aim*W];
    
    if(aim >= 58)
    {
      for(i = left;i < W-3;i++)
      {
        if(*(pic_fea2 + i+1) == white && *(pic_fea2 + i+3) == white)  //����ɨ�裬ȥ���ӵ�
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
    
    if(w_left >= w_right)  //��ֹ���У�ȫ�������
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
    
    if(w_right - w_left < 10)   //���ƣ�
    {
      black_point = 0;
      return black_point;
    }
    
    if(black_point == 0)
      right_line_temp[aim] = w_right,left_line_temp[aim] = w_left;
    return black_point;
}


/**************************��������****************************
* �������ƣ�feature3
* ����˵������һ�κ�ɫ�������
* ��ע�����������ϰ���
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
  
  for(w = w_left+1;w < w_right;w++)      //w_left+1��w_right-1Ϊ��ɫ  
  {
    if(*(pic_fea3 + w) == black)                           //�㷨���Լ��Щ
    {
      black_point_temp++;
      result_temp = result_temp + w;     //�ڵ�λ���ۼ�
      
      if(*(pic_fea3 + w+1) == white)
      {
        if(black_point_temp > black_point)
        {
          black_point = black_point_temp;
          result = result_temp;
        }
      }
    }
    else black_point_temp = 0,result_temp = 0;  //�м�û�кڵ�����
  }
  if(black_point > 0) 
    result = result/black_point;
  else result = W/2;                                
  return result;
}


/********************************************���ֺ���*******************************************************/


/*********************����ֵ����***********************
* �������ƣ�ab
* ����˵������ȡ���͵ľ���ֵ
* ��ע��
*********************************************************/
int ab(int a)                      
{
  if(a > 0) return a;
  else return ( -a );
}


/**************************����ֵ����****************************
* �������ƣ�ab_double
* ����˵������ȡdouble�͵ľ���ֵ
* ��ע��
*****************************************************************/
double ab_double(double a)                      
{
  if(a >= 0) return a;
  else return ( -a );
}


/**************************б�ʺ���****************************
* �������ƣ�xielv
* ����˵�����õ�ͼ����ʼ�㵽Ŀ����б��
* ��ע��
*************************************************************/

double xielv(int aim)
{
  double result = 0;
  int count = 0,subs = 0;
  
  if(aim >= (H-1) || aim < 0)//����
    return 0.0;
  
  for(count = H-2;count >= aim;count--)
  subs += centerline[count] - centerline[count+1];
  result = (double)(subs) / (H - (aim+1));
  return result;
    
}


/****************���ߺ���(�߽�)RR*************
* �������ƣ�Left_lost
* ����˵�����ж�ĳ�ض����Ƿ���߶���
* ��ע��aim Ŀ���� start_point ������
��ĳ��ĳ���������ʼ������
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

/*********************�Ҷ��ߺ���(�߽�)RR*******************
* �������ƣ�Right_lost
* ����˵�����ж�ĳ�ض����Ƿ��ұ߶���
* ��ע��aim Ŀ���� start_point ������
��ĳ��ĳ���������ʼ���Ҷ���
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

/*******************�������߶��ߺ���(�߽�)RR****************
* �������ƣ�Both_lost
* ����˵�����ж�ĳ�ض����Ƿ��������߶�����
* ��ע��aim Ŀ���� start_point ������
��ĳ��ĳ���������ʼ���Ҷ���
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


/**************************Ѱ����߽纯��RR*********************
* �������ƣ�get_left
* ����˵�����õ��ض���(aim)�ض���ʼλ��(start_point)����߽�
* ��ע���������������ߺ������Ի���
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
    for(left_count = start_point;left_count > 0;left_count--)  //ע����>0
    {
      if(*(pic_left + left_count) == black)
        break;
    }
    result = left_count;
  }
  return result;
}


/*********************Ѱ���ұ߽纯��RR**********************
* �������ƣ�get_right
* ����˵�����õ��ض���(aim)�ض���ʼλ�õ��ұ߽�
* ��ע���������������ߺ������Ի���
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




/**************************����****************************
* �������ƣ�centerline_filter
* ����˵���������˲�
* ��ע��
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
	  	  
      //��������ƫ��
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
      //Զ������ƫ��
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


/*********************���ڵ㺯��RR***********************
* �������ƣ�black_count
* ����˵�����õ��ض���(aim)�ڵ����
* ��ע��
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


/********************����RR***********************
* �������ƣ�get_black_length
* ����˵�����õ��ڿ鳤��
* ��ע��
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


/**************************����***********************
* �������ƣ�get_black_wild
* ����˵�����õ��ڿ鳤��
* ��ע��
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


/**************************������������***********************
* �������ƣ�link
* ����˵������������ֱ������
* ��ע�� //a[b]Ϊȷ��ֵ��cΪĿ��������dΪĿ��ֵ

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


/**************************��Ч�к���****************************
* �������ƣ�get_effective
* ����˵�����õ���Զ��Ч��
* ��ע����left��rightÿһ�д��ض��У�start_line����ǰ(0)ɨ��������ֺ�ɫ����
,�õ���Щ����ǰ��������

                **ע�⣺��Ч�С�Ŀ���С������е�Ӧ�á�
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


/***********************��Ч��ɨ��************************
* �������ƣ�effective_row
* ����˵�����õ���Զ��Ч������
* ��ע�������ж�ֱ����СS
*************************************************************/
int effective_row(int left,int right,int start_line)
{
	int w = 0;
	int left_line = 0, right_line = 0, wide_row = 0, wide_effective_row = 0;

    //�õ��߽�
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
	//�õ���Ч�п��
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

/**************************������Ч�к���RR****************************
* �������ƣ�get_single_effective
* ����˵�����õ���Զ��Ч��
* ��ע���õ�����ĳһ�е���Ч��
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


/********************************************��ͨ�������********************************************/

/*****************��ͨ������жϺ���RR***********
* �������ƣ�left_wan_judge
* ����˵������ͨ�����ж�
* ��ע��
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



/****************��ͨ������жϺ���RR*************
* �������ƣ�right_wan_judge
* ����˵������ͨ�����ж�
* ��ע��
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



/**************************����****************************
* �������ƣ�cross_judge
* ����˵����ʮ���ж�
* ��ע��
**************************************************************************/

uint8_t cross_judge(int aim)
{
#define cross_judge_effective (10)                        // ʮ���жϵĳ�ͼ����Ч��Ӧ����С
#define cross_judge_aim (10)                             //ʮ���жϵĳ�ͼ����Ŀ����Ӧ���ϴ�
#define cross_judge_length (45)                           //��������Զ���е��жϷ���

#define cross_judge_double_near (aim-12)                      //����˫�߶���ʱ�жϵ�����
#define cross_judge_double_far (aim-1)                       //Զ��˫�߶���ʱ�жϵ�����

#define cross_judge_single_far1 (aim-1)                      //Զ�����߶���ʱ�жϵ�����
#define cross_judge_single_far2 (aim-2)
#define cross_judge_single_far3 (aim-4)
#define cross_judge_single_near1 (aim-4)
#define cross_judge_single_near2 (aim-5)
#define cross_judge_single_near3 (aim-6)

#define cross_single_lost_error  (10)
  
  if( aim < effective_line_temp + 6 || effective_line_temp > cross_judge_effective)  //ʮ�������Ч�н�Զ
    return 0;                                    
	                                                                                          
  if(aim < cross_judge_aim || block_flag != 0)      //����С��10�����жϳ��ϰ���������
    return 0;
  
  if(feature2(aim-5,left_line_temp[aim],right_line_temp[aim]) != 0)
    return 0;

  //ʮ�ִ���  ˫�߶��� 
  if(left_lost == 1 && right_lost == 1)             
  {
    if(cross_flag == 1 && (img[(aim-1)*W] == white || img[(aim-1)*W+W-1] == white))  //Զ��ʮ���ж�
      return 1;
    if(img[cross_judge_double_far*W+centerline[aim+1]] == black) //ǰһ������Ϊ��
      return 0;
    
    left_line_temp[cross_judge_double_far] = get_left(cross_judge_double_far,centerline[aim+1]);
    right_line_temp[cross_judge_double_far] = get_right(cross_judge_double_far,centerline[aim+1]);
    
    if(aim >= cross_judge_length && cross_flag == 0)         //����ʮ���ж�
    {
      if(img[cross_judge_double_near*W+centerline[aim+1]] == white)     //�жϸ���ǰ14�е����  
      {
        left_line_temp[cross_judge_double_near] = get_left(cross_judge_double_near,centerline[aim+1]);
        right_line_temp[cross_judge_double_near] = get_right(cross_judge_double_near,centerline[aim+1]);
		
        if(left_line_temp[cross_judge_double_near] > left_line_temp[aim] ||
           right_line_temp[cross_judge_double_near] < right_line_temp[aim])
          return 0;
		  
        if((left_line_temp[cross_judge_double_near] == 0 && right_line_temp[cross_judge_double_near] != (W-1)) 
           || (right_line_temp[cross_judge_double_near] == (W-1) && left_line_temp[cross_judge_double_near] != 0))
          return 0;   //Ӧ�������߶�����
      }
    }
    
    if(left_line_temp[cross_judge_double_far] <= left_line_temp[aim] && right_line_temp[cross_judge_double_far] >= right_line_temp[aim])
      return 1;
  }
  
  //�����Ѷ� ��һ�߾������
  if(left_lost == 1 && right_line_temp[aim] > right_line_temp[aim+1])      
  {
    if(cross_flag == 1 && (img[(aim-1)*W] == white || img[(aim-1)*W+W-1] == white))
      return 1;
    if(img[cross_judge_single_far3*W+centerline[aim+1]] == black)
      return 0; 
    
    right_line_temp[cross_judge_single_far1] = get_right(cross_judge_single_far1,centerline[aim+1]);  //ǰ�����ж�
    right_line_temp[cross_judge_single_far2] = get_right(cross_judge_single_far2,centerline[aim+1]);
    right_line_temp[cross_judge_single_far3] = get_right(cross_judge_single_far3,centerline[aim+1]);
    
    if(right_line_temp[cross_judge_single_far3] - right_line_temp[cross_judge_single_far1] < cross_single_lost_error
       && right_line_temp[cross_judge_single_far3] != (W-1))
      return 0;
    
    left_line_temp[cross_judge_single_far3] = get_left(cross_judge_single_far3,centerline[aim+1]);
    
    if(aim >= cross_judge_length && cross_flag == 0)                            //�������ʱ�������������������
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
  
  //�����Ѷ� ��һ�߾������
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


/**************************����****************************
* �������ƣ�cross_deal
* ����˵����ʮ�ִ���
* ��ע��
**************************************************************************/

int cross_deal(int aim)            //���ж�֮ǰ����б�ʣ�ƫ��ƫ�ң��ٸ��������ж�Ѱ�ҷ���
{
#define cross_judge_error_piont (10) //���ҵ����������С��ԭ���������+cross_pointʱ��Ϊ�ҵ�����
//#define cross_way_judge_point (-10)
    
    int cross_count = 0,cross_count2 = 0;      //ʮ���м���
    int subs = 0,way = 0;     // �����ж� 
    int start_point = 0;       //ʮ�ֿ�ʼ��
    int wide_judge = 0,wide_last = 0;         //����ж�
    int right_temp = 0,left_temp = 0;
    int cross_error_point = 0;
    uint8_t *pic_cross = NULL;
    pic_cross=&img[0];

    if(aim >= H-5)               //��������ǰʱ
    {
      if(left_line_temp[aim+1] <= 0 && right_line_temp[aim+1] < (W-1)) 
        subs = -1;
      if(left_line_temp[aim+1] > 0 && right_line_temp[aim+1] >= (W-1)) 
        subs = 1;
      if(left_line_temp[aim+1] <= 0 && right_line_temp[aim+1] >= (W-1)) 
        subs = cross_way_last;       
    }
	
    if(subs == 0)                    //��֮ǰ���ж�ʧЧʱ
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
      return -1;                     //���̫Сʱ�����ͼ���
    
    
    for(cross_count = aim-3;cross_count > 0;cross_count--)              //���жϺ�����������aim����5
    {
      while(*(pic_cross + cross_count*W+start_point) == black)         //�ҵ���ɫ����
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
        //����˫�߶���ʱ���ܳ��� �е��ж���׼
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


/**************************��·�ж�****************************
* �������ƣ�huanlu_judge
* ����˵����ʶ��·
* ��ע��
**************************************************************/

uint8_t huanlu_judge(int aim)
{
	#define huanlu_judge_effective (20)                        //ʮ���жϵĳ�ͼ����Ч��Ӧ����С
	#define huanlu_judge_aim (10)                             //ʮ���жϵĳ�ͼ����Ŀ����Ӧ���ϴ�
    #define huanlu_judge_length (30)                           //��������Զ���е��жϷ���
    #define huanlu_judge_double_near (aim-10)                      //��������ʱ�жϵ�����
    #define huanlu_judge_double_far (aim-3)                       //Զ������ʱ�жϵ�����
	#define black_start_line (5)
	#define black_length (3)
	
	int huanlu_start;  //��·��ʼ��
	int black_start, black_end;  //����ڿ���ʼ ������
	int huanlu_count = 0;  
	
  //��Ч���ж� ��ע��
  if( /*effective_line_temp > huanlu_judge_effective ||*/ aim < effective_line_temp+2)  
    return 0;                                    
	                                                                                          
  if(aim < huanlu_judge_aim)      //����С��12������
    return 0;
  
//  if(feature2(aim-5,left_line_temp[aim],right_line_temp[aim]) != 0)
//    return 0;

  //��·����
  if(((left_line_temp[aim] < left_line_temp[aim+1]+1 && right_line_temp[aim] > right_line_temp[aim+1]+2)
  || (left_line_temp[aim] < left_line_temp[aim+1]-2 && right_line_temp[aim] > right_line_temp[aim+1]-1))
  && (road_wide_temp[aim] >= road_wide_temp[aim+1]+4))             
  {
     
    if(img[huanlu_judge_double_far*W+centerline[aim+1]] == black) //ǰһ������Ϊ��
      return 0;
    
    left_line_temp[huanlu_judge_double_far] = get_left(huanlu_judge_double_far,centerline[aim+1]);
    right_line_temp[huanlu_judge_double_far] = get_right(huanlu_judge_double_far,centerline[aim+1]);
    road_wide_temp[huanlu_judge_double_far] = right_line_temp[huanlu_judge_double_far]-left_line_temp[huanlu_judge_double_far];
/*  
    if(aim > huanlu_judge_length)         //������·�ж�
    {
      if(img[huanlu_judge_double_near*W+centerline[aim+1]] == white)     //�жϸ���ǰ14�е����  
      {
        left_line_temp[huanlu_judge_double_near] = get_left(huanlu_judge_double_near,centerline[aim+1]);
        right_line_temp[huanlu_judge_double_near] = get_right(huanlu_judge_double_near,centerline[aim+1]);
		
        if(left_line_temp[huanlu_judge_double_near] > left_line_temp[aim] ||
           right_line_temp[huanlu_judge_double_near] < right_line_temp[aim])
          return 0;
		  
        if(left_line_temp[huanlu_judge_double_near] == 0 && right_line_temp[huanlu_judge_double_near] == W-1)
          return 0;   //Ӧ�������߶�����
      }
    }
*/	
	if((left_line_temp[huanlu_judge_double_far] < left_line_temp[aim] && right_line_temp[huanlu_judge_double_far] > right_line_temp[aim]) || road_wide_temp[huanlu_judge_double_far] > road_wide_temp[aim])
		{
            //bug_flag1 = 1;
			if(left_line_temp[huanlu_judge_double_far] <= 0 && right_line_temp[huanlu_judge_double_far] >= W-1)
				return 0;
			huanlu_start = aim+1;  //��ʼ��Ϊ��������  effective_line_temp��������Ҫ��
            //bug_flag2 = huanlu_start;
			for(huanlu_count = huanlu_start; huanlu_count > effective_line_temp+1; huanlu_count--)
			{
				if(img[huanlu_count*W+centerline[aim+1]] == black && img[(huanlu_count-1)*W+centerline[aim+1]] == black)
					break;
			}
			black_start = huanlu_count-1;  //�ڿ���ʼ��
			if(black_start < black_start_line) 
				return 0;
                        
			//huanlu_flag = 1;
/*			
			for(huanlu_count = black_start; huanlu_count > effective_line_temp; huanlu_count--)
			{
				if(img[huanlu_count*W+centerline[aim+1]] == white)
					break;
			}
			black_end = huanlu_count;  //�ڿ��յ�
*/			
            if(get_black_length(black_start,centerline[aim+1]) > black_length || get_black_length(black_start-1,centerline[aim+1]) > black_length)
			{
				if(get_black_wild(black_start,centerline[aim+1]) > 20 && (get_black_wild(black_start-1,centerline[aim+1]) >= get_black_wild(black_start,centerline[aim+1])) && 
				(get_black_wild(black_start-2,centerline[aim+1]) >= get_black_wild(black_start,centerline[aim+1])) )
				//huanlu_flag = 1;  //��·��־��1
                                  bug_flag1 = 1;
				return 1;
				
				//return 0;
			}
		}
    
  }
  return 0;
}


/**************************��·�ж�****************************
* �������ƣ�huanlu_deal
* ����˵������·����
* ��ע��
**************************************************************/

int huanlu_deal(int aim)
{
	#define huanlu_judge_error_piont (20) //���ҵ����������С��ԭ���������+cross_pointʱ��Ϊ�ҵ�����
    
    int huanlu_count = 0,huanlu_count2 = 0;      //ʮ���м���
    int way = 0 , subs = 0;     // �����ж� 
    int start_point = 0;       //ʮ�ֿ�ʼ��
    int wide_judge = 0,wide_last = 0;         //����ж�
    int right_temp = 0,left_temp = 0;
//    int huanlu_error_point = 0;
    uint8_t *pic_huanlu = NULL;
    pic_huanlu=&img[0];
	
	//wai�������߷���
    for(huanlu_count = H-2;huanlu_count > aim;huanlu_count--)
        subs += centerline[huanlu_count] - centerline[huanlu_count+1];
    
//    if(subs >= 15) way = -1;
//	else way = 1;
	
    way = 1;
    cross_way_last = way;
	
    start_point =  centerline[aim+1];     
    wide_last = right_line_temp[aim+1] - left_line_temp[aim+1];
    
    for(huanlu_count = aim-2; huanlu_count > 0; huanlu_count--)              //���жϺ�����������aim����5
    {
      while(*(pic_huanlu + huanlu_count*W+start_point) == black)         //�ҵ���ɫ����
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
		
/*        //����˫�߶���ʱ���ܳ��� �е��ж���׼
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

/**************************�����߼��****************************
* �������ƣ�starting_line
* ����˵����ʮ�ִ���
* ��ע��
**************************************************************************/
/*
int end_count_1=0; 

void starting_line(void)
{
int i=0;
int j=0;
int row_temp=0;	//��¼һ�кڵ��������
int starline_row=0;
int starline_stop=0;

for (i=59;i>30;i--)	//������ߺڵ���
{
	row_temp=0;	//��������ʱ����
	for (j=left_line_temp[i]+1;j<right_line_temp[i];j++) 	//���߼��㵽���ߣ����������ұ߽�
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
if (starline_row>=2&&starline_row<=5)	//˵����������
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
//  gpio_init(led_1,GPO,0);          //�Ӹ���ʱ
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
