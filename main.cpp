/*
 * @Author: your name
 * @Date: 2021-10-25 23:34:20
 * @LastEditTime: 2021-11-01 18:32:13
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
#include<windows.h>
#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 20
#define MAXN 10
int *colorList=nullptr;
DWORD buffer[WIDTH*HEIGHT];
DWORD *pMem=nullptr;
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
    pMem=GetImageBuffer();
    //setorigin(WIDTH/2,HEIGHT/2);
    (*root)=new complexNum[MAXN];
    (*root)[0]=complexNum(100,100);
    (*root)[1]=complexNum(200,200);
    (*root)[2]=complexNum(300,120);//set first 3 root to debug
    (*root)[3]=complexNum(170,180);
    (*root)[4]=complexNum(100,100);
    (*root)[5]=complexNum(100,100);
    (*root)[6]=complexNum(100,100);
    (*root)[7]=complexNum(100,100);
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
void putCicle(int n,complexNum *root)
{
    //cleardevice();
    for(int i=0;i<n;i++)
    {
        circle(root[i].real,root[i].image,5);
    }
    FlushBatchDraw();
}
void draw(int n,complexNum *root)
{
    clock_t start=clock();
    #pragma omp parallel for schedule(dynamic)
    for(int i=0;i<HEIGHT;i++)
    {
        for(int j=0;j<WIDTH;j++)
        {
            complexNum sample((double)j,(double)i);
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
            buffer[i*WIDTH+j]=BGR(color);
            //putpixel(i,j,color);
        }
    }
    memcpy(pMem,buffer,sizeof(DWORD)*HEIGHT*WIDTH);
    clock_t dur=clock()-start;
    printf("%lf",(double)dur/CLOCKS_PER_SEC);
}
// int getSelectedroot(complexNum *root,int n,short x,short y)
// {
//     complexNum mouse(x,y);
//     complexNum vec;
//     double min=999999999;
//     int index=-1;
//     for(int i=0;i<n;i++)
//     {
//         vec=mouse-root[i];
//         if(vec.real*vec.real + vec.image*vec.image < min)
//         {
//             min=vec.real*vec.real + vec.image*vec.image;
//             index=i;
//         }
//     }
//     return index;
// }
void setSelectedroot(complexNum* root,int index,short x,short y)
{
    if(index<0) return;
    root[index].real=(double)x;
    root[index].image=(double)y;
    return;
}
void messageLoop(complexNum *root,int n)
{
    ExMessage m;
    bool isDrag=false;
    draw(n,root);
    putCicle(n,root);
    int selectedIndex=-1;
    short mx,my;
    while(1)
    {
        m=getmessage(EM_MOUSE | EM_KEY);
        if(isDrag && m.message==WM_MOUSEMOVE)
        {
            //cleardevice();
            mx=m.x;my=m.y;
            getClosest(n,complexNum(mx,my),root);
            setSelectedroot(root,selectedIndex,mx,my);
            draw(n,root);
            putCicle(n,root);
            flushmessage();
        }
        if((!isDrag) && m.message==WM_RBUTTONUP && n<10)
        {
            root[n].real=m.x;
            root[n].image=m.y;
            n++;
            draw(n,root);
            putCicle(n,root);
        }
        if(m.message==WM_LBUTTONUP && isDrag)
        {
            isDrag=false;
            draw(n,root);
            putCicle(n,root);
        }
        else if(m.message==WM_LBUTTONDOWN && isDrag==false)
        {
            printf("get mouse down");
            selectedIndex=getClosest(n,complexNum(m.x,m.y),root); //(root,n,m.x-WIDTH/2,m.y-HEIGHT/2);
            isDrag=true;
        }
        else if(m.message==WM_KEYDOWN)
        {
            if(m.vkcode==VK_ESCAPE) return;
        }
        //flushmessage();
    }
}
int main(int argc,char *argv[])
{
    //int n=atoi(argv[argc-1]);
    int n=2;//default
    complexNum *root=nullptr;
    init(n,&root);
    BeginBatchDraw();
    messageLoop(root,n);
    EndBatchDraw();
    closegraph();
    return 0;
}