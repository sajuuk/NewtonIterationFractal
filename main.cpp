/*
 * @Author: your name
 * @Date: 2021-10-25 23:34:20
 * @LastEditTime: 2021-10-28 21:27:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \NewtonIterationFractal\main.cpp
 */
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<graphics.h>
#include<ctime>
#include"lib\complex.h"
#define WIDTH 640
#define HEIGHT 480
#define MAX_ITER 20
int *colorList=nullptr;
//f(x)=(x+r0)(x+r1)...(x+rn-1)
complexNum f(complexNum *root,complexNum x,int n)
{
    complexNum temp(1,0);
    for(int i=0;i<n;i++)
    {
        temp=temp*(x+root[i]);
    }
    return temp;
}
//f'(x)=
complexNum df(complexNum *root,complexNum x,int n)
{
    complexNum temp(1,0);
    complexNum ans(0,0);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(i==j)continue;
            temp=temp*(x+root[j]);
        }
        ans=ans+temp;
        temp=complexNum(1,0);
    }
    return ans;
}
complexNum fdivdf(complexNum *root,complexNum x,int n)
{
    complexNum temp(0,0);
    for(int i=0;i<n;i++)
    {
        complexNum t=x-root[i];
        if(t.real*t.real+t.image*t.image<0.00001) 
        {
            return complexNum(0,0);
        }
        temp=temp+complexNum(1,0)/t;
    }
    return complexNum(1,0)/temp;
}
complexNum newtonIter(complexNum x,complexNum *root,int n)
{
    //complexNum div=f(root,x,n)/df(root,x,n);
    complexNum div=fdivdf(root,x,n);
    return x-div;
}

void init(int n,complexNum **root)
{
    initgraph(WIDTH,HEIGHT);
    setorigin(WIDTH/2,HEIGHT/2);
    (*root)=new complexNum[n];
    (*root)[0]=complexNum(-200,200);
    (*root)[1]=complexNum(400,400);
    (*root)[2]=complexNum(280,-240);//set first 3 root to debug
    (*root)[2]=complexNum(-340,-360);
    // colorList=new int[n];
    // for(int i=0;i<n;i++)//set up colors
    // {
    //     colorList[i]=HSLtoRGB(360/(float)n*i,0.6,0.6);
    // }
}
int getClosest(int n,complexNum sample,complexNum *root)
{
    double mindis=99999999999;int index=-1;
    for(int i=0;i<n;i++)
    {
        complexNum vec=sample-root[i];
        double dis=vec.real*vec.real+vec.image*vec.image;
        if(dis<mindis)
        {
            index=i;mindis=dis;
        }
    }
    return index;
}
void draw(int n,complexNum *root)
{
    BeginBatchDraw();
    clock_t start=clock();
    for(int i=-WIDTH/2;i<WIDTH/2;i++)
    {
        for(int j=-HEIGHT/2;j<HEIGHT/2;j++)
        {
            complexNum sample((double)i,(double)j);
            complexNum pre(0,0);
            int k;
            for(k=0;k<MAX_ITER;k++)
            {
                sample=newtonIter(sample,root,n); 
                if(k==0)
                {
                    pre=sample;
                }
                else
                {
                    complexNum delta=sample-pre;
                    if(delta.real*delta.real+delta.image*delta.image<0.001)
                    {
                        break;
                    }
                    pre=sample;
                }
            }
            int cluster=getClosest(n,sample,root);
            float normal=(float)k/(float)MAX_ITER;
            int color=HSLtoRGB(360/(float)n*cluster,0.4+0.3*normal,0.4+0.3*normal);
            putpixel(i,j,color);
        }
    }
    for(int i=0;i<n;i++)
    {
        circle(root[i].real,root[i].image,5);
    }
    clock_t dur=clock()-start;
    printf("%lf",(double)dur/CLOCKS_PER_SEC);
    FlushBatchDraw();
}
int main(int argc,char *argv[])
{
    //int n=atoi(argv[argc-1]);
    int n=4;//default
    complexNum *root=nullptr;
    init(n,&root);
    draw(n,root);
    getchar();
    EndBatchDraw();
    closegraph();
    return 0;
}