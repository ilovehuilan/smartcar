/*****************函数***************/

extern void feature_init();
extern void first_scan(int aim);
extern uint8_t feature2(int aim,int left,int right);


extern int ab(int a);
extern double ab_double(double a);
extern double xielv(int aim);
extern int Left_lost(int aim,int start_point);
extern int Right_lost(int aim,int start_point);
extern int Both_lost(int aim,int start_point);

extern int black_count();
extern int get_left(int aim,int start_point);
extern int get_right(int aim,int start_point);
extern int black_count(int aim);
extern int get_black_length();
extern int get_black_wild();
extern void link();
extern int get_effective();
extern int single_effective(int line);
extern int zhidao_judge();
extern int left_wan_judge(void);
extern int right_wan_judge(void);
extern uint8_t cross_judge();
extern int cross_deal(int aim);

extern void Center_line();

/********************赛道识别**********************/
extern int effective_line_temp;
extern int right_line_temp[H] , left_line_temp[H]; 

