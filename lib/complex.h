/*
 * @Author: your name
 * @Date: 2021-10-25 23:58:10
 * @LastEditTime: 2021-10-26 18:45:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \NewtonIterationFractal\lib\complex.h
 */
struct complexNum
{
    double real,image;
    complexNum(double real ,double image):real(real),image(image){}
    complexNum():real(0),image(0){}
    void operator=(const complexNum &a)
    {
        this->real=a.real;this->image=a.image;
    }
    complexNum operator+(const complexNum &a)
    {
        complexNum temp;
        temp.image=a.image+this->image;
        temp.real=a.real+this->real;
        return temp;
    }
    complexNum operator-(const complexNum &a)
    {
        complexNum temp;
        temp.image=this->image-a.image;
        temp.real=this->real-a.real;
        return temp;
    }
    complexNum operator*(const complexNum &a)
    {
        complexNum temp;
        temp.real=this->real*a.real-this->image*a.image;
        temp.image=this->real*a.image+this->image*a.real;
        return temp;
    }
    complexNum operator/(const complexNum &a)
    {
        complexNum temp;
        temp.real=(this->real*a.real+this->image*a.image)/(a.real*a.real+a.image*a.image);
        temp.image=(this->image*a.real - this->real*a.image)/(a.real*a.real+a.image*a.image);
        return temp;
    }
};