/*
 Основные единицы измерения
  длина - астрономическая единица
  масса  - масса Солнца
  время - средние солнечные сутки 86400 сек. эфемеридного времени
*/

#ifndef _MATH_H_
 #include <math.h>
 #define _MATH_H_
#endif
#ifndef _STDIO_H_
 #include <stdio.h>
 #define _STDIO_H_
#endif
#ifndef _STRING_H_
 #include <string.h>
 #define _STRING_H_
#endif
#ifndef _STDLIB_H_
 #include <stdlib.h>
 #define _STDLIB_H_
#endif


#define Gauss_const 0.01720209895
#define Julian_day_in_sec 86400
#define Julian_century_in_days 36525

#define MAX_Kepler_Iterations 1000
#define Kepler_relative_error 1e-10

#define MAX_LINE_LENGTH 512

double GlobalTimeJD;
double SimulationTime;

typedef struct
{
	double value;
	double change;
}parameter;

typedef struct 
{
	double X_coord;	// [а.е.]
	double Y_coord;	// [а.е.]
	double Z_coord;	// [а.е.]
}position;

typedef struct
{
	double VX_vel;	// [а.е./сутки]
	double VY_vel;	// [а.е./сутки]
	double VZ_vel;	// [а.е./сутки]
}velocity;

typedef struct
{
	double aX_accel;	// [а.е./сутки^2]
	double aY_accel;	// [а.е./сутки^2]
	double aZ_accel;	// [а.е./сутки^2]
}acceleration;

typedef struct
{
	parameter SemiMajorAxis;		// большая полуось		[а.е.]
	parameter Eccentricity;			// эксцентриситет		[безразм.]
	parameter Inclination;			// наклонение орбиты		[радианы]
	parameter LongitudeOfPericenter;	// долгота перицентра		[радианы]
	parameter AscendingNode;		// долгота восходящего узла	[радианы]
	parameter MeanLongitude;		// средняя долгота		[радианы]
	parameter N_r;				// параметр для нахождения средней долготы	[360 градусов * обороты за столетие]
	parameter Epoch;			// эпоха элементов орбиты	[Юлианская дата]
	parameter Mass;				// масса			[масс Солнца]
	position pos;				// прямоугольные координаты	[а.е.]
}planet;

typedef struct
{
	parameter PerihelionDistance;		// перигелийное расстояние			[а.е.]
	parameter Eccentricity;			// эксцентриситет				[безразм.]
	parameter Inclination;			// наклонение орбиты				[радианы]
	parameter ArgumentOfPericenter;		// аргумент перицентра				[радианы]
	parameter AscendingNode;		// долгота восходящего узла			[радианы]
	parameter PericenterTransitTime;	// время прохождения перигелия			[Юлианская дата]
	parameter R_non_grav;			// радиальная компонента негравитационной силы	[а.е./сутки^2]
	parameter T_non_grav;			// трансверсальная //--//--//			[а.е./сутки^2]
	parameter N_non_grav;			// нормальная //--//--//			[а.е./сутки^2]
	position pos;				// прямоугольные координаты			[а.е.]
	velocity vel;				// скорость					[а.е./сутки]
	acceleration grav_accel;		// гравитационное ускорение					[а.е./сутки^2]
	acceleration non_grav_accel;		// негравитационное ускорение					[а.е./сутки^2]
	char Integrate;				// если 0 - нахождение положения по элементам орбиты, иначе интегрирование орбиты
	/* для комет нет данных об изменении орбитальных параметров  */
}comet;

int int_part(double a);				// возвращает целую часть числа без округления
int ssd_Int(double argument);			// возвращает наибольшее целое число, меньшее или равное "argument"
double ssd_Fract(double argument);		// возвращает дробную часть числа (с округлением)
double ssd_Absolute_lf(double argument);	// возвращает абсолютное значение числа типа "double"
double ssd_sqr_lf(double argument);		// возвращает квадрат числа типа "double"
double ssd_cube_lf(double argument);		// возвращает куб числа типа "double"
void LogCurrentPlanetPossition(FILE *PlanetLogFile,	// запись положений планеты в файл (для каждой планеты указывать свой файл)
				planet body);		// 
void LogCurrentCometPossitions(FILE *CometLogFile,	// запись положения, скорости, грав.ускорения, неграв.ускорения кометы
			       comet body);		// в файл (для каждой кометы указывать свой файл)
// переход от Грегорианской даты к Юлианской дате
void GregorianDateToJulianDate(int Year,		// год Грегорианского календаря
			       int Month,		// месяц Грегорианского календаря
			       int Day,			// день Грегорианского календаря
			       double UT,		// всемирное время
			       double *JD);		// Юлианская дата
// переход от Юлианской даты к Грегорианской дате
void JulianDateToGregorianDate(double JD,		// Юлианская дата
			       int *Year,		// год Грегорианского календаря
			       int *Month,		// месяц Грегорианского календаря
			       int *Day,		// день Грегорианского календаря
			       double *UT);		// всемирное время
void KeplersEquation(double MeanAnomaly,		// решение уравнения Кеплера итеративным методом
		     double Eccentricity,		// численное решение ограниченно максимальным числом итераций "MAX_Kepler_Iterations"
		     double *EccentricAnomaly);		// и относительной ошибкой решения "Kepler_relative_error"
void RecalculatePlanetEphemeris(planet *X);		// пересчёт изменений орбиты на данное время "GlobalTimeJD"
void RecalculateCurrentPlanetPosition(planet *body);	// пересчёт положения планеты на данное время
void RecalculateCurrentCometPosition(comet *body);	// пересчёт пложения кометы исходя из элементов орбиты на данное время
