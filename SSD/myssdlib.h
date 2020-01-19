#define  G 6.67256e-11
#define au 1.5e+11
#define dt 1e-9

#define  p 9
#define  q 4 // n= p/q=2.25

struct mech_param
{
	double x,y,z,vx,vy,vz,ax,ay,az,mass,radius,Ar,At,An;
	char b_class; 				/* 0 - planet,star   1 - comet */
};

int int_part(double a);//возвращает целую часть числа
int ssd_int(double argument);//возвращает наибольшее целое число, меньшее или равное "argument"
double ssd_abs(double x);//возврашает абсолютное значение числа
double ssd_fract(double argument);//возвращает дробную часть числа
double sqr(double a);//возвращает квадрат числа
double cube(double a);//возвращает куб числа
double ssd_sqrt(double x);//возвращает квадратный корень числа
double int_power(double x,int k);//x в целой степени k
double n_root(int k,double x);//корень целой n-той степени из x
double distance(struct mech_param one,struct mech_param two);//возвращает значение расстояния между объектами
void gregorian_to_julian(int year, int month, int day, double ut, double *jd, int *flag);//перевод Грегорианской даты в Юлианскую
void julian_to_gregorian(double jd, int *year, int *month, int *day, double *ut);//перевод Юлианской даты в Грегорианскую
//void force_gravity(struct mech_param (*body)[],int num );//вычисляет значения ускорений в поле гравитации для всех объектов num-количество объектов
//void force_nongravity(struct mech_param (*body)[],int num );//вычисляет зн. ускор. реактивной силы
void new_position(struct mech_param (*body)[], int num );//вычисляет новое положение всех объектов исходя из ускорений
double test(struct mech_param one );
