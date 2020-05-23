#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define SWU SDL_WINDOWPOS_UNDEFINED
#define SWS SDL_WINDOW_SHOWN
#define SRA SDL_RENDERER_ACCELERATED
using namespace std;
typedef pair <int,int> ii;
const string tencuaso="CAR RACING";
const string im="image.png",
             dead="dead.jpg";
int die=0,x1,x2,y1,y2,d=0,cnt=0,mau[19],r1=-1,r2=-1;
ii sta[189];
struct grap
{
    SDL_Window* window;
    SDL_Texture* txt;
    vector <SDL_Rect> ima;
    SDL_Renderer* ren;
};
struct car
{
    int tdx1,tdy1,tdx2,tdy2,type;
};
struct vitrixeboss
{
    int vtx1,vtx2,vty1,vty2;
};
vitrixeboss vt;
car xe[10];
void close(grap &a)
{
    SDL_DestroyTexture(a.txt);
    SDL_DestroyRenderer(a.ren);
    SDL_DestroyWindow(a.window);
    IMG_Quit();
    SDL_Quit();
}
void err(const string &msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",msg.c_str(),NULL);
}
SDL_Texture* creat(SDL_Renderer* ren,const string &path)
{
    SDL_Surface* surface=IMG_Load(path.c_str());
    if (surface==NULL)
    {
        err("Unable to load image" + path + "!");
        return NULL;
    }
    SDL_Texture* txt=SDL_CreateTextureFromSurface(ren,surface);
    SDL_FreeSurface(surface);
    return txt;
}
void taoanh(vector<SDL_Rect> &a)
{
    for (int i=0;i<=49;++i)
    {
        for (int j=0;j<=39;++j)
        {
            SDL_Rect rt={j*10,i*10,10,10};
            a.push_back(rt);
        }
    }
}
void vacham(grap &a)
{
    for (int i=1;i<=cnt;++i)
    {
        if (xe[i].tdx1<=vt.vtx1 && vt.vtx1<=xe[i].tdx2 && ((vt.vty1<xe[i].tdy1 && xe[i].tdy1<vt.vty2)||(vt.vty1<=xe[i].tdy2 && xe[i].tdy2<vt.vty2))) die=1;//cac truong hop xe dieu khien
        if (xe[i].tdx1<=vt.vtx2 && vt.vtx2<=xe[i].tdx2 && ((vt.vty1<xe[i].tdy1 && xe[i].tdy1<vt.vty2)||(vt.vty1<=xe[i].tdy2 && xe[i].tdy2<vt.vty2))) die=1;// va cham voi xe chay nguoc
        if (die==1)
        {
            SDL_RenderClear(a.ren);
            a.txt=creat(a.ren,dead);
            SDL_RenderCopy(a.ren,a.txt,0,0);
            SDL_RenderPresent(a.ren);
            SDL_Delay(500);
            close(a);
        }
    }
}
void xuly(grap &a,int &u,int &v)
{
    vt.vtx1+=u;
    if (vt.vtx1<10) {vt.vtx1=10;u=0;}
    if (vt.vtx1>35) {vt.vtx1=35;u=0;}
    vt.vtx2=vt.vtx1+4;
    vt.vty1+=v;
    if (vt.vty1<0) {vt.vty1=0;v=0;}
    if (vt.vty1>40) {vt.vty1=40;v=0;}
    vt.vty2=vt.vty1+9;
    x1=vt.vtx1;
    x2=vt.vtx2;
    y1=vt.vty1;
    y2=vt.vty2;
    if (u==1) x1-=1;
    if (u==-1) x2+=1;
    if (v==1) y1-=1;
    if (v==-1) y2+=1;
}
void tocdo(grap &a,int u,int v)
{
    for (int k=1;k<=2;++k)
    {
        for (int i=y1;i<=y2;++i)
        {
            for (int j=x1;j<=x2;++j)
            {
                SDL_Rect rt;
                if ((u==1&&j==x1)||(u==-1&&j==x2)||(v==1&&i==y1)||(v==-1&&i==y2)) rt=a.ima[i*40+j];
                else rt=a.ima[(i-y1-(v==1?1:0))*40+(j-x1-(u==1?1:0))];
                SDL_Rect td={(j-10)*10,i*10,10,10};
                SDL_RenderCopy(a.ren,a.txt,&rt,&td);
            }
        }
        xuly(a,u,v);
        SDL_Delay(60);
        SDL_RenderPresent(a.ren);
        vacham(a);
    }
}
void add()
{
    int xe1=mau[(++r1)%19],xe2=mau[(++r1)%19];
    int vt1=sta[(++r2)%189].first,vt2=sta[r2%189].second;
    xe[++cnt].tdx1=vt1;
    xe[cnt].tdx2=vt1+4;
    xe[cnt].tdy1=-8;
    xe[cnt].tdy2=1;
    xe[cnt].type=xe1;
    xe[++cnt].tdx1=vt2;
    xe[cnt].tdx2=vt2+4;
    xe[cnt].tdy1=-8;
    xe[cnt].tdy2=1;
    xe[cnt].type=xe2;
}
int check(grap &a,SDL_Rect &rt,int x,int y)
{
    for (int i=1;i<=cnt;++i)
    {
        if (xe[i].tdx1<=x && x<=xe[i].tdx2 && xe[i].tdy1<=y && y<=xe[i].tdy2)
        {
            rt=a.ima[(y-xe[i].tdy1+xe[i].type*10)*40+(x-xe[i].tdx1+5)];
            return 1;
        }
    }
    return 0;
}
void go()
{
    for (int i=1;i<=cnt;++i)
    {
        xe[i].tdy1+=1;
        xe[i].tdy2+=1;
    }
    d=d+1;
    if (d==31) d=1;
    if (d==1) add();
    int p=1;
    while(p<=cnt)
    {
        if (xe[p].tdy2==59)
        {
            cnt-=1;
            for (int i=p;i<=cnt;++i)
            {
                xe[i]=xe[i+1];
            }
            p-=1;
        }
        p+=1;
    }
}
void update(grap &a,int u,int v)
{
    SDL_RenderClear(a.ren);
    go();
    xuly(a,u,v);
    for (int i=0;i<=49;++i)
    {
        for (int j=10;j<=39;++j)
        {
            SDL_Rect rt;
            if (!check(a,rt,j,i))
            {
                if (y1<=i && i<=y2 && j<=x2 && j>=x1) rt=a.ima[(i-y1)*40+(j-x1)];
                else rt=a.ima[i*40+j];
            }
            SDL_Rect td={(j-10)*10,i*10,10,10};
            SDL_RenderCopy(a.ren,a.txt,&rt,&td);
        }
    }
    if (u||v) tocdo(a,u,v);
    if (!u && !v)
    {
        SDL_Delay(120);
        SDL_RenderPresent(a.ren);
        vacham(a);
    }
}
void taogame(grap &a)
{
    a.window=SDL_CreateWindow(tencuaso.c_str(),SWU,SWU,300,500,SWS);
    if (a.window==NULL)
    {
        err("can not make window " + tencuaso +" !");
    }
    a.ren=SDL_CreateRenderer(a.window,-1,SRA);
    a.txt=creat(a.ren,im);
    if (a.txt==NULL)
    {
        err("can not creat Texture " + im + " !");
    }
    taoanh(a.ima);
    for (int i=0;i<=49;++i)
    {
        for (int j=10;j<=39;++j)
        {
            SDL_Rect rt=(i>=40 && j<=26 && j>=22 ?a.ima[(i-40)*40+(j-22)]:a.ima[40*i+j]);
            SDL_Rect vt={(j-10)*10,i*10,10,10};
            SDL_RenderCopy(a.ren,a.txt,&rt,&vt);
        }
    }
    vt.vtx1=22;
    vt.vtx2=26;
    vt.vty1=40;
    vt.vty2=49;
    SDL_RenderPresent(a.ren);
}
void taomang()
{
    for (int i=0;i<=18;++i)
    {
        mau[i]=rand()%5;
        while (mau[i]<0) mau[i]+=1;
        if (i>0 && mau[i]==mau[i-1]) mau[i]=(mau[i]+1)%5;
    }
    for (int i=0;i<=188;++i)
    {
        sta[i].first=rand()%36;
        while (sta[i].first<10) sta[i].first+=1;
        sta[i].second=rand()%36;
        while (sta[i].second<10) sta[i].second+=1;
        int h=sta[i].first-sta[i].second;
        if (-5<=h && h<=0)
        {
            if (sta[i].second+5<=35) sta[i].second+=5;
            else sta[i].first-=5;
        }
        if (5>=h && h>0)
        {
            if (sta[i].second-5>=10) sta[i].second-=5;
            else sta[i].first+=5;
        }
        h=0;
        if (sta[i].first==sta[i].second+5)
        {
            if (sta[i].first==35) {sta[i].second-=10;h=1;}
            if (sta[i].second==10) {sta[i].first+=10;h=1;}
            if (h==0) {sta[i].second-=1;sta[i].first+=1;}
        }
        if (sta[i].first==sta[i].second-5 && h==0)
        {
            if (sta[i].first==10) {sta[i].second+=10;h=1;}
            if (sta[i].second==35) {sta[i].first-=10;h=1;}
            if (h==0) {sta[i].second+=1;sta[i].first-=1;}
        }
    }
}
int main(int argc,char* argv[])
{
    grap a;
    taogame(a);
    taomang();
    while (!die)
    {
        SDL_Event e;
        update(a,0,0);
        while (SDL_PollEvent(&e)!=0)
        {
            if (e.type==SDL_QUIT)
            {
                die=1;
                break;
            }
            if (e.type==SDL_KEYDOWN)
            {
                if (e.key.keysym.sym==SDLK_LEFT) update(a,-1,0);
                if (e.key.keysym.sym==SDLK_RIGHT) update(a,1,0);
                if (e.key.keysym.sym==SDLK_UP) update(a,0,-1);
                if (e.key.keysym.sym==SDLK_DOWN) update(a,0,1);
            }
        }
    }
    return 0;
}
